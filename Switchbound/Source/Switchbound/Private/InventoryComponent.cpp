#include "InventoryComponent.h"
#include "ItemComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    Slots.SetNum(MaxSlots);
}

bool UInventoryComponent::AddItem(UItemComponent* ItemToAdd)
{
    if (!ItemToAdd || IsFull()) return false;

    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (Slots[i].IsEmpty())
        {
            UItemComponent* NewItem = DuplicateObject<UItemComponent>(ItemToAdd, GetOwner());
            NewItem->RegisterComponent();

            Slots[i].Item = NewItem;
            NewItem->Equip(GetOwner());

            OnInventoryUpdated.Broadcast();
            return true;
        }
    }

    return false;
}

void UInventoryComponent::UseItem(int32 SlotIndex)
{
    if (!Slots.IsValidIndex(SlotIndex) || Slots[SlotIndex].IsEmpty()) return;

    UItemComponent* Item = Slots[SlotIndex].Item;
    if (Item && Item->CanUse())
    {
        Item->Use(GetOwner());
    }
}

void UInventoryComponent::RemoveItem(int32 SlotIndex)
{
    if (!Slots.IsValidIndex(SlotIndex) || Slots[SlotIndex].IsEmpty()) return;

    UItemComponent* Item = Slots[SlotIndex].Item;
    if (Item)
    {
        Item->Unequip(GetOwner());
        Item->DestroyComponent();
    }

    Slots[SlotIndex].Item = nullptr;
    OnInventoryUpdated.Broadcast();
}

UItemComponent* UInventoryComponent::GetItemAt(int32 SlotIndex) const
{
    if (!Slots.IsValidIndex(SlotIndex)) return nullptr;
    return Slots[SlotIndex].Item;
}

bool UInventoryComponent::IsFull() const
{
    for (const FInventorySlot& Slot : Slots)
    {
        if (Slot.IsEmpty()) return false;
    }
    return true;
}