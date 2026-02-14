#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTriggerMessageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnTriggerMessage, FText, Message, FLinearColor, Color, int32, Size, bool, ShowBG, FLinearColor, BGColor, float, Duration, bool, Fade);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SWITCHBOUND_API UTriggerMessageComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Message")
    FText DisplayText;

    UPROPERTY(EditAnywhere, Category = "Message")
    FLinearColor TextColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, Category = "Message")
    int32 FontSize = 24;

    UPROPERTY(EditAnywhere, Category = "Box")
    bool bShowBackground = true;

    UPROPERTY(EditAnywhere, Category = "Box")
    FLinearColor BackgroundColor = FLinearColor(0, 0, 0, 0.5f);

    UPROPERTY(EditAnywhere, Category = "Logic")
    float DisplayDuration = 3.0f;

    UPROPERTY(EditAnywhere, Category = "Logic")
    bool bOneTimeOnly = true;

    UPROPERTY(EditAnywhere, Category = "Logic")
    bool bFadeOut = true;
    UPROPERTY(BlueprintAssignable, Category = "Logic|Events")
    FOnTriggerMessage OnTriggerMessage;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

private:
    bool bHasBeenTriggered = false;
};
