#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EscapeCharacter.generated.h"

UCLASS()
class ESCAPEROOM_API AEscapeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AEscapeCharacter();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void GiveKey();

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasKey() const;

    UFUNCTION(BlueprintPure, Category = "Interaction")
    FText GetInteractionText() const;

private:
    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Inventory",
        meta = (AllowPrivateAccess = "true")
    )
    bool bHasKey = false;
};