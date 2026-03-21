#include "PhysicsCube.h"
#include "AkAudioEvent.h"
#include "AkGameplayStatics.h"

APhysicsCube::APhysicsCube()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetNotifyRigidBodyCollision(true);
    SetActorEnableCollision(true);
}

void APhysicsCube::BeginPlay()
{
    Super::BeginPlay();
    MeshComp->OnComponentHit.AddDynamic(this, &APhysicsCube::OnHit);
}

void APhysicsCube::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                          UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                          const FHitResult& Hit)
{
    float CurrentSpeed = GetVelocity().Size();
    float CurrentTime = GetWorld()->GetTimeSeconds();

    if (CubeHitWwiseEvent != nullptr
        && CurrentSpeed > ImpactVelocityThreshold
        && (CurrentTime - LastImpactTime) > ImpactCooldown)
    {
        LastImpactTime = CurrentTime;
        FOnAkPostEventCallback NullCallback;
        UAkGameplayStatics::PostEvent(
            CubeHitWwiseEvent,
            this,
            0,
            NullCallback
        );
    }
}
