#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ABoxActor::ABoxActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
    RootComponent = BoxMesh;

    // Automatically load a default cube mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (BoxMeshAsset.Succeeded())
    {
        BoxMesh->SetStaticMesh(BoxMeshAsset.Object);
    }
}

void ABoxActor::BeginPlay()
{
    Super::BeginPlay();

    if (BoxMesh)
    {
        DynamicMaterial = BoxMesh->CreateAndSetMaterialInstanceDynamic(0);
    }
}

void ABoxActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABoxActor::SetBoxColor(const FLinearColor& Color)
{
    if (DynamicMaterial)
    {
        DynamicMaterial->SetVectorParameterValue("Color", Color);
    }
}

void ABoxActor::SetHealth(int32 NewHealth)
{
    Health = NewHealth;
}

void ABoxActor::ApplyDamage(int32 DamageAmount)
{
    Health -= DamageAmount;

    if (Health <= 0)
    {
        Destroy();
    }
}
