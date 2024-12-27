/*
 * File: z_jukebox.c
 * Overlay: ovl_Bg_Jukebox
 * Description: jukebox that randomly plays a sequence when interacted with
 */
#include <uLib.h>
#include "object/0x0211-JukeCatObj.h"
#include "z_jukebox.h"

#define FLAGS (ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_NO_UPDATE_CULLING | ACTOR_FLAG_SFX_AT_POS| ACTOR_FLAG_OCARINA_NO_FREEZE )

// used to differentiate between fanfares and sequences
// true if you want the sequence in the music pool
// false if you dont want the sequence in the music pool
// 
static bool sSequenceList[] = {
    false,                        /* 0x00 */ //NA_BGM_GENERAL_SFX,      // General Sound Effects
    false,                        /* 0x01 */ //NA_BGM_NATURE_AMBIENCE,  // Environmental nature background sounds
    true,                        /* 0x02 */ //NA_BGM_FIELD_LOGIC,      // Hyrule Field
    false,                        /* 0x03 */ //NA_BGM_FIELD_INIT,       // Hyrule Field	Initial Segment From Loading Area
    false,                        /* 0x04 */ //NA_BGM_FIELD_DEFAULT_1,  // Hyrule Field	Moving Segment 1
    false,                        /* 0x05 */ //NA_BGM_FIELD_DEFAULT_2,  // Hyrule Field	Moving Segment 2
    false,                        /* 0x06 */ //NA_BGM_FIELD_DEFAULT_3,  // Hyrule Field	Moving Segment 3
    false,                        /* 0x07 */ //NA_BGM_FIELD_DEFAULT_4,  // Hyrule Field	Moving Segment 4
    false,                        /* 0x08 */ //NA_BGM_FIELD_DEFAULT_5,  // Hyrule Field	Moving Segment 5
    false,                        /* 0x09 */ //NA_BGM_FIELD_DEFAULT_6,  // Hyrule Field	Moving Segment 6
    false,                        /* 0x0A */ //NA_BGM_FIELD_DEFAULT_7,  // Hyrule Field	Moving Segment 7
    false,                        /* 0x0B */ //NA_BGM_FIELD_DEFAULT_8,  // Hyrule Field	Moving Segment 8
    false,                        /* 0x0C */ //NA_BGM_FIELD_DEFAULT_9,  // Hyrule Field	Moving Segment 9
    false,                        /* 0x0D */ //NA_BGM_FIELD_DEFAULT_A,  // Hyrule Field	Moving Segment 10
    false,                        /* 0x0E */ //NA_BGM_FIELD_DEFAULT_B,  // Hyrule Field	Moving Segment 11
    false,                        /* 0x0F */ //NA_BGM_FIELD_ENEMY_INIT, // Hyrule Field	Enemy Approaches
    false,                        /* 0x10 */ //NA_BGM_FIELD_ENEMY_1,    // Hyrule Field	Enemy Near Segment 1
    false,                        /* 0x11 */ //NA_BGM_FIELD_ENEMY_2,    // Hyrule Field	Enemy Near Segment 2
    false,                        /* 0x12 */ //NA_BGM_FIELD_ENEMY_3,    // Hyrule Field	Enemy Near Segment 3
    false,                        /* 0x13 */ //NA_BGM_FIELD_ENEMY_4,    // Hyrule Field	Enemy Near Segment 4
    false,                        /* 0x14 */ //NA_BGM_FIELD_STILL_1,    // Hyrule Field	Standing Still Segment 1
    false,                        /* 0x15 */ //NA_BGM_FIELD_STILL_2,    // Hyrule Field	Standing Still Segment 2
    false,                        /* 0x16 */ //NA_BGM_FIELD_STILL_3,    // Hyrule Field	Standing Still Segment 3
    false,                        /* 0x17 */ //NA_BGM_FIELD_STILL_4,    // Hyrule Field	Standing Still Segment 4
    true,                        /* 0x18 */ //NA_BGM_DUNGEON,          // Dodongo's Cavern
    true,                        /* 0x19 */ //NA_BGM_KAKARIKO_ADULT,   // Kakariko Village (Adult)
    true,                        /* 0x1A */ //NA_BGM_ENEMY,            // Battle
    true,                        /* 0x1B */ //NA_BGM_BOSS,             // Boss Battle "NA_BGM_BOSS00"
    true,                        /* 0x1C */ //NA_BGM_INSIDE_DEKU_TREE, // Inside the Deku Tree "NA_BGM_FAIRY_DUNGEON"
    true,                        /* 0x1D */ //NA_BGM_MARKET,           // Market
    true,                        /* 0x1E */ //NA_BGM_TITLE,            // Title Theme
    true,                        /* 0x1F */ //NA_BGM_LINK_HOUSE,       // House
    false,                        /* 0x20 */ //NA_BGM_GAME_OVER,        // Game Over
    false,                        /* 0x21 */ //NA_BGM_BOSS_CLEAR,       // Boss Clear
    false,                        /* 0x22 */ //NA_BGM_ITEM_GET,         // Obtain Item
    false,                         /* 0x23 */ //NA_BGM_OPENING_GANON,    // Enter Ganondorf
    false,                         /* 0x24 */ //NA_BGM_HEART_GET,        // Obtain Heart Container
    false,                         /* 0x25 */ //NA_BGM_OCA_LIGHT,        // Prelude of Light
    true,                        /* 0x26 */ //NA_BGM_JABU_JABU,        // Inside Jabu-Jabu's Belly "NA_BGM_BUYO_DUNGEON"
    true,                        /* 0x27 */ //NA_BGM_KAKARIKO_KID,     // Kakariko Village (Child)
    true,                        /* 0x28 */ //NA_BGM_GREAT_FAIRY,      // Great Fairy's Fountain "NA_BGM_GODESS"
    true,                        /* 0x29 */ //NA_BGM_ZELDA_THEME,      // Zelda's Theme "NA_BGM_HIME"
    true,                        /* 0x2A */ //NA_BGM_FIRE_TEMPLE,      // Fire Temple "NA_BGM_FIRE_DUNGEON"
    false,                       /* 0x2B */ //NA_BGM_OPEN_TRE_BOX,     // Open Treasure Chest
    true,                        /* 0x2C */ //NA_BGM_FOREST_TEMPLE,    // Forest Temple "NA_BGM_FORST_DUNGEON"
    true,                        /* 0x2D */ //NA_BGM_COURTYARD,        // Hyrule Castle Courtyard "NA_BGM_HIRAL_GARDEN"
    true,                        /* 0x2E */ //NA_BGM_GANON_TOWER,      // Ganondorf's Theme
    true,                        /* 0x2F */ //NA_BGM_LONLON,           // Lon Lon Ranch "NA_BGM_RONRON"
    true,                        /* 0x30 */ //NA_BGM_GORON_CITY,       // Goron City "NA_BGM_GORON"
    false,                        /* 0x31 */ //NA_BGM_FIELD_MORNING,    // Hyrule Field Morning Theme
    false,                        /* 0x32 */ //NA_BGM_SPIRITUAL_STONE,  // Spiritual Stone Get "NA_BGM_SPIRIT_STONE"
    false,                        /* 0x33 */ //NA_BGM_OCA_BOLERO,       // Bolero of Fire "NA_BGM_OCA_FLAME"
    false,                        /* 0x34 */ //NA_BGM_OCA_MINUET,       // Minuet of Forest "NA_BGM_OCA_WIND"
    false,                        /* 0x35 */ //NA_BGM_OCA_SERENADE,     // Serenade of Water "NA_BGM_OCA_WATER"
    false,                        /* 0x36 */ //NA_BGM_OCA_REQUIEM,      // Requiem of Spirit "NA_BGM_OCA_SOUL"
    false,                        /* 0x37 */ //NA_BGM_OCA_NOCTURNE,     // Nocturne of Shadow "NA_BGM_OCA_DARKNESS"
    true,                        /* 0x38 */ //NA_BGM_MINI_BOSS,        // Mini-Boss Battle "NA_BGM_MIDDLE_BOSS"
    false,                        /* 0x39 */ //NA_BGM_SMALL_ITEM_GET,   // Obtain Small Item "NA_BGM_S_ITEM_GET"
    true,                        /* 0x3A */ //NA_BGM_TEMPLE_OF_TIME,   // Temple of Time "NA_BGM_SHRINE_OF_TIME"
    false,                        /* 0x3B */ //NA_BGM_EVENT_CLEAR,      // Escape from Lon Lon Ranch
    true,                        /* 0x3C */ //NA_BGM_KOKIRI,           // Kokiri Forest
    false,                        /* 0x3D */ //NA_BGM_OCA_FAIRY_GET,    // Obtain Fairy Ocarina "NA_BGM_OCA_YOUSEI"
    true,                        /* 0x3E */ //NA_BGM_SARIA_THEME,      // Lost Woods "NA_BGM_MAYOIMORI"
    true,                        /* 0x3F */ //NA_BGM_SPIRIT_TEMPLE,    // Spirit Temple "NA_BGM_SOUL_DUNGEON"
    true,                        /* 0x40 */ //NA_BGM_HORSE,            // Horse Race
    false,                        /* 0x41 */ //NA_BGM_HORSE_GOAL,       // Horse Race Goal
    true,                        /* 0x42 */ //NA_BGM_INGO,             // Ingo's Theme
    false,                        /* 0x43 */ //NA_BGM_MEDALLION_GET,    // Obtain Medallion "NA_BGM_MEDAL_GET"
    false,                        /* 0x44 */ //NA_BGM_OCA_SARIA,        // Ocarina Saria's Song
    false,                        /* 0x45 */ //NA_BGM_OCA_EPONA,        // Ocarina Epona's Song
    false,                        /* 0x46 */ //NA_BGM_OCA_ZELDA,        // Ocarina Zelda's Lullaby
    false,                        /* 0x47 */ //NA_BGM_OCA_SUNS,         // Ocarina Sun's Song "NA_BGM_OCA_SUNMOON"
    false,                        /* 0x48 */ //NA_BGM_OCA_TIME,         // Ocarina Song of Time
    false,                        /* 0x49 */ //NA_BGM_OCA_STORM,        // Ocarina Song of Storms
    true,                        /* 0x4A */ //NA_BGM_NAVI_OPENING,     // Fairy Flying "NA_BGM_NAVI"
    true,                        /* 0x4B */ //NA_BGM_DEKU_TREE_CS,     // Deku Tree "NA_BGM_DEKUNOKI"
    true,                        /* 0x4C */ //NA_BGM_WINDMILL,         // Windmill Hut "NA_BGM_FUSHA"
    true,                        /* 0x4D */ //NA_BGM_HYRULE_CS,        // Legend of Hyrule "NA_BGM_HIRAL_DEMO"
    true,                        /* 0x4E */ //NA_BGM_MINI_GAME,        // Shooting Gallery
    true,                        /* 0x4F */ //NA_BGM_SHEIK,            // Sheik's Theme "NA_BGM_SEAK"
    true,                        /* 0x50 */ //NA_BGM_ZORA_DOMAIN,      // Zora's Domain "NA_BGM_ZORA"
    false,                        /* 0x51 */ //NA_BGM_APPEAR,           // Enter Zelda
    true,                        /* 0x52 */ //NA_BGM_ADULT_LINK,       // Goodbye to Zelda
    false,                        /* 0x53 */ //NA_BGM_MASTER_SWORD,     // Master Sword
    false,                        /* 0x54 */ //NA_BGM_INTRO_GANON,
    true,                        /* 0x55 */ //NA_BGM_SHOP,             // Shop
    true,                        /* 0x56 */ //NA_BGM_CHAMBER_OF_SAGES, // Chamber of the Sages "NA_BGM_KENJA"
    true,                        /* 0x57 */ //NA_BGM_FILE_SELECT,      // File Select
    true,                        /* 0x58 */ //NA_BGM_ICE_CAVERN,       // Ice Cavern "NA_BGM_ICE_DUNGEON"
    false,                        /* 0x59 */ //NA_BGM_DOOR_OF_TIME,     // Open Door of Temple of Time "NA_BGM_GATE_OPEN"
    true,                        /* 0x5A */ //NA_BGM_OWL,              // Kaepora Gaebora's Theme
    true,                        /* 0x5B */ //NA_BGM_SHADOW_TEMPLE,    // Shadow Temple "NA_BGM_DARKNESS_DUNGEON"
    true,                        /* 0x5C */ //NA_BGM_WATER_TEMPLE,     // Water Temple "NA_BGM_AQUA_DUNGEON"
    false,                        /* 0x5D */ //NA_BGM_BRIDGE_TO_GANONS, // Ganon's Castle Bridge "NA_BGM_BRIDGE"
    true,                        /* 0x5E */ //NA_BGM_OCARINA_OF_TIME,  // Ocarina of Time "NA_BGM_SARIA"
    true,                        /* 0x5F */ //NA_BGM_GERUDO_VALLEY,    // Gerudo Valley "NA_BGM_GERUDO"
    true,                        /* 0x60 */ //NA_BGM_POTION_SHOP,      // Potion Shop "NA_BGM_DRUGSTORE"
    true,                        /* 0x61 */ //NA_BGM_KOTAKE_KOUME,     // Kotake & Koume's Theme
    true,                        /* 0x62 */ //NA_BGM_ESCAPE,           // Escape from Ganon's Castle
    true,                        /* 0x63 */ //NA_BGM_UNDERGROUND,      // Ganon's Castle Under Ground
    true,                        /* 0x64 */ //NA_BGM_GANONDORF_BOSS,   // Ganondorf Battle
    true,                        /* 0x65 */ //NA_BGM_GANON_BOSS,       // Ganon Battle
    true,                        /* 0x66 */ //NA_BGM_END_DEMO,         // Seal of Six Sages
    true,                        /* 0x67 */ //NA_BGM_STAFF_1,          // End Credits I
    true,                        /* 0x68 */ //NA_BGM_STAFF_2,          // End Credits II
    true,                        /* 0x69 */ //NA_BGM_STAFF_3,          // End Credits III
    true,                        /* 0x6A */ //NA_BGM_STAFF_4,          // End Credits IV
    true,                        /* 0x6B */ //NA_BGM_FIRE_BOSS,        // King Dodongo & Volvagia Boss Battle "NA_BGM_BOSS01"
    true,                        /* 0x6C */ //NA_BGM_TIMED_MINI_GAME,  // Mini-Game
    false,                        /* 0x6D */ //NA_BGM_CUTSCENE_EFFECTS, // A small collection of various cutscene sounds
    false,                        /* 0x7F */ //NA_BGM_NO_MUSIC = 0x7F,  // No bgm music is played
    false,                        /* 0x80 */ //NA_BGM_NATURE_SFX_RAIN = 0x80,  // Related to rain
    false,                        /* 0xFF */ //NA_BGM_DISABLED = 0xFFFF
};

OvlMessage gIntroduceText = {
    .type = MSGBOX_TYPE_BLACK,
    .pos = MSGBOX_POS_BOTTOM,
    .txt =
    "Konnichiwa!\x05\x06"
};

OvlMessage gChoiceText = {
    .type = MSGBOX_TYPE_BLACK,
    .txt =
        "What would you want?\n"
        MSG_THREE_CHOICE MSG_COLOR_REG_0 MSG_INSTANT_ON
        "Play next Song\n"
        "Stop playing\n"
        "Nothing."
        MSG_COLOR_DEFAULT MSG_INSTANT_OFF
    

};


void Jukebox_init(Actor* thisx, PlayState* play);
void Jukebox_destroy(Actor* thisx, PlayState* play);
void Jukebox_update(Jukebox* this, PlayState* play);
void Jukebox_draw(Actor* thisx, PlayState* play);

void Jukebox_Talk(Jukebox* this, PlayState* play);
void Jukebox_PlayInitialSong(Jukebox* this, PlayState* play) ;
void Jukebox_SetupPickUpRandomSong(Jukebox* this, PlayState* play);
void Jukebox_PickUpRandomSong(Jukebox* this, PlayState* play);
void Jukebox_GiveBackBGM(Jukebox* this, PlayState* play);
void Jukebox_DoNothing(Jukebox* this, PlayState* play);
void Jukebox_Set_volume(Jukebox* this, PlayState* play);
void Jukebox_UpdateLightning(Jukebox* this, PlayState* play);

const ActorInit Bg_Jukebox_InitVars = {
    0x0221, //ACTOR ID
    ACTORCAT_BG,
    FLAGS,
    0x0211,//OBJECT ID
    sizeof(Jukebox),
    (ActorFunc)Jukebox_init,
    (ActorFunc)Jukebox_destroy,
    (ActorFunc)Jukebox_update,
    (ActorFunc)Jukebox_draw,
};


void Jukebox_init(Actor* thisx, PlayState* play) {
    Jukebox* this = (Jukebox*)thisx;
    thisx->room = -1;
    CollisionHeader* colHeader;
    this->i = true;
    this->songIDs = (this->actor.params >> 8) & 0xFF;//params : **00 (hex).
    this->playFlag = 1;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gJukeBox_CollJukeCatObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    OvlMessage_Init(&this->actor, false);
    Lights_PointNoGlowSetInfo(&this->lightInfo, this->actor.world.pos.x, this->actor.world.pos.y + 30.0f,
                        this->actor.world.pos.z, 0, 0, 255, -1);
    this->lightNode = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfo);
    this->actionFunc = Jukebox_PlayInitialSong;

}

void Jukebox_destroy(Actor* thisx, PlayState* play) {
    Jukebox* this = (void*)thisx;

}

void Jukebox_update(Jukebox* this, PlayState* play) {
    s16 yawDiff;
    s16 yawDiffTemp;
    if (ABS(this->actor.yDistToPlayer) < 50.0f) {
        yawDiffTemp = (this->actor.yawTowardsPlayer - this->actor.world.rot.y);
        yawDiff = ABS(yawDiffTemp);
        if (yawDiff < 0x2000) {
            if(OvlMessage_Prompt(&this->actor,&gIntroduceText,120,0) > 0){
                OvlMessage_RegisterColor(0,0,255,0);
                this->actionFunc = Jukebox_Talk;
            }
        }
    }
    Actor_SetFocus(&this->actor, 50.0f);
    if (this->playFlag) {
        Jukebox_Set_volume(this, play);
    }else{
        Jukebox_GiveBackBGM(this,play);
    }
    Jukebox_UpdateLightning(this,play);
    this->actionFunc(this, play);

}
void Jukebox_UpdateLightning(Jukebox* this, PlayState* play) {
    u8 brightness = 0;
    s32 lightRadius = -1;
    if(this->playFlag){
        lightRadius = 200;
        brightness = (u8)(Rand_ZeroOne() * 127.0f) + 128;
    }
    Lights_PointSetColorAndRadius(&this->lightInfo, brightness, brightness, brightness - 40.0f, lightRadius);
}

void Jukebox_Talk(Jukebox* this, PlayState* play) {
    s8 decision;
    s8 pickUpNextSong = 0;
     if ((OvlMessage_IsClosed(&this->actor))){
            OvlMessage_Start(NULL, &gChoiceText);
     }
    if((decision = OvlMessage_GetChoice(&this->actor))){
            switch (decision){
                case 1://Play next song
                    this->playFlag = 1;
                    pickUpNextSong = 1;
                    break;
                case 2://stop playing
                    this->playFlag = 0;
                    break;
                case 3://quit
                    break;
            }
        if(pickUpNextSong){
             Jukebox_PickUpRandomSong(this,play);
         }
         func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3);
            this->actionFunc = Jukebox_DoNothing;
    }
}

void Jukebox_draw(Actor* thisx, PlayState* play) {
    Jukebox* this = (Jukebox*)thisx;
    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, this->scrollX, 0, 32, 32));
    if(this->playFlag){
        this->scrollX += 1;
    }
    Gfx_DrawDListOpa(play, gJukeBox_DlJukeCatObj);
}

 
void Jukebox_Set_volume(Jukebox* this, PlayState* play) {
    u8 sJukeboxVolume;
    s32 adjDist;
    f32 dist = sqrtf(SQ(this->actor.xzDistToPlayer) + SQ(this->actor.yDistToPlayer)); //ABS(this->actor.xzDistToPlayer);
    if (dist < 1500.0f) {
            if (dist < 250.0f) {
                adjDist = 0.0f;
            } else{
                adjDist = (s32)((dist- 250.0f)/10.0f);
            }
            sJukeboxVolume = (u8)(127 - adjDist);
        sJukeboxVolume = CLAMP(sJukeboxVolume,0,127);
        SEQCMD_SET_PLAYER_VOLUME(SEQ_PLAYER_BGM_SUB, 0, sJukeboxVolume);
        SEQCMD_SET_PLAYER_VOLUME(SEQ_PLAYER_BGM_MAIN, 0, (127 - sJukeboxVolume));
    } else {
        Jukebox_GiveBackBGM(this,play);
    }

}


void Jukebox_PickUpRandomSong(Jukebox* this, PlayState* play) {
    //                               V this should be number of sequences in sSequencesList
        do{
            this->seqId = Rand_S16Offset(0, 0x6C);
        }while(sSequenceList[this->seqId] == false);
        SEQCMD_PLAY_SEQUENCE(SEQ_PLAYER_BGM_SUB, 10, 8, this->seqId);

}

void Jukebox_DoNothing(Jukebox* this, PlayState* play) {
}

void Jukebox_PlayInitialSong(Jukebox* this, PlayState* play) {
    this->seqId = this->songIDs;
    SEQCMD_PLAY_SEQUENCE(SEQ_PLAYER_BGM_SUB, 10, 8, this->seqId);
    this->actionFunc = Jukebox_DoNothing;

}

void Jukebox_GiveBackBGM(Jukebox* this, PlayState* play) {
    SEQCMD_SET_PLAYER_VOLUME(SEQ_PLAYER_BGM_SUB, 0, 0);
    SEQCMD_SET_PLAYER_VOLUME(SEQ_PLAYER_BGM_MAIN, 0, 127);
}
