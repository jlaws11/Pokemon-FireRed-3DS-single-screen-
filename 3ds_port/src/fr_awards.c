/*
 * Awards: this fork's own achievements, kept by the game itself (an emulator
 * that plays this port has no RetroAchievements). Their goals follow those
 * the RetroAchievements set for the GBA FireRed has - the badges, the gym
 * leaders beaten with a small, low-level team, the rival, the gifts and the
 * in-game trades, the legendary Pokemon, the Sevii Islands - with their own
 * names and words.
 *
 * Each award is a test of the save's own state (its flags, the trainers it
 * has beaten, its Pokedex), checked a few times a second while a game is on.
 * The gym challenges look at the party at the moment its badge is won, and
 * the Hall of Fame one at the moment the game is won: those only count when
 * seen happening, not when a save is loaded that has them already.
 *
 * What is won is kept in awards.txt beside the settings, for every save file:
 * an award stays won. A new one is announced with a banner on the top screen
 * (CtrAwards_TakeBanner, fr_bottom_ui.c draws it); the AWARDS tab of OPTIONS
 * lists them all.
 */

#include <stdio.h>
#include <string.h>

#include "global.h"
#include "battle_setup.h"
#include "event_data.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "constants/flags.h"
#include "constants/game_stat.h"
#include "constants/opponents.h"
#include "constants/species.h"

#include "3ds_awards.h"
#include "3ds_data.h"
#include "3ds_log.h"

#define AWARDS_PATH CTR_DATA_DIR "awards.txt"
#define CHECK_FRAMES 20
/* more than this many at once (a save with progress, on its first check)
 * are announced as one banner */
#define BANNER_EACH_MAX 3

enum { KIND_STATE, KIND_BADGE_CHALLENGE, KIND_HALL_OF_FAME_SOLO };

typedef struct
{
    const char *name;
    const char *text;
    u8 kind;
    bool8 (*test)(void);
    /* a badge challenge: which badge, the most Pokemon and highest level */
    u8 badge, partyMax, levelMax;
} Award;

static bool8 Fought3(u16 squirtle)
{
    return HasTrainerBeenFought(squirtle) || HasTrainerBeenFought(squirtle + 1) || HasTrainerBeenFought(squirtle + 2);
}

static bool8 Caught(u16 species)
{
    return GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT);
}

#define FLAG_TEST(fn, flag) static bool8 fn(void) { return FlagGet(flag); }
#define FOUGHT_TEST(fn, trainer) static bool8 fn(void) { return HasTrainerBeenFought(trainer); }
#define RIVAL_TEST(fn, squirtle) static bool8 fn(void) { return Fought3(squirtle); }
#define CAUGHT_TEST(fn, species) static bool8 fn(void) { return Caught(species); }

FLAG_TEST(Badge1, FLAG_BADGE01_GET)
FLAG_TEST(Badge2, FLAG_BADGE02_GET)
FLAG_TEST(Badge3, FLAG_BADGE03_GET)
FLAG_TEST(Badge4, FLAG_BADGE04_GET)
FLAG_TEST(Badge5, FLAG_BADGE05_GET)
FLAG_TEST(Badge6, FLAG_BADGE06_GET)
FLAG_TEST(Badge7, FLAG_BADGE07_GET)
FLAG_TEST(Badge8, FLAG_BADGE08_GET)
FLAG_TEST(Champion, FLAG_SYS_GAME_CLEAR)
RIVAL_TEST(RivalChampionAgain, TRAINER_CHAMPION_REMATCH_SQUIRTLE)
RIVAL_TEST(RivalLab, TRAINER_RIVAL_OAKS_LAB_SQUIRTLE)
RIVAL_TEST(RivalRoute22, TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE)
RIVAL_TEST(RivalCerulean, TRAINER_RIVAL_CERULEAN_SQUIRTLE)
RIVAL_TEST(RivalSSAnne, TRAINER_RIVAL_SS_ANNE_SQUIRTLE)
RIVAL_TEST(RivalTower, TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE)
RIVAL_TEST(RivalSilph, TRAINER_RIVAL_SILPH_SQUIRTLE)
RIVAL_TEST(RivalRoad, TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE)
FOUGHT_TEST(RocketHideout, TRAINER_BOSS_GIOVANNI)
FOUGHT_TEST(RocketSilph, TRAINER_BOSS_GIOVANNI_2)
FLAG_TEST(Magikarp, FLAG_BOUGHT_MAGIKARP)
static bool8 Fossil(void) { return FlagGet(FLAG_GOT_HELIX_FOSSIL) || FlagGet(FLAG_GOT_DOME_FOSSIL); }
FLAG_TEST(OldAmber, FLAG_GOT_OLD_AMBER)
FLAG_TEST(Eevee, FLAG_GOT_EEVEE)
FLAG_TEST(Dojo, FLAG_GOT_HITMON_FROM_DOJO)
FLAG_TEST(Lapras, FLAG_GOT_LAPRAS_FROM_SILPH)
FLAG_TEST(TogepiEgg, FLAG_GOT_TOGEPI_EGG)
FLAG_TEST(TradeMrMime, FLAG_DID_MIMIEN_TRADE)
FLAG_TEST(TradeFarfetchd, FLAG_DID_CH_DING_TRADE)
FLAG_TEST(TradeNidoranF, FLAG_DID_MS_NIDO_TRADE)
FLAG_TEST(TradeJynx, FLAG_DID_ZYNX_TRADE)
FLAG_TEST(TradeNidorina, FLAG_DID_NINA_TRADE)
FLAG_TEST(TradeLickitung, FLAG_DID_MARC_TRADE)
FLAG_TEST(TradeElectrode, FLAG_DID_ESPHERE_TRADE)
FLAG_TEST(TradeTangela, FLAG_DID_TANGENY_TRADE)
FLAG_TEST(TradeSeel, FLAG_DID_SEELOR_TRADE)
CAUGHT_TEST(CatchSnorlax, SPECIES_SNORLAX)
CAUGHT_TEST(CatchChansey, SPECIES_CHANSEY)
CAUGHT_TEST(CatchArticuno, SPECIES_ARTICUNO)
CAUGHT_TEST(CatchZapdos, SPECIES_ZAPDOS)
CAUGHT_TEST(CatchMoltres, SPECIES_MOLTRES)
CAUGHT_TEST(CatchMewtwo, SPECIES_MEWTWO)
static bool8 CatchBeast(void) { return Caught(SPECIES_RAIKOU) || Caught(SPECIES_ENTEI) || Caught(SPECIES_SUICUNE); }
static bool8 KantoDex(void) { return GetKantoPokedexCount(FLAG_GET_CAUGHT) >= 150; }
FLAG_TEST(Lostelle, FLAG_RESCUED_LOSTELLE)
FOUGHT_TEST(Icefall, TRAINER_TEAM_ROCKET_GRUNT_45)
FOUGHT_TEST(Selphy, TRAINER_LADY_SELPHY)
FOUGHT_TEST(Warehouse, TRAINER_TEAM_ROCKET_ADMIN_2)

#define STATE(name, text, test) { name, text, KIND_STATE, test, 0, 0, 0 }
#define CHALLENGE(name, text, badge, party, level) { name, text, KIND_BADGE_CHALLENGE, NULL, badge, party, level }

/* The order they are listed in; awards.txt keeps them by name, so a list
 * that changes keeps what was won. */
static const Award sAwards[] = {
    STATE("Boulder Badge", "Beat Brock in Pewter City", Badge1),
    STATE("Cascade Badge", "Beat Misty in Cerulean City", Badge2),
    STATE("Thunder Badge", "Beat Lt. Surge in Vermilion City", Badge3),
    STATE("Rainbow Badge", "Beat Erika in Celadon City", Badge4),
    STATE("Soul Badge", "Beat Koga in Fuchsia City", Badge5),
    STATE("Marsh Badge", "Beat Sabrina in Saffron City", Badge6),
    STATE("Volcano Badge", "Beat Blaine on Cinnabar Island", Badge7),
    STATE("Earth Badge", "Beat Giovanni in Viridian City", Badge8),
    CHALLENGE("Pebble Squad", "Brock: 2 or fewer Pokemon, Lv 14 max", 1, 2, 14),
    CHALLENGE("Low Tide", "Misty: 2 or fewer Pokemon, Lv 21 max", 2, 2, 21),
    CHALLENGE("Short Circuit", "Lt. Surge: 3 or fewer, Lv 24 max", 3, 3, 24),
    CHALLENGE("Small Bouquet", "Erika: 3 or fewer Pokemon, Lv 29 max", 4, 3, 29),
    CHALLENGE("Quiet Steps", "Koga: 4 or fewer Pokemon, Lv 43 max", 5, 4, 43),
    CHALLENGE("Clear Mind", "Sabrina: 4 or fewer, Lv 43 max", 6, 4, 43),
    CHALLENGE("Cool Head", "Blaine: 4 or fewer Pokemon, Lv 47 max", 7, 4, 47),
    CHALLENGE("Lean Machine", "Giovanni: 5 or fewer, Lv 50 max", 8, 5, 50),
    STATE("First Rivalry", "Beat your rival in Oak's Lab", RivalLab),
    STATE("Road Rage", "Beat your rival early on Route 22", RivalRoute22),
    STATE("Bridge Duel", "Beat your rival in Cerulean City", RivalCerulean),
    STATE("Rough Seas", "Beat your rival on the S.S. Anne", RivalSSAnne),
    STATE("Grave Words", "Beat your rival in Pokemon Tower", RivalTower),
    STATE("Corporate Clash", "Beat your rival in Silph Co.", RivalSilph),
    STATE("Last Stop", "Beat your rival on Route 22 again", RivalRoad),
    STATE("Hideout Raid", "Beat Giovanni under the Game Corner", RocketHideout),
    STATE("Takeover Halted", "Beat Giovanni in Silph Co.", RocketSilph),
    STATE("Champion", "Enter the Hall of Fame", Champion),
    { "One and Only", "Enter the Hall of Fame with 1 Pokemon", KIND_HALL_OF_FAME_SOLO, NULL, 0, 0, 0 },
    STATE("Still Champion", "Beat the Champion's rematch team", RivalChampionAgain),
    STATE("Fishy Deal", "Buy the Magikarp near Mt. Moon", Magikarp),
    STATE("Fossil Hunter", "Take a fossil in Mt. Moon", Fossil),
    STATE("Museum Piece", "Get the Old Amber in Pewter City", OldAmber),
    STATE("Gift of Eevee", "Get the Eevee in Celadon City", Eevee),
    STATE("Dojo Prize", "Win a Pokemon at the Fighting Dojo", Dojo),
    STATE("Thank-You Lapras", "Get the Lapras in Silph Co.", Lapras),
    STATE("Egg Delivery", "Get the Egg on Five Island", TogepiEgg),
    STATE("Mime Swap", "Trade an Abra for Mr. Mime", TradeMrMime),
    STATE("Leek Swap", "Trade a Spearow for Farfetch'd", TradeFarfetchd),
    STATE("Nido Swap", "Trade a Nidoran (M) for Nidoran (F)", TradeNidoranF),
    STATE("Jynx Swap", "Trade a Poliwhirl for Jynx", TradeJynx),
    STATE("Nidorina Swap", "Trade a Nidorino for Nidorina", TradeNidorina),
    STATE("Tongue Swap", "Trade a Golduck for Lickitung", TradeLickitung),
    STATE("Electrode Swap", "Trade a Raichu for Electrode", TradeElectrode),
    STATE("Tangela Swap", "Trade a Venonat for Tangela", TradeTangela),
    STATE("Seel Swap", "Trade a Ponyta for Seel", TradeSeel),
    STATE("Sleeping Giant", "Catch a Snorlax", CatchSnorlax),
    STATE("Lucky Find", "Catch a Chansey", CatchChansey),
    STATE("Frozen Wings", "Catch Articuno", CatchArticuno),
    STATE("Thunder Wings", "Catch Zapdos", CatchZapdos),
    STATE("Flame Wings", "Catch Moltres", CatchMoltres),
    STATE("Made Not Born", "Catch Mewtwo", CatchMewtwo),
    STATE("Roaming Legend", "Catch Raikou, Entei or Suicune", CatchBeast),
    STATE("Kanto Complete", "Catch 150 Kanto Pokemon", KantoDex),
    STATE("Lost and Found", "Rescue Lostelle on Three Island", Lostelle),
    STATE("Cold Shoulder", "Stop Team Rocket in Icefall Cave", Icefall),
    STATE("Lost Cave Rescue", "Find Selphy in the Lost Cave", Selphy),
    STATE("Warehouse Raid", "Beat the Rocket Admin in the warehouse", Warehouse),
};

#define AWARD_COUNT ARRAY_COUNT(sAwards)

static bool8 sWon[AWARD_COUNT];
static bool8 sLoaded;
/* the state the challenges are seen changing from */
static bool8 sPrimed;
static u8 sBadges;
static u32 sHallOfFame;
static u8 sTick;
/* awards won and not yet announced */
static u8 sQueue[AWARD_COUNT];
static u8 sQueued;

static void Load(void)
{
    FILE *file;
    char line[96];

    sLoaded = TRUE;
    file = fopen(AWARDS_PATH, "r");
    if (file == NULL)
        return;
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\r\n")] = 0;
        for (unsigned i = 0; i < AWARD_COUNT; i++)
            if (strcmp(line, sAwards[i].name) == 0)
                sWon[i] = TRUE;
    }
    fclose(file);
}

static void Save(void)
{
    FILE *file = fopen(AWARDS_PATH, "w");

    if (file == NULL)
    {
        CtrLog_Write(CTR_LOG_FS, "awards: cannot write %s", AWARDS_PATH);
        return;
    }
    for (unsigned i = 0; i < AWARD_COUNT; i++)
        if (sWon[i])
            fprintf(file, "%s\n", sAwards[i].name);
    fclose(file);
}

static void Win(unsigned i)
{
    if (sWon[i])
        return;
    sWon[i] = TRUE;
    if (sQueued < AWARD_COUNT)
        sQueue[sQueued++] = (u8)i;
    CtrLog_Write(CTR_LOG_GAME, "awards: %s", sAwards[i].name);
}

/* The party as a challenge counts it: its Pokemon (not Eggs) and the highest level. */
static void PartySize(u8 *count, u8 *maxLevel)
{
    *count = 0;
    *maxLevel = 0;
    for (int i = 0; i < PARTY_SIZE; i++)
    {
        u8 level;

        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            continue;
        (*count)++;
        level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        if (level > *maxLevel)
            *maxLevel = level;
    }
}

static u8 Badges(void)
{
    u8 badges = 0;

    for (int b = 0; b < NUM_BADGES; b++)
        if (FlagGet(FLAG_BADGE01_GET + b))
            badges |= 1 << b;
    return badges;
}

void CtrAwards_Frame(bool inGame)
{
    bool changed = false;
    u8 badges, count, maxLevel;
    u32 hallOfFame;

    if (!sLoaded)
        Load();
    if (!inGame)
    {
        /* another save may be loaded next: watch it afresh */
        sPrimed = FALSE;
        return;
    }
    if (++sTick < CHECK_FRAMES)
        return;
    sTick = 0;

    badges = Badges();
    hallOfFame = GetGameStat(GAME_STAT_ENTERED_HOF);
    PartySize(&count, &maxLevel);
    if (sPrimed)
    {
        for (unsigned i = 0; i < AWARD_COUNT; i++)
        {
            const Award *a = &sAwards[i];
            u8 bit = 1 << (a->badge - 1);

            if (sWon[i])
                continue;
            if (a->kind == KIND_BADGE_CHALLENGE && (badges & bit) && !(sBadges & bit)
                && count <= a->partyMax && maxLevel <= a->levelMax)
                Win(i), changed = true;
            else if (a->kind == KIND_HALL_OF_FAME_SOLO && hallOfFame > sHallOfFame && count == 1)
                Win(i), changed = true;
        }
    }
    for (unsigned i = 0; i < AWARD_COUNT; i++)
        if (!sWon[i] && sAwards[i].kind == KIND_STATE && sAwards[i].test())
            Win(i), changed = true;
    sBadges = badges;
    sHallOfFame = hallOfFame;
    sPrimed = TRUE;
    if (changed)
        Save();
}

int CtrAwards_TakeBanner(const char **name, int *also)
{
    int i;

    if (sQueued == 0)
        return -1;
    if (sQueued > BANNER_EACH_MAX)
    {
        /* a save's earlier progress: one banner for it all */
        *name = NULL;
        *also = sQueued;
        i = sQueue[0];
        sQueued = 0;
        return i;
    }
    i = sQueue[0];
    memmove(sQueue, sQueue + 1, --sQueued);
    *name = sAwards[i].name;
    *also = 0;
    return i;
}

unsigned CtrAwards_Count(void) { return AWARD_COUNT; }

unsigned CtrAwards_WonCount(void)
{
    unsigned n = 0;

    if (!sLoaded)
        Load();
    for (unsigned i = 0; i < AWARD_COUNT; i++)
        n += sWon[i];
    return n;
}

bool CtrAwards_Won(unsigned i) { return i < AWARD_COUNT && sWon[i]; }
const char *CtrAwards_Name(unsigned i) { return i < AWARD_COUNT ? sAwards[i].name : ""; }
const char *CtrAwards_Text(unsigned i) { return i < AWARD_COUNT ? sAwards[i].text : ""; }
