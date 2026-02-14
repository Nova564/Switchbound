#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IInterface.h"
#include "ReceiverComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, Interactor);

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class SWITCHBOUND_API UInteractableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractableComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Requirements")
    ERobotType RequiredRobotType = ERobotType::Both;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|UI")
    FText InteractionText = FText::FromString("Press E to interact");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|State")
    bool bIsEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Behavior")
    bool bIsReusable = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Behavior", meta = (EditCondition = "!bIsReusable"))
    int32 RemainingUses = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Behavior")
    float CooldownDuration = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Requirements")
    float InteractionDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Requirements")
    bool bRequiresLineOfSight = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Requirements")
    FName RequiredActorTag = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|UI")
    FText CannotInteractText = FText::FromString("Cannot interact");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Feedback")
    bool bEnableVisualFeedback = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Feedback", meta = (EditCondition = "bEnableVisualFeedback"))
    FLinearColor HighlightColor = FLinearColor::Green;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Feedback")
    USoundBase* InteractionSound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Feedback")
    UParticleSystem* InteractionParticles = nullptr;

    UPROPERTY(BlueprintAssignable, Category = "Interaction|Events")
    FOnInteracted OnInteracted;

    UPROPERTY(EditInstanceOnly, Category = "Interaction|Chain")
    TArray<AActor*> TargetActors;

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    bool CanInteractCustom(AActor* Interactor) const;

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void OnInteractCustom(AActor* Interactor);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    bool CanInteract(AActor* Interactor) const;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact(AActor* Interactor);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ResetUses();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    FText GetDisplayText() const;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetHighlighted(bool bHighlight);

private:
    FTimerHandle CooldownTimerHandle;
    bool bIsOnCooldown = false;

    void OnCooldownComplete();
    bool CheckRobotType(AActor* Interactor) const;
    bool CheckDistance(AActor* Interactor) const;
    bool CheckLineOfSight(AActor* Interactor) const;
    void ApplyFeedback();
};