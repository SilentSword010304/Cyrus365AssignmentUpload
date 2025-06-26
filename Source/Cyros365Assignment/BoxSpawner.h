#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"           // Required for TSubclassOf
#include "Http.h"   
#include "BoxSpawner.generated.h"

USTRUCT()
struct FBoxType
{
    GENERATED_BODY()

    FString Name;
    FLinearColor Color;
    int32 Health;
    int32 Score;
};

UCLASS()
class CYROS365ASSIGNMENT_API ABoxSpawner : public AActor
{
    GENERATED_BODY()

public:
    ABoxSpawner();

protected:
    virtual void BeginPlay() override;

private:
    void FetchJSON();

    // Correct declaration for response handling
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    TMap<FString, FBoxType> BoxTypes;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class ABoxActor> BoxActorClass;
};
