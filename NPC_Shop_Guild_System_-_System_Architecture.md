# NPC Shop/Guild System - System Architecture

## 1. Overview

This document outlines the system architecture for the NPC Shop/Guild system in the "Hayot Simulyatori" Unreal Engine 5 game. The system will enable players to interact with NPCs to buy and sell items, manage inventory, and incorporate dynamic pricing based on supply, demand, player reputation, and trading skill.

## 2. Core Requirements

Based on the provided Game Design Document (GDD) summary, the key requirements for the NPC Shop/Guild system are:

*   **NPC Interaction:** Players can initiate trade with NPCs.
*   **Item Evaluation & Transaction:** NPCs evaluate items brought by the player, accept them, and pay the player. Players can also purchase items from NPCs.
*   **Comprehensive Trading:** The system should handle all types of items for both buying and selling.
*   **Inventory Management:** Dedicated inventory systems for both players and NPCs.
*   **Currency System:** Support for Copper, Silver, and Gold coins, with coin weight affecting player burden.
*   **Dynamic Pricing:** Item prices fluctuate based on supply and demand.
*   **Reputation System:** Player reputation with an NPC (or faction) influences trade prices.
*   **Trading Skill:** Player's trading skill provides better prices and potentially more trade options.
*   **Barter System:** Support for item-for-item exchanges.
*   **Engine:** Unreal Engine 5, utilizing both C++ and Blueprints.

## 3. Architectural Design

The system will primarily be implemented in C++ for core logic and data structures, with Unreal Motion Graphics (UMG) and Blueprints for UI and game-specific integrations.

### 3.1. C++ Modules and Components

#### 3.1.1. Item System

*   **`UItem` (Base Class):** A base `UObject` or `AActor` derived class for all in-game items. It will contain fundamental properties such as:
    *   `FName ItemID`: Unique identifier for the item.
    *   `FText DisplayName`: Localized display name.
    *   `FText Description`: Localized description.
    *   `float Weight`: Physical weight of the item.
    *   `int32 BaseValue`: Base monetary value of the item.
    *   `int32 MaxStackSize`: Maximum number of this item that can stack in an inventory slot.
    *   `EItemType ItemType`: Enum to categorize items (e.g., Resource, Weapon, Armor, Consumable).
    *   `EResourceType ResourceType`: Enum to specify the type of resource (e.g., Stone, Wood, Meat) if `ItemType` is `Resource`.

*   **Derived Item Classes:** Specific item types will inherit from `UItem` (e.g., `UResourceItem`, `UWeaponItem`, `UConsumableItem`) to add unique properties and functionalities.

#### 3.1.2. Inventory System

*   **`UInventoryComponent` (Actor Component):** An `UActorComponent` that can be added to any `AActor` (Player, NPC, Chests). It will manage a collection of `UItem` instances.
    *   `TArray<FInventorySlot> InventorySlots`: Array of inventory slots, each containing an `UItem` and its quantity.
    *   `float MaxWeightCapacity`: Maximum total weight the inventory can hold.
    *   `float CurrentWeight`: Current total weight of items in the inventory.
    *   **Functions:** `AddItem(UItem* Item, int32 Quantity)`, `RemoveItem(UItem* Item, int32 Quantity)`, `HasItem(UItem* Item, int32 Quantity)`, `GetTotalWeight()`, `GetAvailableSpace()`, etc.

*   **`FInventorySlot` (Struct):** A `USTRUCT` to represent a single slot in the inventory.
    *   `UItem* ItemReference`: Pointer to the `UItem` instance.
    *   `int32 Quantity`: Number of items in this slot.

#### 3.1.3. Currency System

*   **`UCurrencyComponent` (Actor Component):** An `UActorComponent` to manage the player's or NPC's monetary holdings.
    *   `int32 CopperCoins`, `int32 SilverCoins`, `int32 GoldCoins`: Quantities of each coin type.
    *   `float CoinWeightPerUnit`: Weight of a single copper coin (silver and gold will be multiples).
    *   **Functions:** `AddCurrency(ECurrencyType Type, int32 Amount)`, `RemoveCurrency(ECurrencyType Type, int32 Amount)`, `GetTotalWeight()`, `ConvertCurrency()`, etc.

*   **`ECurrencyType` (Enum):** `Copper`, `Silver`, `Gold`.

#### 3.1.4. Trading System

*   **`UTradeComponent` (Actor Component):** An `UActorComponent` attached to NPCs that act as traders. It will encapsulate the trading logic.
    *   `TMap<FName, FTradeItemData> AvailableItemsForSale`: Items the NPC has to sell, along with their current stock and base price adjustments.
    *   `TMap<FName, FTradeItemData> DesiredItemsForPurchase`: Items the NPC is interested in buying, along with their demand and base price adjustments.
    *   **Functions:**
        *   `CalculateSellPrice(UItem* Item, APlayerCharacter* Player)`: Determines the price at which the NPC sells an item to the player, considering supply, demand, player reputation, and trading skill.
        *   `CalculateBuyPrice(UItem* Item, APlayerCharacter* Player)`: Determines the price at which the NPC buys an item from the player, considering demand, supply, player reputation, and trading skill.
        *   `ExecuteTrade(APlayerCharacter* Player, TArray<FInventorySlot> PlayerOffers, TArray<FInventorySlot> NPCOffers)`: Handles the transfer of items and currency for a trade.
        *   `HandleBarterTrade(...)`: Specific logic for barter trades.
        *   `UpdateSupplyAndDemand(UItem* Item, int32 Quantity, bool bSoldByPlayer)`: Adjusts internal supply/demand values after a trade.

*   **`FTradeItemData` (Struct):** Stores data relevant to an item's trading status.
    *   `int32 CurrentStock`: How many of this item the NPC currently has.
    *   `float SupplyFactor`: Multiplier based on how much of this item has been sold to/bought from the NPC recently.
    *   `float DemandFactor`: Multiplier based on how much the NPC needs/wants this item.

#### 3.1.5. Reputation System

*   **`UReputationComponent` (Actor Component):** Attached to the Player character to track reputation with various NPCs or factions.
    *   `TMap<FName, float> NPCReputation`: Map of NPC ID to reputation value.
    *   **Functions:** `AdjustReputation(FName NPCID, float Amount)`, `GetReputation(FName NPCID)`.

#### 3.1.6. Skill System

*   **`USkillComponent` (Actor Component):** Attached to the Player character to manage skills.
    *   `int32 TradingSkillXP`: Current experience points for trading skill.
    *   `int32 TradingSkillLevel`: Current level of trading skill.
    *   **Functions:** `AddTradingSkillXP(int32 Amount)`, `GetTradingSkillBonus()`.

#### 3.1.7. NPC Interaction

*   **`UNPCInteractionComponent` (Actor Component):** Attached to NPCs to handle player interaction.
    *   **Functions:** `OnPlayerInteract(APlayerCharacter* Player)`: Triggers the trading UI and passes relevant NPC data.

### 3.2. Blueprints and UMG

*   **`BP_NPC_Trader` (Blueprint Actor):** A Blueprint class inheriting from a C++ base class (e.g., `ANPCBase`) that will contain the `UTradeComponent`, `UInventoryComponent`, and `UCurrencyComponent`. It will also handle visual representation and basic AI for the NPC.
*   **`WBP_TradeUI` (UMG Widget):** The main trading interface widget.
    *   Displays player's inventory and currency.
    *   Displays NPC's inventory and currency.
    *   Shows calculated buy/sell prices for selected items.
    *   Allows players to select items to sell or buy.
    *   Includes a confirmation button for trades.
    *   Handles barter trade UI elements.
*   **`WBP_InventorySlot` (UMG Widget):** A reusable widget for displaying individual inventory slots within the `WBP_TradeUI`.

## 4. Interaction Flow

1.  Player approaches `BP_NPC_Trader` and interacts (e.g., presses 'E').
2.  `UNPCInteractionComponent` on the NPC triggers `OnPlayerInteract`.
3.  `OnPlayerInteract` creates and displays `WBP_TradeUI`, passing references to the Player's `UInventoryComponent`, `UCurrencyComponent`, `USkillComponent`, and the NPC's `UTradeComponent`, `UInventoryComponent`, `UCurrencyComponent`.
4.  The `WBP_TradeUI` populates with items from both inventories and calculates initial prices using the `UTradeComponent`'s `CalculateBuyPrice` and `CalculateSellPrice` functions.
5.  Player selects items to buy or sell. The UI dynamically updates prices.
6.  Player confirms the trade.
7.  The `WBP_TradeUI` calls `UTradeComponent::ExecuteTrade` on the NPC, which handles item and currency transfers, updates supply/demand, and adjusts player reputation and trading skill XP.
8.  UI updates to reflect the new inventory and currency states.

## 5. Dynamic Pricing Logic (Conceptual)

`FinalPrice = BaseValue * (1 + (DemandFactor - SupplyFactor)) * (1 + PlayerReputationBonus) * (1 - PlayerTradingSkillDiscount)`

*   `BaseValue`: Defined in `UItem`.
*   `DemandFactor`, `SupplyFactor`: Managed by `UTradeComponent` for each item, increasing/decreasing based on recent trades.
*   `PlayerReputationBonus`: Derived from `UReputationComponent`.
*   `PlayerTradingSkillDiscount`: Derived from `USkillComponent`.

## 6. Barter System (Conceptual)

Barter trades will involve a direct exchange of items without currency. The `UTradeComponent` will need a function to evaluate the fairness of a barter trade based on the combined value of items offered by both parties, adjusted by reputation and skill. The UI will need specific elements to facilitate selecting items for barter.

## 7. Next Steps

Proceed with defining the core C++ data structures (Enums and Structs) and classes based on this architecture.
