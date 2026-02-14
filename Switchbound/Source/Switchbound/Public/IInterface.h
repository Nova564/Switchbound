#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInterface.generated.h"

UENUM(BlueprintType)
enum class ERobotType : uint8
{
    SmallRobot UMETA(DisplayName = "Small Robot"),
    BigRobot UMETA(DisplayName = "Big Robot"),
    Both UMETA(DisplayName = "Both Robots")
};

UINTERFACE(MinimalAPI, Blueprintable)
class UIInterface : public UInterface
{
    GENERATED_BODY()
};

class SWITCHBOUND_API IIInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    void OnInteract(AActor* Interactor);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    FText GetInteractText() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    bool CanInteract(AActor* Interactor) const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    ERobotType GetRequiredRobotType() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    bool IsInteractionAvailable() const;
};