// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20;
	MagSize = 10;
	BulletsInMag = MagSize;
	FireRate = 8;
	ReloadTime = 2;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(RootComp);
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

EShotAttemptResult AWeapon::TryToFire()
{
	if (bFireOnCooldown) return EShotAttemptResult::ShotCooldown;
	if (BulletsInMag == 0) return EShotAttemptResult::NoAmmo;
	
	ExecuteFire();
	if (FireRate > 0)
	{
		bFireOnCooldown = true;
		FireCooldown = 1 / FireRate;
	}
	
	return EShotAttemptResult::Success;
}

void AWeapon::SetBulletsInMag(int NumOfBullets)
{
	if (NumOfBullets != BulletsInMag)
	{
		BulletsInMag = NumOfBullets;
		OnBulletsInMagChanged.Broadcast(BulletsInMag);
	}
}


void AWeapon::ReloadFinished_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloaded"));
	OnReloadingStateChanged.Broadcast(EReloadingState::Success);
}

void AWeapon::AbortReloading_Implementation()
{
	OnReloadingStateChanged.Broadcast(EReloadingState::Aborted);
}

void AWeapon::ExecuteFire_Implementation()
{
	SetBulletsInMag(BulletsInMag - 1);
	OnWeaponFired.Broadcast(this);
}

void AWeapon::Reload_Implementation()
{
	if (BulletsInMag < MagSize)
	{
		OnReloadingStateChanged.Broadcast(EReloadingState::InProgress);
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::ReloadFinished, ReloadTime);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFireOnCooldown)
	{
		if (FireCooldown > 0)
		{
			FireCooldown -= DeltaTime;
		}
		else
		{
			bFireOnCooldown = false;
		}
	}
}

