# Pokémon FireRed 3DS — Single Screen (unofficial fork)

> **This is an unofficial fork of [Pokémon FireRed 3DS](https://github.com/NachoGonzalez2001/Pokemon-FireRed-3DS)
> by NachoGonzalez2001.** It adds a **single-screen mode** for playing on one screen: an emulator such as
> Azahar on a Steam Deck, a handheld PC or a TV. It is not made or supported by the FireRed 3DS developer:
> please don't ask them for help with it. The original project's README follows below this section.

## What this fork adds

A **SINGLE SCREEN** option (OPTIONS → ENHANCEMENTS, the first row; off until you turn it on):

- **The game's menus stay on the top screen, as on the GBA.** The START menu (Pokédex, Pokémon, bag,
  save, option...), the party, bag, shops, PC boxes and summaries open on the top screen instead of moving
  to the bottom one.
- **Battles are as on the GBA:** FIGHT / BAG / POKéMON / RUN and the moves are in the battle's own text box
  on the top screen.
- **Everything works with buttons**, so you never need the touch screen and can keep your emulator on a
  single-screen layout. The bottom screen still works when you want it: the region map, quick keys and this
  port's own OPTIONS live there.

Based on the official code as of 2 November 2026 (commit `f10061c`). Details for developers:
[docs/SINGLE_SCREEN.md](docs/SINGLE_SCREEN.md).

## How it fits together

FireRed 3DS is two files in `/3ds/firered3ds/`:

| File | What it is | Where it comes from |
|---|---|---|
| `FireRed3DS.3dsx` | The game engine, with single screen built in. No Pokémon content in it. | Built from this fork's code by GitHub. |
| `firered3ds.pak` | The game data: graphics, maps, text, music, the 3D world. | Made on **your** machine from **your own** Pokémon FireRed ROM. |

The **installer** (an AppImage) on this fork's
[Releases](https://github.com/jlaws11/Pokemon-FireRed-3DS-single-screen-/releases) carries the engine and
makes the data pack from your ROM, then writes both files. It does not turn other GBA games into 3DS games:
FireRed 3DS is a rebuild of Pokémon FireRed alone.

A data pack only works with the engine it was made for. Use this fork's builder, not the official
release's, to make the pack for this fork's engine.

## Install on a Steam Deck (or any Linux PC)

You need **Azahar** (the 3DS emulator; on the Deck from EmuDeck or the Discover store) and your own
**Pokémon FireRed (USA/Europe, Rev 1)** ROM, dumped from your cartridge
(SHA-1 `dd5945db9b930750cb39d00c84da8571feebf417`). Other versions and ROM hacks are refused.

1. In Desktop Mode, download **FireRed-3DS-Single-Screen-Installer.AppImage** from the
   **Steam Deck & Linux installer** release (at the top, marked Latest) on the [Releases](https://github.com/jlaws11/Pokemon-FireRed-3DS-single-screen-/releases)
   page.
2. If you already play FireRed 3DS, back up `firered3ds.sav` from Azahar's `sdmc/3ds/firered3ds/` folder first.
3. Right-click the AppImage → Properties → Permissions → tick **Is executable** (or add it to Gear Lever),
   then open it.
4. Choose your ROM. For **Install to**, choose Azahar's `sdmc` folder. It may say the folder is not a 3DS
   SD card, which is fine. Press **Install** and wait about 10–15 minutes while it builds the 3D world's data.
   It works offline; online, it also fetches the optional follower Pokémon sprites.
5. In Azahar, open `sdmc/3ds/firered3ds/FireRed3DS.3dsx` with its normal two-screen layout. On the bottom
   screen, turn on **OPTIONS → ENHANCEMENTS → SINGLE SCREEN**.
6. Switch Azahar to the **Single Screen** layout (View → Screen Layout). To play from Gaming Mode, add
   Azahar to Steam (Games → Add a Non-Steam Game). Optionally, put Azahar's **Swap Screens** hotkey on a
   back button for when you want the bottom screen (the region map or this port's OPTIONS).

From the command line: `./FireRed-3DS-Single-Screen-Installer.AppImage build --rom firered.gba --output folder`,
then copy `folder/3ds` to the SD card (Azahar's `sdmc`).

## Sharing it

- **Share the link to this fork, never a ROM or a data pack.** Everyone makes their own pack from their own
  cartridge dump with the AppImage. This fork, its releases and the AppImage contain no game data.
- The original project has no licence file. This fork keeps all of its credits and changes only what single
  screen needs; if you plan to share it widely, it is polite to ask the original author first, or to offer
  the change to them as a pull request.
- Windows: no Windows build of this fork's builder yet. The AppImage is for Linux and the Steam Deck.

## When the code changes

Every change to the engine needs a new data pack, and so a new recipe, the file that tells the builder how
to make the pack from a ROM. Making a recipe needs a copy of the ROM once, so it is made privately, never on
GitHub. Then:

1. A push to `main` builds the new `FireRed3DS.3dsx` (the **Single screen release** workflow) in a pinned
   build environment (`.github/release_env`, `.github/release-env.toml`) and publishes it as a
   `single-screen-…` release.
2. The recipe for it is made from the build's data and a ROM (`tools/gen_recipe.py`), audited
   (`tools/release_audit.py --strict --rom`), and committed as `.github/payload/firered3ds.recipe` with
   `.github/payload/engine.toml` (which release and data ABI it is for). The build's data reaches that step
   only encrypted to `.github/pack-inputs.pub.pem`.
3. That commit builds a new **installer** AppImage with the new engine and recipe, published as the
   `steam-deck-installer-…` release (marked Latest; older installers are removed). The
   `single-screen-…` release is the executable alone, for developers.

Changes to docs or this README do not rebuild the game.

## Status and limits

- **Not yet tested in play.** It builds and the AppImage's pack matches the engine. If something looks
  wrong in a menu or a battle, open an issue on this fork with a screenshot.
- Made for emulators. It should also run on a real 3DS, but that is untested.
- English (USA/Europe, Rev 1) FireRed only, like the official release.
- If the official FireRed 3DS changes after the commit above, this fork does not have it until it is
  brought up to date.

---

<p align="center">
  <img src="3ds_port/assets/artwork/FireRed%203DS.png" alt="FireRed 3DS" width="560">
</p>

# Pokémon FireRed 3DS (voxel)

A native Nintendo 3DS port of Pokémon FireRed that uses both screens, with an
HD-2D style voxel world. It is built from the
[pret/pokefirered](https://github.com/pret/pokefirered) decompilation on the
technical groundwork of
[Pokémon Emerald 3Ds Dual Screen](https://github.com/ZallaxDev/pokeemerald-3Ds-dualscreen).

**This repository and its releases do not contain the game.** You need your
own dump of your own Pokémon FireRed cartridge.

## Screenshots

<p align="center">
  <img src="docs/screenshots/dual-screen.png" alt="Cerulean City on the top screen, the region map, menu and party on the bottom screen" width="400">
</p>

<table>
  <tr>
    <td><img src="docs/screenshots/route-25.png" alt="Route 25 with Eevee following"><br><sub>Route 25</sub></td>
    <td><img src="docs/screenshots/vermilion.png" alt="Vermilion City harbour with Squirtle following"><br><sub>Vermilion City, morning</sub></td>
  </tr>
  <tr>
    <td><img src="docs/screenshots/cerulean-dusk.png" alt="Cerulean City at dusk with Psyduck following"><br><sub>Cerulean City, dusk</sub></td>
    <td><img src="docs/screenshots/lavender-night.png" alt="Lavender Town at night with Gengar following"><br><sub>Lavender Town, night</sub></td>
  </tr>
  <tr>
    <td><img src="docs/screenshots/cinnabar-dusk.png" alt="Cinnabar Island at dusk with Arcanine following"><br><sub>Cinnabar Island, dusk</sub></td>
    <td><img src="docs/screenshots/fuchsia.png" alt="Fuchsia City with Snorlax following"><br><sub>Fuchsia City</sub></td>
  </tr>
  <tr>
    <td><img src="docs/screenshots/viridian-forest.png" alt="Viridian Forest with Butterfree following"><br><sub>Viridian Forest</sub></td>
    <td><img src="docs/screenshots/mt-moon.png" alt="Mt. Moon with Clefairy following"><br><sub>Mt. Moon</sub></td>
  </tr>
</table>

**Day and night.** Outdoors the light follows the console's clock (or a day
that passes in 24 minutes, or stays off - OPTIONS > DAY/NIGHT):

<table>
  <tr>
    <td><img src="docs/screenshots/pallet-dawn.png" alt="Pallet Town at dawn"><br><sub>Dawn</sub></td>
    <td><img src="docs/screenshots/pallet-day.png" alt="Pallet Town by day"><br><sub>Day</sub></td>
  </tr>
  <tr>
    <td><img src="docs/screenshots/pallet-dusk.png" alt="Pallet Town at dusk"><br><sub>Dusk</sub></td>
    <td><img src="docs/screenshots/pallet-night.png" alt="Pallet Town at night"><br><sub>Night</sub></td>
  </tr>
</table>

## Features

- **The whole game, native on the 3DS**, at the console's own frame rate.
- **A 3D overworld**: towns, routes, caves and interiors rebuilt in 3D from
  the game's own drawing - buildings, trees, relief, furniture - with lighting,
  shadows, a day/night cycle and an adjustable camera. The classic 2D field
  is one option away.
- **3D battles** (optional): the battle in front of the voxel world.
- **A bottom screen like a Pokédex**: region map, party, quick menu keys, the
  battle's moves and actions to touch; menus run on it while the top screen
  keeps the world.
- **Free movement** (optional): walk any way the circle pad points.
- **A Pokémon that follows you** in the 3D world: the first of your party
  walks behind you, in eight directions, in Mystery Dungeon style (see below).
- **Options**: settings, quality-of-life enhancements (game speed, exp share,
  reusable TMs, HMs without the move...) and cheats, from the bottom screen.

## Install (players)

1. Download `FireRed3DS-v<version>-Windows.zip` from the
   [Releases](../../releases) page and extract it.
2. Run `FireRed3DS-Builder.exe`, choose your ROM - Pokémon FireRed (USA,
   Rev 1), SHA-1 `dd5945db9b930750cb39d00c84da8571feebf417` - and your 3DS's SD
   card. The builder generates the game's data from your ROM on your computer
   and installs it in `/3ds/firered3ds/`. The ROM is never uploaded or copied.
3. Start the game from the Homebrew Launcher, or install
   `FireRed3DS-Forwarder.cia` once with FBI for a HOME Menu icon (it starts
   the installed game, so updates don't need a new CIA).

Needs a 3DS / 2DS family console with custom firmware (Luma3DS). Saves are in
`/3ds/firered3ds/firered3ds.sav`, in the GBA's format: the same 128 KB file
moves between the 3DS, GBA emulators and a cartridge dump. (v0.2.3 converts a
save from an earlier version once, keeping the original as
`firered3ds.sav.bak`.)

**Linux, macOS or a Chromebook (Linux).** The builder is plain Python: with
Python 3.11+ and Pillow, use the `payload` folder from the release ZIP and the
builder's sources from the same version:

```
git clone --branch v<version> https://github.com/NachoGonzalez2001/Pokemon-FireRed-3DS
cd Pokemon-FireRed-3DS/builder
python3 -m firered3ds_builder --payload /path/to/FireRed3DS-v<version>-Windows/payload     build --rom /path/to/firered.gba --output out
```

then copy `out/3ds` to the root of the SD card.

**The Pokémon that follows you.** Its sprites come from
[PMD SpriteCollab](https://github.com/PMDCollab/SpriteCollab), most of them Spike
Chunsoft's own from the Pokémon Mystery Dungeon games, so this repository and
its releases do not include them: the builder downloads them while it installs
(it needs an internet connection for that, once) and writes `followers.bin`
and `followers-credits.txt`, every artist credited, next to the game. Without
them the game works the same, without its follower; it can be turned off in
OPTIONS (3D FOLLOWER).

## Build (developers)

The source tree is the pinned decomp plus this project's patches and port:

```sh
python tools/bootstrap.py --dir C:/fr3ds/upstream     # pret/pokefirered + patches/ + 3ds_port/
make -C C:/fr3ds/upstream/3ds_port all                 # devkitARM, libctru, citro2d/3d
```

`make all` embeds the game data built from the decomp (a development build);
`make release` builds the executable alone. `tools/build_release.py` makes a
release: the recipe the builder rebuilds the data from (`tools/gen_recipe.py`),
the payload, the standalone builder and an audit that nothing from the game
is inside (`tools/release_audit.py`).

| Path | What |
|---|---|
| `patches/pokefirered/` | the changes to the decomp, one patch per topic |
| `3ds_port/` | the 3DS platform layer, renderer, voxel world and bottom screen |
| `3ds_port/scripts/` | generators of the 3D world from the game's art |
| `builder/` | the player's builder: ROM to data pack |
| `tools/` | bootstrap, patch export, release tools |

## Support the project

Pokémon FireRed 3DS is free and always will be. If you enjoy it and want to
support the time that goes into it, you can make a voluntary donation through
PayPal. Donations support development only: they do not buy the game, early
access or anything else, and the releases never include the game.

[![Donate with PayPal](https://img.shields.io/badge/Donate-PayPal-00457C?logo=paypal&logoColor=white)](https://www.paypal.com/donate/?hosted_button_id=YRCM45GNZ59XW)

## Acknowledgements

Special thanks to **[ZallaxDev](https://github.com/ZallaxDev)**, creator of
[Pokémon Emerald 3Ds Dual Screen](https://github.com/ZallaxDev/pokeemerald-3Ds-dualscreen).
This project stands on that port: its 3DS platform layer, audio, bottom-screen
design, voxel renderer and tooling are where this one started, and its
approach to bringing a GBA Pokémon game to both screens of the 3DS made this
port possible.

Thanks as well to the [pret](https://github.com/pret) team for the
pokefirered decompilation.

See [NOTICE.md](NOTICE.md) for credits and licences. Pokémon FireRed 3DS is an
unofficial fan project, not affiliated with or endorsed by Nintendo, Game
Freak, Creatures or The Pokémon Company.
