#include "ItemComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UItemComponent::UItemComponent()
{
    //c'est jamais appelé
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
    LastUseTime = -9999.f;
    CurrentOwner = nullptr;
}

void UItemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    //tick jamais appelé car aucun item n'est en passif le blueprint des items leur logiques dans les details on peut voir si ils sont passif ou pas
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (bIsPassive && CurrentOwner)
    {
        OnPassiveEffect(CurrentOwner, DeltaTime);
    }
}

void UItemComponent::PickUp(AActor* PickedUpBy)
{
    if (!PickedUpBy) return;

    OnPickedUp(PickedUpBy);
    OnItemPickedUp.Broadcast(PickedUpBy);

    if (bDestroyOnPickup && GetOwner())
    {
        GetOwner()->Destroy();
    }
}

void UItemComponent::Equip(AActor* EquippedBy)
{
    if (!EquippedBy) return;

    CurrentOwner = EquippedBy;

    ApplyTags(EquippedBy, true);

    if (bIsPassive)
    {
        SetComponentTickEnabled(true);
    }

    OnEquipped(EquippedBy);
}

void UItemComponent::Unequip(AActor* UnequippedBy)
{
    if (!UnequippedBy) return;

    ApplyTags(UnequippedBy, false);

    SetComponentTickEnabled(false);
    CurrentOwner = nullptr;

    OnUnequipped(UnequippedBy);
}

bool UItemComponent::Use(AActor* UsedBy)
{
    if (!UsedBy || bIsPassive || !CanUse()) return false;

    LastUseTime = GetWorld()->GetTimeSeconds();

    GetWorld()->GetTimerManager().SetTimer(
        CooldownTimerHandle,
        this,
        &UItemComponent::HandleCooldownFinished,
        CooldownDuration,
        false
    );

    OnUsed(UsedBy);
    OnItemUsed.Broadcast(UsedBy);

    return true;
}

bool UItemComponent::CanUse() const
{
    if (bIsPassive) return false;
    return GetRemainingCooldown() <= 0.f;
}

bool UItemComponent::CanBeUsedBy(ERobotType RobotType) const
{
    if (RestrictedTo == ERobotType::Both) return true;
    return RestrictedTo == RobotType;
}

float UItemComponent::GetRemainingCooldown() const
{
    if (bIsPassive) return 0.f;

    float TimeSinceUse = GetWorld()->GetTimeSeconds() - LastUseTime;
    return FMath::Max(0.f, CooldownDuration - TimeSinceUse);
}

void UItemComponent::ApplyTags(AActor* Target, bool bAdd)
{
    if (!Target) return;

    if (bAdd)
    {
        for (const FName& Tag : TagsToAddOnEquip)
        {
            if (!Target->Tags.Contains(Tag))
            {
                Target->Tags.Add(Tag);
            }
        }
    }
    else
    {
        for (const FName& Tag : TagsToRemoveOnUnequip)
        {
            Target->Tags.Remove(Tag);
        }
    }
}

void UItemComponent::HandleCooldownFinished()
{
    OnCooldownFinished.Broadcast();
}


void UItemComponent::OnPickedUp_Implementation(AActor* PickedUpBy)
{
    UE_LOG(LogTemp, Log, TEXT("Item %s picked up by %s"), *ItemName.ToString(), *PickedUpBy->GetName());
}

void UItemComponent::OnEquipped_Implementation(AActor* EquippedBy)
{

}

void UItemComponent::OnUnequipped_Implementation(AActor* UnequippedBy)
{

}

void UItemComponent::OnUsed_Implementation(AActor* UsedBy)
{

}

void UItemComponent::OnPassiveEffect_Implementation(AActor* Owner, float DeltaTime)
{

}