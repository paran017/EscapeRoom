#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeExit.generated.h"

class UBoxComponent;
class UUserWidget;

UCLASS()
class ESCAPEROOM_API AEscapeExit : public AActor
{
    GENERATED_BODY()

public:
    AEscapeExit();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Exit")
    TObjectPtr<UBoxComponent> ExitBox;

    // 에디터에서 성공 화면 블루프린트를 지정한다.
    UPROPERTY(EditAnywhere, Category = "Exit")
    TSubclassOf<UUserWidget> SuccessWidgetClass;

    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> SuccessWidget;

    bool bCompleted = false;

    UFUNCTION()
    void OnExitEntered(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};