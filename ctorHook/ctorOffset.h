#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <jni.h>
#include <pthread.h>
#include <stdio.h>
#include <android/log.h>
#include <iostream>
#include <fstream>
#include <array>
#include <unordered_map>


std::unordered_map< std::string , bool > sBool;

std::vector<itemInfo> itemData;
std::vector<charInfo> charData;
std::vector<watcher> watch;
std::vector<deadbox> deadboxF;
std::vector<planeID> dropplane;

RoleSkinFields* roleskinFields;
RolePackFields* packfields;
CharacterModelFields* characterfields;
RoleConfFields* roleFields;
ItemResourceFields* itemFields;
WeaponConfFields* weaponconfFields;
Item2InventoryFields* item2Fields;
WeaponAssetGroupFields* weaponAssetFields;
WeaponFireEffectFields* weaponfireFields;
WeaponConfExtraFields* weaponextraFields;
MythicArmorFields* mythicarmorFields;
MythicSightFields* mythicsightFields;
KillEffectItemFields* killeffectFields;
BRDeadboxSkinFields* deadboxFields;
BRDropPlaneSkinFields* dropplaneFields;

    uintptr_t Item2InventoryAddress = 0x4C98FE0;
    uintptr_t WeaponConfAddress = 0x4CFA220;
    uintptr_t WeaponConfExtraAddress = 0x4CFB704;
    uintptr_t WeaponFireEffectAddress = 0x4CFCDE8;
    uintptr_t CharacterModelAddress = 0x872B314;
    uintptr_t BRDeadboxSkinAddress = 0x871944C;
    uintptr_t BRDropPlaneSkinAddress = 0x871B91C;
    uintptr_t WeaponAssetGroupAddress = 0x4CF620C;
    uintptr_t MythicArmorConfigAddress = 0x4CB98D8;
    uintptr_t MythicSightConfigAddress = 0x4CB9DD8;
    uintptr_t KillEffectItemConfConfigAddress = 0x4CA8B58;
    uintptr_t WeaponSkinAddress = 0x4D04F10;
    uintptr_t ItemResourceAddress = 0x4CA6544;
    uintptr_t RoleConfAddress = 0x4CCA000;
    uintptr_t RoleSkinAddress = 0x4CCF63C;
    uintptr_t RolePackConfAddress = 0x4CCE174;
    uintptr_t WeaponConfName = 0x4C9B2C8;
    uintptr_t GetDropPlaneName = 0x871BF88;
    uintptr_t GetNameRoleSkin = 0x4CCF760;
    uintptr_t GetRoleConfName = 0x4CCB750;
    uintptr_t GetDeadBoxIDAddress = 0x4EAB130;