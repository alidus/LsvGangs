// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Weapon.h"
#include "WeaponPickup.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "LsvPersonCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedWeapon, AWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDroppedWeapon, AWeapon*, Weapon);

UCLASS()
class LSVGANGS_API ALsvPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALsvPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUpAtRate(float Value);
	void TurnAtRate(float Value);

	void ToggleCrouch();
	
	void ToggleAds();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EnableAds();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisableAdb();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEquippedWeaponFired(AWeapon* FiredEquippedWeapon);

	UFUNCTION(BlueprintCallable)
	void StartFire();
	UFUNCTION(BlueprintCallable)
	void StopFire();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    EShotAttemptResult TryToFireEquippedWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EquipWeapon(AWeapon* Weapon);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void DropWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reload();

	// Pickups
	UFUNCTION(BlueprintCallable)
    void OnOverlappedPickup(AWeaponPickup* WeaponPickup);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CalculateAndRefillEquippedWeaponAmmo();

protected:
	UFUNCTION(BlueprintCallable)
	void InitializeEquippedWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEquippedWeaponReloadingStateChanged(EReloadingState ReloadingState);


public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponHipLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* WeaponAdsLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdsFov;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector DefaultCameraPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AdsCameraOffset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bContinueFiring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bReloading;

	UPROPERTY(BlueprintAssignable)
	FEquippedWeapon OnEquippedWeapon;
	UPROPERTY(BlueprintAssignable)
	FDroppedWeapon OnDroppedWeapon;

	// Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWeapon* EquippedWeapon;
};
