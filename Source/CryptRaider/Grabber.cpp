// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) 
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

//
void UGrabber::Grab() {
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr) {
		return; //nullptr 생기는 거 방지.
	}

	FHitResult HitResult;
	bool hit = GetGrabbableInReach(HitResult);
	
	if(hit) 
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed"); //grab했을 때 grabbed라는 태그를 가고일에 만듦.
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None, //skeletal mesh를 잡고 싶을 때는 이름을 넣지만, 우리는 아니니까 None으로.
			HitResult.ImpactPoint, //어디서 잡을지
			GetComponentRotation() //잡는 rotation 방향. 그냥 그대로.
		);
	}
}

//
bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const 
{
	FVector Start = GetComponentLocation(); //start는 location of the grabber
	FVector End = Start + GetForwardVector() * MaxGrabDistance; 
	//start vector + grabber의 forward vector * N. 이 N 값은 editor에서 조정할 거임.
	DrawDebugLine(GetWorld(), Start, End, FColor::Red); //나머지 input은 디폴트값으로 될 거임.
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(
		OutHitResult, Start, End, 
		FQuat::Identity, 	//FQuat::Identity는 rotation 없다는 걸 알려주는 부분.
		ECC_GameTraceChannel2, //trace channel은 DefaultEngine 파일에서 찾아서 넣는다.  
		Sphere); //위에 FCollisionShape으로 만든 구체. Grabber.h에서 GrabRadius를 만들었음.
}


void UGrabber::Release() {
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) //갖고 있는 게 있는지 체크
	{ 
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent(); //놔줘
	}
}	


UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const 
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return Result;
}
