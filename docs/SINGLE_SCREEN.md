# Single screen (this fork)

An option for playing on one screen (an emulator's single-screen layout): the game's own menus stay on
the top screen, as on the GBA, instead of moving to the bottom screen.

## The option

`single_screen` in the port's extras (`3ds_port/src/fr_extras.c`, OPTIONS → ENHANCE, first row,
OFF/ON, default OFF). `CtrSingleScreen()` in `3ds_port/src/fr_bottom_ui.c` reads it.

## What changes

- **Menus.** Normally, while a menu runs, the bottom screen takes the game's frame
  (`CtrBottom_TakesGameFrame`) and the top screen keeps the world. In single screen,
  `CtrBottom_TakesGameFrame` is false, so the frame with the menu stays on the top screen.
  `CtrBottom_MenuShown` still tells the port that a menu is up, and `Port_SceneShown`
  (`3ds_port/src/3ds_game_full.c`) counts that as a shown scene, so the top screen keeps updating.
- **START menu.** The port normally keeps START from opening the GBA START menu in the field (its
  functions are on the bottom screen). `patches/pokefirered/0015-single-screen-start-menu.patch` lets
  `CtrStartMenu_AcceptStart` accept START when single screen is on.
- **Battles.** The port shows only "What will ... do?" on top and puts the actions and moves on the bottom
  screen. `patches/pokefirered/0016-single-screen-battle-menus.patch` keeps the GBA's own: in single screen
  `PlayerHandleChooseAction` draws the action menu and `gBattle_BG0_Y` stays where the GBA has it (160 for
  the actions, 320 for the moves), in the player's controller and the first battle's (Oak/old man). The
  bottom screen's battle buttons still work.
- **The bottom screen on top.** It is touch only, so in single screen X shows it on the top screen with
  button controls (`PanelOnTopInput`, `fr_bottom_ui.c`). Its targets are found by asking `HitTest` over the
  screen (`ScanTargets`, again after every press and every few frames), so every page's own buttons,
  party, option tabs and rows; the D-pad steps a highlight to the nearest one in its direction, A touches
  its middle (held while A is), and on an option row Left and Right touch its two halves (a value down or
  up). B or X closes it, and the game gets no keys while it is up (`CtrBottom_FilterKeys`). Only in the
  field; a menu it opens (POKeMON, BAG...) leaves the field and so closes it. `3ds_soft_video.c` draws the
  canvas (`CtrBottom_PanelOnTop`) over the dimmed game with the highlight framed.

## Builds

Every engine change needs a new data pack and recipe. See the README's "When the code changes"; the
workflows are `.github/workflows/single-screen-release.yml`, `builder-appimage.yml` and
`cleanup-releases.yml`.
