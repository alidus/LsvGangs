// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Components/SphereComponent.h"

#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class LSVGANGS_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
public:
	AWeaponPickup();
	UFUNCTION(BlueprintCallable)
	void InitializePickup();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponClass;


	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	UFUNCTION()
	void OnSphereCollisionBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
