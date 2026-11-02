# Builder payload

`firered3ds.recipe` tells the builder how to make `firered3ds.pak` from a player's own ROM for the engine
named in `engine.toml`. It holds no game data: no graphics, text, maps or music, only where each piece is
found in the ROM and how to rebuild it. It was made with `tools/gen_recipe.py` and audited with
`tools/release_audit.py --strict --rom`. A commit here builds the Builder AppImage
(`.github/workflows/builder-appimage.yml`).
