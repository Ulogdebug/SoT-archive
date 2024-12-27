#include "z_en_estone.h"
#include <uLib.h>
#include "vt.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

// We need to pass in 14 into SubS_GenShadowTex for the bodyPartsNum, NOT the actual size of the
// parent body parts array. This is both necessary to match and to prevent extra dots from being
// drawn with the shadow.
#define SHADOW_BODY_PARTS_NUM 14

#define BODYPART_NONE -1

MtxF* sCurrentMatrix; //!< original name: "Matrix_now"

Mtx sActorHiliteMtx;

static s16 sintable_mm[0x400] = {
    0x0000, 0x0032, 0x0064, 0x0096, 0x00C9, 0x00FB, 0x012D, 0x0160, 0x0192, 0x01C4, 0x01F7, 0x0229, 0x025B, 0x028E,
    0x02C0, 0x02F2, 0x0324, 0x0357, 0x0389, 0x03BB, 0x03EE, 0x0420, 0x0452, 0x0484, 0x04B7, 0x04E9, 0x051B, 0x054E,
    0x0580, 0x05B2, 0x05E4, 0x0617, 0x0649, 0x067B, 0x06AD, 0x06E0, 0x0712, 0x0744, 0x0776, 0x07A9, 0x07DB, 0x080D,
    0x083F, 0x0871, 0x08A4, 0x08D6, 0x0908, 0x093A, 0x096C, 0x099F, 0x09D1, 0x0A03, 0x0A35, 0x0A67, 0x0A99, 0x0ACB,
    0x0AFE, 0x0B30, 0x0B62, 0x0B94, 0x0BC6, 0x0BF8, 0x0C2A, 0x0C5C, 0x0C8E, 0x0CC0, 0x0CF2, 0x0D25, 0x0D57, 0x0D89,
    0x0DBB, 0x0DED, 0x0E1F, 0x0E51, 0x0E83, 0x0EB5, 0x0EE7, 0x0F19, 0x0F4B, 0x0F7C, 0x0FAE, 0x0FE0, 0x1012, 0x1044,
    0x1076, 0x10A8, 0x10DA, 0x110C, 0x113E, 0x116F, 0x11A1, 0x11D3, 0x1205, 0x1237, 0x1269, 0x129A, 0x12CC, 0x12FE,
    0x1330, 0x1361, 0x1393, 0x13C5, 0x13F6, 0x1428, 0x145A, 0x148C, 0x14BD, 0x14EF, 0x1520, 0x1552, 0x1584, 0x15B5,
    0x15E7, 0x1618, 0x164A, 0x167B, 0x16AD, 0x16DF, 0x1710, 0x1741, 0x1773, 0x17A4, 0x17D6, 0x1807, 0x1839, 0x186A,
    0x189B, 0x18CD, 0x18FE, 0x1930, 0x1961, 0x1992, 0x19C3, 0x19F5, 0x1A26, 0x1A57, 0x1A88, 0x1ABA, 0x1AEB, 0x1B1C,
    0x1B4D, 0x1B7E, 0x1BAF, 0x1BE1, 0x1C12, 0x1C43, 0x1C74, 0x1CA5, 0x1CD6, 0x1D07, 0x1D38, 0x1D69, 0x1D9A, 0x1DCB,
    0x1DFC, 0x1E2D, 0x1E5D, 0x1E8E, 0x1EBF, 0x1EF0, 0x1F21, 0x1F52, 0x1F82, 0x1FB3, 0x1FE4, 0x2015, 0x2045, 0x2076,
    0x20A7, 0x20D7, 0x2108, 0x2139, 0x2169, 0x219A, 0x21CA, 0x21FB, 0x222B, 0x225C, 0x228C, 0x22BD, 0x22ED, 0x231D,
    0x234E, 0x237E, 0x23AE, 0x23DF, 0x240F, 0x243F, 0x2470, 0x24A0, 0x24D0, 0x2500, 0x2530, 0x2560, 0x2591, 0x25C1,
    0x25F1, 0x2621, 0x2651, 0x2681, 0x26B1, 0x26E1, 0x2711, 0x2740, 0x2770, 0x27A0, 0x27D0, 0x2800, 0x2830, 0x285F,
    0x288F, 0x28BF, 0x28EE, 0x291E, 0x294E, 0x297D, 0x29AD, 0x29DD, 0x2A0C, 0x2A3C, 0x2A6B, 0x2A9B, 0x2ACA, 0x2AF9,
    0x2B29, 0x2B58, 0x2B87, 0x2BB7, 0x2BE6, 0x2C15, 0x2C44, 0x2C74, 0x2CA3, 0x2CD2, 0x2D01, 0x2D30, 0x2D5F, 0x2D8E,
    0x2DBD, 0x2DEC, 0x2E1B, 0x2E4A, 0x2E79, 0x2EA8, 0x2ED7, 0x2F06, 0x2F34, 0x2F63, 0x2F92, 0x2FC0, 0x2FEF, 0x301E,
    0x304C, 0x307B, 0x30A9, 0x30D8, 0x3107, 0x3135, 0x3163, 0x3192, 0x31C0, 0x31EF, 0x321D, 0x324B, 0x3279, 0x32A8,
    0x32D6, 0x3304, 0x3332, 0x3360, 0x338E, 0x33BC, 0x33EA, 0x3418, 0x3446, 0x3474, 0x34A2, 0x34D0, 0x34FE, 0x352B,
    0x3559, 0x3587, 0x35B5, 0x35E2, 0x3610, 0x363D, 0x366B, 0x3698, 0x36C6, 0x36F3, 0x3721, 0x374E, 0x377C, 0x37A9,
    0x37D6, 0x3803, 0x3831, 0x385E, 0x388B, 0x38B8, 0x38E5, 0x3912, 0x393F, 0x396C, 0x3999, 0x39C6, 0x39F3, 0x3A20,
    0x3A4D, 0x3A79, 0x3AA6, 0x3AD3, 0x3B00, 0x3B2C, 0x3B59, 0x3B85, 0x3BB2, 0x3BDE, 0x3C0B, 0x3C37, 0x3C64, 0x3C90,
    0x3CBC, 0x3CE9, 0x3D15, 0x3D41, 0x3D6D, 0x3D99, 0x3DC5, 0x3DF1, 0x3E1D, 0x3E49, 0x3E75, 0x3EA1, 0x3ECD, 0x3EF9,
    0x3F25, 0x3F50, 0x3F7C, 0x3FA8, 0x3FD3, 0x3FFF, 0x402B, 0x4056, 0x4082, 0x40AD, 0x40D8, 0x4104, 0x412F, 0x415A,
    0x4186, 0x41B1, 0x41DC, 0x4207, 0x4232, 0x425D, 0x4288, 0x42B3, 0x42DE, 0x4309, 0x4334, 0x435F, 0x4389, 0x43B4,
    0x43DF, 0x4409, 0x4434, 0x445F, 0x4489, 0x44B4, 0x44DE, 0x4508, 0x4533, 0x455D, 0x4587, 0x45B1, 0x45DC, 0x4606,
    0x4630, 0x465A, 0x4684, 0x46AE, 0x46D8, 0x4702, 0x472C, 0x4755, 0x477F, 0x47A9, 0x47D2, 0x47FC, 0x4826, 0x484F,
    0x4879, 0x48A2, 0x48CC, 0x48F5, 0x491E, 0x4948, 0x4971, 0x499A, 0x49C3, 0x49EC, 0x4A15, 0x4A3E, 0x4A67, 0x4A90,
    0x4AB9, 0x4AE2, 0x4B0B, 0x4B33, 0x4B5C, 0x4B85, 0x4BAD, 0x4BD6, 0x4BFE, 0x4C27, 0x4C4F, 0x4C78, 0x4CA0, 0x4CC8,
    0x4CF0, 0x4D19, 0x4D41, 0x4D69, 0x4D91, 0x4DB9, 0x4DE1, 0x4E09, 0x4E31, 0x4E58, 0x4E80, 0x4EA8, 0x4ED0, 0x4EF7,
    0x4F1F, 0x4F46, 0x4F6E, 0x4F95, 0x4FBD, 0x4FE4, 0x500B, 0x5032, 0x505A, 0x5081, 0x50A8, 0x50CF, 0x50F6, 0x511D,
    0x5144, 0x516B, 0x5191, 0x51B8, 0x51DF, 0x5205, 0x522C, 0x5253, 0x5279, 0x52A0, 0x52C6, 0x52EC, 0x5313, 0x5339,
    0x535F, 0x5385, 0x53AB, 0x53D1, 0x53F7, 0x541D, 0x5443, 0x5469, 0x548F, 0x54B5, 0x54DA, 0x5500, 0x5525, 0x554B,
    0x5571, 0x5596, 0x55BB, 0x55E1, 0x5606, 0x562B, 0x5650, 0x5675, 0x569B, 0x56C0, 0x56E5, 0x5709, 0x572E, 0x5753,
    0x5778, 0x579D, 0x57C1, 0x57E6, 0x580A, 0x582F, 0x5853, 0x5878, 0x589C, 0x58C0, 0x58E5, 0x5909, 0x592D, 0x5951,
    0x5975, 0x5999, 0x59BD, 0x59E1, 0x5A04, 0x5A28, 0x5A4C, 0x5A6F, 0x5A93, 0x5AB7, 0x5ADA, 0x5AFD, 0x5B21, 0x5B44,
    0x5B67, 0x5B8B, 0x5BAE, 0x5BD1, 0x5BF4, 0x5C17, 0x5C3A, 0x5C5D, 0x5C7F, 0x5CA2, 0x5CC5, 0x5CE7, 0x5D0A, 0x5D2D,
    0x5D4F, 0x5D71, 0x5D94, 0x5DB6, 0x5DD8, 0x5DFA, 0x5E1D, 0x5E3F, 0x5E61, 0x5E83, 0x5EA5, 0x5EC6, 0x5EE8, 0x5F0A,
    0x5F2C, 0x5F4D, 0x5F6F, 0x5F90, 0x5FB2, 0x5FD3, 0x5FF4, 0x6016, 0x6037, 0x6058, 0x6079, 0x609A, 0x60BB, 0x60DC,
    0x60FD, 0x611E, 0x613E, 0x615F, 0x6180, 0x61A0, 0x61C1, 0x61E1, 0x6202, 0x6222, 0x6242, 0x6263, 0x6283, 0x62A3,
    0x62C3, 0x62E3, 0x6303, 0x6323, 0x6342, 0x6362, 0x6382, 0x63A1, 0x63C1, 0x63E0, 0x6400, 0x641F, 0x643F, 0x645E,
    0x647D, 0x649C, 0x64BB, 0x64DA, 0x64F9, 0x6518, 0x6537, 0x6556, 0x6574, 0x6593, 0x65B2, 0x65D0, 0x65EF, 0x660D,
    0x662B, 0x664A, 0x6668, 0x6686, 0x66A4, 0x66C2, 0x66E0, 0x66FE, 0x671C, 0x673A, 0x6757, 0x6775, 0x6792, 0x67B0,
    0x67CD, 0x67EB, 0x6808, 0x6825, 0x6843, 0x6860, 0x687D, 0x689A, 0x68B7, 0x68D4, 0x68F1, 0x690D, 0x692A, 0x6947,
    0x6963, 0x6980, 0x699C, 0x69B9, 0x69D5, 0x69F1, 0x6A0E, 0x6A2A, 0x6A46, 0x6A62, 0x6A7E, 0x6A9A, 0x6AB5, 0x6AD1,
    0x6AED, 0x6B08, 0x6B24, 0x6B40, 0x6B5B, 0x6B76, 0x6B92, 0x6BAD, 0x6BC8, 0x6BE3, 0x6BFE, 0x6C19, 0x6C34, 0x6C4F,
    0x6C6A, 0x6C84, 0x6C9F, 0x6CBA, 0x6CD4, 0x6CEF, 0x6D09, 0x6D23, 0x6D3E, 0x6D58, 0x6D72, 0x6D8C, 0x6DA6, 0x6DC0,
    0x6DDA, 0x6DF3, 0x6E0D, 0x6E27, 0x6E40, 0x6E5A, 0x6E73, 0x6E8D, 0x6EA6, 0x6EBF, 0x6ED9, 0x6EF2, 0x6F0B, 0x6F24,
    0x6F3D, 0x6F55, 0x6F6E, 0x6F87, 0x6FA0, 0x6FB8, 0x6FD1, 0x6FE9, 0x7002, 0x701A, 0x7032, 0x704A, 0x7062, 0x707A,
    0x7092, 0x70AA, 0x70C2, 0x70DA, 0x70F2, 0x7109, 0x7121, 0x7138, 0x7150, 0x7167, 0x717E, 0x7196, 0x71AD, 0x71C4,
    0x71DB, 0x71F2, 0x7209, 0x7220, 0x7236, 0x724D, 0x7264, 0x727A, 0x7291, 0x72A7, 0x72BD, 0x72D4, 0x72EA, 0x7300,
    0x7316, 0x732C, 0x7342, 0x7358, 0x736E, 0x7383, 0x7399, 0x73AE, 0x73C4, 0x73D9, 0x73EF, 0x7404, 0x7419, 0x742E,
    0x7443, 0x7458, 0x746D, 0x7482, 0x7497, 0x74AC, 0x74C0, 0x74D5, 0x74EA, 0x74FE, 0x7512, 0x7527, 0x753B, 0x754F,
    0x7563, 0x7577, 0x758B, 0x759F, 0x75B3, 0x75C7, 0x75DA, 0x75EE, 0x7601, 0x7615, 0x7628, 0x763B, 0x764F, 0x7662,
    0x7675, 0x7688, 0x769B, 0x76AE, 0x76C1, 0x76D3, 0x76E6, 0x76F9, 0x770B, 0x771E, 0x7730, 0x7742, 0x7754, 0x7767,
    0x7779, 0x778B, 0x779D, 0x77AF, 0x77C0, 0x77D2, 0x77E4, 0x77F5, 0x7807, 0x7818, 0x782A, 0x783B, 0x784C, 0x785D,
    0x786E, 0x787F, 0x7890, 0x78A1, 0x78B2, 0x78C3, 0x78D3, 0x78E4, 0x78F4, 0x7905, 0x7915, 0x7925, 0x7936, 0x7946,
    0x7956, 0x7966, 0x7976, 0x7985, 0x7995, 0x79A5, 0x79B5, 0x79C4, 0x79D4, 0x79E3, 0x79F2, 0x7A02, 0x7A11, 0x7A20,
    0x7A2F, 0x7A3E, 0x7A4D, 0x7A5B, 0x7A6A, 0x7A79, 0x7A87, 0x7A96, 0x7AA4, 0x7AB3, 0x7AC1, 0x7ACF, 0x7ADD, 0x7AEB,
    0x7AF9, 0x7B07, 0x7B15, 0x7B23, 0x7B31, 0x7B3E, 0x7B4C, 0x7B59, 0x7B67, 0x7B74, 0x7B81, 0x7B8E, 0x7B9B, 0x7BA8,
    0x7BB5, 0x7BC2, 0x7BCF, 0x7BDC, 0x7BE8, 0x7BF5, 0x7C02, 0x7C0E, 0x7C1A, 0x7C27, 0x7C33, 0x7C3F, 0x7C4B, 0x7C57,
    0x7C63, 0x7C6F, 0x7C7A, 0x7C86, 0x7C92, 0x7C9D, 0x7CA9, 0x7CB4, 0x7CBF, 0x7CCB, 0x7CD6, 0x7CE1, 0x7CEC, 0x7CF7,
    0x7D02, 0x7D0C, 0x7D17, 0x7D22, 0x7D2C, 0x7D37, 0x7D41, 0x7D4B, 0x7D56, 0x7D60, 0x7D6A, 0x7D74, 0x7D7E, 0x7D88,
    0x7D91, 0x7D9B, 0x7DA5, 0x7DAE, 0x7DB8, 0x7DC1, 0x7DCB, 0x7DD4, 0x7DDD, 0x7DE6, 0x7DEF, 0x7DF8, 0x7E01, 0x7E0A,
    0x7E13, 0x7E1B, 0x7E24, 0x7E2C, 0x7E35, 0x7E3D, 0x7E45, 0x7E4D, 0x7E56, 0x7E5E, 0x7E66, 0x7E6D, 0x7E75, 0x7E7D,
    0x7E85, 0x7E8C, 0x7E94, 0x7E9B, 0x7EA3, 0x7EAA, 0x7EB1, 0x7EB8, 0x7EBF, 0x7EC6, 0x7ECD, 0x7ED4, 0x7EDB, 0x7EE1,
    0x7EE8, 0x7EEE, 0x7EF5, 0x7EFB, 0x7F01, 0x7F08, 0x7F0E, 0x7F14, 0x7F1A, 0x7F20, 0x7F25, 0x7F2B, 0x7F31, 0x7F36,
    0x7F3C, 0x7F41, 0x7F47, 0x7F4C, 0x7F51, 0x7F56, 0x7F5B, 0x7F60, 0x7F65, 0x7F6A, 0x7F6F, 0x7F74, 0x7F78, 0x7F7D,
    0x7F81, 0x7F85, 0x7F8A, 0x7F8E, 0x7F92, 0x7F96, 0x7F9A, 0x7F9E, 0x7FA2, 0x7FA6, 0x7FA9, 0x7FAD, 0x7FB0, 0x7FB4,
    0x7FB7, 0x7FBA, 0x7FBE, 0x7FC1, 0x7FC4, 0x7FC7, 0x7FCA, 0x7FCC, 0x7FCF, 0x7FD2, 0x7FD4, 0x7FD7, 0x7FD9, 0x7FDC,
    0x7FDE, 0x7FE0, 0x7FE2, 0x7FE4, 0x7FE6, 0x7FE8, 0x7FEA, 0x7FEC, 0x7FED, 0x7FEF, 0x7FF1, 0x7FF2, 0x7FF3, 0x7FF5,
    0x7FF6, 0x7FF7, 0x7FF8, 0x7FF9, 0x7FFA, 0x7FFB, 0x7FFB, 0x7FFC, 0x7FFD, 0x7FFD, 0x7FFE, 0x7FFE, 0x7FFE, 0x7FFE,
    0x7FFE, 0x7FFF,
};

s16 sins_mm(u16 x) {
    s16 val;

    x >>= 4;
    if ((x & 0x400) != 0) {
        val = sintable_mm[0x3FF - (x & 0x3FF)];
    } else {
        val = sintable_mm[x & 0x3FF];
    }

    if ((x & 0x800) != 0) {
        return -val;
    }

    return val;
}

s16 coss_mm(u16 x) {
    return sins_mm(x + 0x4000);
}


f32 Math_CosS_MM(s16 angle) {
    return coss_mm(angle) * SHT_MINV;
}

f32 Math_SinS_MM(s16 angle) {
    return sins_mm(angle) * SHT_MINV;
}

Gfx* Hilite_Draw(Vec3f* object, Vec3f* eye, Vec3f* lightDir, GraphicsContext* gfxCtx, Gfx* dl, Hilite** hiliteP) {
    LookAt* lookAt = Graph_Alloc(gfxCtx, sizeof(LookAt));
    f32 correctedEyeX = (eye->x == object->x) && (eye->z == object->z) ? eye->x + 0.001f : eye->x;

    *hiliteP = Graph_Alloc(gfxCtx, sizeof(Hilite));

    guLookAtHilite(&sActorHiliteMtx, lookAt, *hiliteP, correctedEyeX, eye->y, eye->z, object->x, object->y, object->z,
                   0.0f, 1.0f, 0.0f, lightDir->x, lightDir->y, lightDir->z, lightDir->x, lightDir->y, lightDir->z, 0x10,
                   0x10);

    gSPLookAt(dl++, lookAt);
    gDPSetHilite1Tile(dl++, 1, *hiliteP, 0x10, 0x10);

    return dl;
}


Hilite* Hilite_DrawXlu(Vec3f* object, Vec3f* eye, Vec3f* lightDir, GraphicsContext* gfxCtx) {
    Hilite* hilite;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    POLY_XLU_DISP = Hilite_Draw(object, eye, lightDir, gfxCtx, POLY_XLU_DISP, &hilite);

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);

    return hilite;
}


void* Lib_SegmentedToVirtual(void* ptr) {
    return SEGMENTED_TO_VIRTUAL(ptr);
}

void Matrix_RotateXS(s16 x, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (x != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS_MM(x);
            cos = Math_CosS_MM(x);

            tempY = cmf->xy;
            tempZ = cmf->xz;
            cmf->xy = tempY * cos + tempZ * sin;
            cmf->xz = tempZ * cos - tempY * sin;

            tempY = cmf->yy;
            tempZ = cmf->yz;
            cmf->yy = tempY * cos + tempZ * sin;
            cmf->yz = tempZ * cos - tempY * sin;

            tempY = cmf->zy;
            tempZ = cmf->zz;
            cmf->zy = tempY * cos + tempZ * sin;
            cmf->zz = tempZ * cos - tempY * sin;

            tempY = cmf->wy;
            tempZ = cmf->wz;
            cmf->wy = tempY * cos + tempZ * sin;
            cmf->wz = tempZ * cos - tempY * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (x != 0) {
            sin = Math_SinS_MM(x);
            cos = Math_CosS_MM(x);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->zx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->wy = 0.0f;
        cmf->xz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->xx = 1.0f;
        cmf->ww = 1.0f;
        cmf->yy = cos;
        cmf->zz = cos;
        cmf->zy = sin;
        cmf->yz = -sin;
    }
}

void Matrix_RotateXFApply(f32 x) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;
    s32 pad;

    if (x != 0.0f) {
        cmf = sCurrentMatrix;

        sin = sins(RAD_TO_BINANG(x)) * SHT_MINV;
        cos = coss(RAD_TO_BINANG(x)) * SHT_MINV;

        tempY = cmf->xy;
        tempZ = cmf->xz;
        cmf->xy = (tempY * cos) + (tempZ * sin);
        cmf->xz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->yy;
        tempZ = cmf->yz;
        cmf->yy = (tempY * cos) + (tempZ * sin);
        cmf->yz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->zy;
        tempZ = cmf->zz;
        cmf->zy = (tempY * cos) + (tempZ * sin);
        cmf->zz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->wy;
        tempZ = cmf->wz;
        cmf->wy = (tempY * cos) + (tempZ * sin);
        cmf->wz = (tempZ * cos) - (tempY * sin);
    }
}

void Matrix_RotateYS(s16 y, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (y != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS_MM(y);
            cos = Math_CosS_MM(y);

            tempX = cmf->xx;
            tempZ = cmf->xz;
            cmf->xx = tempX * cos - tempZ * sin;
            cmf->xz = tempX * sin + tempZ * cos;

            tempX = cmf->yx;
            tempZ = cmf->yz;
            cmf->yx = tempX * cos - tempZ * sin;
            cmf->yz = tempX * sin + tempZ * cos;

            tempX = cmf->zx;
            tempZ = cmf->zz;
            cmf->zx = tempX * cos - tempZ * sin;
            cmf->zz = tempX * sin + tempZ * cos;

            tempX = cmf->wx;
            tempZ = cmf->wz;
            cmf->wx = tempX * cos - tempZ * sin;
            cmf->wz = tempX * sin + tempZ * cos;
        }
    } else {
        cmf = sCurrentMatrix;

        if (y != 0) {
            sin = Math_SinS_MM(y);
            cos = Math_CosS_MM(y);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->zy = 0.0f;
        cmf->wy = 0.0f;
        cmf->yz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->yy = 1.0f;
        cmf->ww = 1.0f;
        cmf->xx = cos;
        cmf->zz = cos;
        cmf->zx = -sin;
        cmf->xz = sin;
    }
}

void Matrix_RotateYF(f32 y, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempZ;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (y != 0.0f) {
            cmf = sCurrentMatrix;

            sin = sinf(y);
            cos = cosf(y);

            tempX = cmf->xx;
            tempZ = cmf->xz;
            cmf->xx = tempX * cos - tempZ * sin;
            cmf->xz = tempX * sin + tempZ * cos;

            tempX = cmf->yx;
            tempZ = cmf->yz;
            cmf->yx = tempX * cos - tempZ * sin;
            cmf->yz = tempX * sin + tempZ * cos;

            tempX = cmf->zx;
            tempZ = cmf->zz;
            cmf->zx = tempX * cos - tempZ * sin;
            cmf->zz = tempX * sin + tempZ * cos;

            tempX = cmf->wx;
            tempZ = cmf->wz;
            cmf->wx = tempX * cos - tempZ * sin;
            cmf->wz = tempX * sin + tempZ * cos;
        }
    } else {
        cmf = sCurrentMatrix;

        if (y != 0.0f) {
            sin = sinf(y);
            cos = cosf(y);
        } else {
            cos = one;
            sin = zero;
        }

        cmf->yx = zero;
        cmf->wx = zero;
        cmf->xy = zero;
        cmf->zy = zero;
        cmf->wy = zero;
        cmf->yz = zero;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->yy = one;
        cmf->ww = one;
        cmf->xx = cos;
        cmf->zz = cos;
        cmf->zx = -sin;
        cmf->xz = sin;
    }
}

void Matrix_RotateZS(s16 z, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempY;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (z != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS_MM(z);
            cos = Math_CosS_MM(z);

            tempX = cmf->xx;
            tempY = cmf->xy;
            cmf->xx = tempX * cos + tempY * sin;
            cmf->xy = tempY * cos - tempX * sin;

            tempX = cmf->yx;
            tempY = cmf->yy;
            cmf->yx = tempX * cos + tempY * sin;
            cmf->yy = tempY * cos - tempX * sin;

            tempX = cmf->zx;
            tempY = cmf->zy;
            cmf->zx = tempX * cos + tempY * sin;
            cmf->zy = tempY * cos - tempX * sin;

            tempX = cmf->wx;
            tempY = cmf->wy;
            cmf->wx = tempX * cos + tempY * sin;
            cmf->wy = tempY * cos - tempX * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (z != 0) {
            sin = Math_SinS_MM(z);
            cos = Math_CosS_MM(z);
        } else {
            sin = zero;
            cos = one;
        }

        cmf->zx = zero;
        cmf->wx = zero;
        cmf->zy = zero;
        cmf->wy = zero;
        cmf->xz = zero;
        cmf->yz = zero;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->zz = one;
        cmf->ww = one;
        cmf->xx = cos;
        cmf->yy = cos;
        cmf->yx = sin;
        cmf->xy = -sin;
    }
}

void Matrix_RotateZF(f32 z, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempY;

    if (mode == MTXMODE_APPLY) {
        if (z != 0) {
            cmf = sCurrentMatrix;

            sin = sinf(z);
            cos = cosf(z);

            tempX = cmf->xx;
            tempY = cmf->xy;
            cmf->xx = tempX * cos + tempY * sin;
            cmf->xy = tempY * cos - tempX * sin;

            tempX = cmf->yx;
            tempY = cmf->yy;
            cmf->yx = tempX * cos + tempY * sin;
            cmf->yy = tempY * cos - tempX * sin;

            tempX = cmf->zx;
            tempY = cmf->zy;
            cmf->zx = tempX * cos + tempY * sin;
            cmf->zy = tempY * cos - tempX * sin;

            tempX = cmf->wx;
            tempY = cmf->wy;
            cmf->wx = tempX * cos + tempY * sin;
            cmf->wy = tempY * cos - tempX * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (z != 0) {
            sin = sinf(z);
            cos = cosf(z);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->zx = 0.0f;
        cmf->wx = 0.0f;
        cmf->zy = 0.0f;
        cmf->wy = 0.0f;
        cmf->xz = 0.0f;
        cmf->yz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->zz = 1.0f;
        cmf->ww = 1.0f;
        cmf->xx = cos;
        cmf->yy = cos;
        cmf->yx = sin;
        cmf->xy = -sin;
    }
}


#define COLORFILTER_COLORFLAG_NONE 0xC000
#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000


#define SUBS_SHADOW_TEX_WIDTH 64
#define SUBS_SHADOW_TEX_HEIGHT 64
#define SUBS_SHADOW_TEX_SIZE ((s32)sizeof(u8[SUBS_SHADOW_TEX_HEIGHT][SUBS_SHADOW_TEX_WIDTH]))
typedef void* TexturePtr;

typedef enum {
    /*  0 */ ACTOR_DRAW_DMGEFF_FIRE,
    /*  1 */ ACTOR_DRAW_DMGEFF_BLUE_FIRE,
    /* 10 */ ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX = 10,
    /* 11 */ ACTOR_DRAW_DMGEFF_FROZEN_SFX,
    /* 20 */ ACTOR_DRAW_DMGEFF_LIGHT_ORBS = 20,
    /* 21 */ ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS,
    /* 30 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL = 30,
    /* 31 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM,
    /* 32 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE
} ActorDrawDamageEffectType;



u64 gEffFireMask1Tex[] = {
#include "eff_fire_mask_1.i4.inc.h"
};

u64 gEffFireMask2Tex[] = {
#include "eff_fire_mask_2.i4.inc.h"
};

u64 gEffIceFragmentTex[] = {
#include "eff_ice_fragment.i8.inc.h"
};

u64 gElectricSpark1Tex[] = {
#include "electric_spark_1.i4.inc.h"
};

u64 gElectricSpark2Tex[] = {
#include "electric_spark_2.i4.inc.h"
};

u64 gElectricSpark3Tex[] = {
#include "electric_spark_3.i4.inc.h"
};

u64 gElectricSpark4Tex[] = {
#include "electric_spark_4.i4.inc.h"
};

u64 gFlashTex[] = {
#include "flash.i8.inc.h"
};

Vtx gameplay_keepVtx_0502C0[] = {
#include "gameplay_keepVtx_0502C0.vtx.inc"
};

Vtx gameplay_keepVtx_050740[] = {
#include "gameplay_keepVtx_050740.vtx.inc"
};

u64 gEffIceSmokeTex[] = {
#include "eff_ice_smoke.i4.inc.h"
};

Vtx gameplay_keepVtx_051080[] = {
#include "gameplay_keepVtx_051080.vtx.inc"
};

Vtx gameplay_keepVtx_023440[] = {
#include "gameplay_keepVtx_023440.vtx.inc"
};

Vtx gameplay_keepVtx_023308[] = {
#include "gameplay_keepVtx_023308.vtx.inc"
};

u64 gameplay_keep_Tex_06ABF8[] = {
#include "tex_06ABF8.rgba16.inc.h"
};

Vtx gameplay_keepVtx_06A980[] = {
#include "gameplay_keepVtx_06A980.vtx.inc"
};

Gfx gameplay_keep_DL_06AB30[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gameplay_keep_Tex_06ABF8, G_IM_FMT_RGBA, G_IM_SIZ_16b, 36, 36, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(gameplay_keepVtx_06A980, 12, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(10, 0, 2, 0, 10, 2, 11, 0),
    gsSPVertex(&gameplay_keepVtx_06A980[12], 7, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(5, 1, 0, 0, 6, 4, 3, 0),
    gsSPVertex(&gameplay_keepVtx_06A980[19], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPVertex(&gameplay_keepVtx_06A980[23], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment1DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, 15),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR |
                G_TX_WRAP, 5, 1),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0x80, 170, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_0502C0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[33], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP1Triangle(5, 6, 7, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment2MaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, 15),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR |
                G_TX_WRAP, 5, 1),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x08000000),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment2ModelDL[] = {
    gsSPVertex(gameplay_keepVtx_0502C0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[33], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP1Triangle(5, 6, 7, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment3DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 1, G_TX_NOLOD),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, 1, G_TX_NOMIRROR | G_TX_WRAP, 5,
                2),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0x80, 0x80, 170, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_050740, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_050740[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSP2Triangles(13, 15, 16, 0, 17, 18, 19, 0),
    gsSP2Triangles(17, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 2, 1, 0),
    gsSP2Triangles(27, 28, 29, 0, 27, 29, 30, 0),
    gsSPVertex(&gameplay_keepVtx_050740[34], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSP2Triangles(20, 17, 21, 0, 22, 23, 24, 0),
    gsSP2Triangles(25, 24, 26, 0, 27, 28, 29, 0),
    gsSP1Triangle(27, 29, 30, 0),
    gsSPVertex(&gameplay_keepVtx_050740[65], 28, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 5, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSP2Triangles(4, 16, 3, 0, 17, 18, 19, 0),
    gsSP2Triangles(9, 20, 5, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 0, 2, 27, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceSmokeDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(gEffFireMask1Tex, G_IM_FMT_I, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                            G_TX_WRAP, 5, 6, G_TX_NOLOD, 15),
    gsDPLoadMultiBlock_4b(gEffIceSmokeTex, 0x0100, 1, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                          G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, ENV_ALPHA, TEXEL1, TEXEL0, 1, ENVIRONMENT, TEXEL1, PRIMITIVE, ENVIRONMENT, COMBINED,
                       ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetEnvColor(255, 255, 255, 128),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_051080, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gFrozenSteamMaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(gEffFireMask1Tex, G_IM_FMT_I, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                            G_TX_WRAP, 5, 6, G_TX_NOLOD, 15),
    gsDPLoadMultiBlock_4b(gEffIceSmokeTex, 0x0100, 1, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                          G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, ENV_ALPHA, TEXEL1, TEXEL0, 1, ENVIRONMENT, TEXEL1, PRIMITIVE, ENVIRONMENT, COMBINED,
                       ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetEnvColor(255, 255, 255, 128),
    gsSPEndDisplayList(),
};

Gfx gFrozenSteamModelDL[] = {
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_051080, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gElectricSparkMaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(0x08000000, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_OVL_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gElectricSparkModelDL[] = {
    gsSPVertex(gameplay_keepVtx_023440, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gLightOrbMaterial1DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gFlashTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gLightOrbMaterial2DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gFlashTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_DECAL2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gLightOrbModelDL[] = {
    gsSPVertex(gameplay_keepVtx_023308, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};


MtxF* sCurrentMatrix; //!< original name: "Matrix_now"


Hilite* func_800BCC68(Vec3f* arg0, PlayState* play) {
    Vec3f lightDir;

    lightDir.x = play->envCtx.dirLight1.params.dir.x;
    lightDir.y = play->envCtx.dirLight1.params.dir.y;
    lightDir.z = play->envCtx.dirLight1.params.dir.z;

    return Hilite_DrawXlu(arg0, &play->view.eye, &lightDir, play->state.gfxCtx);
}


TexturePtr sElectricSparkTextures[] = {
    gElectricSpark1Tex,
    gElectricSpark2Tex,
    gElectricSpark3Tex,
    gElectricSpark4Tex,
};

/**
 * Draw common damage effects applied to each body part provided in bodyPartsPos
 */
void Actor_DrawDamageEffects(PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale,
                             f32 frozenSteamScale, f32 effectAlpha, u8 type) {
    if (effectAlpha > 0.001f) {
        s32 twoTexScrollParam;
        s16 bodyPartIndex;
        MtxF* currentMatrix;
        f32 alpha;
        f32 frozenScale;
        f32 lightOrbsScale;
        f32 electricSparksScale;
        f32 steamScale;
        Vec3f* bodyPartsPosStart = bodyPartsPos;
        u32 gameplayFrames = play->gameplayFrames;
        f32 effectAlphaScaled;

        currentMatrix = Matrix_GetCurrent();

        // Apply sfx along with damage effect
        if ((actor != NULL) && (effectAlpha > 0.05f) && (play->gameOverCtx.state == GAMEOVER_INACTIVE)) {
            if (type == ACTOR_DRAW_DMGEFF_FIRE) {
                Audio_PlayActorSfx2(actor, NA_SE_EV_BURN_OUT - SFX_FLAG);
            } else if (type == ACTOR_DRAW_DMGEFF_BLUE_FIRE) {
                //Audio_PlayActorSfx2(actor, NA_SE_EN_EXTINCT - SFX_FLAG);
                SoundEffect_PlayHeld(SOUND_MM_ENEMY_EXTINCT_LOOP, 0.5f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
            } else if (type == ACTOR_DRAW_DMGEFF_FROZEN_SFX) {
                Audio_PlayActorSfx2(actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
            } else if ((type == ACTOR_DRAW_DMGEFF_LIGHT_ORBS) || (type == ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS)) {
                //Audio_PlayActorSfx2(actor, NA_SE_EN_COMMON_DEADLIGHT - SFX_FLAG);
                SoundEffect_PlayHeld(SOUND_MM_LIGHT_DAMAGE, 1.0f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
            }
        }

        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        Gfx_SetupDL_25Xlu(play->state.gfxCtx);

        switch (type) {
            case ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX:
            case ACTOR_DRAW_DMGEFF_FROZEN_SFX:
                frozenScale = ((KREG(19) * 0.01f) + 2.3f) * effectScale;
                steamScale = ((KREG(28) * 0.0001f) + 0.035f) * frozenSteamScale;
                func_800BCC68(bodyPartsPos, play);

                // Setup to draw ice over frozen actor

                gSPSegment(POLY_XLU_DISP++, 0x08,
                           Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, gameplayFrames & 0xFF, 32, 16, 1, 0,
                                            (gameplayFrames * 2) & 0xFF, 64, 32));
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 170, 255, 255, 255);
                gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment2MaterialDL);

                effectAlphaScaled = effectAlpha * 255.0f;

                // Apply and draw ice over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    alpha = bodyPartIndex & 3;
                    alpha = effectAlphaScaled - (30.0f * alpha);
                    if (effectAlphaScaled < (30.0f * (bodyPartIndex & 3))) {
                        alpha = 0.0f;
                    }
                    if (alpha > 255.0f) {
                        alpha = 255.0f;
                    }

                    gDPSetEnvColor(POLY_XLU_DISP++, KREG(20) + 200, KREG(21) + 200, KREG(22) + 255, (u8)alpha);

                    Matrix_Translate(bodyPartsPos->x, bodyPartsPos->y, bodyPartsPos->z, MTXMODE_NEW);
                    Matrix_Scale(frozenScale, frozenScale, frozenScale, MTXMODE_APPLY);

                    if (bodyPartIndex & 1) {
                        Matrix_RotateYF(M_PI, MTXMODE_APPLY);
                    }

                    if (bodyPartIndex & 2) {
                        Matrix_RotateZF(M_PI, MTXMODE_APPLY);
                    }

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment2ModelDL);
                }

                bodyPartsPos = bodyPartsPosStart; // reset bodyPartsPos

                // Setup to draw steam over frozen actor

                gDPSetColorDither(POLY_XLU_DISP++, G_CD_BAYER);
                gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_PATTERN);
                gSPDisplayList(POLY_XLU_DISP++, gFrozenSteamMaterialDL);

                alpha = effectAlpha * 100.0f;
                if (alpha > 100.0f) {
                    alpha = 100.0f;
                }

                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 225, 235, (u8)alpha);

                // Apply and draw steam over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    twoTexScrollParam = ((bodyPartIndex * 3) + gameplayFrames);
                    gSPSegment(POLY_XLU_DISP++, 0x08,
                               Gfx_TwoTexScroll(play->state.gfxCtx, 0, twoTexScrollParam * 3, twoTexScrollParam * -12,
                                                32, 64, 1, 0, 0, 32, 32));

                    Matrix_Translate(bodyPartsPos->x, bodyPartsPos->y, bodyPartsPos->z, MTXMODE_NEW);
                    Matrix_ReplaceRotation(&play->billboardMtxF);
                    Matrix_Scale(steamScale, steamScale, 1.0f, MTXMODE_APPLY);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gFrozenSteamModelDL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_FIRE:
            case ACTOR_DRAW_DMGEFF_BLUE_FIRE:
                if (type == ACTOR_DRAW_DMGEFF_FIRE) {
                    gDPSetEnvColor(POLY_XLU_DISP++, 255, 10, 0, 0);
                } else {
                    gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, 0);
                    // Reuse type for blue primitive color
                    type = 255;
                }

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale((effectScale * 0.005f) * 1.35f, (effectScale * 0.005f), (effectScale * 0.005f) * 1.35f,
                             MTXMODE_APPLY);

                effectAlphaScaled = effectAlpha * 255.0f;

                // Apply and draw fire on every body part
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    alpha = bodyPartIndex & 3;
                    alpha = effectAlphaScaled - 30.0f * alpha;
                    if (effectAlphaScaled < 30.0f * (bodyPartIndex & 3)) {
                        alpha = 0.0f;
                    }
                    if (alpha > 255.0f) {
                        alpha = 255.0f;
                    }

                    // Use type for blue primitive color
                    // = 0 for ACTOR_DRAW_DMGEFF_FIRE
                    // = 255 for ACTOR_DRAW_DMGEFF_BLUE_FIRE
                    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, type, (u8)alpha);

                    gSPSegment(POLY_XLU_DISP++, 0x08,
                               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0,
                                                ((bodyPartIndex * 10 + gameplayFrames) * -20) & 0x1FF, 32, 128));

                    Matrix_RotateYF(M_PI, MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = bodyPartsPos->x;
                    currentMatrix->mf[3][1] = bodyPartsPos->y;
                    currentMatrix->mf[3][2] = bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_LIGHT_ORBS:
            case ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS:

                // Setup to draw light orbs on actor

                lightOrbsScale = ((KREG(19) * 0.01f) + 4.0f) * effectScale;

                gSPDisplayList(POLY_XLU_DISP++, gLightOrbMaterial1DL);

                alpha = effectAlpha * 255.0f;
                if (alpha > 255.0f) {
                    alpha = 255.0f;
                }

                if (type == ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS) {
                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)(sREG(16) + 255), (u8)(sREG(17) + 255),
                                    (u8)(sREG(18) + 255), (u8)alpha);

                    gDPSetEnvColor(POLY_XLU_DISP++, (u8)sREG(19), (u8)(sREG(20) + 255), (u8)(sREG(21) + 255), 128);
                } else {
                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 200, (u8)alpha);

                    gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 100, 128);
                }

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale(lightOrbsScale, lightOrbsScale, 1.0f, MTXMODE_APPLY);

                // Apply and draw a light orb over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    Matrix_RotateZF(Rand_CenteredFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = bodyPartsPos->x;
                    currentMatrix->mf[3][1] = bodyPartsPos->y;
                    currentMatrix->mf[3][2] = bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gLightOrbModelDL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL:
            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM:
            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE:
                if (type == ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL) {
                    electricSparksScale = (KREG(19) * 0.01f + 1.0f) * effectScale;
                } else if (type == ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM) {
                    electricSparksScale = (KREG(19) * 0.01f + 1.5f) * effectScale;
                } else {
                    electricSparksScale = (KREG(19) * 0.01f + 2.0f) * effectScale;
                }

                gSPSegment(POLY_XLU_DISP++, 0x08,
                           Lib_SegmentedToVirtual(sElectricSparkTextures[play->gameplayFrames % 4]));

                gSPDisplayList(POLY_XLU_DISP++, gElectricSparkMaterialDL);

                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)(sREG(16) + 255), (u8)(sREG(17) + 255), (u8)(sREG(18) + 150),
                                (u8)(sREG(19) + 255));

                gDPSetEnvColor(POLY_XLU_DISP++, (u8)(sREG(20) + 255), (u8)(sREG(21) + 255), (u8)sREG(22), (u8)sREG(23));

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale(electricSparksScale, electricSparksScale, electricSparksScale, MTXMODE_APPLY);

                // Every body part draws two electric sparks at random orientations
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    // first electric spark
                    Matrix_RotateXFApply(Rand_ZeroFloat(2 * M_PI));
                    Matrix_RotateZF(Rand_ZeroFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->x;
                    currentMatrix->mf[3][1] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->y;
                    currentMatrix->mf[3][2] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gElectricSparkModelDL);

                    // second electric spark
                    Matrix_RotateXFApply(Rand_ZeroFloat(2 * M_PI));
                    Matrix_RotateZF(Rand_ZeroFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->x;
                    currentMatrix->mf[3][1] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->y;
                    currentMatrix->mf[3][2] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gElectricSparkModelDL);
                }

                break;
        }

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
}


void Actor_SpawnIceEffects(PlayState* play, Actor* actor, Vec3f limbPos[], s32 limbPosCount, s32 effectsPerLimb,
                           f32 scale, f32 scaleRange) {
    static Color_RGBA8 primColor = { 170, 255, 255, 255 };
    static Color_RGBA8 envColor = { 200, 200, 255, 255 };
    static Vec3f accel = { 0.0f, -1.0f, 0.0f };
    s32 i;
    s32 pad;
    Vec3f velocity;
    s16 randomYaw;
    s16 yaw;
    s32 j;

    SfxSource_PlaySfxAtFixedWorldPos(play, &actor->world.pos, 30, NA_SE_EV_ICE_BROKEN);
    switch (Rand_S16Offset(0,2))
    {
        case 0:
            SoundEffect_PlayOneshot(SOUND_MM_GLASS_SHATTER, 1.0f, 1.5f, &actor->world.pos, 0.0f, 1000.0f, &actor);
        break;
        case 1:
        default:
            SoundEffect_PlayOneshot(SOUND_RAREWARE_GLASS_SHATTER, 1.0f, 1.5f, &actor->world.pos, 0.0f, 1000.0f, &actor);
        break;
    }

    for (i = 0; i < limbPosCount; i++) {
        yaw = Actor_WorldYawTowardPoint(actor, limbPos);

        for (j = 0; j < effectsPerLimb; j++) {
            randomYaw = ((s32)Rand_Next() >> 0x13) + yaw;

            velocity.z = Rand_ZeroFloat(5.0f);

            velocity.x = Math_SinS_MM(randomYaw) * velocity.z;
            velocity.y = Rand_ZeroFloat(4.0f) + 8.0f;
            velocity.z *= Math_CosS_MM(randomYaw);

            EffectSsEnIce_Spawn(play, limbPos, Rand_ZeroFloat(scaleRange) + scale, &velocity, &accel, &primColor,
                                &envColor, 30);
        }

        limbPos++;
    }
}

MtxF* sCurrentMatrix; //!< original name: "Matrix_now"



void Matrix_MultZero(Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw;
    dest->y = cmf->yw;
    dest->z = cmf->zw;
}

void Matrix_MultVecZ(f32 z, Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw + cmf->xz * z;
    dest->y = cmf->yw + cmf->yz * z;
    dest->z = cmf->zw + cmf->zz * z;
}

const u8 sShadowMaps[4][12][12] = {
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
    },
};


void SubS_FillShadowTex(s32 startCol, s32 startRow, u8* tex, s32 size) {
    s32 i;
    s32 j;
    s32 start;

    for (i = 0; i < 12; i++) {
        start = ((startRow + i) * 64) + startCol - 390;
        for (j = 0; j < 12; j++) {
            if (sShadowMaps[size][i][j] != 0) {
                if ((start + j >= 0) && (start + j < SUBS_SHADOW_TEX_SIZE)) {
                    tex[start + j] = 255;
                }
            }
        }
    }
}


void SubS_GenShadowTex(Vec3f bodyPartsPos[], Vec3f* worldPos, u8* tex, f32 tween, u8 bodyPartsNum, u8 sizes[],
                       s8 parentBodyParts[]) {
    Vec3f pos;
    Vec3f startVec;
    s32 i;
    s32 parentBodyPart;
    Vec3f* bodyPartPos;
    s32 startCol;
    s32 startRow;

    for (i = 0; i < bodyPartsNum; i++) {
        if (parentBodyParts[i] >= 0) {
            parentBodyPart = parentBodyParts[i];
            bodyPartPos = &bodyPartsPos[i];

            pos.x = (bodyPartsPos[parentBodyPart].x - bodyPartPos->x) * tween + (bodyPartPos->x - worldPos->x);
            pos.y = (bodyPartsPos[parentBodyPart].y - bodyPartPos->y) * tween + (bodyPartPos->y - worldPos->y);
            pos.z = (bodyPartsPos[parentBodyPart].z - bodyPartPos->z) * tween + (bodyPartPos->z - worldPos->z);
        } else {
            bodyPartPos = &bodyPartsPos[i];

            pos.x = bodyPartPos->x - worldPos->x;
            pos.y = bodyPartPos->y - worldPos->y;
            pos.z = bodyPartPos->z - worldPos->z;
        }

        Matrix_MultVec3f(&pos, &startVec);
        startCol = 64.0f + startVec.x;
        startRow = 64.0f - startVec.z;
        SubS_FillShadowTex(startCol >> 1, startRow >> 1, tex, sizes[i]);
    }
}


void SubS_DrawShadowTex(Actor* actor, GameState* gameState, u8* tex) {
    s32 pad;
    GraphicsContext* gfxCtx = gameState->gfxCtx;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, 100);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    Matrix_Translate(actor->world.pos.x, 0.0f, actor->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(0.6f, 1.0f, 0.6f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    //gSPDisplayList(POLY_OPA_DISP++, gShadowMaterialDL);
    gDPLoadTextureBlock(POLY_OPA_DISP++, tex, G_IM_FMT_I, G_IM_SIZ_8b, SUBS_SHADOW_TEX_WIDTH, SUBS_SHADOW_TEX_HEIGHT, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
    //gSPDisplayList(POLY_OPA_DISP++, gShadowModelDL);

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}

