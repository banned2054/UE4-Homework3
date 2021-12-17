// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class MULTPLAYER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		FName Muzzle_Socket;
	UFUNCTION(BlueprintCallable)
		void Start_fire();
	UFUNCTION(BlueprintCallable)
		void Stop_fire();
	UFUNCTION(BlueprintImplementableEvent)
		void Spawn_Projectile();
	UFUNCTION(BlueprintCallable)
		bool Calc_Fire_Info(FVector& location, FVector& Dir);
};
