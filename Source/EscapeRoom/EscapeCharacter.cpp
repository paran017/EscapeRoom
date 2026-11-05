#include "EscapeCharacter.h"

#include "EscapeDoor.h"
#include "KeyPickup.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AEscapeCharacter::AEscapeCharacter()
{
    bHasKey = false;
}

void AEscapeCharacter::GiveKey()
{
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

FText AEscapeCharacter::GetInteractionText() const
{
    const APlayerController* PC =
        Cast<APlayerController>(GetController());

    if (!PC || !GetWorld())
    {
        return FText::GetEmpty();
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FVector TraceEnd =
        ViewLocation + ViewRotation.Vector() * 250.0f;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    FHitResult Hit;

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        ViewLocation,
        TraceEnd,
        ECC_Visibility,
        Params
    );

    if (!bHit)
    {
        return FText::GetEmpty();
    }

    if (Cast<AKeyPickup>(Hit.GetActor()))
    {
        return FText::FromString(TEXT("E: Pick up key"));
    }

    if (Cast<AEscapeDoor>(Hit.GetActor()))
    {
        return FText::FromString(TEXT("E: Use door"));
    }

    return FText::GetEmpty();
}