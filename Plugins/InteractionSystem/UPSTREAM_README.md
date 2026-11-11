# InteractionSystem — UE5 Interaction Plugin

[![UE5](https://img.shields.io/badge/UE-5.4--5.8-blue)](https://www.unrealengine.com)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![Platforms](https://img.shields.io/badge/platforms-Win64%20%7C%20Linux%20%7C%20Mac%20%7C%20Android-lightgrey)]()

Complete interaction system for Unreal Engine 5. Raycast-based object interaction with highlights, contextual prompts, hold-to-interact, and a clean interface pattern. Works with any actor — doors, pickups, NPCs, switches, terminals.

## Why InteractionSystem
Every game needs interactions, and every dev rewrites the same raycast → prompt → execute loop. This plugin gives you a battle-tested, interface-driven system that handles look-detection, highlight callbacks, press/hold/toggle triggers, and debug visualization — so you can focus on what your interactables actually *do*.

## Features
- **Raycast detection** — configurable range, radius, collision channel, line-of-sight
- **Interface-driven** — implement `IInteractableInterface` on any actor
- **Three trigger modes** — Press (instant), Hold (timed), Toggle (on/off)
- **Contextual prompts** — each interactable returns its own prompt text + type
- **Highlight callbacks** — `OnLookStart` / `OnLookEnd` for outline/glow effects
- **Delegates** — `OnLookTargetChanged`, `OnInteractionStarted`, `OnInteractionCompleted`
- **Hold progress** — queryable for UI progress bars
- **Debug visualization** — toggle `bDrawDebug` for sphere + line traces
- **Blueprint-ready** — everything BlueprintCallable + BlueprintNativeEvent
- **UE5.8 compiled & verified** — builds on Clang 20.1.8, zero warnings

## Quick Start
```cpp
// 1. Add UInteractionComponent to your character
// 2. Make any actor interactable:

// MyDoor.h
class AMyDoor : public AActor, public IInteractableInterface
{
    GENERATED_BODY()
public:
    virtual bool CanInteract_Implementation(AActor* Interactor) const override { return true; }
    virtual void Interact_Implementation(AActor* Interactor) override { OpenDoor(); }
    virtual FText GetInteractionPrompt_Implementation(AActor* Interactor) const override
        { return FText::FromString("Open Door"); }
    virtual EInteractionType GetInteractionType_Implementation() const override
        { return EInteractionType::Open; }
};

// 3. Player presses interact key → call TryInteract()
InteractionComponent->TryInteract();
```

## Installation
1. Copy `Plugins/InteractionSystem/` to your project's `Plugins/` folder
2. Add `"InteractionSystem"` to your `.uproject` Plugins list
3. Build — that's it

## API Reference

### UInteractionComponent
| Function | Description |
|---|---|
| `TryInteract()` | Execute interaction on current look target |
| `ScanForInteractables()` | Force re-scan for nearest interactable |
| `GetCurrentLookTarget()` | Actor currently being looked at |
| `HasValidTarget()` | Is there a valid interactable in view? |
| `GetTargetPrompt()` | Prompt text from current target |
| `GetHoldProgress()` | 0.0–1.0 hold progress (for UI bars) |

### IInteractableInterface
| Function | Description |
|---|---|
| `CanInteract(Interactor)` | Can this be interacted with now? |
| `Interact(Interactor)` | Execute the interaction |
| `GetInteractionPrompt(Interactor)` | What prompt to show? ("Open Door", "Pick Up Sword") |
| `GetInteractionType()` | Generic, Pickup, Use, Talk, Open, Examine |
| `GetInteractionTrigger()` | Press, Hold, or Toggle |
| `GetHoldDuration()` | How long to hold (seconds) |
| `OnLookStart(Interactor)` | Called when looked at (add highlight) |
| `OnLookEnd(Interactor)` | Called when looked away (remove highlight) |

### Configuration
| Property | Default | Description |
|---|---|---|
| `InteractionRange` | 300 | Max distance in UE units |
| `InteractionRadius` | 10 | Sphere sweep radius |
| `InteractionChannel` | Visibility | Collision channel |
| `bRequiresLineOfSight` | true | Use complex trace |
| `bContinuousLookCheck` | false | Re-scan every tick |
| `bDrawDebug` | false | Show debug sphere + line |

## Requirements
- Unreal Engine 5.4–5.8
- C++20
- No third-party dependencies

## License
MIT — use it in any project, commercial or personal. No strings attached.
