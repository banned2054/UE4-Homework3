// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include <string>

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    //实例化你的类组件

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

    //设置角色网格体变换的位置和旋转
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

    //将你的类组件附加到默认角色的骨骼网格体组件。
    SpringArmComp->SetupAttachment(GetMesh());
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    //设置弹簧臂的类变量
    SpringArmComp->bUsePawnControlRotation = true;

    //设置角色动作组件的类变量
    GetCharacterMovement()->bOrientRotationToMovement = true;

    //初始化投射物类
    ProjectileClass = AThirdPersonMPProjectile::StaticClass();
    //初始化射速
    FireRate = 0.25f;
    bIsFiringWeapon = false;

    //初始化玩家生命值
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    team_numb = 1;

    muzzle_socket = FName("Muzzle");
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    weapon_ = Cast<AWeapon>(Get_Weapon());
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::Look_up);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Start_jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::Stop_jump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
    if (Controller != nullptr && CurrentHealth > 0)
    {
        if (AxisValue != 0.0f)
        {
            // 找出向前方向
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            // 获取向前矢量
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            AddMovementInput(Direction, AxisValue);
        }
    }

}

void APlayerCharacter::MoveRight(float AxisValue)
{
    if (Controller != nullptr && CurrentHealth > 0)
    {
        if (AxisValue != 0.0f)
        {
            // 找出向右方向
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            // 获取向右矢量 
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

            // 添加该方向动作
            AddMovementInput(Direction, AxisValue);
        }
    }
}
void APlayerCharacter::Turn(float InputAxis)
{
    if (Controller != nullptr)
    {
        AddControllerYawInput(InputAxis);
    }
}

void APlayerCharacter::Look_up(float InputAxis)
{
    if (Controller != nullptr && CurrentHealth > 0)
    {
        APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        FRotator cam_dir = CameraManager->GetCameraRotation();
        AddControllerPitchInput(InputAxis);
        if (cam_dir.Pitch < -53) cam_dir.Pitch = -52;
    }
}

void APlayerCharacter::Start_jump()
{
    bPressedJump = true;
}

void APlayerCharacter::Stop_jump()
{
    bPressedJump = false;
}

void APlayerCharacter::StartFire()
{
    if (!bIsFiringWeapon)
    {
        APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        FVector cam_dir = CameraManager->GetCameraRotation().Vector();
        FVector cam_loc = CameraManager->GetCameraLocation();
        FVector shoot_loc = weapon_->Mesh->GetSocketLocation(muzzle_socket);
        FVector shoot_dir = cam_loc + cam_dir * 10000 - shoot_loc;
        bIsFiringWeapon = true;
        UWorld* World = GetWorld();
        World->GetTimerManager().SetTimer(FiringTimer, this, &APlayerCharacter::StopFire, FireRate, false);
        HandleFire(shoot_loc, shoot_dir);
    }
}

void APlayerCharacter::StopFire()
{
    bIsFiringWeapon = false;
}

void APlayerCharacter::HandleFire_Implementation(FVector Shoot_loc, FVector Shoot_dir)
{
    FVector spawnLocation = Shoot_loc;
    FRotator spawnRotation = Shoot_dir.Rotation();

    FString TheFloatStr = FString::SanitizeFloat(Shoot_dir.X) + FString::SanitizeFloat(Shoot_dir.Y) + FString::SanitizeFloat(Shoot_dir.Z);
    UKismetSystemLibrary::PrintString(this, *TheFloatStr, true);

    FActorSpawnParameters spawnParameters;
    spawnParameters.Instigator = GetInstigator();
    spawnParameters.Owner = this;

    AThirdPersonMPProjectile* spawnedProjectile = GetWorld()->SpawnActor<AThirdPersonMPProjectile>(spawnLocation, spawnRotation, spawnParameters);
}


void APlayerCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //复制当前生命值。
    DOREPLIFETIME(APlayerCharacter, CurrentHealth);
}

void APlayerCharacter::OnHealthUpdate()
{
    //客户端特定的功能
    if (IsLocallyControlled())
    {
        FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

        if (CurrentHealth <= 0)
        {
            FString deathMessage = FString::Printf(TEXT("You have been killed."));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
        }
    }

    //服务器特定的功能
    if (GetLocalRole() == ROLE_Authority)
    {
        FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, healthMessage);
    }

    //在所有机器上都执行的函数。 
    /*
        因任何因伤害或死亡而产生的特殊功能都应放在这里。
    */
}

void APlayerCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void APlayerCharacter::SetCurrentHealth(float healthValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
        OnHealthUpdate();
    }
}

float APlayerCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float damageApplied = CurrentHealth - DamageTaken;
    SetCurrentHealth(damageApplied);
    return damageApplied;
}
