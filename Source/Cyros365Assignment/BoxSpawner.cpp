#include "BoxSpawner.h"
#include "BoxActor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"

ABoxSpawner::ABoxSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // Optional: Add visual to locate spawner in scene
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    Arrow->SetupAttachment(RootComponent);
}

void ABoxSpawner::BeginPlay()
{
    Super::BeginPlay();
    FetchJSON();
}

void ABoxSpawner::FetchJSON()
{
    FString URL = "https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json";

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ABoxSpawner::OnResponseReceived);
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->ProcessRequest();
}

void ABoxSpawner::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to fetch JSON data"));
        return;
    }

    FString ResponseStr = Response->GetContentAsString();
    UE_LOG(LogTemp, Warning, TEXT("JSON Data Received"));

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // Read types
        const TArray<TSharedPtr<FJsonValue>>* TypesArray;
        if (JsonObject->TryGetArrayField("types", TypesArray))
        {
            for (const TSharedPtr<FJsonValue>& Value : *TypesArray)
            {
                const TSharedPtr<FJsonObject> TypeObj = Value->AsObject();
                FBoxType NewType;
                NewType.Name = TypeObj->GetStringField("name");

                const TArray<TSharedPtr<FJsonValue>> ColorArray = TypeObj->GetArrayField("color");
                NewType.Color = FLinearColor(
                    ColorArray[0]->AsNumber() / 255.f,
                    ColorArray[1]->AsNumber() / 255.f,
                    ColorArray[2]->AsNumber() / 255.f
                );

                NewType.Health = TypeObj->GetIntegerField("health");
                NewType.Score = TypeObj->GetIntegerField("score");

                BoxTypes.Add(NewType.Name, NewType);
            }
        }

        // Read objects to spawn
        const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
        if (JsonObject->TryGetArrayField("objects", ObjectsArray))
        {
            for (const TSharedPtr<FJsonValue>& Value : *ObjectsArray)
            {
                const TSharedPtr<FJsonObject> Obj = Value->AsObject();
                FString TypeName = Obj->GetStringField("type");

                if (BoxTypes.Contains(TypeName))
                {
                    const TSharedPtr<FJsonObject> TransformObj = Obj->GetObjectField("transform");

                    const TArray<TSharedPtr<FJsonValue>> LocArray = TransformObj->GetArrayField("location");
                    const TArray<TSharedPtr<FJsonValue>> RotArray = TransformObj->GetArrayField("rotation");
                    const TArray<TSharedPtr<FJsonValue>> ScaleArray = TransformObj->GetArrayField("scale");

                    FVector Location(LocArray[0]->AsNumber(), LocArray[1]->AsNumber(), LocArray[2]->AsNumber());
                    FRotator Rotation(RotArray[0]->AsNumber(), RotArray[1]->AsNumber(), RotArray[2]->AsNumber());
                    FVector Scale(ScaleArray[0]->AsNumber(), ScaleArray[1]->AsNumber(), ScaleArray[2]->AsNumber());

                    UE_LOG(LogTemp, Warning, TEXT("Spawning Box %s at Location: %s"), *TypeName, *Location.ToString());

                    FActorSpawnParameters Params;
                    ABoxActor* SpawnedBox = GetWorld()->SpawnActor<ABoxActor>(BoxActorClass, Location, Rotation, Params);

                    if (SpawnedBox)
                    {
                        SpawnedBox->SetBoxColor(BoxTypes[TypeName].Color);
                        SpawnedBox->SetHealth(BoxTypes[TypeName].Health);
                        SpawnedBox->SetActorScale3D(Scale);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to spawn BoxActor"));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Type %s not found in BoxTypes map"), *TypeName);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No 'objects' array found in JSON"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
    }
}
