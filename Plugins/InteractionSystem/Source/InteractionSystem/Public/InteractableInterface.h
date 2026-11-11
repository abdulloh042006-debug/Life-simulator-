// Copyright Dredd 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionSystemTypes.h"
#include "InteractableInterface.generated.h"

UINTERFACE(BlueprintType)
class INTERACTIONSYSTEM_API UInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Implement on any actor to make it interactable.
 * Only requires CanInteract + Interact — everything else is optional.
 */
class INTERACTIONSYSTEM_API IInteractableInterface
{
    GENERATED_BODY()

public:
    /** Can this object be interacted with right now? */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    bool CanInteract(AActor* Interactor) const;

    /** Execute the interaction. Called by InteractionComponent. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AActor* Interactor);

    /** Override to supply custom prompt text (e.g. "Open Door" vs "Pick Up Sword"). */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FText GetInteractionPrompt(AActor* Interactor) const;

    /** What type of interaction is this? (affects prompt icon/sound). */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    EInteractionType GetInteractionType() const;

    /** Press, Hold, or Toggle? Default: Press. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    EInteractionTrigger GetInteractionTrigger() const;

    /** How long to hold? Only relevant for Hold trigger. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    float GetHoldDuration() const;

    /** Override for custom highlight behavior. Called when looked at. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnLookStart(AActor* Interactor);

    /** Override to remove highlight when looked away. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnLookEnd(AActor* Interactor);
};
