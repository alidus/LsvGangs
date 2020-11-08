// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "LsvPersonCharacter.h"

AWeaponPickup::AWeaponPickup()
{
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
    SkeletalMesh->SetSimulatePhysics(true);
    SkeletalMesh->SetCollisionProfileName("BlockAll");
    SetRootComponent(SkeletalMesh);
    
    SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    SphereCollision->SetupAttachment(RootComponent);
    SphereCollision->InitSphereRadius(50);
    SphereCollision->SetCollisionProfileName("OverlapOnlyPawn");
    InitializePickup();
}

void AWeaponPickup::InitializePickup()
{
    if (WeaponClass)
    {
        USkeletalMesh* NewSkeletalMesh = WeaponClass.GetDefaultObject()->WeaponMesh->SkeletalMesh;
        if (SkeletalMesh && NewSkeletalMesh) SkeletalMesh->SetSkeletalMesh(WeaponClass.GetDefaultObject()->WeaponMesh->SkeletalMesh);
    }
}

void AWeaponPickup::BeginPlay()
{
    Super::BeginPlay();
    if (SphereCollision) SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickup::OnSphereCollisionBeginOverlap);
}

void AWeaponPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    InitializePickup();
}

void AWeaponPickup::OnSphereCollisionBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("UE4"));
    ALsvPersonCharacter* CollidedPersonCharacter = Cast<ALsvPersonCharacter>(OtherActor);
    if (CollidedPersonCharacter)
    {
        CollidedPersonCharacter->OnOverlappedPickup(this);
    }
}

