// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"


UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if(Mover == nullptr)
    {
        return;
    }//

    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr) {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent()); 
        if (Component != nullptr) 
        {
            Component->SetSimulatePhysics(false);
        }
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else {
        Mover->SetShouldMove(false);
    }
}

void UTriggerComponent::SetMover(UMover* NewMover) 
{
    Mover = NewMover; 
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> Actors; //Array 선언
    GetOverlappingActors(Actors);

    for(AActor* Actor : Actors) //box component과 overlap되는 모든 actor를 살피는 중
    {
        bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");
        if(HasAcceptableTag && !IsGrabbed) //unlock이라는 태그가 있는 애들만 이름을 출력.
        {  
            return Actor; //태그가 맞는 애가 나오면 그 actor를 return
        }
    }

    return nullptr; //여기까지 온 거면 tag에 맞는 애가 없었다는 것.
}