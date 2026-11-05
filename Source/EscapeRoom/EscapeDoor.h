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
    // ���� ȸ���ϴ� ��ø ��ġ
    UPROPERTY(VisibleAnywhere, Category = "Door")
    TObjectPtr<USceneComponent> Hinge;

    // ȭ�鿡 ���̴� ��
    UPROPERTY(VisibleAnywhere, Category = "Door")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, Category = "Door")
    float OpenAngle = 90.0f;

    // �ʴ� ȸ�� ����
    UPROPERTY(EditAnywhere, Category = "Door")
    float OpenSpeed = 120.0f;

    // ��ȣ�ۿ� �Ÿ�: �𸮾� �⺻ ������ cm
    UPROPERTY(EditAnywhere, Category = "Door")
    float InteractionDistance = 250.0f;

    bool bIsOpen = false;
    float CurrentAngle = 0.0f;
    FRotator ClosedRotation;
    void TryInteract();
    // �����Ϳ��� ��� ���θ� ������ �� �ִ�.
    UPROPERTY(EditAnywhere, Category = "Door")
    bool bIsLocked = true;
};