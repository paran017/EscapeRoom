#include "EscapeDoor.h"

#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "EscapeCharacter.h"
#include "Engine/Engine.h"
AEscapeDoor::AEscapeDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    // 액터 원점이 문 가장자리의 경첩 위치가 된다.
    Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
    SetRootComponent(Hinge);
    Hinge->SetMobility(EComponentMobility::Movable);

    DoorMesh =
        CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));

    DoorMesh->SetupAttachment(Hinge);
    DoorMesh->SetMobility(EComponentMobility::Movable);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (CubeAsset.Succeeded())
    {
        DoorMesh->SetStaticMesh(CubeAsset.Object);
    }

    // 기본 큐브: 한 변이 100cm.
    // 문 크기: 두께 10cm, 폭 100cm, 높이 220cm.
    DoorMesh->SetRelativeScale3D(FVector(0.1f, 1.0f, 2.2f));

    // 문 중심을 옮겨 한쪽 가장자리가 경첩에 닿게 한다.
    DoorMesh->SetRelativeLocation(FVector(0.0f, 50.0f, 110.0f));
    DoorMesh->SetRelativeRotation(FRotator::ZeroRotator);

    DoorMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AEscapeDoor::BeginPlay()
{
    Super::BeginPlay();

    // 이전에 배치된 액터에 남은 문짝 회전·위치도 초기화한다.
    DoorMesh->SetRelativeLocation(FVector(0.0f, 50.0f, 110.0f));
    DoorMesh->SetRelativeRotation(FRotator::ZeroRotator);

    // 레벨에서 배치한 액터의 방향을 닫힌 방향으로 기억한다.
    ClosedRotation = GetActorRotation();
    CurrentAngle = 0.0f;
    bIsOpen = false;

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
            &AEscapeDoor::TryInteract
        );

        Binding.bConsumeInput = false;
    }
}

void AEscapeDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const float TargetAngle = bIsOpen ? OpenAngle : 0.0f;

    CurrentAngle = FMath::FInterpConstantTo(
        CurrentAngle,
        TargetAngle,
        DeltaTime,
        OpenSpeed
    );

    // 초기 방향을 기준으로 경첩의 로컬 Z축 주위로 회전한다.
    // 액터 위치는 바꾸지 않으므로 경첩은 제자리에 고정된다.
    const FQuat DoorRotation =
        ClosedRotation.Quaternion()
        * FRotator(0.0f, CurrentAngle, 0.0f).Quaternion();

    SetActorRotation(DoorRotation);
}

void AEscapeDoor::TryInteract()
{
    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    if (!PC || !GetWorld())
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FVector TraceEnd =
        ViewLocation + ViewRotation.Vector() * InteractionDistance;

    FCollisionQueryParams Params;

    if (APawn* PlayerPawn = PC->GetPawn())
    {
        Params.AddIgnoredActor(PlayerPawn);
    }

    FHitResult Hit;

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        ViewLocation,
        TraceEnd,
        ECC_Visibility,
        Params
    );

    if (!bHit || Hit.GetActor() != this)
    {
        return;
    }

    if (bIsLocked)
    {
        AEscapeCharacter* Player =
            Cast<AEscapeCharacter>(PC->GetPawn());

        if (!Player || !Player->HasKey())
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    2.0f,
                    FColor::Red,
                    TEXT("You need a key.")
                );
            }

            return;
        }

        bIsLocked = false;

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                2.0f,
                FColor::Green,
                TEXT("Door unlocked!")
            );
        }
    }

    bIsOpen = !bIsOpen;
}