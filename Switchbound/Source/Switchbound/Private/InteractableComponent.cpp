#include "InteractableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

UInteractableComponent::UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UInteractableComponent::CanInteract(AActor* Interactor) const
{
    if (!bIsEnabled || !Interactor)
    {
        return false;
    }

    if (!bIsReusable && RemainingUses <= 0)
    {
        return false;
    }

    if (bIsOnCooldown)
    {
        return false;
    }

    if (!CheckRobotType(Interactor))
    {
        return false;
    }

    if (!CheckDistance(Interactor))
    {
        return false;
    }

    if (bRequiresLineOfSight && !CheckLineOfSight(Interactor))
    {
        return false;
    }

    if (RequiredActorTag != NAME_None && !Interactor->ActorHasTag(RequiredActorTag))
    {
        return false;
    }

    return CanInteractCustom(Interactor);
}

void UInteractableComponent::Interact(AActor* Interactor)
{
    if (!CanInteract(Interactor))
    {
        return;
    }

    if (CooldownDuration > 0.0f)
    {
        bIsOnCooldown = true;
        GetWorld()->GetTimerManager().SetTimer(
            CooldownTimerHandle,
            this,
            &UInteractableComponent::OnCooldownComplete,
            CooldownDuration,
            false
        );
    }

    if (!bIsReusable)
    {
        RemainingUses--;
        if (RemainingUses <= 0)
        {
            bIsEnabled = false;
        }
    }
    for (AActor* Target : TargetActors)
    {
        if (Target)
        {
            UReceiverComponent* Receiver = Target->FindComponentByClass<UReceiverComponent>();
            if (Receiver)
            {
                Receiver->Trigger(Interactor);
            }
        }
    }

    ApplyFeedback();
    OnInteracted.Broadcast(Interactor);
    OnInteractCustom(Interactor);
}

void UInteractableComponent::OnCooldownComplete()
{
    bIsOnCooldown = false;
}

bool UInteractableComponent::CheckRobotType(AActor* Interactor) const
{
    if (RequiredRobotType == ERobotType::Both)
    {
        return true;
    }

    if (RequiredRobotType == ERobotType::SmallRobot)
    {
        return Interactor->ActorHasTag("SmallRobot");
    }

    if (RequiredRobotType == ERobotType::BigRobot)
    {
        return Interactor->ActorHasTag("BigRobot");
    }

    return false;
}

bool UInteractableComponent::CheckDistance(AActor* Interactor) const
{
    if (InteractionDistance <= 0.0f)
    {
        return true;
    }

    float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Interactor->GetActorLocation());
    return Distance <= InteractionDistance;
}

bool UInteractableComponent::CheckLineOfSight(AActor* Interactor) const
{
    FHitResult HitResult;
    FVector Start = Interactor->GetActorLocation();
    FVector End = GetOwner()->GetActorLocation();

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Interactor);
    QueryParams.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

    return !bHit;
}

void UInteractableComponent::ApplyFeedback()
{
    if (InteractionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractionSound, GetOwner()->GetActorLocation());
    }

    if (InteractionParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InteractionParticles, GetOwner()->GetActorLocation());
    }
}

void UInteractableComponent::SetEnabled(bool bEnabled)
{
    bIsEnabled = bEnabled;
}

void UInteractableComponent::ResetUses()
{
    RemainingUses = bIsReusable ? -1 : 1;
    bIsEnabled = true;
    bIsOnCooldown = false;

    if (CooldownTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
    }
}

FText UInteractableComponent::GetDisplayText() const
{
    return bIsEnabled ? InteractionText : CannotInteractText;
}

void UInteractableComponent::SetHighlighted(bool bHighlight)
{
    if (!bEnableVisualFeedback)
    {
        return;
    }

    TArray<UStaticMeshComponent*> MeshComponents;
    GetOwner()->GetComponents<UStaticMeshComponent>(MeshComponents);

    for (UStaticMeshComponent* Mesh : MeshComponents)
    {
        Mesh->SetRenderCustomDepth(bHighlight);
    }
}

bool UInteractableComponent::CanInteractCustom_Implementation(AActor* Interactor) const
{
    return true;
}

void UInteractableComponent::OnInteractCustom_Implementation(AActor* Interactor)
{
}