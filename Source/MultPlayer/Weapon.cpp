// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));

	Muzzle_Socket = FName("Muzzle");
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeapon::Start_fire()
{
	Spawn_Projectile();
}

void AWeapon::Stop_fire()
{
}

bool AWeapon::Calc_Fire_Info(FVector& location, FVector& Dir)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (CameraManager)
	{
		location = Mesh->GetSocketLocation(Muzzle_Socket);
		FVector cam_loc = CameraManager->GetCameraLocation();
		FVector cam_dir = CameraManager->GetCameraRotation().Vector();
		Dir = ((cam_loc + cam_dir * 1000) - location).GetSafeNormal();
		return true;
	}
	return false;
}
