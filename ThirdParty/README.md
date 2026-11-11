# Third-party modules

## Vendored directly

### InteractionSystem
- Path: Plugins/InteractionSystem
- Upstream: https://github.com/justin6577-spec/InteractionSystem-UE5
- License: MIT
- Original plugin descriptor, README and LICENSE are retained.
- Local fixes and pending tests: INTERACTION_REVIEW.md.

## Pinned Git submodules

The following plugins are tracked as Git submodules to avoid copying hundreds of upstream files into this repository and to preserve clean upstream history:

- Plugins/InventorySystem -> https://github.com/DanieltheDeveloper/inventory-plugin-community-version
- Plugins/DlgSystem -> https://github.com/NotYetGames/DlgSystem

Clone the repository with submodules:

    git clone --recurse-submodules https://github.com/abdulloh042006-debug/Life-simulator-.git

If the repository is already cloned:

    git submodule update --init --recursive

Both submodules are pinned to specific upstream commits in .gitmodules plus the Git tree entries. They are not modified locally. Check each plugin's upstream LICENSE before redistribution; both selected upstream repositories declare MIT.

## Integration policy

1. Keep external plugin code isolated under Plugins/.
2. Do not edit vendor code for game-specific features unless the fix is clearly a general bug and is recorded.
3. Put game-specific adapters and gameplay rules in the project's own Source/ modules.
4. Keep upstream commit, license, UE version, local patch list, compile result and runtime test result in the corresponding review file.
5. Never describe a plugin as integrated or working until a real Unreal project compiles and the relevant runtime tests pass.

## Current validation status

- InteractionSystem source copied and one static code correction applied: see INTERACTION_REVIEW.md.
- InventorySystem submodule pinned; UE compile/runtime test pending.
- DlgSystem submodule pinned; UE compile/runtime test pending.
- Main game project compile and packaged build: not run; the current repository does not yet contain a .uproject file.
