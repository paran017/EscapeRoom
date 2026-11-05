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

    // ���� ������ �� �����ڸ��� ��ø ��ġ�� �ȴ�.
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

    // �⺻ ť��: �� ���� 100cm.
    // �� ũ��: �β� 10cm, �� 100cm, ���� 220cm.
    DoorMesh->SetRelativeScale3D(FVector(0.1f, 1.0f, 2.2f));

    // �� �߽��� �Ű� ���� �����ڸ��� ��ø�� ��� �Ѵ�.
    DoorMesh->SetRelativeLocation(FVector(0.0f, 50.0f, 110.0f));
    DoorMesh->SetRelativeRotation(FRotator::ZeroRotator);

    DoorMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AEscapeDoor::BeginPlay()
{
    Super::BeginPlay();

    // ������ ��ġ�� ���Ϳ� ���� ��¦ ȸ������ġ�� �ʱ�ȭ�Ѵ�.
    DoorMesh->SetRelativeLocation(FVector(0.0f, 50.0f, 110.0f));
    DoorMesh->SetRelativeRotation(FRotator::ZeroRotator);

    // �������� ��ġ�� ������ ������ ���� �������� ����Ѵ�.
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

    // �ʱ� ������ �������� ��ø�� ���� Z�� ������ ȸ���Ѵ�.
    // ���� ��ġ�� �ٲ��� �����Ƿ� ��ø�� ���ڸ��� �����ȴ�.
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