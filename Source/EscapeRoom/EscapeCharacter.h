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

    // 열쇠 획득
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void GiveKey();

    // 열쇠 보유 여부 확인
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasKey() const;

private:
    // 플레이 중 확인 가능하며 블루프린트에서는 읽기만 허용
    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Inventory",
        meta = (AllowPrivateAccess = "true")
    )
    bool bHasKey = false;
};