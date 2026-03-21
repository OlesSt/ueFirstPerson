#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AkAudioEvent.h"

#include "PhysicsCube.generated.h"

UCLASS()
class UEFIRSTPERSON_API APhysicsCube : public AActor
{
    GENERATED_BODY()

public:
    APhysicsCube();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
               UPrimitiveComponent* OtherComp, FVector NormalImpulse,
               const FHitResult& Hit);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    UAkAudioEvent* CubeHitWwiseEvent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
    UStaticMeshComponent* MeshComp;

private:
    float LastImpactTime = -999.f;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    float ImpactCooldown = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    float ImpactVelocityThreshold = 150.f;
};
