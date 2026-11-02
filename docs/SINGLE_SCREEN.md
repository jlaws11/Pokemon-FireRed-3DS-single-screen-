# Single screen (this fork)

An option for playing on one screen (an emulator's single-screen layout): the game's own menus stay on
the top screen, as on the GBA, instead of moving to the bottom screen.

## The option

`single_screen` in the port's extras (`3ds_port/src/fr_extras.c`, OPTIONS → ENHANCEMENTS, first row,
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
- **Battles.** Unchanged: FireRed 3DS already draws the battle's own text box and menus on the top
  screen; the bottom screen's battle keys stay as an alternative.

## Builds

Every engine change needs a new data pack and recipe. See the README's "When the code changes"; the
workflows are `.github/workflows/single-screen-release.yml`, `builder-appimage.yml` and
`cleanup-releases.yml`.
