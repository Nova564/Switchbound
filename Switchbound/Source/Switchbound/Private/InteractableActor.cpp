#include "InteractableActor.h"
#include "InteractableComponent.h"
#include "Components/StaticMeshComponent.h"

AInteractableActor::AInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
}

void AInteractableActor::OnInteract_Implementation(AActor* Interactor)
{
    if (InteractableComponent)
    {
        InteractableComponent->Interact(Interactor);
    }
}

FText AInteractableActor::GetInteractText_Implementation() const
{
    return InteractableComponent ? InteractableComponent->GetDisplayText() : FText::GetEmpty();
}

bool AInteractableActor::CanInteract_Implementation(AActor* Interactor) const
{
    return InteractableComponent && InteractableComponent->CanInteract(Interactor);
}

ERobotType AInteractableActor::GetRequiredRobotType_Implementation() const
{
    return InteractableComponent ? InteractableComponent->RequiredRobotType : ERobotType::Both;
}

bool AInteractableActor::IsInteractionAvailable_Implementation() const
{
    return InteractableComponent && InteractableComponent->bIsEnabled;
}