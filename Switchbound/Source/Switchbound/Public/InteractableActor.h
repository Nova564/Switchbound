#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInterface.h"
#include "InteractableActor.generated.h"

UCLASS(Blueprintable)
class SWITCHBOUND_API AInteractableActor : public AActor, public IIInterface
{
    GENERATED_BODY() 

public:
    AInteractableActor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UInteractableComponent* InteractableComponent;

public:
    virtual void OnInteract_Implementation(AActor* Interactor) override;
    virtual FText GetInteractText_Implementation() const override;
    virtual bool CanInteract_Implementation(AActor* Interactor) const override;
    virtual ERobotType GetRequiredRobotType_Implementation() const override;
    virtual bool IsInteractionAvailable_Implementation() const override;
};