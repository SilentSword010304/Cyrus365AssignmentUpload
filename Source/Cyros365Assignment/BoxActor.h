#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxActor.generated.h"

UCLASS()
class CYROS365ASSIGNMENT_API ABoxActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void SetBoxColor(const FLinearColor& Color);
    void SetHealth(int32 NewHealth);
    void ApplyDamage(int32 DamageAmount);

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BoxMesh;

    int32 Health;
    int32 Score;

    // Dynamic material is a runtime variable, no need for UPROPERTY
    UMaterialInstanceDynamic* DynamicMaterial;
};

