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

	/** ���Ը��� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//��Ҹ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* CameraComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//������ұ���������ĵ��ɱ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;
	
	//Ϊ��ǰ/������������
	void MoveForward(float InputAxis);

	//Ϊ��/�Ҳ����������
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

	/** ���֮����ӳ٣���λΪ�롣���ڿ��Ʋ��Է����������ٶȣ����ɷ�ֹ������������������½�SpawnProjectileֱ�Ӱ������롣*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	/** ��Ϊtrue�������ڷ���Ͷ���*/
	bool bIsFiringWeapon;

	/** ����������������ĺ�����*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartFire();

	/** ���ڽ�����������ĺ�����һ��������δ��룬��ҿ��ٴ�ʹ��StartFire��*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StopFire();

	/** ��������Ͷ����ķ�����������*/
	UFUNCTION(Server, Reliable)
		void HandleFire(FVector Shoot_loc,FVector Shoot_dir);

	/** ��ʱ������������ṩ���ɼ��ʱ���ڵ������ӳ١�*/
	FTimerHandle FiringTimer;

	/** ��ҵ��������ֵ��������ҵ��������ֵ��Ҳ�ǳ���ʱ������ֵ��*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth;

	/** ��ҵĵ�ǰ����ֵ������0�ͱ�ʾ������*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float CurrentHealth;

	/** RepNotify������ͬ���Ե�ǰ����ֵ�����ĸ��ġ�*/
	UFUNCTION()
		void OnRep_CurrentHealth();

	/** ��ӦҪ���µ�����ֵ���޸ĺ������ڷ������ϵ��ã����ڿͻ����ϵ�������ӦRepNotify*/
	void OnHealthUpdate();

	UPROPERTY(EditAnywhere)
		FName muzzle_socket;

	AWeapon* weapon_;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** �������ֵ��ȡֵ������*/
	UFUNCTION(BlueprintCallable,Category = "Health", BlueprintCallable)
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ��ǰ����ֵ��ȡֵ������*/
	UFUNCTION(BlueprintCallable,Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ��ǰ����ֵ�Ĵ�ֵ����������ֵ�ķ�Χ�޶���0��MaxHealth֮�䣬������OnHealthUpdate�����ڷ������ϵ��á�*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** �����˺����¼�����APawn���ǡ�*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
		AActor* Get_Weapon();
};