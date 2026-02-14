// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReceiverComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivated, AActor*, Activator);

UCLASS(meta = (BlueprintSpawnableComponent))
class UReceiverComponent : public UActorComponent {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
    FOnActivated OnActivated; 

    UFUNCTION(BlueprintCallable)
    void Trigger(AActor* Activator) { OnActivated.Broadcast(Activator); }
};
