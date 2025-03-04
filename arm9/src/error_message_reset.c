#include "error_message_reset.h"
#include "GX_layers.h"
#include "gx.h"
#include "unk_02031734.h"
#include "unk_0202F150.h"
#include "unk_02016B94.h"
#include "PAD_pad.h"


const struct UnkStruct_02016B94_4 UNK_020FF49C = { 0, 3, 3, 0x1a, 0x12, 1, 0x23 };

const struct HeapParam UNK_020FF4A4[] = {
    {0x00020000, OS_ARENA_MAIN}
};

const struct GraphicsModes UNK_020FF4AC = { dispMode : GX_DISPMODE_GRAPHICS };

const struct UnkStruct_02016B94_1 UNK_020FF4BC = { 0, 0, 0x800, 0, 1, 0, 0, 6, 0, 1, 0, 0, 0 };

const struct GraphicsBanks UNK_020FF4D8 = { bg : 3 };

u32 sErrorMessagePrinterLock;

extern void FUN_0200E3A0(PMLCDTarget, int);
extern void FUN_0200CB00(struct UnkStruct_02016B94_2 *param0, u32 param1, u32 param2, u32 param3, u8 param4, u32 param5);
extern void FUN_02002ED0(u32 param0, u32 param1, u32 param2);
extern void FUN_0200CCA4(struct Window *param0, u32 param1, u32 param2, u32 param3);
extern void FUN_0200E394(u32 param0);
extern void FUN_0200A274(u32 param0, u32 param1, u32 param2);

THUMB_FUNC void VBlankHandler()
{
    *(vu32 *)HW_INTR_CHECK_BUF |= 1;

    MI_WaitDma(3);
}

THUMB_FUNC void PrintErrorMessageAndReset()
{

    struct UnkStruct_02016B94_2 *ptr;
    struct Window buf;

    if (sErrorMessagePrinterLock != 1)
    {
        sErrorMessagePrinterLock = 1;
        OS_SetArenaHi(OS_ARENA_MAIN, OS_GetInitArenaHi(OS_ARENA_MAIN));
        OS_SetArenaLo(OS_ARENA_MAIN, OS_GetInitArenaLo(OS_ARENA_MAIN));

        InitHeapSystem(UNK_020FF4A4, NELEMS(UNK_020FF4A4), 1, 0);
        FUN_0200E3A0(PM_LCD_TOP, 0);
        FUN_0200E3A0(PM_LCD_BOTTOM, 0);

        OS_DisableIrqMask(OS_IE_V_BLANK);
        OS_SetIrqFunction(OS_IE_V_BLANK, &VBlankHandler);
        OS_EnableIrqMask(OS_IE_V_BLANK);

        Main_SetVBlankIntrCB(NULL, NULL);

        FUN_02015F34(NULL, NULL);
        GX_DisableEngineALayers();
        GX_DisableEngineBLayers();

        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;

        SetKeyRepeatTimers(4, 8);

        gMain.unk65 = 0;
        GX_SwapDisplay();

        reg_G2_BLDCNT = 0;
        reg_G2S_DB_BLDCNT = 0;
        reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_OW_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_W0_MASK);
        reg_GXS_DB_DISPCNT &= ~(REG_GXS_DB_DISPCNT_OW_MASK | REG_GXS_DB_DISPCNT_W1_MASK | REG_GXS_DB_DISPCNT_W0_MASK);

        GX_SetBanks(&UNK_020FF4D8);
        ptr = FUN_02016B94(0);
        FUN_02016BBC(&UNK_020FF4AC);

        FUN_02016C18(ptr, 0, &UNK_020FF4BC, 0);
        FUN_02018744(ptr, 0);

        FUN_0200CB00(ptr, 0, 503, 2, 0, 0);

        FUN_02002ED0(0, 0x20, 0);
        FUN_02017F18(0, 0x20, 0, 0);
        FUN_02017FE4(0, 0x6C21);
        FUN_02017FE4(4, 0x6C21);

        struct MsgData *msg_data = NewMsgDataFromNarc(1, NARC_MSGDATA_MSG, 0xc8, 0);
        struct String *str = String_ctor(6 << 6, 0);

        FUN_0201BD5C();
        FUN_02019150(ptr, &buf, &UNK_020FF49C);
        FillWindowPixelRect(&buf, 15, 0, 0, 0xd0, 0x90);
        FUN_0200CCA4(&buf, 0, 0x1f7, 2);

        ReadMsgDataIntoString(msg_data, 3, str);

        AddTextPrinterParameterized(&buf, 0, str, 0, 0, 0, NULL);

        String_dtor(str);
        GX_BothDispOn();
        FUN_0200E394(0);
        FUN_0200E394(1);
        FUN_0200A274(0, 0x3f, 3);
        FUN_02032DAC();

        while (1)
        {
            HandleDSLidAction();
            FUN_0202FB80();
            if (FUN_02033678())
                break;
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        }

        while (1)
        {
            HandleDSLidAction();
            if ((PAD_Read() & PAD_BUTTON_A))
                break;
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        }
        FUN_0200E3A0(PM_LCD_TOP, 0x7FFF);
        FUN_0200E3A0(PM_LCD_BOTTOM, 0x7FFF);

        FUN_02019178(&buf);

        DestroyMsgData(msg_data);
        FreeToHeap(ptr);

        OS_ResetSystem(0);
    }
}
