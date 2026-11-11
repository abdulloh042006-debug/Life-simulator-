// Copyright Dredd 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystemTypes.generated.h"

/** How the interaction is triggered. */
UENUM(BlueprintType)
enum class EInteractionTrigger : uint8
{
    Press       UMETA(DisplayName = "Press (Single)"),
    Hold        UMETA(DisplayName = "Hold (Timed)"),
    Toggle      UMETA(DisplayName = "Toggle (On/Off)")
};

/** What kind of interaction this is. */
UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    Generic     UMETA(DisplayName = "Generic"),
    Pickup      UMETA(DisplayName = "Pickup"),
    Use         UMETA(DisplayName = "Use (Lever/Switch)"),
    Talk        UMETA(DisplayName = "Talk (NPC)"),
    Open        UMETA(DisplayName = "Open (Door/Chest)"),
    Examine     UMETA(DisplayName = "Examine")
};

/** Delegate: interaction started. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionStarted, AActor*, Interactor, AActor*, Interactable);

/** Delegate: interaction completed. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionCompleted, AActor*, Interactor, AActor*, Interactable);

/** Delegate: look target changed (for prompts). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLookTargetChanged, AActor*, NewTarget);
