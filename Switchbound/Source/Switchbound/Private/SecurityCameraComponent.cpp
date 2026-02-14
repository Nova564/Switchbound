#include "SecurityCameraComponent.h"
#include "ReceiverComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

USecurityCameraComponent::USecurityCameraComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    ReceiverComponent = nullptr;
}

void USecurityCameraComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        ReceiverComponent = Owner->FindComponentByClass<UReceiverComponent>();

        if (ReceiverComponent)
        {
            ReceiverComponent->OnActivated.AddDynamic(this, &USecurityCameraComponent::OnCameraToggled);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(DetectionTimerHandle, this, &USecurityCameraComponent::PerformDetection, ScanInterval, true);
}

void USecurityCameraComponent::OnCameraToggled(AActor* Activator)
{
    SetCameraEnabled(!bIsCameraActive);
}

void USecurityCameraComponent::SetCameraEnabled(bool bEnabled)
{
    bIsCameraActive = bEnabled;
}

void USecurityCameraComponent::PerformDetection()
{
    if (!bIsCameraActive) {
        OnSecurityAlert.Broadcast(false);
        return;
    }

    bool bAnyRobotDetected = false;
    AActor* Owner = GetOwner();
    if (!Owner) return;

    TArray<AActor*> FoundRobots;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Robot"), FoundRobots);

    for (AActor* Robot : FoundRobots)
    {
        if (!Robot || Robot->ActorHasTag(TEXT("IsInvisible"))) continue;

        float Distance = FVector::Dist(Owner->GetActorLocation(), Robot->GetActorLocation());
        if (Distance > DetectionRange) continue;

        FVector DirectionToRobot = (Robot->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();
        float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Owner->GetActorForwardVector(), DirectionToRobot)));

        if (Angle <= DetectionAngle && IsActorVisible(Robot))
        {
            bAnyRobotDetected = true;
            DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Robot->GetActorLocation(), FColor::Red, false, 0.1f, 0, 2.0f);
            break;
        }
    }

    OnSecurityAlert.Broadcast(bAnyRobotDetected);
}
bool USecurityCameraComponent::IsActorVisible(AActor* TargetActor)
{
    AActor* Owner = GetOwner();
    if (!Owner) return false;

    FHitResult Hit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    QueryParams.AddIgnoredActor(TargetActor);

    FVector Start = Owner->GetActorLocation();
    FVector End = TargetActor->GetActorLocation();

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);

    return !bHit;
}