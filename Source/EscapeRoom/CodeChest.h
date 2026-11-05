#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodeChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ESCAPEROOM_API ACodeChest : public AActor
{
    GENERATED_BODY()

public:
    ACodeChest();
    UFUNCTION(BlueprintCallable, Category = "Chest")
    bool SubmitCode(const FString& EnteredCode);

    UFUNCTION(BlueprintImplementableEvent, Category = "Chest")
    void OpenCodeWidget();

    UFUNCTION(BlueprintCallable, Category = "Chest")
    void SetChestCursorVisible(bool bVisible);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Chest")
    TObjectPtr<UStaticMeshComponent> ChestMesh;

    void TryInteract();
    UPROPERTY(EditAnywhere, Category = "Chest")
    FString CorrectCode = TEXT("4312");

    bool bOpened = false;
};