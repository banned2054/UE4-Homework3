// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	team_numb = 255;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}

// ���Ƶ�����

void UHealthComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //���Ƶ�ǰ����ֵ��
    DOREPLIFETIME(UHealthComponent, team_numb);
}