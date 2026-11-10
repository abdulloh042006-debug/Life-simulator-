# NPC Shop/Guild System Integration Guide for Unreal Engine 5

## 1. Introduction

This document provides a comprehensive guide for integrating the NPC Shop/Guild system into an Unreal Engine 5 project for the "Hayot Simulyatori" game. The system is designed to facilitate player-NPC interactions for buying, selling, and bartering items, incorporating dynamic pricing, player reputation, and trading skills. The core logic is implemented in C++ for performance and flexibility, with Unreal Motion Graphics (UMG) and Blueprints for user interface and game-specific customizations.

## 2. Project Setup

To integrate this system, you will need to add the provided C++ source files to your Unreal Engine 5 project. Follow these steps:

1.  **Create C++ Project:** Ensure your Unreal Engine project is a C++ project. If it's a Blueprint-only project, you can add a new C++ class (e.g., `None`) from the Unreal Editor to convert it.
2.  **Copy Source Files:** Place the provided C++ header (`.h`) and source (`.cpp`) files into the `Source/YourProjectName/` directory of your Unreal Engine project, maintaining the directory structure as follows:

    ```
    YourProjectName/
    ├── Source/
    │   └── YourProjectName/
    │       ├── Public/
    │       │   ├── Characters/
    │       │   │   ├── NPCBase.h
    │       │   │   └── PlayerCharacter.h
    │       │   ├── Components/
    │       │   │   ├── CurrencyComponent.h
    │       │   │   ├── InventoryComponent.h
    │       │   │   ├── NPCInteractionComponent.h
    │       │   │   ├── ReputationComponent.h
    │       │   │   ├── SkillComponent.h
    │       │   │   └── TradeComponent.h
    │       │   └── Data/
    │       │       └── HayotSimulyatoriDataTypes.h
    │       └── Private/
    │           ├── Characters/
    │           │   ├── NPCBase.cpp
    │           │   └── PlayerCharacter.cpp
    │           ├── Components/
    │           │   ├── CurrencyComponent.cpp
    │           │   ├── InventoryComponent.cpp
    │           │   ├── NPCInteractionComponent.cpp
    │           │   ├── ReputationComponent.cpp
    │           │   ├── SkillComponent.cpp
    │           │   └── TradeComponent.cpp
    │           └── Data/
    │               └── HayotSimulyatoriDataTypes.cpp
    └── ... (other project files)
    ```

3.  **Regenerate Project Files:** Right-click on your `.uproject` file and select "Generate Visual Studio project files" (or your IDE's equivalent). This ensures the new C++ files are recognized by the build system.
4.  **Build Project:** Open your project in Visual Studio (or your IDE) and build the solution. Resolve any compilation errors that may arise.

## 3. Core Data Types and Item System

The `HayotSimulyatoriDataTypes.h` file defines essential enumerations and structures, including the base `UItem` class for all in-game items.

### `HayotSimulyatoriDataTypes.h`

This header contains:

*   **`EItemType` Enum:** Categorizes items (e.g., `Resource`, `Weapon`, `Armor`, `Consumable`).
*   **`EResourceType` Enum:** Specifies types of resources (e.g., `Stone`, `Wood`, `Meat`).
*   **`ECurrencyType` Enum:** Defines currency types (`Copper`, `Silver`, `Gold`).
*   **`FInventorySlot` Struct:** Represents an item and its quantity within an inventory.
*   **`FTradeItemData` Struct:** Stores data for dynamic pricing, including `CurrentStock`, `SupplyFactor`, and `DemandFactor`.
*   **`UItem` Class:** The base class for all items. Extend this class in C++ or Blueprints to create specific item definitions. Key properties include `ItemID`, `DisplayName`, `Weight`, `BaseValue`, `MaxStackSize`, `ItemType`, and `ResourceType`.

**Example: Extending `UItem` in C++**

To create a new item type, for instance, a `UWeaponItem`, you would create a new C++ class inheriting from `UItem`:

```cpp
// WeaponItem.h
#pragma once
#include "CoreMinimal.h"
#include "Data/HayotSimulyatoriDataTypes.h"
#include "WeaponItem.generated.h"

UCLASS()
class HAYOTSIMULYATORI_API UWeaponItem : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackSpeed;
};
```

## 4. Core Components

The system relies on several `UActorComponent` classes that can be attached to `AActor` instances (e.g., `APlayerCharacter`, `ANPCBase`).

### 4.1. `UInventoryComponent`

Manages a collection of `UItem` instances. Attach this to any actor that needs an inventory.

**Key Properties:**

*   `MaxWeightCapacity`: Maximum total weight the inventory can hold.
*   `CurrentWeight`: Current total weight of items.
*   `InventorySlots`: Array of `FInventorySlot`.
*   `OnInventoryUpdated`: Delegate broadcast when inventory changes.

**Key Functions:**

*   `AddItem(TSubclassOf<UItem> ItemClass, int32 Quantity)`: Adds items, handling stacking and weight limits.
*   `RemoveItem(TSubclassOf<UItem> ItemClass, int32 Quantity)`: Removes items.
*   `HasItem(TSubclassOf<UItem> ItemClass, int32 Quantity)`: Checks for item presence.
*   `GetItemQuantity(TSubclassOf<UItem> ItemClass)`: Returns current quantity of an item.
*   `GetTotalWeight()`: Returns current total weight.

### 4.2. `UCurrencyComponent`

Manages an actor's monetary holdings and currency conversions. Attach this to any actor that needs to hold currency.

**Key Properties:**

*   `CopperCoins`, `SilverCoins`, `GoldCoins`: Quantities of each coin type.
*   `CoinWeightPerUnit`: Weight of a single copper coin (multiplied for other types).
*   `OnCurrencyUpdated`: Delegate broadcast when currency changes.

**Key Functions:**

*   `AddCurrency(ECurrencyType Type, int32 Amount)`: Adds currency.
*   `RemoveCurrency(ECurrencyType Type, int32 Amount)`: Removes currency.
*   `GetCurrencyAmount(ECurrencyType Type)`: Returns amount of a specific currency.
*   `GetTotalWeight()`: Returns total weight of all coins.
*   `ConvertCurrency(ECurrencyType FromType, ECurrencyType ToType, int32 Amount)`: Converts currency between types.

### 4.3. `UReputationComponent`

Tracks an actor's reputation with various NPCs or factions. Typically attached to the player character.

**Key Properties:**

*   `NPCReputation`: Map of NPC ID (`FName`) to reputation value (`float`).
*   `OnReputationChanged`: Delegate broadcast when reputation changes.

**Key Functions:**

*   `AdjustReputation(FName NPCID, float Amount)`: Modifies reputation with a specific NPC.
*   `GetReputation(FName NPCID)`: Returns current reputation.
*   `GetReputationTradeBonus(FName NPCID)`: Calculates a trade bonus/penalty based on reputation.

### 4.4. `USkillComponent`

Manages player skills, specifically the trading skill for this system. Typically attached to the player character.

**Key Properties:**

*   `TradingSkillXP`: Current experience points for trading skill.
*   `TradingSkillLevel`: Current level of trading skill.
*   `OnTradingSkillUpdated`: Delegate broadcast when trading skill changes.

**Key Functions:**

*   `AddTradingSkillXP(int32 Amount)`: Adds XP and handles level-ups.
*   `GetTradingSkillBonus()`: Returns a trade bonus/discount based on skill level.

### 4.5. `UTradeComponent`

Encapsulates the core trading logic for NPCs. Attach this to any NPC that acts as a trader.

**Key Properties:**

*   `AvailableItemsForSale`: Items the NPC sells, with `FTradeItemData` for dynamic pricing.
*   `DesiredItemsForPurchase`: Items the NPC buys, with `FTradeItemData` for dynamic pricing.
*   `OnTradeInitiated`, `OnTradeCompleted`: Delegates for trade events.

**Key Functions:**

*   `InitiateTrade(APlayerCharacter* Player)`: Starts a trade session.
*   `CalculateSellPrice(TSubclassOf<UItem> ItemClass, APlayerCharacter* Player)`: Determines the price for selling an item to the player.
*   `CalculateBuyPrice(TSubclassOf<UItem> ItemClass, APlayerCharacter* Player)`: Determines the price for buying an item from the player.
*   `ExecuteTrade(...)`: Handles the transfer of items and currency during a trade.
*   `UpdateSupplyAndDemand(TSubclassOf<UItem> ItemClass, int32 Quantity, bool bSoldByPlayer)`: Adjusts dynamic pricing factors after a trade.

### 4.6. `UNPCInteractionComponent`

Handles player interaction with NPCs. Attach this to any NPC that players can interact with.

**Key Properties:**

*   `InteractionRadius`: The distance within which a player can interact.
*   `OnPlayerInteract`: Delegate broadcast when a player interacts.

**Key Functions:**

*   `Interact(APlayerCharacter* InteractingPlayer)`: Triggers interaction logic.
*   `IsPlayerInInteractionRange(APlayerCharacter* Player)`: Checks if a player is within range.

## 5. Player and NPC Integration

### 5.1. `APlayerCharacter`

The `APlayerCharacter` class is a basic character class that includes instances of the player-specific components.

**Integration Steps:**

1.  **Inherit from `APlayerCharacter`:** Your main player character Blueprint should inherit from `APlayerCharacter` (or a C++ class that inherits from it).
2.  **Component Access:** The `InventoryComponent`, `CurrencyComponent`, `ReputationComponent`, and `SkillComponent` are created as `UDefaultSubobject` in the constructor, making them accessible in Blueprints.

### 5.2. `ANPCBase`

The `ANPCBase` class is a basic character class for NPCs, including the necessary components for trading and interaction.

**Integration Steps:**

1.  **Inherit from `ANPCBase`:** Create Blueprint classes for your trading NPCs (e.g., `BP_NPC_Trader`) that inherit from `ANPCBase`.
2.  **Component Access:** The `InventoryComponent`, `CurrencyComponent`, `TradeComponent`, and `InteractionComponent` are created as `UDefaultSubobject` in the constructor, making them accessible in Blueprints.
3.  **Configure `UTradeComponent`:** In your `BP_NPC_Trader` Blueprint, select the `TradeComponent`. You can then configure `AvailableItemsForSale` and `DesiredItemsForPurchase` in the Details panel. This is where you define what the NPC sells and buys, along with initial stock, supply, and demand factors.

## 6. UI Design and Interaction Logic (Blueprints/UMG)

The user interface for trading will be built using Unreal Motion Graphics (UMG) widgets, with interaction logic primarily handled in Blueprints.

### 6.1. `WBP_TradeUI` (UMG Widget)

This will be the main trading interface displayed to the player.

**Key Elements:**

*   **Player Inventory Panel:** Displays the player's items and quantities.
*   **NPC Inventory Panel:** Displays the NPC's items and quantities.
*   **Player Currency Display:** Shows player's Copper, Silver, Gold.
*   **NPC Currency Display:** Shows NPC's Copper, Silver, Gold.
*   **Trade Offer Panels:** Areas where players drag and drop items/currency they wish to offer and receive.
*   **Price Summary:** Dynamically updates to show the total cost/gain of the proposed trade.
*   **Confirm/Cancel Buttons:** Buttons to execute or cancel the trade.

**Interaction Flow:**

1.  **Player Interaction:** When the player interacts with an `ANPCBase` (e.g., `BP_NPC_Trader`), the `UNPCInteractionComponent`'s `OnPlayerInteract` delegate is triggered.
2.  **UI Creation:** The `BP_NPC_Trader` Blueprint (or a dedicated Game Mode/Player Controller) should create an instance of `WBP_TradeUI` and add it to the viewport.
3.  **Initialization:** The `WBP_TradeUI` should have an `InitializeTradeUI` function (Blueprint callable) that takes references to the `APlayerCharacter` and the NPC's `UTradeComponent`, `UInventoryComponent`, and `UCurrencyComponent`.
4.  **Display Data:** `InitializeTradeUI` will then call functions to populate the UI elements with current inventory and currency data from both the player and NPC.
5.  **Dynamic Pricing:** As the player adds or removes items from their trade offer, the `WBP_TradeUI` will call `UTradeComponent::CalculateSellPrice` and `UTradeComponent::CalculateBuyPrice` to update the price summary in real-time.
6.  **Trade Execution:** When the player clicks "Confirm Trade," the `WBP_TradeUI` will gather the proposed items and currency from both sides and call `UTradeComponent::ExecuteTrade` on the NPC's `UTradeComponent`.
7.  **UI Updates:** Upon successful trade, the `OnInventoryUpdated` and `OnCurrencyUpdated` delegates (from `UInventoryComponent` and `UCurrencyComponent`) will trigger the `WBP_TradeUI` to refresh its displays, reflecting the new states.

### 6.2. `WBP_InventorySlot` (UMG Widget)

This is a reusable widget to display individual item slots within the inventory panels of `WBP_TradeUI`. It should display the item's icon, name, and quantity. It can also implement drag-and-drop functionality for moving items between inventories or into trade offer slots.

## 7. Dynamic Pricing Configuration

The dynamic pricing system is driven by the `SupplyFactor` and `DemandFactor` within the `FTradeItemData` struct, which is used in the `AvailableItemsForSale` and `DesiredItemsForPurchase` maps of the `UTradeComponent`.

*   **Initial Values:** You can set initial `SupplyFactor` and `DemandFactor` values for each item in the `BP_NPC_Trader` Blueprint's `TradeComponent` details panel.
*   **Runtime Adjustment:** The `UTradeComponent::UpdateSupplyAndDemand` function automatically adjusts these factors based on trade activity. For example, if a player sells a lot of wood to an NPC, the NPC's `SupplyFactor` for wood will increase, and its `DemandFactor` will decrease, leading to lower buy prices for the player and potentially higher sell prices from the NPC.
*   **Tuning:** The `0.01f` and `0.005f` multipliers in `UpdateSupplyAndDemand` (and the `0.1f` weighting in `GetSupplyDemandPriceAdjustment`) are example values. These should be tuned during gameplay testing to achieve the desired economic balance.

## 8. Barter System Considerations

The `UTradeComponent::ExecuteTrade` function is designed to handle both currency-based and barter trades. For a pure barter trade, the `PlayerCurrencyOffers` and `NPCCurrencyOffers` maps would simply be empty. The core logic remains the same: items are exchanged if both parties have them and the transaction is valid.

**UI for Barter:** The `WBP_TradeUI` would need specific visual elements to clearly differentiate between items being offered for sale/purchase and items being offered for barter. This might involve separate sections or visual indicators for items involved in a direct item-for-item exchange.

## 9. Further Enhancements

*   **Data Tables for Items:** For a large number of items, consider using Unreal Engine's Data Tables to define `UItem` properties instead of creating individual C++ classes or Blueprint assets for every item. This allows for easier data management and iteration.
*   **Save/Load System:** Implement a save/load system to persist inventory, currency, reputation, and skill data.
*   **Networking:** Ensure all components and their properties are properly replicated for multiplayer games.
*   **AI for NPCs:** Develop more sophisticated AI for NPCs to dynamically adjust their `AvailableItemsForSale` and `DesiredItemsForPurchase` based on their daily activities, environment, and game events.
*   **Visual Feedback:** Add visual and audio feedback for trade actions, such as item transfer animations and sound effects.

## 10. Conclusion

This guide provides the foundational C++ code and architectural overview for the NPC Shop/Guild system. By following these steps and extending the provided classes and components, you can create a robust and dynamic trading experience in your Unreal Engine 5 game. Remember to thoroughly test and iterate on the system to balance the economy and ensure a smooth player experience.

---

**Author:** Manus AI
**Date:** April 20, 2026

[1]: Unreal Engine Documentation: UActorComponent - https://docs.unrealengine.com/5.0/en-US/API/Runtime/Engine/Components/UActorComponent/
[2]: Unreal Engine Documentation: UMG UI Designer - https://docs.unrealengine.com/5.0/en-US/UMG/
