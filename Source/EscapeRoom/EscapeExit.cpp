#include "EscapeExit.h"

#include "EscapeCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

AEscapeExit::AEscapeExit()
{
    PrimaryActorTick.bCanEverTick = false;

    ExitBox =
        CreateDefaultSubobject<UBoxComponent>(TEXT("ExitBox"));

    SetRootComponent(ExitBox);

    ExitBox->SetBoxExtent(FVector(80.0f, 100.0f, 150.0f));

    // �÷��̾�� ��ġ������ �˻��ϰ� �̵��� ���� �ʴ´�.
    ExitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    ExitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ExitBox->SetGenerateOverlapEvents(true);
}

void AEscapeExit::BeginPlay()
{
    Super::BeginPlay();

    ExitBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &AEscapeExit::OnExitEntered
    );
}

void AEscapeExit::OnExitEntered(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (bCompleted)
    {
        return;
    }

    AEscapeCharacter* Player = Cast<AEscapeCharacter>(OtherActor);

    if (!Player)
    {
        return;
    }

    APlayerController* PC =
        Cast<APlayerController>(Player->GetController());

    if (!PC || !SuccessWidgetClass)
    {
        return;
    }

    SuccessWidget = CreateWidget<UUserWidget>(
        PC,
        SuccessWidgetClass
    );

    if (!SuccessWidget)
    {
        return;
    }

    bCompleted = true;

    Player->GetCharacterMovement()->StopMovementImmediately();
    Player->GetCharacterMovement()->DisableMovement();

    SuccessWidget->AddToViewport();

    // �Է��� ���� ȭ�鿡 �����Ѵ�.
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(SuccessWidget->TakeWidget());

    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;
}