// Fill out your copyright notice in the Description page of Project Settings.


#include "LsvPersonCharacter.h"

// Sets default values
ALsvPersonCharacter::ALsvPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultFov = 100.f;
	AdsFov = 60.f;
	bAds = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetMesh(), "neck_01_Camera");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	WeaponHipLocation = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponHitLocation"));
	WeaponHipLocation->SetupAttachment(CameraComponent);
	
	WeaponAdsLocation = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAdsLocation"));
	WeaponAdsLocation->SetupAttachment(CameraComponent);
}

// Called when the game starts or when spawned
void ALsvPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CameraComponent = FindComponentByClass<UCameraComponent>();
	if (CameraComponent)
	{
		DefaultFov = CameraComponent->FieldOfView;
		DefaultCameraPosition = CameraComponent->GetRelativeLocation();
	}
}

// Called every frame
void ALsvPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EquippedWeapon && bContinueFiring && !bReloading)
	{
		TryToFireEquippedWeapon();
	}
}

// Called to bind functionality to input
void ALsvPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    InputComponent->BindAxis("LookUp", this, &ALsvPersonCharacter::LookUpAtRate);
	InputComponent->BindAxis("Turn", this, &ALsvPersonCharacter::TurnAtRate);
	InputComponent->BindAxis("MoveForward", this, &ALsvPersonCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALsvPersonCharacter::MoveRight);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &ALsvPersonCharacter::ToggleCrouch);
	InputComponent->BindAction("ADS", IE_Pressed, this, &ALsvPersonCharacter::ToggleAds);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ALsvPersonCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ALsvPersonCharacter::StopFire);

	InputComponent->BindAction("DropWeapon", IE_Pressed, this, &ALsvPersonCharacter::DropWeapon);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ALsvPersonCharacter::Reload);
}

void ALsvPersonCharacter::MoveForward(float Value)
{
	if (Value == 0.f) return;
	
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALsvPersonCharacter::MoveRight(float Value)
{
	if (Value == 0.f) return;
	
	AddMovementInput(GetActorRightVector(), Value);
}

void ALsvPersonCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value);
}

void ALsvPersonCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value);
}

void ALsvPersonCharacter::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching()) UE_LOG(LogTemp, Warning, TEXT("Trying to stand up"))
	else UE_LOG(LogTemp, Warning, TEXT("Trying to crouch"));

	GetCharacterMovement()->bWantsToCrouch = !GetCharacterMovement()->IsCrouching();

}

void ALsvPersonCharacter::ToggleAds()
{
	if (!bAds) EnableAds(); else DisableAdb();
}

void ALsvPersonCharacter::EnableAds_Implementation()
{
	bAds = true;
}


void ALsvPersonCharacter::DisableAdb_Implementation()
{
	bAds = false;
}

void ALsvPersonCharacter::OnEquippedWeaponFired_Implementation(AWeapon* FiredEquippedWeapon)
{
}

void ALsvPersonCharacter::StartFire()
{
	bContinueFiring = true;
}

void ALsvPersonCharacter::StopFire()
{
	bContinueFiring = false;
}

EShotAttemptResult ALsvPersonCharacter::TryToFireEquippedWeapon_Implementation()
{
	return EquippedWeapon->TryToFire();
}

void ALsvPersonCharacter::EquipWeapon_Implementation(AWeapon* Weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Equipped weapon"));
	EquippedWeapon = Weapon;
	InitializeEquippedWeapon();
	OnEquippedWeapon.Broadcast(EquippedWeapon);
}

void ALsvPersonCharacter::DropWeapon_Implementation()
{
	FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	if (EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(DetachmentTransformRules);
		// Spawn pickup actor
		FTransform SpawnTransform = FTransform(GetActorRotation(), GetActorLocation() + GetActorForwardVector() * 200.f, FVector(1, 1, 1));
		AWeaponPickup* SpawningWeaponPickup = GetWorld()->SpawnActorDeferred<AWeaponPickup>(AWeaponPickup::StaticClass(), SpawnTransform);
		SpawningWeaponPickup->WeaponClass = EquippedWeapon->GetClass();
		SpawningWeaponPickup->SkeletalMesh->SetSkeletalMesh(EquippedWeapon->WeaponMesh->SkeletalMesh);
		SpawningWeaponPickup->FinishSpawning(SpawnTransform);
		SpawningWeaponPickup->SkeletalMesh->AddImpulse(GetActorForwardVector() * 500.f);
		
		OnDroppedWeapon.Broadcast(EquippedWeapon);
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;

		bReloading = false;
	}
}


void ALsvPersonCharacter::Reload_Implementation()
{
	if (!bReloading && EquippedWeapon) EquippedWeapon->Reload();
}

void ALsvPersonCharacter::OnOverlappedPickup(AWeaponPickup* WeaponPickup)
{
	if (!EquippedWeapon)
	{
		FActorSpawnParameters ActorSpawnParameters;
		AWeapon* SpawnedWeaponActor = GetWorld()->SpawnActor<AWeapon>(WeaponPickup->WeaponClass, WeaponHipLocation->GetComponentLocation(), WeaponHipLocation->GetComponentRotation());
		if (SpawnedWeaponActor)
		{
			EquipWeapon(SpawnedWeaponActor);
			WeaponPickup->Destroy();
		}
	}
}

void ALsvPersonCharacter::CalculateAndRefillEquippedWeaponAmmo_Implementation()
{
	EquippedWeapon->SetBulletsInMag(EquippedWeapon->MagSize);
}

void ALsvPersonCharacter::InitializeEquippedWeapon()
{
	FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	EquippedWeapon->AttachToComponent(WeaponHipLocation, AttachmentTransformRules);
	EquippedWeapon->OnWeaponFired.AddDynamic(this, &ALsvPersonCharacter::OnEquippedWeaponFired);
	EquippedWeapon->OnReloadingStateChanged.AddDynamic(this, &ALsvPersonCharacter::OnEquippedWeaponReloadingStateChanged);
}

void ALsvPersonCharacter::OnEquippedWeaponReloadingStateChanged_Implementation(EReloadingState ReloadingState)
{
	switch (ReloadingState)
	{
	case EReloadingState::Success:
		bReloading = false;
		CalculateAndRefillEquippedWeaponAmmo();
		break;
	case EReloadingState::Aborted:
		bReloading = false;
		break;
	case EReloadingState::InProgress:
		bReloading = true;
		break;
	default: ;
	}
}

