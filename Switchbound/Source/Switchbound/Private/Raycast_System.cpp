// Fill out your copyright notice in the Description page of Project Settings.

#include "Raycast_System.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ARaycast_System::ARaycast_System()
{
    PrimaryActorTick.bCanEverTick = false; // Desactive le Tick car on n'en a pas besoin
}

void ARaycast_System::BeginPlay()
{
    Super::BeginPlay();

    //timer
    GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, this, &ARaycast_System::CheckInteraction, 0.1f, true);
}

void ARaycast_System::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Plus de raycast automatique ici !
    // Le raycast se fera uniquement quand vous appellerez RaycastFromCamera() depuis Blueprint
}

bool ARaycast_System::RaycastFromCamera(FHitResult& OutHit)
{
    // Recuperer le controleur du joueur
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        return false;
    }

    // Obtenir la position et rotation de la camera
    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    // Calculer le point de fin du raycast
    FVector End = CameraLocation + (CameraRotation.Vector() * MaxDistance);

    // Effectuer le raycast
    return RaycastBetweenPoints(CameraLocation, End, OutHit);
}

TScriptInterface<IIInterface> ARaycast_System::GetAvailableInteractable(AActor* Interactor, FHitResult& OutHit)
{
    if (RaycastFromCamera(OutHit))
    {
        AActor* HitActor = OutHit.GetActor();
        if (HitActor && HitActor->Implements<UIInterface>())
        {
            if (IIInterface::Execute_CanInteract(HitActor, Interactor))
            {
                return HitActor;
            }
        }
    }
    return nullptr;
}

void ARaycast_System::CheckInteraction()
{
    FHitResult Hit;
    bool bHit = RaycastFromCamera(Hit);
    FText TextToDisplay = FText::GetEmpty(); 

    if (bHit && Hit.GetActor())
    {
        AActor* HitActor = Hit.GetActor();

        if (HitActor->Implements<UIInterface>())
        {
            APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

            if (IIInterface::Execute_CanInteract(HitActor, PlayerPawn))
            {
                CurrentInteractableActor = HitActor;
                TextToDisplay = IIInterface::Execute_GetInteractText(HitActor);
            }
            else { CurrentInteractableActor = nullptr; }
        }
        else { CurrentInteractableActor = nullptr; }
    }
    else { CurrentInteractableActor = nullptr; }

    OnUpdateInteractionText.Broadcast(TextToDisplay);
}

bool ARaycast_System::RaycastBetweenPoints(FVector Start, FVector End, FHitResult& OutHit)
{
    // Parametres de collision
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true;
    TraceParams.bReturnPhysicalMaterial = true;
    TraceParams.AddIgnoredActor(this);

    // Effectuer le raycast
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        OutHit,
        Start,
        End,
        ECC_Visibility,
        TraceParams
    );

    // Debug visuel
    if (bShowDebug)
    {
        DrawDebugRaycast(Start, End, bHit, OutHit);
    }

    return bHit;
}

void ARaycast_System::DrawDebugRaycast(FVector Start, FVector End, bool bHit, const FHitResult& Hit)
{
    // Calculer le point de fin pour le debug
    FVector DebugEnd = bHit ? FVector(Hit.ImpactPoint) : End;

    // Choisir la couleur
    FColor LineColor = bHit ? FColor::Green : FColor::Red;

    // Dessiner la ligne du raycast
    DrawDebugLine(
        GetWorld(),
        Start,
        DebugEnd,
        LineColor,
        false,
        DebugDuration,
        0,
        2.0f
    );

    if (bHit)
    {
        // Point d'impact
        DrawDebugPoint(
            GetWorld(),
            Hit.ImpactPoint,
            10.0f,
            FColor::Yellow,
            false,
            DebugDuration
        );

        // Normale de surface
        DrawDebugLine(
            GetWorld(),
            Hit.ImpactPoint,
            Hit.ImpactPoint + (Hit.ImpactNormal * 100.0f),
            FColor::Blue,
            false,
            DebugDuration,
            0,
            2.0f
        );
    }

}