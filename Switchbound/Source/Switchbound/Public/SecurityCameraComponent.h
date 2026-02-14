#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SecurityCameraComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecurityAlert, bool, bIsDetected);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SWITCHBOUND_API USecurityCameraComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USecurityCameraComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
    float DetectionRange = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
    float DetectionAngle = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
    float ScanInterval = 0.2f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UReceiverComponent* ReceiverComponent;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera")
    bool GetIsCameraActive() const { return bIsCameraActive; }

    UPROPERTY(BlueprintAssignable, Category = "Camera")
    FOnSecurityAlert OnSecurityAlert;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
    bool bIsCameraActive = true;

    FTimerHandle DetectionTimerHandle;

    UFUNCTION()
    void PerformDetection();

    UFUNCTION()
    void OnCameraToggled(AActor* Activator);

    bool IsActorVisible(AActor* TargetActor);
};