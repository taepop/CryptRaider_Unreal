// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetShouldMove(bool ShouldMove);
	
private:
	UPROPERTY(EditAnywhere)
	FVector MoveOffset; //얼마나 이동할 건지 나타내는 백터

	UPROPERTY(EditAnywhere)
	float MoveTime = 4; //actor가 움직이는 시간
	
	bool ShouldMove = false; //이걸 켜면 움직이게

	FVector OriginalLocation;
};
