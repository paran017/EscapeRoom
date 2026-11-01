#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeDoor.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class AEscapeDoor : public AActor
{
    GENERATED_BODY()

public:
    AEscapeDoor();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    // 문이 회전하는 경첩 위치
    UPROPERTY(VisibleAnywhere, Category = "Door")
    TObjectPtr<USceneComponent> Hinge;

    // 화면에 보이는 문
    UPROPERTY(VisibleAnywhere, Category = "Door")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, Category = "Door")
    float OpenAngle = 90.0f;

    // 초당 회전 각도
    UPROPERTY(EditAnywhere, Category = "Door")
    float OpenSpeed = 120.0f;

    // 상호작용 거리: 언리얼 기본 단위는 cm
    UPROPERTY(EditAnywhere, Category = "Door")
    float InteractionDistance = 250.0f;

    bool bIsOpen = false;
    float CurrentAngle = 0.0f;
    FRotator ClosedRotation;
    void TryInteract();
    // 에디터에서 잠금 여부를 설정할 수 있다.
    UPROPERTY(EditAnywhere, Category = "Door")
    bool bIsLocked = true;
};