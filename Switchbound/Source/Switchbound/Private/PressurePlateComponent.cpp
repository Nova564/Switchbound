#include "PressurePlateComponent.h"
#include "ReceiverComponent.h"
#include "Components/StaticMeshComponent.h"

UPressurePlateComponent::UPressurePlateComponent() {
    OnComponentBeginOverlap.AddDynamic(this, &UPressurePlateComponent::OnOverlapBegin);
    OnComponentEndOverlap.AddDynamic(this, &UPressurePlateComponent::OnOverlapEnd);
}

void UPressurePlateComponent::BeginPlay() {
    Super::BeginPlay();
    UpdateVisuals();
}

bool UPressurePlateComponent::IsValidActivator(AActor* Actor) {
    if (!Actor) return false;

    if (RequiredTag != NAME_None && !Actor->ActorHasTag(RequiredTag)) return false;

    if (AllowedRobotType == ERobotType::Both) return true;
    if (AllowedRobotType == ERobotType::SmallRobot && Actor->ActorHasTag("SmallRobot")) return true;
    if (AllowedRobotType == ERobotType::BigRobot && Actor->ActorHasTag("BigRobot")) return true;

    return (RequiredTag != NAME_None); 
}

void UPressurePlateComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (IsValidActivator(OtherActor)) {
        OverlappingCount++;
        bIsLocallyOccupied = true;
        EvaluateState();
    }
}

void UPressurePlateComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (IsValidActivator(OtherActor)) {
        OverlappingCount = FMath::Max(0, OverlappingCount - 1);

        if (bOneTimeActivate && bIsLockedActive) {
            return;
        }

        if (OverlappingCount == 0 && !bIsSingleUse) {
            bIsLocallyOccupied = false;
        }
        EvaluateState();
    }
}

void UPressurePlateComponent::EvaluateState() {
    if (bOneTimeActivate && bIsLockedActive) {
        return;
    }

    EPlateState NewState = EPlateState::Inactive;

    if (bIsLocallyOccupied) {
        bool bOthersAreActive = true;
        for (AActor* PlateActor : LinkedPlates) {
            if (PlateActor) {
                UPressurePlateComponent* Other = PlateActor->FindComponentByClass<UPressurePlateComponent>();
                if (Other && !Other->IsLocallyOccupied()) {
                    bOthersAreActive = false;
                    break;
                }
            }
        }
        NewState = bOthersAreActive ? EPlateState::Full : EPlateState::Partial;
    }

    if (CurrentState != NewState) {
        CurrentState = NewState;
        UpdateVisuals();

        bool bIsFull = (CurrentState == EPlateState::Full);

        if (bIsFull && bOneTimeActivate && !bIsLockedActive) {
            bIsLockedActive = true;
        }

        if (bIsFull != bWasFullyActivated) {
            bWasFullyActivated = bIsFull;

            TriggerChain(bIsFull);

            for (AActor* PlateActor : LinkedPlates) {
                if (PlateActor) {
                    UPressurePlateComponent* Other = PlateActor->FindComponentByClass<UPressurePlateComponent>();
                    if (Other) {
                        if (Other->bWasFullyActivated != bIsFull) {
                            Other->bWasFullyActivated = bIsFull;
                            if (Other->TargetActors.Num() > 0) {
                                Other->TriggerChain(bIsFull);
                            }
                        }
                    }
                }
            }
        }

        for (AActor* PlateActor : LinkedPlates) {
            if (PlateActor && PlateActor != GetOwner()) {
                UPressurePlateComponent* Other = PlateActor->FindComponentByClass<UPressurePlateComponent>();
                if (Other) {
                    Other->EvaluateState();
                }
            }
        }
    }
}

void UPressurePlateComponent::UpdateVisuals() {
    FLinearColor Color = InactiveColor;
    if (CurrentState == EPlateState::Partial) Color = PartialColor;
    if (CurrentState == EPlateState::Full) Color = FullColor;

    UStaticMeshComponent* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    if (Mesh) Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Color));
}

void UPressurePlateComponent::TriggerChain(bool bActivate) {
    for (AActor* Target : TargetActors) {
        if (Target) {
            UReceiverComponent* Receiver = Target->FindComponentByClass<UReceiverComponent>();
            if (Receiver) Receiver->Trigger(GetOwner());
        }
    }
}