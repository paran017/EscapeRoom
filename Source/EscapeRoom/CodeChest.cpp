#include "CodeChest.h"

#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "EscapeCharacter.h"

ACodeChest::ACodeChest()
{
    PrimaryActorTick.bCanEverTick = false;

    ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("ChestMesh")
    );
    SetRootComponent(ChestMesh);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (Cube.Succeeded())
    {
        ChestMesh->SetStaticMesh(Cube.Object);
    }

    ChestMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    ChestMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void ACodeChest::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    if (!PC)
    {
        return;
    }

    EnableInput(PC);

    if (InputComponent)
    {
        FInputKeyBinding& Binding = InputComponent->BindKey(
            EKeys::E,
            IE_Pressed,
            this,
            &ACodeChest::TryInteract
        );

        // Let the existing door and key receive E as well.
        Binding.bConsumeInput = false;
    }
}

void ACodeChest::TryInteract()
{
    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    UWorld* World = GetWorld();

    if (!PC || !World)
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    FCollisionQueryParams Params;
    if (APawn* PlayerPawn = PC->GetPawn())
    {
        Params.AddIgnoredActor(PlayerPawn);
    }

    FHitResult Hit;
    const FVector End = ViewLocation + ViewRotation.Vector() * 250.0f;

    if (World->LineTraceSingleByChannel(
        Hit, ViewLocation, End, ECC_Visibility, Params
    ) && Hit.GetActor() == this && !bOpened)
    {
        OpenCodeWidget();
    }
}

bool ACodeChest::SubmitCode(const FString& EnteredCode)
{
    if (bOpened || EnteredCode.TrimStartAndEnd() != CorrectCode)
    {
        return false;
    }

    AEscapeCharacter* Player = Cast<AEscapeCharacter>(
        UGameplayStatics::GetPlayerPawn(this, 0)
    );

    if (!Player)
    {
        return false;
    }

    Player->GiveKey();
    bOpened = true;
    return true;
}

void ACodeChest::SetChestCursorVisible(bool bVisible)
{
    if (APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0))
    {
        PC->bShowMouseCursor = bVisible;
    }
}