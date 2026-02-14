#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IInterface.h"
#include "ItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUp, AActor*, PickedUpBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUsed, AActor*, UsedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownFinished);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class SWITCHBOUND_API UItemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UItemComponent();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Config")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Config")
    UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Config")
    FText InteractionMessage = FText::FromString("Appuyez pour ramasser");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Config")
    ERobotType RestrictedTo = ERobotType::Both;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Config")
    bool bDestroyOnPickup = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Behavior")
    bool bIsPassive = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Behavior", meta = (EditCondition = "!bIsPassive", ClampMin = "0.0"))
    float CooldownDuration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Tags")
    TArray<FName> TagsToAddOnEquip;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Tags")
    TArray<FName> TagsToRemoveOnUnequip;

    UPROPERTY(BlueprintAssignable, Category = "Item|Events")
    FOnItemPickedUp OnItemPickedUp;

    UPROPERTY(BlueprintAssignable, Category = "Item|Events")
    FOnItemUsed OnItemUsed;

    UPROPERTY(BlueprintAssignable, Category = "Item|Events")
    FOnCooldownFinished OnCooldownFinished;


    UFUNCTION(BlueprintCallable, Category = "Item")
    void PickUp(AActor* PickedUpBy);

    UFUNCTION(BlueprintCallable, Category = "Item")
    void Equip(AActor* EquippedBy);

    UFUNCTION(BlueprintCallable, Category = "Item")
    void Unequip(AActor* UnequippedBy);

    UFUNCTION(BlueprintCallable, Category = "Item")
    bool Use(AActor* UsedBy);

    UFUNCTION(BlueprintPure, Category = "Item")
    bool CanUse() const;

    UFUNCTION(BlueprintPure, Category = "Item")
    bool CanBeUsedBy(ERobotType RobotType) const;

    UFUNCTION(BlueprintPure, Category = "Item")
    float GetRemainingCooldown() const;

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintNativeEvent, Category = "Item|Overridable")
    void OnPickedUp(AActor* PickedUpBy);

    UFUNCTION(BlueprintNativeEvent, Category = "Item|Overridable")
    void OnEquipped(AActor* EquippedBy);

    UFUNCTION(BlueprintNativeEvent, Category = "Item|Overridable")
    void OnUnequipped(AActor* UnequippedBy);

    UFUNCTION(BlueprintNativeEvent, Category = "Item|Overridable")
    void OnUsed(AActor* UsedBy);

    UFUNCTION(BlueprintNativeEvent, Category = "Item|Overridable")
    void OnPassiveEffect(AActor* Owner, float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "Item")
    UTexture2D* GetItemIcon() const { return ItemIcon; }

    UFUNCTION(BlueprintPure, Category = "Item")
    FText GetItemName() const { return ItemName; }

private:
    UPROPERTY()
    AActor* CurrentOwner;

    float LastUseTime;
    FTimerHandle CooldownTimerHandle;

    void ApplyTags(AActor* Target, bool bAdd);
    void HandleCooldownFinished();

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};