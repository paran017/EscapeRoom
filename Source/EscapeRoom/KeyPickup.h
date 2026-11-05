#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyPickup.generated.h"

class UStaticMeshComponent;

UCLASS()
class ESCAPEROOM_API AKeyPickup : public AActor
{
    GENERATED_BODY()

public:
    AKeyPickup();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Key")
    TObjectPtr<UStaticMeshComponent> KeyMesh;

    UPROPERTY(EditAnywhere, Category = "Key")
    float InteractionDistance = 250.0f;

    void TryPickup();
};