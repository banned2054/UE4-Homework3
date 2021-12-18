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

// 复制的属性

void UHealthComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //复制当前生命值。
    DOREPLIFETIME(UHealthComponent, team_numb);
}