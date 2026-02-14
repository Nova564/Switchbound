// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInterface.h"
#include "Raycast_System.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateInteractionText, FText, NewText);

UCLASS()
class SWITCHBOUND_API ARaycast_System : public AActor
{
    GENERATED_BODY()

public:
    ARaycast_System();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Distance maximale du raycast
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float MaxDistance = 5000.0f;

    // Activer le debug visuel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    bool bShowDebug = true;

    // Duree d'affichage du debug
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    float DebugDuration = 2.0f;

    // Fonction principale de raycast depuis la camera
    UFUNCTION(BlueprintCallable, Category = "Raycast")
    bool RaycastFromCamera(FHitResult& OutHit);

    // Raycast personnalise entre deux points
    UFUNCTION(BlueprintCallable, Category = "Raycast")
    bool RaycastBetweenPoints(FVector Start, FVector End, FHitResult& OutHit);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    TScriptInterface<IIInterface> GetAvailableInteractable(AActor* Interactor, FHitResult& OutHit);

    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    AActor* CurrentInteractableActor;

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnUpdateInteractionText OnUpdateInteractionText;

private:
    // Fonction de debug visuel
    void DrawDebugRaycast(FVector Start, FVector End, bool bHit, const FHitResult& Hit);

    FTimerHandle InteractionTimerHandle;
    AActor* CurrentFocusedActor;


    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void CheckInteraction();
};