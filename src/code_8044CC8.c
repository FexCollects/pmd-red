#include "global.h"
#include "constants/dungeon_action.h"
#include "dungeon_map_access.h"
#include "dungeon_util.h"
#include "code_8044CC8.h"
#include "items.h"

// size: 0x8
typedef struct ItemText
{
    u8 *desc;
    u8 *useText;
} ItemText;

// size: 0x8
typedef struct unkStr_80F7C54
{
    u32 unk0;
    u8 *text;
} unkStr_80F7C54;



EWRAM_DATA unkStruct_202EE44 gUnknown_202EE44[10] = {0};

extern s32 gUnknown_202EE6C;

extern const ItemText gActions[];
extern u16 gUnknown_80F6964[NUM_ITEM_CATEGORIES];
extern u8 gUnknown_80F697C[];
extern u8 *gUnknown_80F7C50[10];
extern const unkStr_80F7C54 gUnknown_80F7C54[65];
extern u8 *gUnknown_80F91EC[];
extern bool8 sub_8045888(Entity *);
extern u8 GetFloorType(void);
void sub_80460F8(DungeonPos *, Item *, u32);
bool8 sub_80461C8(DungeonPos *, u32);

Item * sub_8044CC8(Entity *param_1, ActionParameter *param_2, UNUSED s32 a3)
{
  const Tile *tile;
  Item *item;
  EntityInfo *info;

  if ((u8)(param_2->actionUseIndex - 1) < INVENTORY_SIZE) {
    item = &gTeamInventoryRef->teamItems[param_2->actionUseIndex - 1];
  }
  else if (param_2->actionUseIndex == 0x80) {
    tile = GetTile((param_2->itemPos).x,(param_2->itemPos).y);
    item = GetItemData(tile->object);
  }
  else {
    if (param_2->actionUseIndex == 0x81) {
      info = GetEntInfo(param_1);
    }
    else {
      u8 id = param_2->actionUseIndex - 0x90;
      if (3 < id) {
        return NULL;
      }
      info = GetEntInfo(gDungeon->teamPokemon[param_2->actionUseIndex - 0x90]);
    }
    item = &info->heldItem;
  }
  return item;
}

bool8 sub_8044D40(ActionContainer *param_1,s32 index)
{
  Item *item;
  ActionParameter *puVar1;

  puVar1 = &param_1->actionParameters[index];
  if ((u8)(puVar1->actionUseIndex - 1) < INVENTORY_SIZE) {
    item = &gTeamInventoryRef->teamItems[puVar1->actionUseIndex - 1];
    item->id = ITEM_NOTHING;
    item->quantity = 0;
    item->flags = 0;
    FillInventoryGaps();
  }
  else {
    if (puVar1->actionUseIndex != 0x80) {
      return FALSE;
    }
    sub_80461C8(&puVar1->itemPos,1);
  }
  return TRUE;
}

Item *sub_8044D90(Entity *entity, s32 index, s32 unused)
{
    EntityInfo *info = GetEntInfo(entity);
    return sub_8044CC8(entity, &info->action.actionParameters[index], unused);
}

Entity *sub_8044DA4(Entity *entity, s32 index)
{
    EntityInfo *info = GetEntInfo(entity);
    return gDungeon->teamPokemon[info->action.actionParameters[index].actionUseIndex];
}

u16 sub_8044DC8(Item *param_1)
{
  if (param_1->id == ITEM_SWITCH_BOX) {
    return 0x35;
  }
  else {
    return gUnknown_80F6964[GetItemCategory(param_1->id)];
  }
}

void sub_8044DF0(Entity *entity, s32 index, u32 unused)
{
  Item *item;
  EntityInfo *info;

  info = GetEntInfo(entity);
  item = sub_8044D90(entity,index,unused);
  if ((info->action).actionParameters[0].actionUseIndex == 0x80) {
    sub_80461C8(&(info->action).actionParameters[0].itemPos,1);
  }
  else {
    item->id = ITEM_NOTHING;
    item->quantity = 0;
    item->flags = 0;
    FillInventoryGaps();
  }
}


void sub_8044E24(Entity *entity,int index,u32 unused)
{
  Item *itemPtr;
  EntityInfo *info;
  DungeonPos *pos;
  Item item;

  itemPtr = sub_8044D90(entity,index,unused);
  info = GetEntInfo(entity);
  if (!IsHMItem(itemPtr->id)) {
    if (GetItemCategory(itemPtr->id) == CATEGORY_TMS_HMS) {
      if (info->action.actionParameters[index].actionUseIndex == 0x80) {
        item = *itemPtr;
        pos = &info->action.actionParameters[index].itemPos;
        sub_80461C8(pos,1);
        item.quantity = itemPtr->id - 125;
        item.id = ITEM_TM_USED_TM;
        sub_80460F8(pos,&item,1);
      }
      else {
        itemPtr->quantity = itemPtr->id - 125;
        itemPtr->id = ITEM_TM_USED_TM;
      }
    }
    else {
      sub_8044DF0(entity,index,unused);
    }
  }
}

// Similar to sub_8044BA8
u8 *sub_8044EC8(s32 param_1)
{
    u16 uVar1;
    u32 uVar3;
    u32 uVar4;

    uVar1 = gUnknown_202EE44[param_1].unk0;

    if ((uVar1 == 0x26) && (GetFloorType() == 2)) {
        return *gUnknown_80F91EC;
    }
    else {
        uVar3 = uVar4 = strcmp(gUnknown_80F7C50[uVar1 << 1], gUnknown_80F697C);
        if (uVar3 != 0) {
            uVar4 = 1;
        }
        if ((u8)(uVar4) == 0) {
            return gActions[GetItemActionType(gUnknown_202EE44[param_1].unk2)].useText;
        }
        else
        {
            return gUnknown_80F7C50[uVar1 << 1];
        }
    }
}

bool8 sub_8044F3C(s32 param_1)
{
    if(param_1 < 0)
        return FALSE;
    else
        return gUnknown_202EE44[param_1].unk3;
}

void sub_8044F5C(u16 param_1, u8 param_2)
{
  int index;

  if (gUnknown_202EE6C < 10) {
    for(index = 0; index < gUnknown_202EE6C; index++)
    {
        if (gUnknown_202EE44[index].unk0 == param_1) {
          return;
        }
    }
    gUnknown_202EE44[gUnknown_202EE6C].unk0 = param_1;
    gUnknown_202EE44[gUnknown_202EE6C].unk2 = param_2;
    gUnknown_202EE44[gUnknown_202EE6C].unk3 = TRUE;
    gUnknown_202EE6C++;
  }
}

s32 sub_8044FB4(u16 param_1)
{
    int index;

    for(index = 0; index < gUnknown_202EE6C; index++)
    {
        if (gUnknown_202EE44[index].unk0 == param_1) {
            return index;
        }
    }
    return -1;
}

void sub_8044FF0(u16 param_1)
{
    int index;

    for(index = 0; index < gUnknown_202EE6C; index++)
    {
        if (gUnknown_202EE44[index].unk0 == param_1) {
           gUnknown_202EE44[index].unk3 = FALSE;
           return;
        }
    }
}

bool8 IsNotAttacking(Entity *param_1, bool8 param_2)
{
    EntityInfo *info;
    u16 *action;

    info = GetEntInfo(param_1);
    action = &(info->action).action;
    if ((param_2 == 0) || (sub_8045888(param_1)))
    {
        if(*action == ACTION_NOTHING)
            return TRUE;
        if (*action == ACTION_WALK)
            return TRUE;
        if (*action == ACTION_PASS_TURN)
            return TRUE;
        if (*action == ACTION_UNK5)
            return TRUE;
        return FALSE;
    }
    else {
        return TRUE;
    }
}

void sub_8045064(void)
{
    s32 i;
    s32 j;
    unkStruct_202EE44 *iPtr;
    unkStruct_202EE44 *jPtr;
    unkStruct_202EE44 temp;

    for (i = 0; i < gUnknown_202EE6C; i++) {
        for (j = i + 1; j < gUnknown_202EE6C; j++) {
            iPtr = &gUnknown_202EE44[i];
            jPtr = &gUnknown_202EE44[j];
            if ((s32)gUnknown_80F7C54[iPtr->unk0].unk0 > (s32)gUnknown_80F7C54[jPtr->unk0].unk0) {
                temp = *iPtr;
                *iPtr = *jPtr;
                *jPtr = temp;
            }
        }
    }
}
