// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonMPProjectile.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Weapon.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MULTPLAYER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** 属性复制 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//玩家跟随摄像机
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* CameraComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//跟随玩家背后摄像机的弹簧臂组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;
	
	//为向前/向后输入而调用
	void MoveForward(float InputAxis);

	//为左/右侧输入而调用
	void MoveRight(float InputAxis);

	void Turn(float InputAxis);

	void Look_up(float InputAxis);

	void Start_jump();

	void Stop_jump();

	void Change_stay();

	UFUNCTION(BlueprintImplementableEvent)
		void Start_fire();

	UFUNCTION(BlueprintImplementableEvent)
		void Stop_fire();

	UFUNCTION(BlueprintImplementableEvent)
		void Change_spot();
	UFUNCTION(BlueprintImplementableEvent)
		void Start_grenade();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
		TSubclassOf<class AThirdPersonMPProjectile> ProjectileClass;

	/** 射击之间的延迟，单位为秒。用于控制测试发射物的射击速度，还可防止服务器函数的溢出导致将SpawnProjectile直接绑定至输入。*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	/** 若为true，则正在发射投射物。*/
	bool bIsFiringWeapon;

	/** 用于启动武器射击的函数。*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartFire();

	/** 用于结束武器射击的函数。一旦调用这段代码，玩家可再次使用StartFire。*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StopFire();

	/** 用于生成投射物的服务器函数。*/
	UFUNCTION(Server, Reliable)
		void HandleFire(FVector Shoot_loc,FVector Shoot_dir);

	/** 定时器句柄，用于提供生成间隔时间内的射速延迟。*/
	FTimerHandle FiringTimer;

	/** 玩家的最大生命值。这是玩家的最高生命值，也是出生时的生命值。*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth;

	/** 玩家的当前生命值。降到0就表示死亡。*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float CurrentHealth;

	/** RepNotify，用于同步对当前生命值所做的更改。*/
	UFUNCTION()
		void OnRep_CurrentHealth();

	/** 响应要更新的生命值。修改后，立即在服务器上调用，并在客户端上调用以响应RepNotify*/
	void OnHealthUpdate();

	UPROPERTY(EditAnywhere)
		FName muzzle_socket;

	AWeapon* weapon_;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 最大生命值的取值函数。*/
	UFUNCTION(BlueprintCallable,Category = "Health", BlueprintCallable)
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** 当前生命值的取值函数。*/
	UFUNCTION(BlueprintCallable,Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** 当前生命值的存值函数。将此值的范围限定在0到MaxHealth之间，并调用OnHealthUpdate。仅在服务器上调用。*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** 承受伤害的事件。从APawn覆盖。*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
		AActor* Get_Weapon();
};