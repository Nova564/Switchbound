#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    UItemComponent* Item = nullptr;

    bool IsEmpty() const { return Item == nullptr; }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SWITCHBOUND_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    int32 MaxSlots = 4;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventorySlot> Slots;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(UItemComponent* ItemToAdd);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UseItem(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveItem(int32 SlotIndex);

    UFUNCTION(BlueprintPure, Category = "Inventory")
    UItemComponent* GetItemAt(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool IsFull() const;

protected:
    virtual void BeginPlay() override;
};