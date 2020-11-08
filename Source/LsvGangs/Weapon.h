// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBulletsInMagChanged, int, NumOfBullets);

UENUM(BlueprintType)
enum class EReloadingState : uint8 { Success, Aborted, InProgress };

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadingStateChanged, EReloadingState, ReloadingState);

UENUM(BlueprintType)
enum class EShotAttemptResult : uint8 { Success, NoAmmo, Jammed, ShotCooldown};


class AWeapon;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponFired, AWeapon*, Weapon);

UCLASS()
class LSVGANGS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ExecuteFire();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
    virtual EShotAttemptResult TryToFire();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reload();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ReloadFinished();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void AbortReloading();

	UFUNCTION(BlueprintCallable)
    void SetBulletsInMag(int NumOfBullets);
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MagSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BulletsInMag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bFireOnCooldown;

	float FireCooldown;

	FTimerHandle ReloadTimerHandle;

	UPROPERTY(BlueprintAssignable)
	FReloadingStateChanged OnReloadingStateChanged;
	UPROPERTY(BlueprintAssignable)
	FWeaponFired OnWeaponFired;
	UPROPERTY(BlueprintAssignable)
	FBulletsInMagChanged OnBulletsInMagChanged;
	 
};
