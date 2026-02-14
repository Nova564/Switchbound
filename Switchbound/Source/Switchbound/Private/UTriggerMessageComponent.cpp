#include "UTriggerMessageComponent.h"
#include "Components/ShapeComponent.h"

void UTriggerMessageComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        UPrimitiveComponent* Primitive = Owner->FindComponentByClass<UPrimitiveComponent>();
        if (Primitive)
        {
            Primitive->OnComponentBeginOverlap.AddDynamic(this, &UTriggerMessageComponent::OnOverlapBegin);
        }
    }
}

void UTriggerMessageComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bHasBeenTriggered && bOneTimeOnly) return;

    if (OtherActor && (OtherActor->ActorHasTag("SmallRobot") || OtherActor->ActorHasTag("BigRobot")))
    {
        bHasBeenTriggered = true;
        OnTriggerMessage.Broadcast(DisplayText, TextColor, FontSize, bShowBackground, BackgroundColor, DisplayDuration, bFadeOut);
    }
}