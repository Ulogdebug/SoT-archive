#include <uLib.h>
#include "code/z_sram.h"
#include "state/state.h"
asm ("hackSavePlayerData = 0x8012A4F8");
asm ("hackSaveEquips = 0x8012A544");
asm ("hackSaveInventory = 0x8012A550");
asm ("sNewSaveChecksum = 0x8012A5B0");
asm ("sDungeonEntrances = 0x8012A670");

Asm_VanillaHook(Sram_InitNewSave);
void Sram_InitNewSave(void) {
    SaveContext* temp = &gSaveContext;
    SavePlayerData hackSavePlayerData = {
        { '\0', '\0', '\0', '\0', '\0', '\0' },             // newf
        0,                                                  // deaths
        { 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E }, // playerName
        0,                                                  // n64ddFlag
        0x50,                                               // healthCapacity
        0x50,                                               // defense
        1,                                                  // magicLevel
        MAGIC_NORMAL_METER,                                 // magic
        5,                                                  // rupees
        KNIFLE_HEALTH,                                       // swordHealth
        0,                                                  // naviTimer
        true,                                              // isMagicAcquired
        0,                                                  // unk_1F
        false,                                              // isDoubleMagicAcquired
        false,                                              // isDoubleDefenseAcquired
        0,                                                  // bgsFlag
        0,                                                  // ocarinaGameRoundNum
        {
            { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
            { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
            0,                                              // equipment
        },                                                  // childEquips
        {
            { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
            { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
            0,                                              // equipment
        },                                                  // adultEquips
        0,                                                  // unk_38
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },       // unk_3C
        Patch_SaveStartSceneID,                                    // savedSceneId
    };

    Inventory hackSaveInventory = {
        // items
        {
            ITEM_NONE, // SLOT_STICK
            ITEM_NONE, // SLOT_NUT
            ITEM_NONE, // SLOT_BOMB
            ITEM_BOW, // SLOT_BOW
            ITEM_NONE, // SLOT_ARROW_FIRE
            ITEM_NONE, // SLOT_DINS_FIRE
            ITEM_NONE, // SLOT_SLINGSHOT
            ITEM_OCARINA_FAIRY, // SLOT_OCARINA
            ITEM_NONE, // SLOT_BOMBCHU
            ITEM_NONE, // SLOT_HOOKSHOT
            ITEM_NONE, // SLOT_ARROW_ICE
            ITEM_NONE, // SLOT_FARORES_WIND
            ITEM_NONE, // SLOT_BOOMERANG
            ITEM_NONE, // SLOT_LENS
            ITEM_NONE, // SLOT_BEAN
            ITEM_NONE, // SLOT_HAMMER
            ITEM_NONE, // SLOT_ARROW_LIGHT
            ITEM_NONE, // SLOT_NAYRUS_LOVE
            ITEM_NONE, // SLOT_BOTTLE_1
            ITEM_NONE, // SLOT_BOTTLE_2
            ITEM_NONE, // SLOT_BOTTLE_3
            ITEM_NONE, // SLOT_BOTTLE_4
            ITEM_NONE, // SLOT_TRADE_ADULT
            ITEM_NONE, // SLOT_TRADE_CHILD
        },
        // ammo
        {
            0, // SLOT_STICK
            0, // SLOT_NUT
            0, // SLOT_BOMB
            15, // SLOT_BOW
            0, // SLOT_ARROW_FIRE
            0, // SLOT_DINS_FIRE
            0, // SLOT_SLINGSHOT
            0, // SLOT_OCARINA
            0, // SLOT_BOMBCHU
            1, // SLOT_HOOKSHOT
            0, // SLOT_ARROW_ICE
            0, // SLOT_FARORES_WIND
            0, // SLOT_BOOMERANG
            0, // SLOT_LENS
            0, // SLOT_BEAN
            0, // SLOT_HAMMER
        },
        // equipment
        (((1 << EQUIP_INV_TUNIC_KOKIRI) << (EQUIP_TYPE_TUNIC * 4)) |
        ((1 << EQUIP_INV_BOOTS_KOKIRI) << (EQUIP_TYPE_BOOTS * 4))),
        00001101,                                                              // upgrades
        0,                                                              // questItems
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // dungeonItems
        {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        }, // dungeonKeys
        0, // defenseHearts
        0, // gsTokens
    };
    ItemEquips hackSaveEquips = {
        { ITEM_SWORD_MASTER, ITEM_BOW, ITEM_OCARINA_FAIRY, ITEM_NONE }, // buttonItems
        { ITEM_BOW, ITEM_OCARINA_FAIRY, ITEM_NONE },                          // cButtonSlots
        0x1100,
    };

    //gSaveContext.zTargetSetting = 1;

    bzero(&SAVE_INFO, sizeof(SaveInfo));
    gSaveContext.totalDays = 0;
    gSaveContext.bgsDayCount = 0;

    SAVE_PLAYER_DATA = hackSavePlayerData;
    gSaveContext.equips = hackSaveEquips;
    gSaveContext.inventory = hackSaveInventory;
    //ghackSave.modernAimingControl = 1;
    temp->checksum = sNewSaveChecksum;
    gSaveContext.horseData.sceneId = SCENE_SPOT00;
    gSaveContext.horseData.pos.x = -1840;
    gSaveContext.horseData.pos.y = 72;
    gSaveContext.horseData.pos.z = 5497;
    gSaveContext.horseData.angle = -0x6AD9;
    gSaveContext.magicLevel = 1;
    gSaveContext.infTable[INFTABLE_1DX_INDEX] = 1;
    gSaveContext.sceneFlags[5].swch = 0x40000000;

    gSaveContext.entranceIndex = 0x8000;
    gExitParam.nextEntranceIndex = 0x8000;
    gExitParam.exit = (NewExit) {
        .sceneIndex = 0x37,//Bar
        .spawnIndex = 0x0,
        .headerIndex = 0x0,
        .fadeIn = 2,
        .fadeOut = 2,
    };
}

  Asm_VanillaHook(Sram_OpenSave);
  void Sram_OpenSave(SramContext* sramCtx) {
      u16 i;
      u16 j;
      s16 warpSceneID ;
      s16 warpSpawnIndex ;
      u8* ptr;
      i = gSramSlotOffsets[gSaveContext.fileNum];

      MemCpy(&gSaveContext, sramCtx->readBuff + i, sizeof(Save));
      switch (gSaveContext.savedSceneId) {
          case 0x0:
              warpSceneID = 0x62;
              warpSpawnIndex = 0x0;
              break;
          case 0x1:
          case 0x2:
          case 0x3:
          case 0x5:
          case 0x6:
          case 0xA:
          case 0xB:
          case 0xC:
          case 0xD:
          case 0x10:
          case 0x42:
          case 0x60:
          case 0x68:
          case 0x62:
          case 0x4E:
          case 0x3F:
          case 0x55:
          case 0x25:
          case 0x21:
          case 0x5A:
              warpSceneID = gSaveContext.savedSceneId;
              warpSpawnIndex = 0x0;
              break;
          case 0x4:
              warpSceneID = gSaveContext.savedSceneId;
              warpSpawnIndex = 0x1;
              break;
          case 0x0E:
          case 0x9:
              warpSceneID = 0x9;
              warpSpawnIndex = 0x0;
              break;
          case 0x36:
              //gSaveContext.entranceIndex = sDungeonEntrances[gSaveContext.savedSceneId];
              warpSceneID = gSaveContext.savedSceneId;
              warpSpawnIndex = 0x0;
              break;
          case 0x08:
          case 0x07:
              warpSceneID = 0x25;//Library
              warpSpawnIndex = 0x3;
              break;     
          case 0x48:
          case 0x49:
          case 0x4B:
              warpSceneID = 0x23;//Fun Barrel
              warpSpawnIndex = 0x0;
              break;
          case 0x39:
          case 0x47:
              warpSceneID = 0x4E;//Goron shop
              warpSpawnIndex = 0;  
              break;
          case 0x4C:
          case 0x2D:
          case 0x5B:
              warpSceneID = 0x55; //Swamphell
              warpSpawnIndex = 0;
              break;
          case 0x3E:
          case 0x61:
              warpSceneID = 0x60; //Moblin Moutain
              warpSpawnIndex = 0;
              break;          
          case 0x22:
          case 0x6A:
              warpSceneID = 0x21; //Train station
              warpSpawnIndex = 0;
              break;
          case 0x3A:
          case 0x65:
              warpSceneID = 0x5A; //Border Fortress
              warpSpawnIndex = 0;
              break;
          case 0x15: //BossSce-SundownCastles
              warpSceneID = 0x04;
              warpSpawnIndex = 0x1;
              break;
          case 0x14: //BoosSce-TheXFactory
              warpSceneID = 0x03;
              warpSpawnIndex = 0x1;
              break;
          case 0x1E: //BossSec-GanonTomb 
              warpSceneID = 0x06;
              warpSpawnIndex = 0x0;
              break;
          case 0x53: //BossSec-Sweatshop
              warpSceneID = 0x05;
              warpSpawnIndex = 0x1;
              break;  
          case 0x18: //Death Temple Boss room
          case 0x69: //Triforces
              warpSceneID = 0x07;
              warpSpawnIndex = 0x1;
              break;
          default:
                warpSceneID = 0x37;
                warpSpawnIndex = 0x0;
              break;
      }

    gSaveContext.entranceIndex = 0x8000;
    gExitParam.nextEntranceIndex = 0x8000;
    gExitParam.exit = (NewExit) {
        .sceneIndex = warpSceneID,
        .spawnIndex = warpSpawnIndex,
        .headerIndex = 0x0,
        .fadeIn = 2,
        .fadeOut = 2,
    };

      osSyncPrintf("scene_no = %d\n", gSaveContext.entranceIndex);
      osSyncPrintf(VT_RST);
      if (gSaveContext.health < gSaveContext.healthCapacity) {
          gSaveContext.health = gSaveContext.healthCapacity;
      }
      if (gSaveContext.scarecrowLongSongSet) {
          osSyncPrintf(VT_FGCOL(BLUE));
          osSyncPrintf("\n====================================================================\n");
          MemCpy(gScarecrowLongSongPtr, gSaveContext.scarecrowLongSong, sizeof(gSaveContext.scarecrowLongSong));
          ptr = (u8*)gScarecrowLongSongPtr;
          for (i = 0; i < ARRAY_COUNT(gSaveContext.scarecrowLongSong); i++, ptr++) {
              osSyncPrintf("%d, ", *ptr);
          }
          osSyncPrintf("\n====================================================================\n");
          osSyncPrintf(VT_RST);
      }
      if (gSaveContext.scarecrowSpawnSongSet) {
          osSyncPrintf(VT_FGCOL(GREEN));
          osSyncPrintf("\n====================================================================\n");
          MemCpy(gScarecrowSpawnSongPtr, gSaveContext.scarecrowSpawnSong, sizeof(gSaveContext.scarecrowSpawnSong));
          ptr = gScarecrowSpawnSongPtr;
          for (i = 0; i < ARRAY_COUNT(gSaveContext.scarecrowSpawnSong); i++, ptr++) {
              osSyncPrintf("%d, ", *ptr);
          }
          osSyncPrintf("\n====================================================================\n");
          osSyncPrintf(VT_RST);
      }
    //   // if zelda cutscene has been watched but lullaby was not obtained, restore cutscene and take away letter
    //   if (GET_EVENTCHKINF(EVENTCHKINF_40) && !CHECK_QUEST_ITEM(QUEST_SONG_LULLABY)) {
    //       i = gSaveContext.eventChkInf[EVENTCHKINF_40_INDEX] & ~EVENTCHKINF_40_MASK;
    //       gSaveContext.eventChkInf[EVENTCHKINF_40_INDEX] = i;
    //       INV_CONTENT(ITEM_LETTER_ZELDA) = ITEM_CHICKEN;
    //       for (j = 1; j < 4; j++) {
    //           if (gSaveContext.equips.buttonItems[j] == ITEM_LETTER_ZELDA) {
    //               gSaveContext.equips.buttonItems[j] = ITEM_CHICKEN;
    //           }
    //       }
    //   }
      if (LINK_AGE_IN_YEARS == YEARS_ADULT && !CHECK_OWNED_EQUIP(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER)) {
          gSaveContext.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER);
          gSaveContext.equips.buttonItems[0] = ITEM_SWORD_MASTER;
          gSaveContext.equips.equipment &= ~(0xF << (EQUIP_TYPE_SWORD * 4));
          gSaveContext.equips.equipment |= EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4);
      }
      for (i = 0; i < ARRAY_COUNT(gSpoilingItems); i++) {
          if (INV_CONTENT(ITEM_TRADE_ADULT) == gSpoilingItems[i]) {
              INV_CONTENT(gSpoilingItemReverts[i]) = gSpoilingItemReverts[i];
              for (j = 1; j < 4; j++) {
                  if (gSaveContext.equips.buttonItems[j] == gSpoilingItems[i]) {
                      gSaveContext.equips.buttonItems[j] = gSpoilingItemReverts[i];
                  }
              }
          }
      }
      gSaveContext.magicLevel = 0;
  }

  Asm_VanillaHook(Sram_InitDebugSave);
void Sram_InitDebugSave(void) {
    SaveContext* temp = &gSaveContext;

    SavePlayerData hackDebugSavePlayerData = {
        { 'Z', 'E', 'L', 'D', 'A', 'Z' },                   // newf
        0,                                                  // deaths
        { 0x15, 0x12, 0x17, 0x14, 0x3E, 0x3E, 0x3E, 0x3E }, // playerName ( "LINK" )
        0,                                                  // n64ddFlag
        0xE0,                                               // healthCapacity
        0xE0,                                               // health
        1,                                                  // magicLevel
        MAGIC_NORMAL_METER,                                 // magic
        150,                                                // rupees
        KNIFLE_HEALTH,                                                  // swordHealth
        0,                                                  // naviTimer
        true,                                               // isMagicAcquired
        0,                                                  // unk_1F
        false,                                              // isDoubleMagicAcquired
        false,                                              // isDoubleDefenseAcquired
        1,                                                  // bgsFlag
        0,                                                  // ocarinaGameRoundNum
        {
            { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
            { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
            0,                                              // equipment
        },                                                  // childEquips
        {
            { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
            { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
            0,                                              // equipment
        },                                                  // adultEquips
        0,                                                  // unk_38
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },       // unk_3C
        Patch_SaveStartSceneID,                                 // savedSceneId
    };
    ItemEquips hackDebugSaveEquips = {
        { ITEM_SWORD_MASTER, ITEM_BOW, ITEM_BOMB, ITEM_OCARINA_FAIRY }, // buttonItems
        { SLOT_BOW, SLOT_BOMB, SLOT_OCARINA },                          // cButtonSlots
        // equipment
        (EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4)) | (EQUIP_VALUE_SHIELD_HYLIAN << (EQUIP_TYPE_SHIELD * 4)) |
            (EQUIP_VALUE_TUNIC_KOKIRI << (EQUIP_TYPE_TUNIC * 4)) | (EQUIP_VALUE_BOOTS_KOKIRI << (EQUIP_TYPE_BOOTS * 4)),
    };

    Inventory hackDebugSaveInventory = {
    // items
    {
            ITEM_NONE,          // SLOT_DEKU_STICK
            ITEM_NUT,            // SLOT_DEKU_NUT
            ITEM_BOMB,                // SLOT_BOMB
            ITEM_BOW,                 // SLOT_BOW
            ITEM_ARROW_FIRE,          // SLOT_ARROW_FIRE
            ITEM_NONE,           // SLOT_DINS_FIRE
            ITEM_NONE,           // SLOT_SLINGSHOT
            ITEM_OCARINA_FAIRY,       // SLOT_OCARINA
            ITEM_BOMBCHU,             // SLOT_BOMBCHU
            ITEM_HOOKSHOT,            // SLOT_HOOKSHOT
            ITEM_NONE,           // SLOT_ARROW_ICE
            ITEM_NONE,        // SLOT_FARORES_WIND
            ITEM_BOOMERANG,           // SLOT_BOOMERANG
            ITEM_NONE,       // SLOT_LENS_OF_TRUTH
            ITEM_NONE,          // SLOT_MAGIC_BEAN
            ITEM_HAMMER,              // SLOT_HAMMER
            ITEM_NONE,         // SLOT_ARROW_LIGHT
            ITEM_NONE,         // SLOT_NAYRUS_LOVE
            ITEM_BOTTLE,        // SLOT_BOTTLE_1
            ITEM_POTION_RED,   // SLOT_BOTTLE_2
            ITEM_POTION_GREEN, // SLOT_BOTTLE_3
            ITEM_POTION_BLUE,  // SLOT_BOTTLE_4
            ITEM_NONE,          // SLOT_TRADE_ADULT
            ITEM_NONE,           // SLOT_TRADE_CHILD
        },
        // ammo
        {
            50, // SLOT_DEKU_STICK
            50, // SLOT_DEKU_NUT
            10, // SLOT_BOMB
            30, // SLOT_BOW
            1,  // SLOT_ARROW_FIRE
            1,  // SLOT_DINS_FIRE
            30, // SLOT_SLINGSHOT
            1,  // SLOT_OCARINA
            50, // SLOT_BOMBCHU
            1,  // SLOT_HOOKSHOT
            1,  // SLOT_ARROW_ICE
            1,  // SLOT_FARORES_WIND
            1,  // SLOT_BOOMERANG
            1,  // SLOT_LENS_OF_TRUTH
            1,  // SLOT_MAGIC_BEAN
            1   // SLOT_HAMMER
        },
        // equipment
        ((((1 << EQUIP_INV_SWORD_KOKIRI) << (EQUIP_TYPE_SWORD * 4)) |
          ((1 << EQUIP_INV_SWORD_MASTER) << (EQUIP_TYPE_SWORD * 4)) |
          ((1 << EQUIP_INV_SWORD_BGS) << (EQUIP_TYPE_SWORD * 4))) |
         (((1 << EQUIP_INV_SHIELD_DEKU) << (EQUIP_TYPE_SHIELD * 4)) |
          ((1 << EQUIP_INV_SHIELD_HYLIAN) << (EQUIP_TYPE_SHIELD * 4)) |
          ((1 << EQUIP_INV_SHIELD_MIRROR) << (EQUIP_TYPE_SHIELD * 4))) |
         (((1 << EQUIP_INV_TUNIC_KOKIRI) << (EQUIP_TYPE_TUNIC * 4)) |
          ((1 << EQUIP_INV_TUNIC_GORON) << (EQUIP_TYPE_TUNIC * 4)) |
          ((1 << EQUIP_INV_TUNIC_ZORA) << (EQUIP_TYPE_TUNIC * 4))) |
         (((1 << EQUIP_INV_BOOTS_KOKIRI) << (EQUIP_TYPE_BOOTS * 4)) |
          ((1 << EQUIP_INV_BOOTS_IRON) << (EQUIP_TYPE_BOOTS * 4)) |
          ((1 << EQUIP_INV_BOOTS_HOVER) << (EQUIP_TYPE_BOOTS * 4)))),
        0x125249,                                                       // upgrades
        0,                                                      // questItems
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // dungeonItems
        { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // dungeonKeys
        0,                                                              // defenseHearts
        0,                                                              // gsTokens
    };

    bzero(&SAVE_INFO, sizeof(SaveInfo));
    gSaveContext.totalDays = 0;
    gSaveContext.bgsDayCount = 0;

    SAVE_PLAYER_DATA = hackDebugSavePlayerData;
    gSaveContext.equips = hackDebugSaveEquips;
    gSaveContext.inventory = hackDebugSaveInventory;

    temp->checksum = 0;
    gSaveContext.inventory.gsTokens = 120;
    gSaveContext.horseData.sceneId = 0x51;
    gSaveContext.horseData.pos.x = 0;
    gSaveContext.horseData.pos.y = 0;
    gSaveContext.horseData.pos.z = 0;
    gSaveContext.horseData.angle = -0x6AD9;
    // gSaveContext.infTable[0] |= 0x5009;
    // gSaveContext.eventChkInf[0] |= 0x123F;
    // SET_EVENTCHKINF(EVENTCHKINF_80);
    // SET_EVENTCHKINF(EVENTCHKINF_C4);
    Flags_SetEventChkInf(EVENTCHKINF_18); //have Epona
    gSaveContext.infTable[INFTABLE_1DX_INDEX] = 1;
    gSaveContext.sceneFlags[5].swch = 0x40000000;
    
    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        gSaveContext.equips.buttonItems[0] = ITEM_SWORD_KOKIRI;
        Inventory_ChangeEquipment(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_KOKIRI);
        if (gSaveContext.fileNum == 0xFF) {
            gSaveContext.equips.buttonItems[1] = ITEM_SLINGSHOT;
            gSaveContext.equips.cButtonSlots[0] = SLOT_SLINGSHOT;
            Inventory_ChangeEquipment(EQUIP_TYPE_SHIELD, EQUIP_VALUE_SHIELD_DEKU);
        }
    }
     CustomFlag_Set(0x7,1);
    //ghackSave.modernAimingControl = 1;
    gSaveContext.entranceIndex = 0x0374; //Title Screen scene
    gSaveContext.magicLevel = 1;
}