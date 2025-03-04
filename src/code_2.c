#include "global.h"
#include "bg_control.h"
#include "bg_palette_buffer.h"
#include "code_800558C.h"
#include "code_8009804.h"
#include "code_800C9CC.h"
#include "code_80118A4.h"
#include "string_format.h"
#include "code_8094F88.h"
#include "code_8097670.h"
#include "constants/bg_music.h"
#include "constants/main_menu.h"
#include "cpu.h"
#include "debug.h"
#include "event_flag.h"
#include "exclusive_pokemon.h"
#include "file_system.h"
#include "friend_area.h"
#include "game_options.h"
#include "main_menu1.h"
#include "main_menu2.h"
#include "memory.h"
#include "moves.h"
#include "music.h"
#include "play_time.h"
#include "pokemon.h"
#include "rescue_team_info.h"
#include "save_read.h"
#include "save.h"
#include "sprite.h"
#include "text.h"

extern void NDS_LoadOverlay_GroundMain(void);
extern void sub_8014144(void);
extern void sub_800DAAC(void);
extern void xxx_script_related_8001334(u32);
extern void LoadTitleScreen(void);
extern void nullsub_33(void);
extern u32 sub_80009D0(u32);

extern u16 gUnknown_2026E4E;
EWRAM_DATA s32 sTitleBrightness = 0;

// TODO: figure out the struct
struct UnkStruct203B040
{
    u8 unk0[48];
};

EWRAM_INIT OpenedFile *gTitlePaletteFile = NULL;
extern u32 gUnknown_203B03C;

extern char gPMDBuildVersion[];

void xxx_update_stuff(u32);

void GameLoop(void)
{
    u32 tmp;
    u8 tmp3 = 1;
    u32 nextMenu;
    u32 flag;

    InitHeap();
    NDS_DebugInit();
    sub_801180C();
    NDS_LoadOverlay_GroundMain();
    sub_8014144();
    LoadMonsterParameters();
    sub_8097670();
    InitializePlayTime();
    LoadGameOptions();
    SetWindowBGColor();
    LoadExclusivePokemon();
    LoadFriendAreas();
    LoadItemParameters();
    LoadRescueTeamInfo();
    LoadWazaParameters();
    sub_80950BC();
    LoadMailInfo();
    sub_800CD64(0x8000, FALSE);
    sub_8012284();
    InitializeGameOptions(TRUE);
    SetWindowTitle(gPMDBuildVersion);
    sub_800DAAC();
    SetSavingIconCoords(NULL);
    tmp = 0;
    if (ReadSaveFromPak(&tmp))
        ThoroughlyResetScriptVars();
    xxx_script_related_8001334(14);
    xxx_script_related_8001334(15);
    ClearScriptVarArray(NULL, EVENT_LOCAL);
    xxx_script_related_8001334(16);
    if (GetScriptVarValue(NULL, EVENT_LOCAL) == 0)
        xxx_script_related_8001334(17);

    while (1) {
        gUnknown_203B03C = 0; // ???
        sub_800A8F8(2);
        ResetSprites(TRUE);
        UpdateFadeInTile(0);
        InitFontPalette();
        sub_800CDA8(2);
        ShowWindows(NULL, TRUE, TRUE);
        gUnknown_2026E4E = 0x1000;
        LoadTitleScreen();
        SetBG2RegOffsets(0, 0);
        SetBG3RegOffsets(0, 0);
        sTitleBrightness = 0;
        while (sTitleBrightness < 32) {
            s32 i;
            sTitleBrightness++;
            for (i = 0; i < 240; i++)
                SetBGPaletteBufferColorRGB(i, &((RGB*)gTitlePaletteFile->data)[i], sTitleBrightness, NULL);
            xxx_update_stuff(0);
        }
        if (tmp3) {
            PrepareSavePakRead();
            while ((tmp3 = ReadSavePak()))
                xxx_update_stuff(0);
            FinishReadSavePak();
        }
        tmp3 = 1;
        StartNewBGM(MUS_FILE_SELECT);
        flag = TRUE;
        sub_80095CC(0, 20);
        InitMainMenu();
        while (1) {
            if (sub_80363E0()) {
                if (gRealInputs.pressed & L_BUTTON) {
                    flag = FALSE;
                    SetBGOBJEnableFlags(19);
                } else if (!flag && (gRealInputs.held & L_BUTTON) == 0) {
                    flag = TRUE;
                    SetBGOBJEnableFlags(0);
                }
                if (!flag) {
                    xxx_update_stuff(0);
                    continue;
                }
            }
            SetUpMenu();
            xxx_update_stuff(0);
            nextMenu = UpdateMenu();
            CleanUpMenu();
            if (nextMenu == 2) break;
            if (nextMenu == 4) break;
            if (nextMenu == MENU_NEW_GAME) break;
        }
        DeleteMainMenu();
        while (sTitleBrightness > 0) {
            s32 i;
            sTitleBrightness--;
            for (i = 0; i < 240; i++)
                SetBGPaletteBufferColorRGB(i, &((RGB*)gTitlePaletteFile->data)[i], sTitleBrightness, NULL);
            xxx_update_stuff(0);
        }
        CloseFile(gTitlePaletteFile);
        switch (nextMenu) {
            case 2: {
                s32 mailIndex = GetFirstIndexofMailType(7);
                if (mailIndex != -1) {
                    DeleteMailAtIndex(mailIndex);
                    tmp3 = sub_80009D0(2);
                } else {
                    tmp3 = sub_80009D0(1);
                }
                break;
            }
            case 4:
                tmp3 = sub_80009D0(3);
                break;
            case MENU_NEW_GAME:
                sub_80122A8();
                nullsub_33();
                tmp3 = sub_80009D0(0);
                break;
        }

    }
}

void xxx_update_stuff(u32 r0)
{
    DrawDialogueBoxString();
    sub_8005838(NULL, 0);
    nullsub_8(gGameOptionsRef->unkA);
    sub_8005180();
    sub_80060EC();
    sub_8011860();
    sub_800CB20();
    LoadBufferedInputs();
    CopySpritesToOam();
    sub_8005304();
    TransferBGPaletteBuffer();
    xxx_call_update_bg_vram();
    sub_8009908();
    xxx_call_update_bg_sound_input();
    ResetSprites(FALSE);
}
