#include "EscapeCharacter.h"
#include "Engine/Engine.h"

AEscapeCharacter::AEscapeCharacter()
{
    bHasKey = false;
}

void AEscapeCharacter::GiveKey()
{
    // 이미 가지고 있으면 중복 획득하지 않는다.
    if (bHasKey)
    {
        return;
    }

    bHasKey = true;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            3.0f,
            FColor::Yellow,
            TEXT("Key acquired!")
        );
    }
}

bool AEscapeCharacter::HasKey() const
{
    return bHasKey;
}