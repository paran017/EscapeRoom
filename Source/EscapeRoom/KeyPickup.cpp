#include "KeyPickup.h"

#include "EscapeCharacter.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AKeyPickup::AKeyPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    KeyMesh =
        CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));

    SetRootComponent(KeyMesh);

    // 임시 열쇠 모양으로 구체 사용
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(
        TEXT("/Engine/BasicShapes/Sphere.Sphere")
    );

    if (SphereAsset.Succeeded())
    {
        KeyMesh->SetStaticMesh(SphereAsset.Object);
    }

    KeyMesh->SetRelativeScale3D(FVector(0.25f));

    // 이동은 막지 않고, 바라보기 검사에는 잡히도록 설정
    KeyMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    KeyMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    KeyMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AKeyPickup::BeginPlay()
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
            &AKeyPickup::TryPickup
        );

        Binding.bConsumeInput = false;
    }
}

void AKeyPickup::TryPickup()
{
    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    if (!PC)
    {
        return;
    }

    AEscapeCharacter* Player =
        Cast<AEscapeCharacter>(PC->GetPawn());

    if (!Player)
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FVector TraceEnd =
        ViewLocation + ViewRotation.Vector() * InteractionDistance;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Player);

    FHitResult Hit;

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        ViewLocation,
        TraceEnd,
        ECC_Visibility,
        Params
    );

    if (bHit && Hit.GetActor() == this)
    {
        Player->GiveKey();

        // 획득한 열쇠를 레벨에서 제거
        Destroy();
    }
}