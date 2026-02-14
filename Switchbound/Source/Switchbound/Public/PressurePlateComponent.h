#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "IInterface.h"
#include "PressurePlateComponent.generated.h"

UENUM(BlueprintType)
enum class EPlateState : uint8 { Inactive, Partial, Full };

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SWITCHBOUND_API UPressurePlateComponent : public UBoxComponent
{
    GENERATED_BODY()

public:
    UPressurePlateComponent();

    UPROPERTY(EditAnywhere, Category = "Logic")
    bool bIsSingleUse = false;

    UPROPERTY(EditAnywhere, Category = "Logic")
    bool bOneTimeActivate = false;

    UPROPERTY(EditAnywhere, Category = "Logic")
    ERobotType AllowedRobotType = ERobotType::Both;

    UPROPERTY(EditAnywhere, Category = "Logic")
    FName RequiredTag = NAME_None; 

    UPROPERTY(EditInstanceOnly, Category = "Logic")
    TArray<AActor*> LinkedPlates;

    UPROPERTY(EditInstanceOnly, Category = "Logic")
    TArray<AActor*> TargetActors;

    UPROPERTY(EditAnywhere, Category = "Visuals")
    FLinearColor InactiveColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, Category = "Visuals")
    FLinearColor PartialColor = FLinearColor::Yellow;

    UPROPERTY(EditAnywhere, Category = "Visuals")
    FLinearColor FullColor = FLinearColor::Green;

    UFUNCTION(BlueprintCallable)
    bool IsLocallyOccupied() const { return bIsLocallyOccupied; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    void EvaluateState();
    void UpdateVisuals();
    void TriggerChain(bool bActivate);
    bool IsValidActivator(AActor* Actor);

    bool bIsLocallyOccupied = false; 
    bool bWasFullyActivated = false;
    bool bIsLockedActive = false; 
    EPlateState CurrentState = EPlateState::Inactive;
    int32 OverlappingCount = 0;
};