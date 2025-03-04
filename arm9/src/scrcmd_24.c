#include "scrcmd.h"
#include "heap.h"
#include "party.h"
#include "unk_020377F0.h"
#include "unk_02088DD8.h"

extern void* FUN_02039438(struct UnkSavStruct80*, int idx);

extern struct UnkStruct_02037CF0* FUN_02037CF0(u32 heap_id, struct UnkSavStruct80*, u8);
extern u8 FUN_02037D5C(struct UnkStruct_02037CF0*);
extern void FUN_02038864(struct UnkSavStruct80*, struct UnkStruct_02088DD8*);
extern BOOL FUN_0203BC04(struct ScriptContext* ctx);
extern struct UnkStruct_02088DD8* FUN_02088DD8(u32 heap_id);
extern void FUN_02088DF0(struct UnkStruct_02037CF0*);
extern void* FUN_02088DF8(struct Pokemon* pokemon, u32 heap_id);
extern BOOL FUN_02088EF8(void*);

THUMB_FUNC BOOL ScrCmd_Unk01C6(struct ScriptContext* ctx)
{
    u16 unk = VarGet(ctx->unk80, ScriptReadHalfword(ctx));
    struct UnkStruct_02037CF0** unk_ret_ptr = FUN_02039438(ctx->unk80, 19);
    *unk_ret_ptr = FUN_02037CF0(32, ctx->unk80, (u8)unk);

    SetupNativeScript(ctx, FUN_0203BC04);
    return TRUE;
}

#ifdef NONMATCHING
THUMB_FUNC BOOL ScrCmd_Unk01C7(struct ScriptContext* ctx)
{
    u16* ret_ptr = GetVarPointer(ctx->unk80, ScriptReadHalfword(ctx));
    struct UnkStruct_02037CF0** unk_ptr = FUN_02039438(ctx->unk80, 19);
    GF_ASSERT(*unk_ptr != NULL);

    *ret_ptr = FUN_02037D5C(*unk_ptr);
    if (*ret_ptr == 4)
    {
        *ret_ptr = 0xFF;
    }

    FreeToHeap(*unk_ptr);
    *unk_ptr = NULL;
    return FALSE;
}
#else
THUMB_FUNC asm BOOL ScrCmd_Unk01C7(struct ScriptContext* ctx)
{
    push {r3-r5, lr}
    add r4, r0, #0x0
    bl ScriptReadHalfword
    add r1, r0, #0x0
    add r0, r4, #0x0
    add r0, #0x80
    ldr r0, [r0, #0x0]
    bl GetVarPointer
    add r4, #0x80
    add r5, r0, #0x0
    ldr r0, [r4, #0x0]
    mov r1, #0x13
    bl FUN_02039438
    add r4, r0, #0x0
    ldr r0, [r4, #0x0]
    cmp r0, #0x0
    bne _02045DFC
    bl GF_AssertFail
_02045DFC:
    ldr r0, [r4, #0x0]
    bl FUN_02037D5C
    strh r0, [r5, #0x0]
    ldrh r0, [r5, #0x0]
    cmp r0, #0x4
    bne _02045E0E
    mov r0, #0xff
    strh r0, [r5, #0x0]
_02045E0E:
    ldr r0, [r4, #0x0]
    bl FreeToHeap
    mov r0, #0x0
    str r0, [r4, #0x0]
    pop {r3-r5, pc}
}
#endif

THUMB_FUNC BOOL ScrCmd_Unk021E(struct ScriptContext* ctx)
{
#pragma unused(ctx)
    return FALSE;
}

THUMB_FUNC BOOL ScrCmd_Unk021F(struct ScriptContext* ctx)
{
    u16* ret_ptr = GetVarPointer(ctx->unk80, ScriptReadHalfword(ctx));
    u16 mon_idx = VarGet(ctx->unk80, ScriptReadHalfword(ctx));
    struct PlayerParty* party = SavArray_PlayerParty_get(ctx->unk80->saveBlock2);
    struct Pokemon* pokemon = GetPartyMonByIndex(party, mon_idx);
    void* unk_ptr = FUN_02088DF8(pokemon, 32);

    *ret_ptr = (u16)FUN_02088EF8(unk_ptr);
    FreeToHeap(unk_ptr);

    return FALSE;
}

THUMB_FUNC void FUN_02045E74(struct ScriptContext* ctx, u8 a1, struct Pokemon* pokemon, void* a3)
{
    struct UnkStruct_02088DD8** unk = FUN_02039438(ctx->unk80, 19);
    struct UnkStruct_02088DD8* unk2 = FUN_02088DD8(32);
    *unk = unk2;

    unk2->pokemon = pokemon;

    struct SaveBlock2* sav2 = ScriptEnvironment_GetSav2Ptr(ctx->unk80);
    unk2->player = Sav2_PlayerData_GetProfileAddr(sav2);

    unk2->options = Sav2_PlayerData_GetOptionsAddr(ctx->unk80->saveBlock2);
    unk2->unkC = a3;
    unk2->unk15 = a1;
    FUN_02038864(ctx->unk80, unk2);

    SetupNativeScript(ctx, FUN_0203BC04);
    FreeToHeap(a3);
}

THUMB_FUNC BOOL ScrCmd_Unk0220(struct ScriptContext* ctx)
{
#pragma unused(ctx)
    return TRUE;
}

THUMB_FUNC BOOL ScrCmd_Unk0221(struct ScriptContext* ctx)
{
    u16 mon_idx = VarGet(ctx->unk80, ScriptReadHalfword(ctx));
    struct PlayerParty* party = SavArray_PlayerParty_get(ctx->unk80->saveBlock2);
    struct Pokemon* pokemon = GetPartyMonByIndex(party, mon_idx);
    void* unk_ptr = FUN_02088DF8(pokemon, 32);

    FUN_02045E74(ctx, 1, pokemon, unk_ptr);
    return TRUE;
}

THUMB_FUNC BOOL ScrCmd_Unk0224(struct ScriptContext* ctx)
{
    u16 mon_idx = VarGet(ctx->unk80, ScriptReadHalfword(ctx));
    u16 unk2 = VarGet(ctx->unk80, ScriptReadHalfword(ctx));
    struct PlayerParty* party = SavArray_PlayerParty_get(ctx->unk80->saveBlock2);
    struct Pokemon* pokemon = GetPartyMonByIndex(party, mon_idx);

    u16* unk_array = AllocFromHeap(32, 2 * sizeof(u16));
    unk_array[0] = unk2;
    unk_array[1] = 0xFFFF;

    FUN_02045E74(ctx, 0, pokemon, unk_array);
    return TRUE;
}

THUMB_FUNC BOOL ScrCmd_Unk0222(struct ScriptContext* ctx)
{
#pragma unused(ctx)
    return FALSE;
}

#ifdef NONMATCHING
THUMB_FUNC BOOL ScrCmd_Unk0223(struct ScriptContext* ctx)
{
    u16* ret_ptr = GetVarPointer(ctx->unk80, ScriptReadHalfword(ctx));
    struct UnkStruct_02037CF0** unk = FUN_02039438(ctx->unk80, 19);
    struct UnkStruct_02037CF0* unk_sub = *unk;

    GF_ASSERT(unk_sub != NULL);

    if (unk_sub->unk16 == 0)
    {
        *ret_ptr = 0;
    }
    else
    {
        *ret_ptr = 0xFF;
    }

    FUN_02088DF0(unk_sub);
    return FALSE;
}
#else
THUMB_FUNC asm BOOL ScrCmd_Unk0223(struct ScriptContext* ctx)
{
    push {r3-r5, lr}
    add r4, r0, #0x0
    bl ScriptReadHalfword
    add r1, r0, #0x0
    add r0, r4, #0x0
    add r0, #0x80
    ldr r0, [r0, #0x0]
    bl GetVarPointer
    add r4, #0x80
    add r5, r0, #0x0
    ldr r0, [r4, #0x0]
    mov r1, #0x13
    bl FUN_02039438
    ldr r4, [r0, #0x0]
    cmp r4, #0x0
    bne _02045FB2
    bl GF_AssertFail
_02045FB2:
    ldrb r0, [r4, #0x16]
    cmp r0, #0x0
    bne _02045FBC
    mov r0, #0x0
    b _02045FBE
_02045FBC:
    mov r0, #0xff
_02045FBE:
    strh r0, [r5, #0x0]
    add r0, r4, #0x0
    bl FUN_02088DF0
    mov r0, #0x0
    pop {r3-r5, pc}
}
#endif

#ifdef NONMATCHING
THUMB_FUNC BOOL ScrCmd_Unk0225(struct ScriptContext* ctx)
{
    u16* ret_ptr = GetVarPointer(ctx->unk80, ScriptReadHalfword(ctx));
    struct UnkStruct_02037CF0** unk = FUN_02039438(ctx->unk80, 19);
    struct UnkStruct_02037CF0* unk_sub = *unk;

    GF_ASSERT(unk_sub != NULL);

    if (unk_sub->unk16 == 0)
    {
        *ret_ptr = 0;
    }
    else
    {
        *ret_ptr = 0xFF;
    }

    FUN_02088DF0(unk_sub);
    return FALSE;
}
#else
THUMB_FUNC asm BOOL ScrCmd_Unk0225(struct ScriptContext* ctx)
{
    push {r3-r5, lr}
    add r4, r0, #0x0
    bl ScriptReadHalfword
    add r1, r0, #0x0
    add r0, r4, #0x0
    add r0, #0x80
    ldr r0, [r0, #0x0]
    bl GetVarPointer
    add r4, #0x80
    add r5, r0, #0x0
    ldr r0, [r4, #0x0]
    mov r1, #0x13
    bl FUN_02039438
    ldr r4, [r0, #0x0]
    cmp r4, #0x0
    bne _02045FB2
    bl GF_AssertFail
_02045FB2:
    ldrb r0, [r4, #0x16]
    cmp r0, #0x0
    bne _02045FBC
    mov r0, #0x0
    b _02045FBE
_02045FBC:
    mov r0, #0xff
_02045FBE:
    strh r0, [r5, #0x0]
    add r0, r4, #0x0
    bl FUN_02088DF0
    mov r0, #0x0
    pop {r3-r5, pc}
}
#endif
