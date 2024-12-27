
typedef struct Actor {
    /* 0x000 */ s16 id; // Actor ID
    /* 0x002 */ u8 category; // Actor category. Refer to the corresponding enum for values
    /* 0x003 */ s8 room; // Room number the actor is in. -1 denotes that the actor won't despawn on a room change
    /* 0x004 */ u32 flags; // Flags used for various purposes
    /* 0x008 */ PosRot home; // Initial position/rotation when spawned. Can be used for other purposes
    /* 0x01C */ s16 params; // Configurable variable set by the actor's spawn data; original name: "args_data"
    /* 0x01E */ s8 objectSlot; // Object slot (in ObjectContext) corresponding to the actor's object; original name: "bank"
    /* 0x01F */ s8 targetMode; // Controls how far the actor can be targeted from and how far it can stay locked on
    /* 0x020 */ s16 halfDaysBits; // Bitmask indicating which half-days this actor is allowed to not be killed(?) (TODO: not sure how to word this). If the current halfDayBit is not part of this mask then the actor is killed when spawning the setup actors
    /* 0x024 */ PosRot world; // Position/rotation in the world
    /* 0x038 */ s8 csId; // ActorCutscene index, see `CutsceneId`
    /* 0x039 */ u8 audioFlags; // Another set of flags? Seems related to sfx or bgm
    /* 0x03C */ PosRot focus; // Target reticle focuses on this position. For player this represents head pos and rot
    /* 0x050 */ u16 sfxId; // Id of sound effect to play. Plays when value is set, then is cleared the following update cycle
    /* 0x054 */ f32 targetArrowOffset; // Height offset of the target arrow relative to `focus` position
    /* 0x058 */ Vec3f scale; // Scale of the actor in each axis
    /* 0x064 */ Vec3f velocity; // Velocity of the actor in each axis
    /* 0x070 */ f32 speed; // Context dependent speed value. Can be used for XZ or XYZ depending on which move function is used
    /* 0x074 */ f32 gravity; // Acceleration due to gravity. Value is added to Y velocity every frame
    /* 0x078 */ f32 terminalVelocity; // Sets the lower bounds cap on velocity along the Y axis
    /* 0x07C */ struct CollisionPoly* wallPoly; // Wall polygon the actor is touching
    /* 0x080 */ struct CollisionPoly* floorPoly; // Floor polygon directly below the actor
    /* 0x084 */ u8 wallBgId; // Bg ID of the wall polygon the actor is touching
    /* 0x085 */ u8 floorBgId; // Bg ID of the floor polygon directly below the actor
    /* 0x086 */ s16 wallYaw; // Y rotation of the wall polygon the actor is touching
    /* 0x088 */ f32 floorHeight; // Y position of the floor polygon directly below the actor
    /* 0x08C */ f32 depthInWater; // Directed distance to the surface of active waterbox. Negative value means water is below.
    /* 0x090 */ u16 bgCheckFlags; // Flags indicating how the actor is interacting with collision
    /* 0x092 */ s16 yawTowardsPlayer; // Y rotation difference between the actor and the player
    /* 0x094 */ f32 xyzDistToPlayerSq; // Squared distance between the actor and the player in the x,y,z axis
    /* 0x098 */ f32 xzDistToPlayer; // Distance between the actor and the player in the XZ plane
    /* 0x09C */ f32 playerHeightRel; // Directed distance is negative if the player is below.
    /* 0x0A0 */ CollisionCheckInfo colChkInfo; // Variables related to the Collision Check system
    /* 0x0BC */ ActorShape shape; // Variables related to the physical shape of the actor
    /* 0x0EC */ Vec3f projectedPos; // Position of the actor in projected space
    /* 0x0F8 */ f32 projectedW; // w component of the projected actor position
    /* 0x0FC */ f32 uncullZoneForward; // Amount to increase the uncull zone forward by (in projected space)
    /* 0x100 */ f32 uncullZoneScale; // Amount to increase the uncull zone scale by (in projected space)
    /* 0x104 */ f32 uncullZoneDownward; // Amount to increase uncull zone downward by (in projected space)
    /* 0x108 */ Vec3f prevPos; // World position from the previous update cycle
    /* 0x114 */ u8 isLockedOn; // Set to true if the actor is currently being targeted by the player
    /* 0x115 */ u8 targetPriority; // Lower values have higher priority. Resets to 0 when player stops targeting
    /* 0x116 */ u16 textId; // Text ID to pass to link/display when interacting with the actor
    /* 0x118 */ u16 freezeTimer; // Actor does not update when set. Timer decrements automatically
    /* 0x11A */ u16 colorFilterParams; // Set color filter to red, blue, or white. Toggle opa or xlu
    /* 0x11C */ u8 colorFilterTimer; // A non-zero value enables the color filter. Decrements automatically
    /* 0x11D */ u8 isDrawn; // Set to true if the actor is currently being drawn. Always stays false for lens actors
    /* 0x11E */ u8 dropFlag; // Configures what item is dropped by the actor from `Item_DropCollectibleRandom`
    /* 0x11F */ u8 hintId; // Sets what 0600 dialog to display when talking to Tatl. Default 0xFF
    /* 0x120 */ struct Actor* parent; // Usage is actor specific. Set if actor is spawned via `Actor_SpawnAsChild`
    /* 0x124 */ struct Actor* child; // Usage is actor specific. Set if actor is spawned via `Actor_SpawnAsChild`
    /* 0x128 */ struct Actor* prev; // Previous actor of this category
    /* 0x12C */ struct Actor* next; // Next actor of this category
    /* 0x130 */ ActorFunc init; // Initialization Routine. Called by `Actor_InitContext` or `Actor_UpdateAll`
    /* 0x134 */ ActorFunc destroy; // Destruction Routine. Called by `Actor_Destroy`
    /* 0x138 */ ActorFunc update; // Update Routine. Called by `Actor_UpdateAll`
    /* 0x13C */ ActorFunc draw; // Draw Routine. Called by `Actor_Draw`
    /* 0x140 */ ActorOverlay* overlayEntry; // Pointer to the overlay table entry for this actor
} Actor; // size = 0x144
