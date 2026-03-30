/*
 * ╔═══════════════════════════════════════════════════════════════════════════╗
 * ║                                                                           ║
 * ║                     __            ___     __    ____                      ║
 * ║                    |  |   _ _ ___| | |___|  |  |    \                     ║
 * ║                    |  |__| | |  _|_  |   |  |__|  |  |                    ║
 * ║                    |_____|_  |___| |_|_|_|_____|____/                     ║
 * ║                          |___|                                            ║
 * ║                                                                           ║
 * ║                    AYLNHAX V3 — COD:M Mod Source                          ║
 * ║                                                                           ║
 * ║  This is an open-source COD:M mod released for educational purposes.      ║
 * ║  Made for future modders to study, learn and build on.                    ║
 * ║                                                                           ║
 * ║  NOTE: Offsets, feature implementations, and bypass                       ║
 * ║  have been intentionally removed to prevent fake devs from                ║
 * ║  renaming and selling this work as their own without any efforts          ║
 * ║  put into it.                                                             ║
 * ║                                                                           ║
 * ║  + DISCLAIMER                                                             ║
 * ║  Do NOT just rename this project and sell it. At least put some effort    ║
 * ║  into it. Use it to learn, to improve, not for personal profits.          ║
 * ║  Fake devs who profit off others work are a plague, don't be one.         ║
 * ║                                                                           ║
 * ║  Author      │  Lyc4nLĐ (@lyc4nld) >w<                                    ║
 * ║  License     │  Open Source — Free to learn, NOT to sell  OwO             ║
 * ║  Version     │  V3                                                        ║
 * ╚═══════════════════════════════════════════════════════════════════════════╝
 */
                                   
#include "Includes/Logger.h"
#include "Includes/Macros.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "ImGui/Call_ImGui.h"
#include "IL2CppSDKGenerator/BasicStructs/Call_BasicStructs.h"
#include "IL2CppSDKGenerator/IL2Cpp/Call_IL2Cpp.h"
#include "Hacks/Hacks.h"
#include "IL2CppSDKGenerator/KittyMemory/MemoryPatch.h"
// #include <LycnProc/AntiCrack.h>
#include "foxcheats/include/ScanEngine.hpp"
#include "ctorHook/ConstructorHook.hpp"
#include "FTools/Iconcpp.h"
#include "FTools/Font.h"
#include "FTools/TitleFont.h"
#include "FTools/Icon.h"
//#include "FTools/Battery.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>
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
#include "IMAGE/astral.h"
#include <android/bitmap.h>
#include <jni.h>
#include "Fields.h"

#include "oxorany/source/oxorany.h"
#include "oxorany/source/oxorany.cpp"
#include "oxorany/source/oxorany_include.h"

#include "Noctivagous/StrEnc.h"
#include "Noctivagous/json.hpp"
#include "Noctivagous/md5.h"
#include "curl/curl.h"
#include "HideProcess/ProcessHide.h"

class _BYTE;

class _BOOL4;

class _BOOL8;

class _WORD;

class _DWORD;

class _QWORD;

bool ClearDisplay = false;
bool SnowB = false;
float SnowBsize = 0.0f;
bool isSpeedHackEnabled = false;
float speedHackMultiplier = 0.0f;
bool isJumpAdjustmentEnabled = false;
float jumpHeightMultiplier = 1.0f;

// Astral Texture variables
static GLuint astralTextureID = 0;
static bool astralLoaded = false;

// Title Font variable
static ImFont* TitleFont = nullptr;

// Forward declaration for JavaVM
extern JavaVM *VM;

// Load texture from memory using Android's BitmapFactory
GLuint LoadAstralTexture(const unsigned char* data, size_t size) {
    if (astralTextureID != 0) return astralTextureID;
    
    JNIEnv* env = nullptr;
    JavaVM* vm = VM;
    if (!vm) return 0;
    
    bool needsDetach = false;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        if (vm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
            needsDetach = true;
        } else {
            return 0;
        }
    }
    
    // Find the byte array class and constructor
    jclass byteArrayClass = env->FindClass("[B");
    if (!byteArrayClass) {
        if (needsDetach) vm->DetachCurrentThread();
        return 0;
    }
    
    // Create byte array
    jbyteArray byteArray = env->NewByteArray(size);
    if (!byteArray) {
        if (needsDetach) vm->DetachCurrentThread();
        return 0;
    }
    
    // Copy data to byte array
    env->SetByteArrayRegion(byteArray, 0, size, (const jbyte*)data);
    
    // Find BitmapFactory.decodeByteArray
    jclass bitmapFactoryClass = env->FindClass("android/graphics/BitmapFactory");
    if (!bitmapFactoryClass) {
        env->DeleteLocalRef(byteArray);
        if (needsDetach) vm->DetachCurrentThread();
        return 0;
    }
    
    jmethodID decodeByteArrayMethod = env->GetStaticMethodID(bitmapFactoryClass, "decodeByteArray", "([BII)Landroid/graphics/Bitmap;");
    if (!decodeByteArrayMethod) {
        env->DeleteLocalRef(byteArray);
        if (needsDetach) vm->DetachCurrentThread();
        return 0;
    }
    
    // Decode the bitmap
    jobject bitmap = env->CallStaticObjectMethod(bitmapFactoryClass, decodeByteArrayMethod, byteArray, 0, size);
    env->DeleteLocalRef(byteArray);
    
    if (!bitmap) {
        if (needsDetach) vm->DetachCurrentThread();
        return 0;
    }
    
    // Get bitmap info
    jmethodID getWidth = env->GetMethodID(env->GetObjectClass(bitmap), "getWidth", "()I");
    jmethodID getHeight = env->GetMethodID(env->GetObjectClass(bitmap), "getHeight", "()I");
    int width = env->CallIntMethod(bitmap, getWidth);
    int height = env->CallIntMethod(bitmap, getHeight);
    
    // Lock pixels
    AndroidBitmapInfo info;
    info.width = width;
    info.height = height;
    info.format = ANDROID_BITMAP_FORMAT_RGBA_8888;
    
    void* pixels = nullptr;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        env->DeleteLocalRef(bitmap);
        if (needsDetach) vm->DetachCurrentThread();
        return 0;
    }
    
    // Create OpenGL texture
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Use GL_LINEAR for smooth rendering with transparent PNGs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    
    AndroidBitmap_unlockPixels(env, bitmap);
    env->DeleteLocalRef(bitmap);
    
    if (needsDetach) vm->DetachCurrentThread();
    
    return textureId;
}


std::unordered_map< std::string , bool > sBool;
std::unordered_map<int, int> activeKillEffects;

int (*orig_GetCurrentWeaponKillEffect)(Weapon);
int _GetCurrentWeaponKillEffect(Weapon weapon) {
    Pawn* localPawn = GamePlay::get_LocalPawn();
    if (!localPawn) return 0;

    Weapon* currentWeapon = localPawn->get_CurrentWeapon();
    if (!currentWeapon) return 0;

    int currentID = currentWeapon->get_WeaponID();

    auto it = activeKillEffects.find(currentID);
    if (it != activeKillEffects.end()) {
        return it->second;
    }

    return 0;
}
    


#define ReadInt(base, offset) (*(int*)((uintptr_t)(base) + (offset)))
#define ReadBool(base, offset) (*(bool*)((uintptr_t)(base) + (offset)))
#define ReadFloat(base, offset) (*(float*)((uintptr_t)(base) + (offset)))
#define ReadByte(base, offset) (*(uint8_t*)((uintptr_t)(base) + (offset)))
#define READ_PTR(type, base, offset) (*(type**)((uintptr_t)(base) + (offset)))


std::unordered_map<std::string, bool> getplane;
std::unordered_map<std::string, bool> getguns;


void *GameUpdate(void *) {
    while (true) {
        
                    
    if (!loadCharacter && CharacterModelConfigInstance.size() > 0 && itemResourceConfigInstance.size() > 0
    && RoleConfConfigInstance.size() > 0 && RoleSkinConfigInstance.size() > 0
    && RolePackConfConfigInstance.size() > 0 && BRDeadboxSkinConfigInstance.size() > 0) {

    
    for (auto &a : RoleSkinConfigInstance) {
        if (!a) continue;

        auto *fx = (RoleSkinFields *) ((uintptr_t) a + 0x18); // public byte ClothType; //0x18 
        if ((fx->FxAssetID_1P ^ 0) != 0) {
            std::string n = GetNameString(GetNameRoleSkin, (void *) a);
            watch.push_back({n, fx->FxAssetID_1P});
        }
    }

    std::string _g = "";
    for (auto &z : BRDeadboxSkinConfigInstance) {
        if (!z) continue;

        auto *y = (BRDeadboxSkinFields *) ((uintptr_t) z + 0x10); //public int ColorID;
        bool __b = false;

        for (auto &q : RoleConfConfigInstance) {
            if (!q) continue;

            auto *f = (RoleConfFields *) ((uintptr_t) q + 0x14); // public int DeadBoxEffectAsset;
            std::string s = GetNameString(GetRoleConfName, q);

            if ((y->ID & 0xFFFFFFF) != 0) {
                auto fx = (int (*)(void *))(getRealOffset(GetDeadBoxIDAddress));
                int k = fx(q);

                if (y->ID == k && !__b) {
                    auto dump = [&](const std::string &nm) {
                        deadboxF.push_back({nm, {
                            y->ColorID,
                            y->DeadBoxEffectAsset,
                            y->Flag,
                            y->FlagAsset,
                            y->ModelAsset3P,
                            y->ModelAssetUI
                        }});
                    };

                    if (y->ColorID == 5) {
                        _g = s;
                        dump(s);
                        __b = true;
                    } else {
                        dump(s);
                    }
                }

                if (__b) break;

                if (k + 1 == y->ID) {
                    if (y->ColorID == 5 && y->FlagAsset != 0 && !_g.empty()) {
                        std::string v = _g + " (Variant)";
                        deadboxF.push_back({v, {
                            y->ColorID,
                            y->DeadBoxEffectAsset,
                            y->Flag,
                            y->FlagAsset,
                            y->ModelAsset3P,
                            y->ModelAssetUI
                        }});
                    }
                }
            }
        }
    }








//=================================================================================
            
    for (auto X0 : CharacterModelConfigInstance) {
    if (!X0) continue;
    auto *X1 = (CharacterModelFields *)((uintptr_t)X0 + 0x10); // public bool Female:
    auto A = X1->ItemID;

    auto B = X1->BRBagModel;
    auto C = X1->BRHeadModel;
    auto D = X1->BRLobby;
    auto E = X1->BRModel;
    auto F = X1->BindEffect1P;
    auto G = X1->ChangeClipEffect1P;
    auto H = X1->DefaultModelID;
    auto I = X1->Guarder1P;
    auto J = X1->Guarder3P;
    auto K = X1->GuarderBagModel;
    auto L = X1->GuarderHeadModel;
    auto M = X1->GuarderLobby;
    auto N = X1->Traitor1P;

    for (auto Y0 : itemResourceConfigInstance) {
        if (!Y0) continue;
        auto *Y1 = (ItemResourceFields *)((uintptr_t)Y0 + 0x10); //public int FxAssetID;

        auto A_ = Y1->AvatarModelID;

        if (A != A_) continue;

        auto P = Y1->FxAssetID;
        auto Q = Y1->InventoryModelID;
        auto R = Y1->ModelAssetIDRaw;

        auto S = Y1->UIMiniSpriteName;
        auto T = Y1->UISmallSpriteName;
        auto U = Y1->UISpriteName;
        auto V = Y1->UISquareSpriteName;

        auto W = Y1->ID;

        for (auto Z0 : RoleConfConfigInstance) {
            if (!Z0) continue;
            auto *Z1 = (RoleConfFields *)((uintptr_t)Z0 + 0x14); //private uint roleLeftArmID;

            if (Z1->ID != W) continue;

            auto a = Z1->roleLeftArmID;
            auto b = Z1->roleFinalSuitID;
            auto c = Z1->roleBasicHologramID;
            auto d = Z1->ColorID;
            auto e = Z1->ColorSubID;
            auto f = Z1->ShowRare;
            auto g = Z1->RoleLvGroupID;
            auto h = Z1->RolePackID;

            int i = 0, j = 0;
            for (auto RP : RolePackConfConfigInstance) {
                if (!RP) continue;
                auto *rpF = (RolePackFields *)((uintptr_t)RP + 0x10); // private uint mMatchRoomPoseID;
                if (rpF->RolePackID != h) continue;
                if (rpF->LoadingFrame && rpF->LobbySceneType == 0) {
                    i = rpF->RolePackID;
                    j = rpF->LoadingFrame;
                }
            }

            std::string Zz, n0 = GetNameString(GetRoleConfName, Z0);
            if (n0.empty()) {
                if (!lastKnownName.empty()) {
                    char c_ = 'A' + emptyNameCount++;
                    n0 = lastKnownName + " " + c_ + "+";
                } else n0 = "UnknownName";
            } else {
                lastKnownName = n0;
                emptyNameCount = 0;
            }

            std::string key = n0 + "_" + std::to_string(A);
            if (nameCountChar.count(key)) {
                nameCountChar[key]++;
                n0 += " +" + std::to_string(nameCountChar[key]);
            } else nameCountChar[key] = 0;

            if (d == 5) Zz = "[M] " + n0;
            else if (d == 4) Zz = "[L] " + n0;
            else if (d == 3) Zz = "[E] " + n0;
            else Zz = "[C] " + n0;

            if (Zz.find("A+") != std::string::npos) h--;

            charData.push_back({
                Zz,
                {B, C, D, E, F, G, H, I, J, K, L, M},
                {P, Q, R},
                {S, T, U, V},
                {a, b, c, d, e, f, g, h},
                {i, j}
            });
        }

        break;
    }

    loadCharacter = true;
}
}



//=================================================================================

            
        if (!loadskinhack 
    && weaponConfInstance.size() > 0 
    && itemInventoryInstance.size() > 0 
    && weaponAssetGroupInstance.size() > 0 
    && weaponFireEffectInstance.size() > 0 
    && weaponExtraInstance.size() > 0 
    && killEffectItemInstance.size() > 0
    && mythicArmorInstance.size() > 0
    && mythicSightInstance.size() > 0
    && itemResourceConfigInstance.size() > 0) {
             
       for (auto conf : weaponConfInstance) {       
            if (!conf) continue;
            
            auto baseID = *(int *) ((uintptr_t) conf + 0x34); // weaponconfFields->BaseWeaponId; //public uint BaseWeaponId;
            auto confID = *(int *) ((uintptr_t) conf + 0x40); // public uint ID;
            for (auto item : itemInventoryInstance) {    
                if (!item) continue;
                
                auto itemIDbase = *(int *) ((uintptr_t) item + 0x20); // classname: item2inventory //public int ItemID;
                int itemBaseModified;
            
                    int itemBase;
                    if (baseID == itemIDbase) {
                    itemBase =  *(int *) ((uintptr_t) item + 0x20); // public int ItemID;
                    itemBaseModified = itemBase + 200; 
                    }
                    
                    
                    if (confID == itemIDbase) {
                    auto confcolorID = *(uint8_t *) ((uintptr_t) conf + 0x22); // classname: class WeaponConfConfig //public byte ColorID;
                    auto itemIDskin2 = *(int *) ((uintptr_t) item + 0x24); //public int WeaponAssetGroupID;
                    auto itemIDskin3 = *(int *) ((uintptr_t) item + 0x28); //public int WeaponIconID;
                                   
  
                    std::string AName;
                    std::string getName = GetNameString(WeaponConfName, conf);
                        
                    if (confcolorID == 5) {
                    AName += "[M] " + getName;                     
                    } else if (confcolorID == 4) {
                    AName += "[L] " + getName;     
                    } else if (confcolorID == 3) {
                    AName += "[E] " + getName;     
                    } else {
                    AName += "[C] " + getName;     
                    }
                      
                    if (nameCountMap.find(AName) != nameCountMap.end()) {
                    nameCountMap[AName]++; 
                    AName += " +" + std::to_string(nameCountMap[AName]);
                    } else {
                    nameCountMap[AName] = 0;  
                    }
                        
                  
                        
                    if (!AName.empty()) {
                          
                               
int fireIds = 0, fireIds2 = 0, assetIds = 0;
int originalFireID = 0;

for (auto asset : weaponAssetGroupInstance) {
    if (!asset) continue;

    auto assetID = *(int *) ((uintptr_t) asset + 0x44); //public int Id;

    if (itemIDskin2 == assetID) {
        auto fireEffectID = *(int *) ((uintptr_t) asset + 0x40); // public int FireEffectGroupID;

        for (auto fireConf : weaponFireEffectInstance) {
            if (!fireConf) continue;

            int fireID = *(int *) ((uintptr_t) fireConf + 0x84); // classname: WeaponFireEffectConfigConfig //public int Id;
            int assetIdBulletSmoke = *(int *) ((uintptr_t) fireConf + 0x18); //public int AssetIdForBulletSmoke1P;

            if (AName.find("[M]") != std::string::npos) {
                if (fireEffectID == fireID) {

                    if (assetIdBulletSmoke != 0) {
                        fireIds = fireID;
                        originalFireID = fireID;
                        break; 
                    }

                    int nextFireID = fireID + 1;
                    bool found = false;

                    for (int i = 0; i < 10; i++) { 
                        for (auto nextFireConf : weaponFireEffectInstance) {
                            if (!nextFireConf) continue;

                            int nextFireIDCheck = *(int *) ((uintptr_t) nextFireConf + 0x84); // FIXED same with another
                            int nextAssetIdBulletSmoke = *(int *) ((uintptr_t) nextFireConf + 0x18); // FIXED same with another

                            if (nextFireIDCheck == nextFireID && nextAssetIdBulletSmoke != 0) {
                                fireIds = nextFireIDCheck;
                                found = true;
                                break; 
                            }
                        }
                        if (found) break; 
                        nextFireID++; 
                    }

                    if (fireIds == 0) fireIds = fireID; 

                    fireIds2 = fireID;
                    assetIds = assetID;
                }
            }
        }
    }
}


                        
                               
                               
                               
                                //============WeaponConfConfig
                                int confbaseskin = 0, confSkinID = 0, confBrocastID = 0, confColorID = 0;
                                if (confID == itemIDbase) { 
                                confbaseskin = ReadInt(conf, 0x34); //public uint BaseWeaponId;
                                confColorID = ReadByte(conf, 0x22); //public byte ColorID;
                                confSkinID = ReadInt(conf, 0x38); //public int DefWeaponSkinID;
                                confBrocastID = ReadInt(conf, 0x3C); //public int DefaultKillBrocast;         
                                }
                                
                                
                                int mythicArmor = 0, deadReplay = 0, killEffect = 0, extraOrig = 0;  
                                for (auto extra : weaponExtraInstance) {
                                    if (!extra) continue;
                                        weaponextraFields = (WeaponConfExtraFields*) ((uintptr_t) extra + 0x10); //public byte DefaultMythicArmor;
                                        
                                    auto extraID = weaponextraFields->ID;
                                
                                    if (baseID == extraID) {
                                    extraOrig = weaponextraFields->ID;
                                    }
                                    
                                    
                                    
                                    
                                    if (confID == extraID) {                                   
                                  
                                    deadReplay = weaponextraFields->DefaultDeadReplayEffectId;
                                    killEffect = weaponextraFields->DefaultKillEffectId;
                                    
                                    }
                                }
                                
                                
                                for (auto armor : mythicArmorInstance) {
                                    if (!armor) continue;
                                    mythicarmorFields = (MythicArmorFields*) ((uintptr_t) armor + 0x14); // public int AssetID; //0x14             
                                    
                                    auto armorassetID = mythicarmorFields->AssetID;                                 
                                    auto secondTab = mythicarmorFields->SecondTab;
                                    auto thirdTab = mythicarmorFields->ThirdTab;
                                    if (itemIDskin3 == secondTab) {
                                        if (thirdTab == 5) {
                                            if (AName.find("[M]") != std::string::npos) {
                                            mythicArmor = armorassetID;         
                                            }
                                        }
                                    }
                                    
                                }
                                
                                int sightMythic = 0;
                                for (auto sight : mythicSightInstance) {
                                    if (!sight) continue;
                                    mythicsightFields = (MythicSightFields*) ((uintptr_t) sight + 0x10); //public bool ShowInBag;
                                    
                                    auto sightID = *(int *) ((uintptr_t) sight + 0x14); //public int AssetID;
                                                                      
                                    auto equipArray = *(Array<int>**) ((uintptr_t) sight + 0x38); //public int[] WeaponCanEquip;
                                    
                                    if (equipArray && equipArray->getLength() > 0) {
                                    if (itemIDskin3 == equipArray->m_Items[equipArray->getLength()]) {
                                        if (AName.find("[M]") != std::string::npos) {
                                        sightMythic = sightID;
                                        }
                                    }
                                    }                                                                                
                                    
                                 }
                              
                             
                              
                              for (auto wKilling : killEffectItemInstance) {
                                  if (!wKilling) continue;
                                    killeffectFields = (KillEffectItemFields*) ((uintptr_t) wKilling + 0x10); // public int[] RealAssetIDs;
                                  
                                  auto equipArray = *(Array<int> **) ((uintptr_t) wKilling + 0x90); // *killeffectFields->WeaponCanEquip; //public int[] WeaponCanEquip;

                                  int canEquip = 0;
                                  if (equipArray && equipArray->getLength() > 0) {
                                  canEquip = equipArray->m_Items[equipArray->getLength()];
                                  }
                                    
                                  auto realAssetIDs_1 = *(Array<int> **) ((uintptr_t) wKilling + 0x10); //*killeffectFields->RealAssetIDs;  // public int[] RealAssetIDs;                        
                                  int lastAssetID = 0;                                 
                                  if (realAssetIDs_1 && realAssetIDs_1->getLength() > 0) {                                     
                                  lastAssetID = realAssetIDs_1->m_Items[realAssetIDs_1->getLength()];                              
                                  }
                                       
                                  if (itemIDskin3 == canEquip) {
                                      if (AName.find("[M]") != std::string::npos) {
                                          killEffect = lastAssetID;
                                      }
                                  }                                 
                                  
                              }
                              
                              
                              
                    

                              
                              void *spr1 = NULL;
                              void *spr2 = NULL;
                              void *spr3 = NULL;
                              void *spr4 = NULL;
                              int xItem1 = 0, xItem2 = 0, xItem3 = 0;
                              for (auto itemResource : itemResourceConfigInstance) {
                                if (!itemResource) continue;
                              itemFields = (ItemResourceFields*) ((uintptr_t) itemResource + 0x10); //public int FxAssetID;
                    
                              auto checkItem = itemFields->ID;
                              
                              auto sItem1 = itemFields->FxAssetID;
                              auto sItem2 = itemFields->InventoryModelID;
                              auto sItem3 = itemFields->ModelAssetIDRaw;
                              
                              auto sprite1 = itemFields->UISmallSpriteName;
                              auto sprite2 = itemFields->UIMiniSpriteName;
                              auto sprite3 = itemFields->UISpriteName;
                              auto sprite4 = itemFields->UISquareSpriteName;
                              
                              if (checkItem == confID) {
                              xItem1 = sItem1;
                              xItem2 = sItem2;
                              xItem3 = sItem3;
                              
                              spr1 = sprite1;
                              spr2 = sprite2;
                              spr3 = sprite3;
                              spr4 = sprite4;
                              
                              }                      
                              }
                              
                              
                              
                              
                              
                                
                              
                                
                                itemData.push_back({AName, 
                                {itemBase, itemIDskin2, itemIDskin3, itemBaseModified}, //Item2Inventory
                                {confbaseskin, confColorID, confID, confBrocastID},//WeaponConf
                                {extraOrig, mythicArmor, sightMythic, deadReplay, killEffect}, //WeaponExtra
                                {assetIds, fireIds, fireIds2}, //WeaponAsset
                                {xItem1, xItem2, xItem3},
                                {spr1, spr2, spr3, spr4}
                                });            
                            
                                
                                
                            
                            //  }
                        }
                        
                    }
                
                }     
                
            }     
            loadskinhack = true; 
    }
    
    
    
    
   
   if (BRDropPlaneSkinConfigInstance.size() > 0) {
    for (auto planedrop : BRDropPlaneSkinConfigInstance) {
        if (!planedrop) continue;
        
        dropplaneFields = (BRDropPlaneSkinFields*)((uintptr_t)planedrop + 0x10); //public int ColorID
        std::string plane = GetNameString(GetDropPlaneName, planedrop);
        
        if (dropplaneFields->ModelAsset1P != 0 && !getplane[plane]) {
            getplane[plane] = true;

            dropplane.push_back({plane, {
                dropplaneFields->ColorID, 
                dropplaneFields->ModelAsset1P, 
                dropplaneFields->ModelAsset3P, 
                dropplaneFields->ModelAssetCutScene, 
                dropplaneFields->ModelAssetUI, 
                dropplaneFields->Priority        
            }});

            if (getplane.size() == BRDropPlaneSkinConfigInstance.size()) {
                break; 
            }
        }
    }
}

       
        



            
        
        
              
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    }   
    return NULL;
}




struct ConfigData {
    // Tab 1 - ESP Settings
    bool Esp;
    bool PlayerLine;
    bool Box;
    bool Health;
    bool Name;
    bool Distance;
    bool Count;
    bool Alert;
    int CrosshairType;
    float AimSize;
    bool Crosshair;
    bool Aimline;

    // Tab 2 - Memory Settings
    bool AimAssist;
    float AimAssistSize;
    bool Kinetic;
    bool Recoil;
    bool Spread;
    bool Reload;
    bool Scope;
    bool Switch;
    bool Shake;
    bool Flash;
    bool Small;
    bool Cost;
    bool Rpd;
    bool Range;
    bool AutoSnipe;
    bool Hit;
    bool Auto;
    bool Fire;

    // Tab 3 - Ability Settings
    bool SnowB;
    float SnowBsize;
    bool SlideTackle;
    float SlideRange;
    bool SpeedHack;
    float SpeedMultiplier;
    bool JumpAdjustment;
    float JumpMultiplier;
    bool Spectate;
    bool Outline;
    bool WallHack;
    bool Red;
    bool JetJ;
    bool JetS;
    bool Execute;
    bool Radar1;
    bool Parachute;
    bool Diving;

    // Tab 4 - Aimbot Settings
    bool Aimbot360;
    bool AimSilent;
    int Target;
    int Trigger;
    int By;

    // Tab 5 - Skins Settings
    bool Memento;
    bool Morning;
    bool Amoeba;
    bool Aks;
    bool Quan;
    bool Glorious;
    bool Neon;
    bool Celestial;
    bool Bam;
    bool Demon;
    bool Lotus;
    bool Phoenix;
    bool Qq;
    bool Song;
    bool Siege;
    bool Templar;
    bool Spectre;
    bool Darkside;

    // Tab 6 - Events (No settings to save)

// Tab 7 - Performance Settings
bool FpsLevel;                // Toggle for base frame rate unlock
float FpsLevel1;              // Value for base FPS (e.g., 30–180)
bool showFPSLevelSlider;      // Show/hide base FPS slider

bool FpsLevelUltra;           // Toggle for ultra frame rate unlock
float FpsLevelUltra1;         // Value for ultra FPS (e.g., 60–240)
bool showFPSLevelUltraSlider; // Show/hide ultra FPS slider

bool Frame;                   // Possibly another setting (e.g., VSync)
bool Fpss;                    // Possibly for showing FPS counter

};

void saveAllConfig() {
    ConfigData config;
    
    // Tab 1
    config.Esp = Config.ESPMenu.Esp;
    config.PlayerLine = Config.ESPMenu.isPlayerLine;
    config.Box = Config.ESPMenu.Box;
    config.Health = Config.ESPMenu.Health;
    config.Name = Config.ESPMenu.Name;
    config.Distance = Config.ESPMenu.Distance;
    config.Count = Config.ESPMenu.Count;
    config.Alert = Config.ESPMenu.Alert;
    config.CrosshairType = static_cast<int>(Config.ESPMenu.CrosshairType);
    config.AimSize = Config.Aim.size;
    config.Crosshair = Config.ESPMenu.Crosshair;
    config.Aimline = Config.ESPMenu.Aimline;

    // Tab 2
    config.AimAssist = Config.Aim.AimAssist;
    config.AimAssistSize = Config.Aim.AimAssistSize;
    config.Kinetic = Config.ExtraMenu.Kinetic;
    config.Recoil = Config.ExtraMenu.Recoil;
    config.Spread = Config.ExtraMenu.Spread;
    config.Reload = Config.ExtraMenu.Reload;
    config.Scope = Config.ExtraMenu.Scope;
    config.Switch = Config.ExtraMenu.Switch;
    config.Shake = Config.ExtraMenu.Shake;
    config.Flash = Config.ExtraMenu.Flash;
    config.Small = Config.ExtraMenu.Small;
    config.Cost = Config.ExtraMenu.Cost;
    config.Rpd = Config.ExtraMenu.Rpd;
    config.Range = Config.ExtraMenu.Range;
    config.AutoSnipe = Config.ExtraMenu.AutoSnipe;
    config.Hit = Config.ExtraMenu.Hit;
    config.Auto = Config.ExtraMenu.Auto;
    config.Fire = Config.ExtraMenu.Fire;

    // Tab 3
    config.SnowB = SnowB;
    config.SnowBsize = SnowBsize;
    config.SlideTackle = SlideTackle;
    config.SlideRange = SlideRange;
    config.SpeedHack = isSpeedHackEnabled;
    config.SpeedMultiplier = speedHackMultiplier;
    config.JumpAdjustment = isJumpAdjustmentEnabled;
    config.JumpMultiplier = jumpHeightMultiplier;
    config.Spectate = Config.ExtraMenu.Spectate;
    config.Outline = Config.ExtraMenu.Outline;
    config.WallHack = Config.ExtraMenu.WallHack;
    config.Red = Config.ExtraMenu.Red;
    config.JetJ = Config.ExtraMenu.JetJ;
    config.JetS = Config.ExtraMenu.JetS;
    config.Execute = isExecute;
    config.Radar1 = Config.ExtraMenu.Radar1;
    config.Parachute = Config.ExtraMenu.Parachute;
    config.Diving = Config.ExtraMenu.Diving;

    // Tab 4
    config.Aimbot360 = Config.Aim.Aimbot360;
    config.AimSilent = Config.Aim.AimSilent;
    config.Target = static_cast<int>(Config.Aim.Target);
    config.Trigger = static_cast<int>(Config.Aim.Trigger);
    config.By = static_cast<int>(Config.Aim.By);

    // Tab 5
    config.Memento = Config.ExtraMenu.Memento;
    config.Morning = Config.ExtraMenu.Morning;
    config.Amoeba = Config.ExtraMenu.Amoeba;
    config.Aks = Config.ExtraMenu.Aks;
    config.Quan = Config.ExtraMenu.Quan;
    config.Glorious = Config.ExtraMenu.Glorious;
    config.Neon = Config.ExtraMenu.Neon;
    config.Celestial = Config.ExtraMenu.Celestial;
    config.Bam = Config.ExtraMenu.Bam;
    config.Demon = Config.ExtraMenu.Demon;
    config.Lotus = Config.ExtraMenu.Lotus;
    config.Phoenix = Config.ExtraMenu.Phoenix;
    config.Qq = Config.ExtraMenu.Qq;
    config.Song = Config.ExtraMenu.Song;
    config.Siege = Config.ExtraMenu.Siege;
    config.Templar = Config.ExtraMenu.Templar;
    config.Spectre = Config.ExtraMenu.Spectre;
    config.Darkside = Config.ExtraMenu.Darkside;

    // Tab 7
    config.FpsLevel = Config.Aim.FpsLevel;
    config.FpsLevel1 = Config.Aim.FpsLevel1;
    config.Frame = Config.ExtraMenu.Frame;
    config.Fpss = Config.ExtraMenu.Fpss;

    std::ofstream file("/storage/emulated/0/Download/ys_config.dat", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&config), sizeof(ConfigData));
        file.close();
        ImGui::OpenPopup("Config Saved");
    } else {
        ImGui::OpenPopup("Config Save Error");
    }
}

bool loadAllConfig() {
    ConfigData config;
    std::ifstream file("/storage/emulated/0/Download/ys_config.dat", std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&config), sizeof(ConfigData));
        file.close();

        // Tab 1
        Config.ESPMenu.Esp = config.Esp;
        Config.ESPMenu.isPlayerLine = config.PlayerLine;
        Config.ESPMenu.Box = config.Box;
        Config.ESPMenu.Health = config.Health;
        Config.ESPMenu.Name = config.Name;
        Config.ESPMenu.Distance = config.Distance;
        Config.ESPMenu.Count = config.Count;
        Config.ESPMenu.Alert = config.Alert;
        Config.ESPMenu.CrosshairType = static_cast<decltype(Config.ESPMenu.CrosshairType)>(config.CrosshairType);
        Config.Aim.size = config.AimSize;
        Config.ESPMenu.Crosshair = config.Crosshair;
        Config.ESPMenu.Aimline = config.Aimline;

        // Tab 2
        Config.Aim.AimAssist = config.AimAssist;
        Config.Aim.AimAssistSize = config.AimAssistSize;
        Config.Aim.showAimAssistSlider = config.AimAssist;
        Config.ExtraMenu.Kinetic = config.Kinetic;
        Config.ExtraMenu.Recoil = config.Recoil;
        Config.ExtraMenu.Spread = config.Spread;
        Config.ExtraMenu.Reload = config.Reload;
        Config.ExtraMenu.Scope = config.Scope;
        Config.ExtraMenu.Switch = config.Switch;
        Config.ExtraMenu.Shake = config.Shake;
        Config.ExtraMenu.Flash = config.Flash;
        Config.ExtraMenu.Small = config.Small;
        Config.ExtraMenu.Cost = config.Cost;
        Config.ExtraMenu.Rpd = config.Rpd;
        Config.ExtraMenu.Range = config.Range;
        Config.ExtraMenu.AutoSnipe = config.AutoSnipe;
        Config.ExtraMenu.Hit = config.Hit;
        Config.ExtraMenu.Auto = config.Auto;
        Config.ExtraMenu.Fire = config.Fire;

        // Tab 3
        SnowB = config.SnowB;
        SnowBshow = config.SnowB;
        SnowBsize = config.SnowBsize;
        SlideTackle = config.SlideTackle;
        SlideShow = config.SlideTackle;
        SlideRange = config.SlideRange;
        isSpeedHackEnabled = config.SpeedHack;
        showSpeedHackSlider = config.SpeedHack;
        speedHackMultiplier = config.SpeedMultiplier;
        isJumpAdjustmentEnabled = config.JumpAdjustment;
        jumpHeightMultiplier = config.JumpMultiplier;
        Config.ExtraMenu.Spectate = config.Spectate;
        Config.ExtraMenu.Outline = config.Outline;
        Config.ExtraMenu.WallHack = config.WallHack;
        Config.ExtraMenu.Red = config.Red;
        Config.ExtraMenu.JetJ = config.JetJ;
        Config.ExtraMenu.JetS = config.JetS;
        isExecute = config.Execute;
        Config.ExtraMenu.Radar1 = config.Radar1;
        Config.ExtraMenu.Parachute = config.Parachute;
        Config.ExtraMenu.Diving = config.Diving;

        // Tab 4
        Config.Aim.Aimbot360 = config.Aimbot360;
        Config.Aim.AimSilent = config.AimSilent;
        Config.Aim.Target = static_cast<decltype(Config.Aim.Target)>(config.Target);
        Config.Aim.Trigger = static_cast<decltype(Config.Aim.Trigger)>(config.Trigger);
        Config.Aim.By = static_cast<decltype(Config.Aim.By)>(config.By);

        // Tab 5
        Config.ExtraMenu.Memento = config.Memento;
        Config.ExtraMenu.Morning = config.Morning;
        Config.ExtraMenu.Amoeba = config.Amoeba;
        Config.ExtraMenu.Aks = config.Aks;
        Config.ExtraMenu.Quan = config.Quan;
        Config.ExtraMenu.Glorious = config.Glorious;
        Config.ExtraMenu.Neon = config.Neon;
        Config.ExtraMenu.Celestial = config.Celestial;
        Config.ExtraMenu.Bam = config.Bam;
        Config.ExtraMenu.Demon = config.Demon;
        Config.ExtraMenu.Lotus = config.Lotus;
        Config.ExtraMenu.Phoenix = config.Phoenix;
        Config.ExtraMenu.Qq = config.Qq;
        Config.ExtraMenu.Song = config.Song;
        Config.ExtraMenu.Siege = config.Siege;
        Config.ExtraMenu.Templar = config.Templar;
        Config.ExtraMenu.Spectre = config.Spectre;
        Config.ExtraMenu.Darkside = config.Darkside;

        // Tab 7
        Config.Aim.FpsLevel = config.FpsLevel;
        Config.Aim.showFPSLevelSlider = config.FpsLevel;
        Config.Aim.FpsLevel1 = config.FpsLevel1;
        Config.ExtraMenu.Frame = config.Frame;
        Config.ExtraMenu.Fpss = config.Fpss;

        return true;
    }
    return false;
}




#define _BYTE uint8_t
#define _WORD  uint8_t
#define _DWORD uint64_t
#define _QWORD uint64_t
#define _BOOL4 uint8_t

#define SPAMSINT1() int __xbypass_any = __LINE__; int any_xbypass = __LINE__; if (!strstr(oxorany("stoler"), oxorany("relots"))) { __xbypass_any--; any_xbypass++; if (__xbypass_any != any_xbypass && strstr(oxorany("better"), oxorany("better"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("learn"), oxorany("nrael"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("first"), oxorany("tsrif"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("you"), oxorany("you"))) { __xbypass_any++; any_xbypass--; if (!strstr(oxorany("are"), oxorany("era"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("DUMB"), oxorany("DUMB"))) { __xbypass_any = __xbypass_any - any_xbypass; any_xbypass = any_xbypass + __xbypass_any; } else { __xbypass_any = __xbypass_any + any_xbypass; any_xbypass = any_xbypass - __xbypass_any; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; }  } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } __xbypass_any++; return 1
#define SPAMS() int __xbypass_any = __LINE__; int any_xbypass = __LINE__; if (!strstr(oxorany("stoler"), oxorany("relots"))) { __xbypass_any--; any_xbypass++; if (__xbypass_any != any_xbypass && strstr(oxorany("better"), oxorany("better"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("learn"), oxorany("nrael"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("first"), oxorany("tsrif"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("you"), oxorany("you"))) { __xbypass_any++; any_xbypass--; if (!strstr(oxorany("are"), oxorany("era"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("DUMB"), oxorany("DUMB"))) { __xbypass_any = __xbypass_any - any_xbypass; any_xbypass = any_xbypass + __xbypass_any; } else { __xbypass_any = __xbypass_any + any_xbypass; any_xbypass = any_xbypass - __xbypass_any; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; }  } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } any_xbypass = any_xbypass + __xbypass_any
#define SPAMSINT0() int __xbypass_any = __LINE__; int any_xbypass = __LINE__; if (!strstr(oxorany("stoler"), oxorany("relots"))) { __xbypass_any--; any_xbypass++; if (__xbypass_any != any_xbypass && strstr(oxorany("better"), oxorany("better"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("learn"), oxorany("nrael"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("first"), oxorany("tsrif"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("you"), oxorany("you"))) { __xbypass_any++; any_xbypass--; if (!strstr(oxorany("are"), oxorany("era"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("DUMB"), oxorany("DUMB"))) { __xbypass_any = __xbypass_any - any_xbypass; any_xbypass = any_xbypass + __xbypass_any; } else { __xbypass_any = __xbypass_any + any_xbypass; any_xbypass = any_xbypass - __xbypass_any; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; }  } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } return (__xbypass_any & any_xbypass)
#define SPAMSINTTYPE(typex, any) int __xbypass_any = __LINE__; int any_xbypass = __LINE__; if (!strstr(oxorany("stoler"), oxorany("relots"))) { __xbypass_any--; any_xbypass++; if (__xbypass_any != any_xbypass && strstr(oxorany("better"), oxorany("better"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("learn"), oxorany("nrael"))) { __xbypass_any--; any_xbypass++; if (!strstr(oxorany("first"), oxorany("tsrif"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("you"), oxorany("you"))) { __xbypass_any++; any_xbypass--; if (!strstr(oxorany("are"), oxorany("era"))) { __xbypass_any++; any_xbypass--; if (strstr(oxorany("DUMB"), oxorany("DUMB"))) { __xbypass_any = __xbypass_any - any_xbypass; any_xbypass = any_xbypass + __xbypass_any; } else { __xbypass_any = __xbypass_any + any_xbypass; any_xbypass = any_xbypass - __xbypass_any; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; } } else { __xbypass_any--; any_xbypass++; }  } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } } else { __xbypass_any++; any_xbypass--; } return (typex)(__xbypass_any & any_xbypass)

#include <fstream>
using namespace std;

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

ImFont* medium;
ImFont* bold;
ImFont* tab_icons;
ImFont* logo;
ImFont* tab_title;
ImFont* tab_title_icon;
ImFont* subtab_title;
ImFont* combo_arrow;


struct My_Patches
{
    MemoryPatch
    A1, rpd, grap, smk, Waller, chute, wing, tutorial, cost, red, jets, jetj, fire, fpss, frame, revive, outline, range, kinetic, autosnipe;

} Patches;



float AVIWA = 119.167f;

bool wallh, smoke;
bool Memento, Glorious, Neon, Radiance, Celestial, Bam,
     Demon, Lotus, Qq, Phoenix, Song, Siege,
     Templar, Spectre, Darkside, Amoeba, Morning, Aks, Quan = false;

struct sRegion
{
    uintptr_t start, end;

};

std::chrono::steady_clock::time_point appStartTime = std::chrono::steady_clock::now();

std::string FormatTimeDuration(std::chrono::steady_clock::duration duration) {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration % std::chrono::hours(1)).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration % std::chrono::minutes(1)).count();

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    return std::string(buffer);
}

static std::string EXP ="";
static std::string storedKey ="";
static char s[256];

void RenderFloatingInfo(ImDrawList* draw, float screenWidth, float screenHeight) {
    if (Config.ExtraMenu.ClearDisplay) return;

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);
    
    char dateStr[64], timeStr[64];
    strftime(dateStr, sizeof(dateStr), "%B %d, %Y", localTime);
    strftime(timeStr, sizeof(timeStr), "%I:%M %p", localTime);
    
    for (char* p = dateStr; *p; p++) *p = toupper(*p);
    for (char* p = timeStr; *p; p++) *p = toupper(*p);

    auto currentDuration = std::chrono::steady_clock::now() - appStartTime;
    std::string playTime = FormatTimeDuration(currentDuration);
    for (char& c : playTime) c = toupper(c);

    float padding = 30.0f;
    float lineSpacing = 28.0f;
    float startY = screenHeight - padding - (lineSpacing * 5);

    auto drawTextWithSpacing = [&](const char* text, ImVec2 pos, ImColor color) {
        float xOffset = 0.0f;
        float charSpacing = 1.5f;
        for (int i = 0; text[i] != '\0'; i++) {
            char c[2] = { text[i], '\0' };
            draw->AddText(ImVec2(pos.x + xOffset, pos.y), color, c);
            xOffset += ImGui::CalcTextSize(c).x + charSpacing;
        }
    };

    char line2[128], line3[64], line4[64];
    snprintf(line2, sizeof(line2), "Date: %s", dateStr);
    snprintf(line3, sizeof(line3), "Time: %s", timeStr);
    snprintf(line4, sizeof(line4), "PLAYTIME : %s", playTime.c_str());

    ImColor gold(255, 215, 0, 255);
    ImColor white(255, 255, 255, 255);

    drawTextWithSpacing("CALL OF DUTY: MOBILE GR | 1.6.54", ImVec2(padding, startY), gold);
    drawTextWithSpacing(line2, ImVec2(padding, startY + lineSpacing), white);
    drawTextWithSpacing(line3, ImVec2(padding, startY + lineSpacing * 2), white);
    drawTextWithSpacing(line4, ImVec2(padding, startY + lineSpacing * 3), white);
    drawTextWithSpacing("DEVELOPER : @aylnhax", ImVec2(padding, startY + lineSpacing * 4), white);
}



void saveKeyToFile(const std::string& key) {
    std::ofstream outFile("/storage/emulated/0/Download/yoshi.dat");
    if (outFile) {
        outFile << key;
        outFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "ImGui", "Error: Could not save key to file.");
    }
}

std::string loadKeyFromFile() {
    std::ifstream inFile("/storage/emulated/0/Download/yoshi.dat");
    std::string key;
    if (inFile) {
        std::getline(inFile, key);
        inFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "ImGui", "Error: Could not load key from file.");
    }
    return key;
}



std::vector<sRegion> trapRegions;

uintptr_t address = 0;

JavaVM *VM;
std::string md5(std::string s);
using json = nlohmann::json;

uintptr_t g_il2cpp;
std::string g_Token, g_Auth;
bool bValid = false;

std::string getClipboard() {


    std::string result;
    JNIEnv *env;

    VM->AttachCurrentThread(&env, NULL);

    auto looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
    auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"),
                         OBFUSCATE("()V"));
    env->CallStaticVoidMethod(looperClass, prepareMethod);

    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE(
            "sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass,
                                     sCurrentActivityThreadField);

    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass,
                                        OBFUSCATE("mInitialApplication"),
                                        OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread,
                                  mInitialApplicationField);

    auto contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    auto getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"),
                                  OBFUSCATE(
                                      "(Ljava/lang/String;)Ljava/lang/Object;"));

    auto str = env->NewStringUTF(OBFUSCATE("clipboard"));
    auto clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
    env->DeleteLocalRef(str);

    jclass ClipboardManagerClass = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    auto getText = env->GetMethodID(ClipboardManagerClass, OBFUSCATE("getText"),
                                    OBFUSCATE("()Ljava/lang/CharSequence;"));

    jclass CharSequenceClass = env->FindClass(OBFUSCATE("java/lang/CharSequence"));
    auto toStringMethod = env->GetMethodID(CharSequenceClass, OBFUSCATE("toString"),
                                           OBFUSCATE("()Ljava/lang/String;"));

    auto text = env->CallObjectMethod(clipboardManager, getText);
    if (text) {
        str = (jstring) env->CallObjectMethod(text, toStringMethod);
        result = env->GetStringUTFChars(str, 0);
        env->DeleteLocalRef(str);
        env->DeleteLocalRef(text);
    }

    env->DeleteLocalRef(CharSequenceClass);
    env->DeleteLocalRef(ClipboardManagerClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    VM->DetachCurrentThread();
    return result.c_str();
}




struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    auto *mem = (struct MemoryStruct *) userp;

    mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == nullptr) {
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

const char *GetAndroidID(JNIEnv *env, jobject context) {
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID getContentResolverMethod = env->GetMethodID(contextClass,
                                         OBFUSCATE("getContentResolver"),
                                         OBFUSCATE(
                                                 "()Landroid/content/ContentResolver;"));
    jclass settingSecureClass = env->FindClass(OBFUSCATE("android/provider/Settings$Secure"));
    jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass, OBFUSCATE("getString"),
                                OBFUSCATE(
                                    "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;"));
    auto obj = env->CallObjectMethod(context, getContentResolverMethod);
    auto str = (jstring) env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj,
               env->NewStringUTF(OBFUSCATE("android_id")));
    return env->GetStringUTFChars(str, nullptr);
}

const char *GetDeviceModel(JNIEnv *env) {
    jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("MODEL"),
                       OBFUSCATE("Ljava/lang/String;"));
    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, nullptr);
}

const char *GetDeviceBrand(JNIEnv *env) {
    jclass buildClass = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID modelId = env->GetStaticFieldID(buildClass, OBFUSCATE("BRAND"),
                       OBFUSCATE("Ljava/lang/String;"));
    auto str = (jstring) env->GetStaticObjectField(buildClass, modelId);
    return env->GetStringUTFChars(str, nullptr);
}

const char *GetPackageName(JNIEnv *env, jobject context) {
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID getPackageNameId = env->GetMethodID(contextClass, OBFUSCATE("getPackageName"),
                                 OBFUSCATE("()Ljava/lang/String;"));
    auto str = (jstring) env->CallObjectMethod(context, getPackageNameId);
    return env->GetStringUTFChars(str, nullptr);
}

const char *GetDeviceUniqueIdentifier(JNIEnv *env, const char *uuid) {
    jclass uuidClass = env->FindClass(OBFUSCATE("java/util/UUID"));
    auto len = strlen(uuid);
    jbyteArray myJByteArray = env->NewByteArray(len);
    env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *) uuid);
    jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass,
                                        OBFUSCATE("nameUUIDFromBytes"),
                                        OBFUSCATE("([B)Ljava/util/UUID;"));
    jmethodID toStringMethod = env->GetMethodID(uuidClass, OBFUSCATE("toString"),
                               OBFUSCATE("()Ljava/lang/String;"));
    auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
    auto str = (jstring) env->CallObjectMethod(obj, toStringMethod);
    return env->GetStringUTFChars(str, nullptr);
}



//========================= MAIN LOGIN =======================\\

std::string Login(const char *user_key) {

    JNIEnv *env;
    VM->AttachCurrentThread(&env, nullptr);
    std::string hwid = user_key;
    jobject context = getGlobalContext(env);
    hwid += GetAndroidID(env, context);
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);
    std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());
    VM->DetachCurrentThread();
    std::string errMsg;

    struct MemoryStruct chunk {};
    chunk.memory = (char *) malloc(1);
    chunk.size = 0;

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();


    if (curl) {
      
      
        std::string api_key = oxorany("https://panel.x10.mx/connect");  
     curl_easy_setopt(curl, CURLOPT_URL, (api_key.c_str()));  
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");  
          
        struct curl_slist *headers = NULL;  
        headers = curl_slist_append(headers, oxorany("Content-Type: application/x-www-form-urlencoded"));  
          
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);  
        char data[4096];  
          
        sprintf(data, oxorany("game=CODMGR&user_key=%s&serial=%s"), user_key, UUID.c_str());  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);  
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);  
          
        res = curl_easy_perform(curl);  
        if (res == CURLE_OK) {  
            try {  
                json result = json::parse(chunk.memory);  
                if (result[std::string(OBFUSCATE("status"))] == true) {  
                    std::string token = result[std::string(OBFUSCATE("data"))][std::string(OBFUSCATE("token"))].get<std::string>();  
                    time_t rng = result[std::string(OBFUSCATE("data"))][std::string(OBFUSCATE("rng"))].get<time_t>();  
                      
                    EXP = result["data"]["EXP"].get<std::string>();  
                               
                    
                    
                    if (rng + 30 > time(0)) {  
                        std::string auth = "CODMGR";  
                        auth += "-";  
                        auth += user_key;  
                        auth += "-";  
                        auth += hwid;  
                        auth += "-";          
                        auth += "Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E";  
                                                                        
                        bValid = g_Token == g_Auth;  
                                             
                        if (g_Token == g_Auth) {  

                        }
                    }
                } else {
                    auto REASON = std::string{"reason"};
                    errMsg = result[REASON].get<std::string>();
                }
            } catch (nlohmann::json::exception &e) {
                errMsg = "{";
                errMsg += e.what();
                errMsg += "}\n{";
                errMsg += chunk.memory;
                errMsg += "}";
            }
        } else {
            errMsg = curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    //VM->DetachCurrentThread();

    return bValid ? OBFUSCATE("OK") : errMsg;
}


static bool isMenuVisible = true;

// Floating logo variables
static bool collapsedPosInitialized = false;
static ImVec2 collapsedLogoPos;
static ImVec2 logoWindowSize = ImVec2(110.0f, 110.0f);
static bool wasDragging = false;
static float logoScale = 1.0f; // For scale animation
static bool logoAnimating = false;

// Menu animation state
static float menuFade = 1.0f;
static float dockSlide = 0.0f;
static float tabIndicatorPosY = 0.0f;
static float tabIconFade = 1.0f;
static int lastTab = 1;

static bool CustomCheckbox(const char* label, bool* v)
{
    ImGuiStyle& style = ImGui::GetStyle();
    const float square_sz = ImGui::GetFrameHeight();
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImVec2 total_size = ImVec2(square_sz + style.ItemInnerSpacing.x + label_size.x, square_sz);

    ImGui::InvisibleButton(label, total_size);
    bool hovered = ImGui::IsItemHovered();
    bool pressed = ImGui::IsItemClicked();
    if (pressed)
        *v = !*v;

    ImGuiID id = ImGui::GetID(label);
    ImGuiStorage* storage = ImGui::GetStateStorage();
    float t = storage->GetFloat(id, *v ? 1.0f : 0.0f);
    const float target = *v ? 1.0f : 0.0f;
    const float speed = 18.0f;
    const float dt = ImGui::GetIO().DeltaTime;
    t += (target - t) * dt * speed;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    storage->SetFloat(id, t);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 item_min = ImGui::GetItemRectMin();
    ImVec2 item_max = ImGui::GetItemRectMax();
    const ImVec2 box_min = item_min;
    const ImVec2 box_max = ImVec2(box_min.x + square_sz, box_min.y + square_sz);

    const ImU32 bg_color = ImGui::GetColorU32(hovered ? ImVec4(0.50f, 0.20f, 0.80f, 0.95f) : ImVec4(0.18f, 0.05f, 0.30f, 0.90f));
    const ImU32 border_color = ImGui::GetColorU32(ImGuiCol_Border);
    draw_list->AddRectFilled(box_min, box_max, bg_color, square_sz * 0.25f);
    draw_list->AddRect(box_min, box_max, border_color, square_sz * 0.25f, 0, 1.5f);

    if (t > 0.0f) {
        const float inner = square_sz * 0.58f;
        const ImVec2 check_min = ImVec2(box_min.x + (square_sz - inner) * 0.5f, box_min.y + (square_sz - inner) * 0.5f);
        const ImVec2 check_max = ImVec2(check_min.x + inner, check_min.y + inner);
        ImU32 fill_color = ImGui::GetColorU32(ImVec4(0.84f, 0.44f, 1.00f, t));
        draw_list->AddRectFilled(check_min, check_max, fill_color, square_sz * 0.18f);

        const float mark_alpha = t;
        const ImU32 mark_line = ImColor(1.0f, 1.0f, 1.0f, mark_alpha);
        const float line_width = 2.5f;
        draw_list->AddLine(ImVec2(check_min.x + inner * 0.18f, check_min.y + inner * 0.52f), ImVec2(check_min.x + inner * 0.42f, check_min.y + inner * 0.75f), mark_line, line_width);
        draw_list->AddLine(ImVec2(check_min.x + inner * 0.42f, check_min.y + inner * 0.75f), ImVec2(check_min.x + inner * 0.82f, check_min.y + inner * 0.24f), mark_line, line_width);
    }

    ImVec2 text_pos = ImVec2(box_max.x + style.ItemInnerSpacing.x, box_min.y + (square_sz - label_size.y) * 0.5f);
    draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
    return pressed;
}

static bool CustomSliderFloat(const char* label, float* value, float min, float max, const char* format = "%.2f", float width = -1.0f)
{
    ImGuiStyle& style = ImGui::GetStyle();
    const float height = ImGui::GetFrameHeight();
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 item_size;
    if (width > 0.0f)
        item_size = ImVec2(width, height);
    else
        item_size = ImVec2(ImGui::GetContentRegionAvail().x, height);

    ImGui::InvisibleButton(label, item_size);
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive();
    bool pressed = ImGui::IsItemClicked();

    ImVec2 item_min = ImGui::GetItemRectMin();
    ImVec2 item_max = ImGui::GetItemRectMax();
    const float slider_width = item_size.x - label_size.x - style.ItemInnerSpacing.x;
    const float slider_height = height * 0.32f;
    const ImVec2 slider_min = ImVec2(item_min.x + label_size.x + style.ItemInnerSpacing.x, item_min.y + (item_size.y - slider_height) * 0.5f);
    const ImVec2 slider_max = ImVec2(slider_min.x + slider_width, slider_min.y + slider_height);
    const float value_ratio = (max > min) ? ((*value - min) / (max - min)) : 0.0f;
    const float value_clamped = value_ratio < 0.0f ? 0.0f : (value_ratio > 1.0f ? 1.0f : value_ratio);

    if ((hovered && pressed) || active) {
        ImGuiIO& io = ImGui::GetIO();
        float mouse_x = io.MousePos.x;
        float t = (mouse_x - slider_min.x) / (slider_width);
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        *value = min + (max - min) * t;
        if (*value < min) *value = min;
        if (*value > max) *value = max;
    }

    ImGuiID id = ImGui::GetID(label);
    ImGuiStorage* storage = ImGui::GetStateStorage();
    float anim = storage->GetFloat(id, value_clamped);
    const float target = value_clamped;
    const float speed = 18.0f;
    const float dt = ImGui::GetIO().DeltaTime;
    anim += (target - anim) * dt * speed;
    if (anim < 0.0f) anim = 0.0f;
    if (anim > 1.0f) anim = 1.0f;
    storage->SetFloat(id, anim);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImU32 track_bg = ImGui::GetColorU32(ImVec4(0.20f, 0.10f, 0.30f, hovered ? 0.95f : 0.80f));
    const ImU32 fill_col = ImGui::GetColorU32(ImVec4(0.75f, 0.35f, 1.00f, 1.0f));
    const ImU32 border_col = ImGui::GetColorU32(ImGuiCol_Border);
    const ImU32 knob_col = ImGui::GetColorU32(ImVec4(0.88f, 0.55f, 1.0f, 1.0f));

    const float knob_radius = height * 0.35f;
    const float knob_x = slider_min.x + anim * slider_width;
    const ImVec2 knob_center = ImVec2(knob_x, slider_min.y + slider_height * 0.5f);
    const float rounded = slider_height * 0.5f;

    draw_list->AddRectFilled(slider_min, slider_max, track_bg, rounded);
    draw_list->AddRectFilled(slider_min, ImVec2(slider_min.x + anim * slider_width, slider_max.y), fill_col, rounded);
    draw_list->AddRect(slider_min, slider_max, border_col, rounded, 0, 1.5f);
    draw_list->AddCircleFilled(knob_center, knob_radius, knob_col);
    draw_list->AddCircle(knob_center, knob_radius, border_col, 16, 1.5f);

    ImVec2 text_pos = ImVec2(item_min.x, item_min.y + (item_size.y - label_size.y) * 0.5f);
    draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

    char value_buf[64];
    snprintf(value_buf, sizeof(value_buf), format, *value);
    ImVec2 value_size = ImGui::CalcTextSize(value_buf);
    ImVec2 value_pos = ImVec2(item_max.x - value_size.x, item_min.y + (item_size.y - value_size.y) * 0.5f);
    draw_list->AddText(value_pos, ImGui::GetColorU32(ImGuiCol_Text), value_buf);

    return *value != min + (max - min) * value_ratio;
}

#define CREATE_COLOR(r, g, b, a) new float[4]{(float)r, (float)g, (float)b, (float)a};



EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{



    if (!Config.ImGuiMenu.g_Initialized)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle *style = &ImGui::GetStyle();
        style->WindowPadding = ImVec2(15, 15);
        style->FrameBorderSize = 2.0f;
        style->WindowBorderSize = 2.0f;

        style->FramePadding = ImVec2(8, 6);
        style->ItemSpacing = ImVec2(8, 6);
        style->IndentSpacing = 16;
        style->ScrollbarSize = 12.0f;

        style->WindowRounding = 12.0f;
        style->FrameRounding = 8.0f;
        style->ScrollbarRounding = 6.0f;
        style->WindowBorderSize = 1.0f;
        style->FrameBorderSize = 1.0f;

        style->WindowTitleAlign = ImVec2(0.5, 0.5);
        style->ButtonTextAlign = ImVec2(0.5, 0.5);
        style->WindowPadding = ImVec2(12, 10);
        style->FramePadding = ImVec2(8, 6);

        style->Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.08f, 0.08f, 0.85f); 
        style->Colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.08f, 0.08f, 0.85f);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.08f, 0.08f, 0.95f);



style->Colors[ImGuiCol_Text]                 = ImVec4(0.95f, 0.9f, 0.9f, 1.0f);
style->Colors[ImGuiCol_TextDisabled]         = ImVec4(0.5f, 0.4f, 0.4f, 0.7f); 
style->Colors[ImGuiCol_WindowBg]             = ImVec4(0.1f, 0.08f, 0.08f, 0.95f);
style->Colors[ImGuiCol_TitleBg]              = ImVec4(0.15f, 0.1f, 0.1f, 1.0f);
style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.2f, 0.12f, 0.12f, 1.0f); 
style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.1f, 0.08f, 0.08f, 1.0f);
style->Colors[ImGuiCol_Border]               = ImVec4(0.6f, 0.2f, 0.2f, 0.5f);
style->Colors[ImGuiCol_FrameBg]              = ImVec4(0.12f, 0.08f, 0.08f, 1.0f); 
style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.4f, 0.15f, 0.15f, 1.0f);
style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.6f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.12f, 0.08f, 0.08f, 1.0f);
style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.6f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.8f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
style->Colors[ImGuiCol_Separator]            = ImVec4(0.6f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.8f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_SeparatorActive]      = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);

style->Colors[ImGuiCol_Button]               = ImVec4(0.2f, 0.1f, 0.1f, 1.0f);
style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_ButtonActive]         = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_Tab]                  = ImVec4(0.15f, 0.1f, 0.1f, 1.0f); 
style->Colors[ImGuiCol_TabHovered]           = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_TabActive]            = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_Header]               = ImVec4(0.2f, 0.1f, 0.1f, 1.0f);
style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_HeaderActive]         = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.6f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.8f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
style->Colors[ImGuiCol_CheckMark]            = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
style->Colors[ImGuiCol_PopupBg]              = ImVec4(0.1f, 0.08f, 0.08f, 0.95f);
style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.12f, 0.08f, 0.08f, 1.0f);


        style->ScrollbarSize *= 1.0f;

        style->GrabRounding = 8.0f;





        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");

        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;
        static const ImWchar icons_ranges[] = {
            0x0020,
            0x00FF,
            0x3000,
            0x30FF,
            0x31F0,
            0x31FF,
            0xFF00,
            0xFFEF,
            0x4e00,
            0x9FAF,
            0xf000,
            0xf3ff,
            0xe000,
            0xf8ff,
            0
        };

        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = true;
        ImFontConfig title_font_config;
        title_font_config.FontDataOwnedByAtlas = true;
        ImFontConfig icons_config;
        icons_config.FontDataOwnedByAtlas = false;

        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;

        io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 29.f, &CustomFont);
        io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data1, font_awesome_size1, 28.0f, &icons_config, icons_ranges);
        TitleFont = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Title), sizeof(Title), 32.0f, &title_font_config);


        io.Fonts->AddFontFromMemoryCompressedTTF(Custom_data, Custom_size, 26.0f, &CustomFont,
                io.Fonts->GetGlyphRangesJapanese());

        ImGui::GetStyle().ScaleAllSizes(1.5f);
        memset(&Config, 0, sizeof(sConfig));

        Config.sColorsESPPLAYER.LinePLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.BoxPLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.NamePLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.DistancePLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.HealthPLAYER = CREATE_COLOR(255, 0, 0, 255);

        Config.sColorsESPBOT.LineBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.BoxBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.NameBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.HealthBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.DistanceBOT = CREATE_COLOR(0, 255, 0, 180);

        Config.sColorsESPOTHERS.PovOTHERS = CREATE_COLOR(225, 0, 0, 180);

        Config.ImGuiMenu.g_Initialized = true;
    }

    ImGuiIO *io = &ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(get_width(), get_height());
    ImGui::NewFrame();





    static bool ic;
    static bool show;
    static bool HideWindow = true;

    DrawESP(ImGui::GetBackgroundDrawList(), get_width(), get_height(), get_density());
    AimBot_Func();

    // Render floating logo when menu is collapsed
    if (!isMenuVisible) {
        // Check if logo should be hidden
        if (!Config.ExtraMenu.HideLogo) {
            // Initialize collapsed position
            if (!collapsedPosInitialized) {
                ImVec2 vp = ImGui::GetMainViewport()->Pos;
                collapsedLogoPos = ImVec2(vp.x + Config.ExtraMenu.LogoPosX, vp.y + Config.ExtraMenu.LogoPosY);
                collapsedPosInitialized = true;
                logoScale = 0.0f; // Start from 0 for scale-in animation
                logoAnimating = true;
            }

            if (logoAnimating) {
                float deltaTime = ImGui::GetIO().DeltaTime;
                logoScale += deltaTime * 4.0f;
                if (logoScale >= 1.0f) {
                    logoScale = 1.0f;
                    logoAnimating = false;
                }
            }

            ImVec2 scaledSize = ImVec2(logoWindowSize.x * logoScale, logoWindowSize.y * logoScale);

            ImGui::SetNextWindowPos(ImVec2(collapsedLogoPos.x - (scaledSize.x - logoWindowSize.x) / 2.0f, collapsedLogoPos.y - (scaledSize.y - logoWindowSize.y) / 2.0f), ImGuiCond_Always);
            ImGui::SetNextWindowSize(scaledSize, ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);

            bool restoreMenu = false;

            if (ImGui::Begin("MENU_COLLAPSED", nullptr,
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoBringToFrontOnFocus |
                             ImGuiWindowFlags_NoSavedSettings))
            {
                if (!astralLoaded)
                {
                    astralTextureID = LoadAstralTexture(astral_data, sizeof(astral_data));
                    astralLoaded = true;
                }

                ImVec2 winSize = ImGui::GetWindowSize();
                ImGui::SetCursorPos(ImVec2(0, 0));
                ImGui::InvisibleButton("##DragClickArea", winSize, ImGuiButtonFlags_MouseButtonLeft);

                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
                {
                    ImVec2 delta = ImGui::GetIO().MouseDelta;
                    collapsedLogoPos.x += delta.x;
                    collapsedLogoPos.y += delta.y;
                    wasDragging = true;
                }

                if (ImGui::IsItemDeactivated())
                {
                    if (!wasDragging)
                    {
                        restoreMenu = true;
                    } else {
                        // Save the dragged position
                        ImVec2 vp = ImGui::GetMainViewport()->Pos;
                        Config.ExtraMenu.LogoPosX = collapsedLogoPos.x - vp.x;
                        Config.ExtraMenu.LogoPosY = collapsedLogoPos.y - vp.y;
                    }
                    wasDragging = false;
                }

                if (astralLoaded && astralTextureID)
                {
                    float logoOpacity = Config.ExtraMenu.LogoOpacity;
                    float logoSizeMultiplier = Config.ExtraMenu.LogoSize;
                    float baseSize = 90.0f;
                    float size = baseSize * logoSizeMultiplier;

                    float logoX = (logoWindowSize.x - size) / 2.0f;
                    float logoY = (logoWindowSize.y - size) / 2.0f;

                    ImVec2 windowPos = ImGui::GetWindowPos();
                    ImVec2 clipMin = windowPos;
                    ImVec2 clipMax = ImVec2(windowPos.x + logoWindowSize.x, windowPos.y + logoWindowSize.y);

                    ImGui::PushClipRect(clipMin, clipMax, true);
                    ImGui::SetCursorPos(ImVec2(logoX, logoY));
                    ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, logoOpacity);
                    ImGui::Image((void *)(intptr_t)astralTextureID, ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), tintColor);
                    ImGui::PopClipRect();
                    
                    if (Config.ExtraMenu.ShowFPS)
                    {
                        float fps = ImGui::GetIO().Framerate;
                        char fpsText[32];
                        snprintf(fpsText, sizeof(fpsText), "%.0f FPS", fps);
                        ImDrawList* draw = ImGui::GetWindowDrawList();

                        if (TitleFont)
                        {
                            ImVec2 textSize = TitleFont->CalcTextSizeA(26.0f, FLT_MAX, 0.0f, fpsText);
                            float textX = windowPos.x + logoWindowSize.x + 5.0f;
                            float textY = windowPos.y + (logoWindowSize.y - textSize.y) / 2.0f;

                            draw->AddText(TitleFont, 26.0f, ImVec2(textX + 1, textY + 1), ImColor(0, 0, 0, 180), fpsText);
                            draw->AddText(TitleFont, 26.0f, ImVec2(textX, textY), ImColor(255, 160, 0, 255), fpsText);
                        }
                    }
                }
            }
            ImGui::End();
            ImGui::PopStyleColor(2);
            ImGui::PopStyleVar(3);

            if (restoreMenu)
            {
                isMenuVisible = true;
                collapsedPosInitialized = false;
                logoScale = 1.0f;
                logoAnimating = true;
                menuFade = 0.0f;
                dockSlide = -104.0f;
            }
        } else {
            ImGuiIO& io = ImGui::GetIO();
            float screenWidth = io.DisplaySize.x;

            float triggerWidth = 200.0f;
            float triggerHeight = 100.0f;
            float triggerX = (screenWidth - triggerWidth) / 2.0f;
            float triggerY = 0.0f;

            if (io.MouseClicked[0] || io.MouseClicked[1] || io.MouseClicked[2]) {
                ImVec2 mousePos = io.MousePos;
                if (mousePos.x >= triggerX && mousePos.x <= triggerX + triggerWidth &&
                    mousePos.y >= triggerY && mousePos.y <= triggerY + triggerHeight) {
                    isMenuVisible = true;
                    collapsedPosInitialized = false;
                    logoScale = 1.0f;
                    logoAnimating = true;
                    menuFade = 0.0f;
                    dockSlide = -104.0f;
                }
            }
        }
    }

if (isMenuVisible) {
    float deltaTime = ImGui::GetIO().DeltaTime;
    const float dockVisibleX = -8.0f;
    menuFade = std::min(menuFade + deltaTime * 3.0f, 1.0f);
    dockSlide = std::min(dockSlide + deltaTime * 380.0f, dockVisibleX);

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menuFade);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 24.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 14.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 12));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.03f, 0.16f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.05f, 0.24f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.55f, 0.16f, 0.82f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.10f, 0.04f, 0.26f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.25f, 0.12f, 0.45f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.32f, 0.10f, 0.52f, 1.00f));

    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(885, 600), ImGuiCond_Once);


char title[64];
sprintf(title, "###MainMenu");

if (ImGui::Begin(title, nullptr,
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoTitleBar)) {
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12,12));
    static bool isLogin = true;
    static std::string err;
    static char s[64] = "";
    static std::string storedKey = loadKeyFromFile();


    ImGuiIO& io = ImGui::GetIO();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10,10));

    ImGui::BeginChild("HeaderBar", ImVec2(0, 50), false,
        ImGuiWindowFlags_NoScrollbar);

    float width = ImGui::GetContentRegionAvail().x;
    float headerHeight = 50.0f;

    ImGui::SetCursorPosY((headerHeight - 40) * 0.5f);

    // SAVE button
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.38f, 0.12f, 0.58f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.25f, 0.90f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.78f, 0.15f, 1.00f, 1.0f));

    if (ImGui::Button(ICON_FA_FOLDER, ImVec2(80,44)))
    {
        saveAllConfig();
        ImGui::OpenPopup("Config Saved");
    }

    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    // LOAD button
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.48f, 0.18f, 0.68f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.30f, 0.98f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.88f, 0.22f, 1.00f, 1.0f));

    if (ImGui::Button(ICON_FA_FILE, ImVec2(80,44)))
    {
        if (loadAllConfig())
            ImGui::OpenPopup("Config Loaded");
        else
            ImGui::OpenPopup("Config Error");
    }

    ImGui::PopStyleColor(3);

    ImGui::PushFont(TitleFont);

    const char* titleText = "AYLNHAX V3 | 1.6.54";
    float titleWidth = ImGui::CalcTextSize(titleText).x;
    float centerY = (headerHeight - ImGui::GetTextLineHeight()) * 0.5f;

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - titleWidth) * 0.5f);
    ImGui::SetCursorPosY(centerY);

    ImGui::TextColored(ImVec4(0.82f,0.54f,1.00f,1), titleText);

    ImGui::PopFont();

    char fpsText[32];
    sprintf(fpsText, "FPS: %.0f", io.Framerate);

    float textWidth = ImGui::CalcTextSize(fpsText).x;

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textWidth - 60);
    ImGui::SetCursorPosY(centerY);

    ImGui::TextColored(ImVec4(0.72f,0.50f,1.00f,1), fpsText);

    ImGui::EndChild();
    ImGui::PopStyleVar();
    
    // Glass separator line under header
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    draw->AddLine(
        ImVec2(p.x, p.y),
        ImVec2(p.x + ImGui::GetWindowWidth(), p.y),
        IM_COL32(255,60,60,120),
        2.0f
    );
    
    if (!isLogin) {
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(OBFUSCATE("AYLNHAX LOGIN")).x) / 2);
        ImGui::TextColored(ImVec4(0.2f, 0.7f, 1.0f, 1.0f), OBFUSCATE("AYLNHAX LOGIN"));
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextWrapped(OBFUSCATE("To access Alwyn Mod, please enter your key and log in:"));
        ImGui::Spacing();
        
        ImGui::Text(OBFUSCATE("Enter Your Key:"));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        ImGui::PushItemWidth(-1);
        if (ImGui::InputTextWithHint(OBFUSCATE("##key"), OBFUSCATE("Paste your key here..."), s, sizeof(s), ImGuiInputTextFlags_EnterReturnsTrue)) {
            err = Login(s);
            if (err == std::string(OBFUSCATE("OK"))) {
                isLogin = true;
                storedKey = s; 
                saveKeyToFile(storedKey); 
            }
        }
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();
        ImGui::Spacing();


        float buttonWidth = (ImGui::GetContentRegionAvail().x / 2) - 5;
        
        if (ImGui::Button(OBFUSCATE("Paste License"), ImVec2(buttonWidth, 40))) {
            auto key = getClipboard();
            strncpy(s, key.c_str(), sizeof(s));
            s[sizeof(s) - 1] = '\0'; 
        }
        ImGui::SameLine();
        if (ImGui::Button(OBFUSCATE("Load License"), ImVec2(buttonWidth, 40))) {
            if (!storedKey.empty()) {
                strncpy(s, storedKey.c_str(), sizeof(s));
                s[sizeof(s) - 1] = '\0'; 
            } else {
                err = "No key has been stored yet.";
            }
        }

        ImGui::Spacing();

        // Login Button
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        if (ImGui::Button(OBFUSCATE("Login"), ImVec2(-1, 50))) {
            if (strlen(s) == 0) {
                err = "Please enter a valid license.";
            } else {
                err = Login(s);
                if (err == std::string(OBFUSCATE("OK"))) {
                    isLogin = true;
                    storedKey = s; 
                    saveKeyToFile(storedKey); 
                }
            }
        }
        ImGui::PopStyleVar();
        ImGui::Spacing();

        // Error / Success Message
        if (!err.empty()) {
            ImVec4 msgColor = (err == "OK") ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
            const char* msg = (err == "OK") ? "Login successful! Welcome to Alwyn Mod." : ("Error: " + err).c_str();
            ImGui::TextColored(msgColor, msg);
        }
  
} 
    else
    {
    
    static int tab = 1;
    static bool tabChanged = false;
    
    // Load image texture
    if (!astralLoaded) {
        astralTextureID = LoadAstralTexture(astral_data, sizeof(astral_data));
        astralLoaded = astralTextureID != 0;
    }
    if (!astralLoaded) {
        astralTextureID = LoadAstralTexture(astral_data, sizeof(astral_data));
    }
    
    const float sidebarWidth = 92.0f;
    const float dockHeight = std::max(460.0f, ImGui::GetIO().DisplaySize.y - 160.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
    ImGui::SetNextWindowPos(ImVec2(dockSlide, 80.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSizeConstraints(ImVec2(sidebarWidth, 280.0f), ImVec2(sidebarWidth, ImGui::GetIO().DisplaySize.y - 120.0f));
    ImGui::SetNextWindowSize(ImVec2(sidebarWidth, dockHeight), ImGuiCond_Always);
    ImGui::Begin("DockWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoBackground);
    {
        ImDrawList* dockDraw = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImU32 dockBg = ImColor(26, 11, 38, 245);
        ImU32 dockBorder = ImColor(153, 71, 229, 240);
        const float dockCorners = 22.0f;
        dockDraw->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y), dockBg, dockCorners, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);
        dockDraw->AddRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y), dockBorder, dockCorners, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight, 2.0f);

        ImGui::SetCursorPosY(14);
        ImGui::SetCursorPosX((sidebarWidth - 44.0f) * 0.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 14.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.08f, 0.36f, 0.80f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.48f, 0.18f, 0.70f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.64f, 0.24f, 0.98f, 1.0f));
        if (ImGui::Button(ICON_FA_TIMES, ImVec2(42, 42))) {
            isMenuVisible = false;
            collapsedPosInitialized = false;
            logoScale = 1.0f;
            logoAnimating = true;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);

        static const char* tabIcons[] = { ICON_FA_EYE, ICON_FA_MICROCHIP, ICON_FA_GAMEPAD, ICON_FA_CROSSHAIRS, ICON_FA_BOMB, ICON_FA_COG };
        static const char* tabTips[] = { "Visual", "Memory", "Ability", "Aimbot", "Skins", "Settings" };
        const float iconSize = 48.0f;
        const float closeButtonHeight = 44.0f;
        const float topOffset = 14.0f + closeButtonHeight + 14.0f;
        const float occupiedIcons = iconSize * 6.0f;
        const float availableHeight = windowSize.y - topOffset - 18.0f;
        const float spacing = std::max(16.0f, (availableHeight - occupiedIcons) / 5.0f);
        const float iconX = (sidebarWidth - iconSize) * 0.5f;
        const float targetIndicatorY = topOffset + (tab - 1) * (iconSize + spacing);

        if (tab != lastTab) {
            tabIconFade = 0.0f;
            lastTab = tab;
        }
        tabIconFade = std::min(tabIconFade + deltaTime * 5.5f, 1.0f);
        if (tabIndicatorPosY == 0.0f) {
            tabIndicatorPosY = targetIndicatorY;
        }
        float lerpAmount = std::min(deltaTime * 14.0f, 1.0f);
        tabIndicatorPosY = tabIndicatorPosY + (targetIndicatorY - tabIndicatorPosY) * lerpAmount;

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        for (int id = 1; id <= 6; ++id) {
            const float buttonY = topOffset + (id - 1) * (iconSize + spacing);
            ImGui::SetCursorPosY(buttonY);
            ImGui::SetCursorPosX(iconX);
            bool selected = (tab == id);
            float iconAlpha = selected ? (0.75f + 0.25f * tabIconFade) : 0.70f;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.16f, 0.82f, 0.16f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.55f, 0.20f, 0.94f, 0.26f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, iconAlpha));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 14.0f);
            if (ImGui::Button(tabIcons[id - 1], ImVec2(iconSize, iconSize))) {
                tab = id;
            }
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(4);

            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", tabTips[id - 1]);
            }
        }

        const float barWidth = 6.0f;
        const float barPadding = 4.0f;
        dockDraw->AddRectFilled(ImVec2(windowPos.x + iconX - barWidth - barPadding, windowPos.y + tabIndicatorPosY + barPadding), ImVec2(windowPos.x + iconX - barPadding, windowPos.y + tabIndicatorPosY + iconSize - barPadding), ImColor(0.84f, 0.44f, 1.00f, 1.0f), 8.0f, ImDrawCornerFlags_All);
        dockDraw->AddRect(ImVec2(windowPos.x + iconX - barWidth - barPadding, windowPos.y + tabIndicatorPosY + barPadding), ImVec2(windowPos.x + iconX - barPadding, windowPos.y + tabIndicatorPosY + iconSize - barPadding), ImColor(0.84f, 0.44f, 1.00f, 1.0f), 8.0f, ImDrawCornerFlags_All, 1.5f);

        ImGui::PopStyleVar(2);
    }
    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    // Main content area
    ImGui::BeginChild("MainContent", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
    
    // RGB Animation
    static float rgb_timer = 0.0f;
    rgb_timer += ImGui::GetIO().DeltaTime * 0.5f;
    ImVec4 rgb_color = ImVec4(
        sin(rgb_timer)*0.5f+0.5f,
        sin(rgb_timer+2.0f)*0.5f+0.5f,
        sin(rgb_timer+4.0f)*0.5f+0.5f,
        1.0f
    );

if (tab == 1) {
    const float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.5f;
    const float rightPanelWidth = ImGui::GetContentRegionAvail().x - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;
    const float childHeight = ImGui::GetContentRegionAvail().y;

    ImGui::SetCursorPos(ImVec2(10, 10));
    
    // Left Panel
    ImGui::BeginChild("ESP Nav##1", ImVec2(leftPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Extra Sensory Perception");
        ImGui::Spacing();
        
        if (Config.ESPMenu.Esp) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));
            if (ImGui::Button("ESP (ON)", ImVec2(-1, 46))) Config.ESPMenu.Esp = false;
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
            if (ImGui::Button("ESP (OFF)", ImVec2(-1, 46))) Config.ESPMenu.Esp = true;
            ImGui::PopStyleColor();
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(rgb_color, "Player ESP Options");
        ImGui::Spacing();
        CustomCheckbox("Line", &Config.ESPMenu.isPlayerLine);
        CustomCheckbox("Box", &Config.ESPMenu.Box);
        CustomCheckbox("Health", &Config.ESPMenu.Health);
        CustomCheckbox("Name", &Config.ESPMenu.Name);
        CustomCheckbox("Distance", &Config.ESPMenu.Distance);
        CustomCheckbox("Counter", &Config.ESPMenu.Count);
        CustomCheckbox("360 Alert", &Config.ESPMenu.Alert);
        ImGui::Separator();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right Panel
    ImGui::BeginChild("ESP Content##1", ImVec2(rightPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Extra Visuals");
        ImGui::Spacing();
        
        // ESP Color Customization
        if (ImGui::CollapsingHeader("Player ESP Colors")) {
            // Convert float* to ImVec4 for color editing
            static ImVec4 playerLineColor, playerBoxColor, playerNameColor, playerHealthColor, playerDistanceColor;
            
            if (Config.sColorsESPPLAYER.LinePLAYER) {
                playerLineColor = ImVec4(Config.sColorsESPPLAYER.LinePLAYER[0], Config.sColorsESPPLAYER.LinePLAYER[1], 
                                       Config.sColorsESPPLAYER.LinePLAYER[2], Config.sColorsESPPLAYER.LinePLAYER[3]);
                if (ImGui::ColorEdit4("Player Line", (float*)&playerLineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPPLAYER.LinePLAYER[0] = playerLineColor.x;
                    Config.sColorsESPPLAYER.LinePLAYER[1] = playerLineColor.y;
                    Config.sColorsESPPLAYER.LinePLAYER[2] = playerLineColor.z;
                    Config.sColorsESPPLAYER.LinePLAYER[3] = playerLineColor.w;
                }
            }
            
            if (Config.sColorsESPPLAYER.BoxPLAYER) {
                playerBoxColor = ImVec4(Config.sColorsESPPLAYER.BoxPLAYER[0], Config.sColorsESPPLAYER.BoxPLAYER[1], 
                                      Config.sColorsESPPLAYER.BoxPLAYER[2], Config.sColorsESPPLAYER.BoxPLAYER[3]);
                if (ImGui::ColorEdit4("Player Box", (float*)&playerBoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPPLAYER.BoxPLAYER[0] = playerBoxColor.x;
                    Config.sColorsESPPLAYER.BoxPLAYER[1] = playerBoxColor.y;
                    Config.sColorsESPPLAYER.BoxPLAYER[2] = playerBoxColor.z;
                    Config.sColorsESPPLAYER.BoxPLAYER[3] = playerBoxColor.w;
                }
            }
            
            if (Config.sColorsESPPLAYER.NamePLAYER) {
                playerNameColor = ImVec4(Config.sColorsESPPLAYER.NamePLAYER[0], Config.sColorsESPPLAYER.NamePLAYER[1], 
                                       Config.sColorsESPPLAYER.NamePLAYER[2], Config.sColorsESPPLAYER.NamePLAYER[3]);
                if (ImGui::ColorEdit4("Player Name", (float*)&playerNameColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPPLAYER.NamePLAYER[0] = playerNameColor.x;
                    Config.sColorsESPPLAYER.NamePLAYER[1] = playerNameColor.y;
                    Config.sColorsESPPLAYER.NamePLAYER[2] = playerNameColor.z;
                    Config.sColorsESPPLAYER.NamePLAYER[3] = playerNameColor.w;
                }
            }
            
            if (Config.sColorsESPPLAYER.HealthPLAYER) {
                playerHealthColor = ImVec4(Config.sColorsESPPLAYER.HealthPLAYER[0], Config.sColorsESPPLAYER.HealthPLAYER[1], 
                                          Config.sColorsESPPLAYER.HealthPLAYER[2], Config.sColorsESPPLAYER.HealthPLAYER[3]);
                if (ImGui::ColorEdit4("Player Health", (float*)&playerHealthColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPPLAYER.HealthPLAYER[0] = playerHealthColor.x;
                    Config.sColorsESPPLAYER.HealthPLAYER[1] = playerHealthColor.y;
                    Config.sColorsESPPLAYER.HealthPLAYER[2] = playerHealthColor.z;
                    Config.sColorsESPPLAYER.HealthPLAYER[3] = playerHealthColor.w;
                }
            }
            
            if (Config.sColorsESPPLAYER.DistancePLAYER) {
                playerDistanceColor = ImVec4(Config.sColorsESPPLAYER.DistancePLAYER[0], Config.sColorsESPPLAYER.DistancePLAYER[1], 
                                           Config.sColorsESPPLAYER.DistancePLAYER[2], Config.sColorsESPPLAYER.DistancePLAYER[3]);
                if (ImGui::ColorEdit4("Player Distance", (float*)&playerDistanceColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPPLAYER.DistancePLAYER[0] = playerDistanceColor.x;
                    Config.sColorsESPPLAYER.DistancePLAYER[1] = playerDistanceColor.y;
                    Config.sColorsESPPLAYER.DistancePLAYER[2] = playerDistanceColor.z;
                    Config.sColorsESPPLAYER.DistancePLAYER[3] = playerDistanceColor.w;
                }
            }
            ImGui::Separator();
        }

        if (ImGui::CollapsingHeader("Bot ESP Colors")) {
            // Convert float* to ImVec4 for color editing
            static ImVec4 botLineColor, botBoxColor, botNameColor, botHealthColor, botDistanceColor;
            
            if (Config.sColorsESPBOT.LineBOT) {
                botLineColor = ImVec4(Config.sColorsESPBOT.LineBOT[0], Config.sColorsESPBOT.LineBOT[1], 
                                    Config.sColorsESPBOT.LineBOT[2], Config.sColorsESPBOT.LineBOT[3]);
                if (ImGui::ColorEdit4("Bot Line", (float*)&botLineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPBOT.LineBOT[0] = botLineColor.x;
                    Config.sColorsESPBOT.LineBOT[1] = botLineColor.y;
                    Config.sColorsESPBOT.LineBOT[2] = botLineColor.z;
                    Config.sColorsESPBOT.LineBOT[3] = botLineColor.w;
                }
            }
            
            if (Config.sColorsESPBOT.BoxBOT) {
                botBoxColor = ImVec4(Config.sColorsESPBOT.BoxBOT[0], Config.sColorsESPBOT.BoxBOT[1], 
                                   Config.sColorsESPBOT.BoxBOT[2], Config.sColorsESPBOT.BoxBOT[3]);
                if (ImGui::ColorEdit4("Bot Box", (float*)&botBoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPBOT.BoxBOT[0] = botBoxColor.x;
                    Config.sColorsESPBOT.BoxBOT[1] = botBoxColor.y;
                    Config.sColorsESPBOT.BoxBOT[2] = botBoxColor.z;
                    Config.sColorsESPBOT.BoxBOT[3] = botBoxColor.w;
                }
            }
            
            if (Config.sColorsESPBOT.NameBOT) {
                botNameColor = ImVec4(Config.sColorsESPBOT.NameBOT[0], Config.sColorsESPBOT.NameBOT[1], 
                                    Config.sColorsESPBOT.NameBOT[2], Config.sColorsESPBOT.NameBOT[3]);
                if (ImGui::ColorEdit4("Bot Name", (float*)&botNameColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPBOT.NameBOT[0] = botNameColor.x;
                    Config.sColorsESPBOT.NameBOT[1] = botNameColor.y;
                    Config.sColorsESPBOT.NameBOT[2] = botNameColor.z;
                    Config.sColorsESPBOT.NameBOT[3] = botNameColor.w;
                }
            }
            
            if (Config.sColorsESPBOT.HealthBOT) {
                botHealthColor = ImVec4(Config.sColorsESPBOT.HealthBOT[0], Config.sColorsESPBOT.HealthBOT[1], 
                                      Config.sColorsESPBOT.HealthBOT[2], Config.sColorsESPBOT.HealthBOT[3]);
                if (ImGui::ColorEdit4("Bot Health", (float*)&botHealthColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPBOT.HealthBOT[0] = botHealthColor.x;
                    Config.sColorsESPBOT.HealthBOT[1] = botHealthColor.y;
                    Config.sColorsESPBOT.HealthBOT[2] = botHealthColor.z;
                    Config.sColorsESPBOT.HealthBOT[3] = botHealthColor.w;
                }
            }
            
            if (Config.sColorsESPBOT.DistanceBOT) {
                botDistanceColor = ImVec4(Config.sColorsESPBOT.DistanceBOT[0], Config.sColorsESPBOT.DistanceBOT[1], 
                                        Config.sColorsESPBOT.DistanceBOT[2], Config.sColorsESPBOT.DistanceBOT[3]);
                if (ImGui::ColorEdit4("Bot Distance", (float*)&botDistanceColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPBOT.DistanceBOT[0] = botDistanceColor.x;
                    Config.sColorsESPBOT.DistanceBOT[1] = botDistanceColor.y;
                    Config.sColorsESPBOT.DistanceBOT[2] = botDistanceColor.z;
                    Config.sColorsESPBOT.DistanceBOT[3] = botDistanceColor.w;
                }
            }
            ImGui::Separator();
        }

        if (ImGui::CollapsingHeader("Other ESP Colors")) {
            if (Config.sColorsESPOTHERS.PovOTHERS) {
                static ImVec4 povColor = ImVec4(Config.sColorsESPOTHERS.PovOTHERS[0], Config.sColorsESPOTHERS.PovOTHERS[1],
                                               Config.sColorsESPOTHERS.PovOTHERS[2], Config.sColorsESPOTHERS.PovOTHERS[3]);
                if (ImGui::ColorEdit4("POV Color", (float*)&povColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
                    Config.sColorsESPOTHERS.PovOTHERS[0] = povColor.x;
                    Config.sColorsESPOTHERS.PovOTHERS[1] = povColor.y;
                    Config.sColorsESPOTHERS.PovOTHERS[2] = povColor.z;
                    Config.sColorsESPOTHERS.PovOTHERS[3] = povColor.w;
                }
            }
            ImGui::Separator();
        }

        if (ImGui::Button("Reset All Colors to Default", ImVec2(-1, 44))) {
            // Player colors (red)
            if (Config.sColorsESPPLAYER.LinePLAYER) {
                Config.sColorsESPPLAYER.LinePLAYER[0] = 1.0f;
                Config.sColorsESPPLAYER.LinePLAYER[1] = 0.0f;
                Config.sColorsESPPLAYER.LinePLAYER[2] = 0.0f;
                Config.sColorsESPPLAYER.LinePLAYER[3] = 1.0f;
            }
            if (Config.sColorsESPPLAYER.BoxPLAYER) {
                Config.sColorsESPPLAYER.BoxPLAYER[0] = 1.0f;
                Config.sColorsESPPLAYER.BoxPLAYER[1] = 0.0f;
                Config.sColorsESPPLAYER.BoxPLAYER[2] = 0.0f;
                Config.sColorsESPPLAYER.BoxPLAYER[3] = 1.0f;
            }
            if (Config.sColorsESPPLAYER.NamePLAYER) {
                Config.sColorsESPPLAYER.NamePLAYER[0] = 1.0f;
                Config.sColorsESPPLAYER.NamePLAYER[1] = 0.0f;
                Config.sColorsESPPLAYER.NamePLAYER[2] = 0.0f;
                Config.sColorsESPPLAYER.NamePLAYER[3] = 1.0f;
            }
            if (Config.sColorsESPPLAYER.HealthPLAYER) {
                Config.sColorsESPPLAYER.HealthPLAYER[0] = 1.0f;
                Config.sColorsESPPLAYER.HealthPLAYER[1] = 0.0f;
                Config.sColorsESPPLAYER.HealthPLAYER[2] = 0.0f;
                Config.sColorsESPPLAYER.HealthPLAYER[3] = 1.0f;
            }
            if (Config.sColorsESPPLAYER.DistancePLAYER) {
                Config.sColorsESPPLAYER.DistancePLAYER[0] = 1.0f;
                Config.sColorsESPPLAYER.DistancePLAYER[1] = 0.0f;
                Config.sColorsESPPLAYER.DistancePLAYER[2] = 0.0f;
                Config.sColorsESPPLAYER.DistancePLAYER[3] = 1.0f;
            }
            
            // Bot colors (green with 70% alpha)
            if (Config.sColorsESPBOT.LineBOT) {
                Config.sColorsESPBOT.LineBOT[0] = 0.0f;
                Config.sColorsESPBOT.LineBOT[1] = 1.0f;
                Config.sColorsESPBOT.LineBOT[2] = 0.0f;
                Config.sColorsESPBOT.LineBOT[3] = 0.7f;
            }
            if (Config.sColorsESPBOT.BoxBOT) {
                Config.sColorsESPBOT.BoxBOT[0] = 0.0f;
                Config.sColorsESPBOT.BoxBOT[1] = 1.0f;
                Config.sColorsESPBOT.BoxBOT[2] = 0.0f;
                Config.sColorsESPBOT.BoxBOT[3] = 0.7f;
            }
            if (Config.sColorsESPBOT.NameBOT) {
                Config.sColorsESPBOT.NameBOT[0] = 0.0f;
                Config.sColorsESPBOT.NameBOT[1] = 1.0f;
                Config.sColorsESPBOT.NameBOT[2] = 0.0f;
                Config.sColorsESPBOT.NameBOT[3] = 0.7f;
            }
            if (Config.sColorsESPBOT.HealthBOT) {
                Config.sColorsESPBOT.HealthBOT[0] = 0.0f;
                Config.sColorsESPBOT.HealthBOT[1] = 1.0f;
                Config.sColorsESPBOT.HealthBOT[2] = 0.0f;
                Config.sColorsESPBOT.HealthBOT[3] = 0.7f;
            }
            if (Config.sColorsESPBOT.DistanceBOT) {
                Config.sColorsESPBOT.DistanceBOT[0] = 0.0f;
                Config.sColorsESPBOT.DistanceBOT[1] = 1.0f;
                Config.sColorsESPBOT.DistanceBOT[2] = 0.0f;
                Config.sColorsESPBOT.DistanceBOT[3] = 0.7f;
            }
            
            // POV color (red with 70% alpha)
            if (Config.sColorsESPOTHERS.PovOTHERS) {
                Config.sColorsESPOTHERS.PovOTHERS[0] = 0.88f;
                Config.sColorsESPOTHERS.PovOTHERS[1] = 0.0f;
                Config.sColorsESPOTHERS.PovOTHERS[2] = 0.0f;
                Config.sColorsESPOTHERS.PovOTHERS[3] = 0.7f;
            }
        }
        
        ImGui::Text("Crosshair Style");
        const char* crosshairStyles[] = { "Circle", "Plus", "Ekis" };
        for (int i = 0; i < IM_ARRAYSIZE(crosshairStyles); i++) {
            if (ImGui::RadioButton(crosshairStyles[i], static_cast<int>(Config.ESPMenu.CrosshairType) == i))
                Config.ESPMenu.CrosshairType = static_cast<decltype(Config.ESPMenu.CrosshairType)>(i);
        } ImGui::SameLine();
        
        ImGui::Spacing();
        ImGui::Separator();
        
        ImGui::Text("FOV Size:");
        CustomSliderFloat("FOV", &Config.Aim.size, 0, 500, "%.0f");
        
        ImGui::Spacing();
        ImGui::Separator();
        
        ImGui::Text("Custom Settings:");
        CustomCheckbox("Render FOV", &Config.ESPMenu.Crosshair);
        CustomCheckbox("Render AimLine", &Config.ESPMenu.Aimline);
    }
    ImGui::EndChild();
}
        

if (tab == 2) {
    const float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.5f;
    const float rightPanelWidth = ImGui::GetContentRegionAvail().x - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;
    const float childHeight = ImGui::GetContentRegionAvail().y;
    
    // Reset cursor position for tab content
    ImGui::SetCursorPos(ImVec2(10, 10));

    ImGui::BeginChild("Memory Nav##2", ImVec2(leftPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Adjustable Aim Assist");
        ImGui::Spacing();
        
        if (CustomCheckbox("AimAssist", &Config.Aim.AimAssist)) {
            Config.Aim.showAimAssistSlider = Config.Aim.AimAssist;
        }
        
        if (Config.Aim.showAimAssistSlider) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            ImGui::Text("AimAssist Settings");
            ImGui::PopStyleColor();
            CustomSliderFloat("Locking", &Config.Aim.AimAssistSize, 0, 100, "%.0f");
            ImGui::Spacing();
            ImGui::Separator();
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(rgb_color, "Weapon Memory V1");
        ImGui::Spacing();
        CustomCheckbox("Weapon Kinetic", &Config.ExtraMenu.Kinetic);
        CustomCheckbox("No Recoil", &Config.ExtraMenu.Recoil);
        CustomCheckbox("No Spread", &Config.ExtraMenu.Spread);
        CustomCheckbox("No Reload", &Config.ExtraMenu.Reload);
        CustomCheckbox("No Scope", &Config.ExtraMenu.Scope);
        CustomCheckbox("No Switch", &Config.ExtraMenu.Switch);
        CustomCheckbox("No Shake", &Config.ExtraMenu.Shake);
        CustomCheckbox("No Flashbang", &Config.ExtraMenu.Flash);
		ImGui::Separator();
    }
	
    ImGui::EndChild();
    ImGui::SameLine();

    // Right Panel - Auto-scroll only when needed
    ImGui::BeginChild("Memory Content##2", ImVec2(rightPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Weapon Memory V2");
        ImGui::Spacing();
        //CustomCheckbox("Small Crosshair", &Config.ExtraMenu.Small);
        //CustomCheckbox("Unli Bullets", &Config.ExtraMenu.Cost);
        CustomCheckbox("No Overheat", &Config.ExtraMenu.Rpd);
        CustomCheckbox("Long Range", &Config.ExtraMenu.Range);
        CustomCheckbox("Instant Shot", &Config.ExtraMenu.AutoSnipe);
        CustomCheckbox("Hitbox Increase", &Config.ExtraMenu.Hit);
        CustomCheckbox("Quick Firing", &Config.ExtraMenu.Auto);
        CustomCheckbox("Firerate", &Config.ExtraMenu.Fire);		
		ImGui::Separator();
		
		ImGui::TextColored(rgb_color, "Revival & Medicine");
        ImGui::Spacing();
        CustomCheckbox("Revival Speed", &Config.ExtraMenu.Revive);
        CustomCheckbox("Medicine Speed", &Config.ExtraMenu.Medicine);
		ImGui::Separator();
    }
    ImGui::EndChild();
}

if (tab == 3) {
    const float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.5f;
    const float rightPanelWidth = ImGui::GetContentRegionAvail().x - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;
    const float childHeight = ImGui::GetContentRegionAvail().y;

    // Reset cursor position for tab content
    ImGui::SetCursorPos(ImVec2(10, 10));

    // Left Panel - Movement Abilities
    ImGui::BeginChild("Ability Nav##3", ImVec2(leftPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Movement Abilities");
        ImGui::Spacing();

        // Snowboard Section
        if (CustomCheckbox("Snowboard Boost", &SnowB)) SnowBshow = SnowB;
        
        // Slide Section
        if (CustomCheckbox("Long Slide", &SlideTackle)) SlideShow = SlideTackle;
        
        // Speed Section
        if (CustomCheckbox("Speed Hack", &isSpeedHackEnabled)) showSpeedHackSlider = isSpeedHackEnabled;
        
        // Jump Section
        if (CustomCheckbox("High Jump", &isJumpAdjustmentEnabled)) {
            if (!isJumpAdjustmentEnabled) jumpHeightMultiplier = 1.0f;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(rgb_color, "Visual Abilities");
        ImGui::Spacing();

        // Visual Features
        CustomCheckbox("Spectate No Delay", &Config.ExtraMenu.Spectate);
        CustomCheckbox("Wallhack Outline", &Config.ExtraMenu.Outline);
        CustomCheckbox("Wallhack Glow", &Config.ExtraMenu.WallHack);
        //CustomCheckbox("Wallhack Red", &Config.ExtraMenu.Red);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Ability Content##3", ImVec2(rightPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Ability Settings");
        ImGui::Spacing();

        // Adjustable Parameters
        if (SnowBshow) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            ImGui::Text("Snowboard Settings:");
            ImGui::PopStyleColor();
            CustomSliderFloat("Boost", &SnowBsize, 0.0f, 100.0f, "%.1f");
            ImGui::Spacing();
        }

        if (SlideShow) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            ImGui::Text("Slide Settings:");
            ImGui::PopStyleColor();
            CustomSliderFloat("Slide", &SlideRange, 0.0f, 30.0f, "%.1f");
            ImGui::Spacing();
        }

        if (showSpeedHackSlider) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            ImGui::Text("Speed Settings:");
            ImGui::PopStyleColor();
            CustomSliderFloat("Speed", &speedHackMultiplier, 0.5f, 10.0f, "%.1f");
            ImGui::Spacing();
        }

        if (isJumpAdjustmentEnabled) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            ImGui::Text("Jump Settings:");
            ImGui::PopStyleColor();
            CustomSliderFloat("Height", &jumpHeightMultiplier, 0.5f, 5.0f, "%.1f");
            ImGui::Spacing();
        }

        ImGui::Separator();
        ImGui::TextColored(rgb_color, "Additional Features");
        ImGui::Spacing();

        // Additional Features
        CustomCheckbox("Jet Boost + Jump", &Config.ExtraMenu.JetJ);
        CustomCheckbox("Jet Boost + Slide", &Config.ExtraMenu.JetS);
        CustomCheckbox("Long Execute", &isExecute);
        CustomCheckbox("Radar MP", &Config.ExtraMenu.Radar1);
        CustomCheckbox("No Parachute", &Config.ExtraMenu.Parachute);
        CustomCheckbox("Sky Diving Speed", &Config.ExtraMenu.Diving);
    }
    ImGui::EndChild();
}
   
    if (tab == 4) {
        const float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.40f;
        const float rightPanelWidth = ImGui::GetContentRegionAvail().x - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;
        const float childHeight = ImGui::GetContentRegionAvail().y;

        // Reset cursor position for tab content
        ImGui::SetCursorPos(ImVec2(10, 10));

        ImGui::BeginChild("Aimbot Controls##4", ImVec2(leftPanelWidth, childHeight), true);
        {
            ImGui::TextColored(rgb_color, "Aimbot Controls");
            ImGui::Spacing();
            ImGui::Separator();
            
            CustomCheckbox("Aimbot 360", &Config.Aim.Aimbot360);
            CustomCheckbox("Bullet Track", &Config.Aim.AimSilent);
            
            ImGui::Spacing();
            ImGui::Separator();
            
            // Additional Aimbot Features
            ImGui::TextColored(rgb_color, "Aimbot Features");
            ImGui::Spacing();
            CustomCheckbox("Auto Aim Assist", &Config.Aim.AimAssist);
            if (Config.Aim.AimAssist) {
                CustomSliderFloat("Assist Strength", &Config.Aim.AimAssistSize, 0, 100, "%.0f");
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right Panel - Aimbot Settings
        ImGui::BeginChild("Aimbot Settings##4", ImVec2(rightPanelWidth, childHeight), true);
        {
            ImGui::TextColored(rgb_color, "Aimbot Configuration");
            ImGui::Spacing();
            ImGui::Separator();
            
            ImGui::Text("Target Location");
            const char* targets[] = { "Head", "Chest", "Body" };
            ImGui::Columns(3, NULL, false);
            for (int i = 0; i < IM_ARRAYSIZE(targets); i++) {
                if (ImGui::RadioButton(targets[i], static_cast<int>(Config.Aim.Target) == i))
                    Config.Aim.Target = static_cast<decltype(Config.Aim.Target)>(i);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Spacing();
            
            ImGui::Text("Trigger Mode");
            const char* triggers[] = { "None", "Shooting", "Scoping" };
            ImGui::Columns(3, NULL, false);
            for (int i = 0; i < IM_ARRAYSIZE(triggers); i++) {
                if (ImGui::RadioButton(triggers[i], static_cast<int>(Config.Aim.Trigger) == i))
                    Config.Aim.Trigger = static_cast<decltype(Config.Aim.Trigger)>(i);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Spacing();
            
            ImGui::Text("Target Selection");
            const char* tarets[] = { "Distance", "FOV" };
            ImGui::Columns(2, NULL, false);
            for (int i = 0; i < IM_ARRAYSIZE(tarets); i++) {
                if (ImGui::RadioButton(tarets[i], static_cast<int>(Config.Aim.By) == i))
                    Config.Aim.By = static_cast<decltype(Config.Aim.By)>(i);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            
            ImGui::Spacing();
            ImGui::Separator();
            
            // FOV Size Control
            ImGui::Text("Field of View");
            CustomSliderFloat("FOV Size", &Config.Aim.size, 0, 500, "%.0f");
            
            // Crosshair Settings
            ImGui::Text("Crosshair Style");
            const char* crosshairStyles[] = { "Circle", "Plus", "Ekis" };
            for (int i = 0; i < IM_ARRAYSIZE(crosshairStyles); i++) {
                if (ImGui::RadioButton(crosshairStyles[i], static_cast<int>(Config.ESPMenu.CrosshairType) == i))
                    Config.ESPMenu.CrosshairType = static_cast<decltype(Config.ESPMenu.CrosshairType)>(i);
            }
        }
        ImGui::EndChild();
    }

    
    if (tab == 5) {
    float availWidth = ImGui::GetContentRegionAvail().x;
    const float childHeight = ImGui::GetContentRegionAvail().y - 60; // Leave room for keyboard toggle
    
    // Reset cursor position for tab content
    ImGui::SetCursorPos(ImVec2(10, 10));
    
    static bool keyboardVisible = false;
    
    // Keyboard toggle button
    if (keyboardVisible)
    {
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.75f, 0.15f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.95f, 0.25f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(1.00f, 0.35f, 0.35f, 1.0f));
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.35f, 0.10f, 0.10f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.55f, 0.15f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.70f, 0.20f, 0.20f, 1.0f));
    }

    if (ImGui::Button(
            keyboardVisible ? ICON_FA_KEYBOARD "  Hide Keyboard"
                            : ICON_FA_KEYBOARD "  Show Keyboard",
            ImVec2(availWidth - 20, 40)))
    {
        keyboardVisible = !keyboardVisible;
    }

    ImGui::PopStyleColor(3);
    
    static ImVec2 keyboardPos;
    static bool keyboardPosInit = false;

    if (keyboardVisible)
    {
        ImVec2 vp = ImGui::GetMainViewport()->Pos;
        ImVec2 vpSize = ImGui::GetMainViewport()->Size;

        ImVec2 kbSize = ImVec2(availWidth, 490.0f);

        if (!keyboardPosInit)
        {
            keyboardPos = ImVec2(
                vp.x + 20,
                vp.y + vpSize.y - kbSize.y - 40   // keep it above bottom
            );
            keyboardPosInit = true;
        }

        ImGui::SetNextWindowPos(keyboardPos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(kbSize, ImGuiCond_Always);

        ImGui::Begin("Lyc4n's Keyboard (>w<)", &keyboardVisible,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings);

        keyboardPos = ImGui::GetWindowPos(); // save drag position

        ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));
        KeyboardWindow(searchQuery, true);

        ImGui::End();
    }
    
    ImGui::BeginChild("Gun Skin", ImVec2(-1, childHeight), true);                           
    if (!itemData.empty()) {
    
    std::string searchLower = ToLower(searchQuery); 

    for (const auto& getitem : itemData) {      
         
    std::string itemNameLower = ToLower(getitem.itemName); 

    if (!searchLower.empty() && itemNameLower.find(searchLower) == std::string::npos) {
    continue; 
    }
                
    ImGui::Checkbox(getitem.itemName.c_str(), &sBool[getitem.itemName]);       
       
    if (sBool[getitem.itemName]) {
    int baseWeaponID = getitem.WeaponConf[0];
    for (auto item : itemInventoryInstance) {
    if (!item) continue;
    item2Fields = (Item2InventoryFields*) ((uintptr_t) item + 0x20); //public int ItemID; 
    if (item2Fields->ItemID == getitem.Item2Inventory[0] || item2Fields->ItemID == getitem.Item2Inventory[3]) {
    item2Fields->WeaponAssetGroupID = getitem.Item2Inventory[1];
    item2Fields->WeaponIconID = getitem.Item2Inventory[2];                       
    }                
    }
            
    for (auto conf : weaponConfInstance) {
    if (!conf) continue;
    weaponconfFields = (WeaponConfFields*) ((uintptr_t) conf + 0x20); //public byte Accuracy;
                
    if (getitem.WeaponConf[0] == weaponconfFields->ID || getitem.Item2Inventory[3] == weaponconfFields->ID) {
    weaponconfFields->ColorID = getitem.WeaponConf[1];
    weaponconfFields->DefWeaponSkinID = getitem.WeaponConf[2];
    weaponconfFields->DefaultKillBrocast = getitem.WeaponConf[3];  
    weaponconfFields->WinnerCircleWeaponMountType = getitem.WeaponConf[4];
    } 
    }          
       
    if (getitem.WeaponExtra[4] > 0)
    activeKillEffects[baseWeaponID] = getitem.WeaponExtra[4];
    
    for (auto extra : weaponExtraInstance) {
    if (!extra) continue;
    weaponextraFields = (WeaponConfExtraFields*) ((uintptr_t) extra + 0x10); //public byte DefaultMythicArmor;
                      
    if (weaponextraFields->ID == getitem.WeaponExtra[2] && weaponextraFields->ID == getitem.WeaponConf[0]) {
                    
    if (getitem.itemName.find("[M]") != std::string::npos) {
    weaponextraFields->DefaultMythicArmor = getitem.WeaponExtra[1];
    weaponextraFields->DefaultMythicSig = getitem.WeaponExtra[2];               
    }
                    
    weaponextraFields->DefaultDeadReplayEffectId = getitem.WeaponExtra[3];
    weaponextraFields->DefaultKillEffectId = getitem.WeaponExtra[4];     
 
    }
    }

    for (auto asset : weaponAssetGroupInstance) {
    if (!asset) continue;
    weaponAssetFields = (WeaponAssetGroupFields*) ((uintptr_t) asset + 0x40); // public int FireEffectGroupID;
                                    
    if (weaponAssetFields->Id == getitem.WeaponAsset[0]) {
    if (getitem.WeaponAsset[1] / 10  == getitem.WeaponAsset[2] / 10 && getitem.WeaponAsset[1] > 0) {
    weaponAssetFields->FireEffectGroupID = getitem.WeaponAsset[1];
    }
    }
                
    for (auto itemResource : itemResourceConfigInstance) {
    if (!itemResource) continue;
    itemFields = (ItemResourceFields*) ((uintptr_t) itemResource + 0x10); //public int FxAssetID;
                               
    if (itemFields->ID == getitem.Item2Inventory[0]) {
                    
    itemFields->FxAssetID = getitem.ItemResInt[0];
    itemFields->InventoryModelID = getitem.ItemResInt[1];
    itemFields->ModelAssetIDRaw = getitem.ItemResInt[2];
                        
    itemFields->UIMiniSpriteName = getitem.ItemRes[0];
    itemFields->UISmallSpriteName = getitem.ItemRes[1];
    itemFields->UISpriteName = getitem.ItemRes[2];
    itemFields->UISquareSpriteName = getitem.ItemRes[3];
                                
    }                   
    }                        
            
    sBool[getitem.itemName] = false;
    }}}}             
    ImGui::EndChild();
    
    }
    
    /*
    if (tab == 5) {
    const float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.5f;
    const float rightPanelWidth = ImGui::GetContentRegionAvail().x - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;
    const float childHeight = ImGui::GetContentRegionAvail().y;

    // RGB Animation
    static float rgb_timer = 0.0f;
    rgb_timer += ImGui::GetIO().DeltaTime * 0.5f;
    ImVec4 rgb_color = ImVec4(
        (sin(rgb_timer) * 0.5f + 0.5f),
        (sin(rgb_timer + 2.0f) * 0.5f + 0.5f),
        (sin(rgb_timer + 4.0f) * 0.5f + 0.5f),
        1.0f
    );

    // Left Panel - Primary Weapons
    ImGui::BeginChild("Skins Nav##5", ImVec2(leftPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Primary Weapon Skins");
        ImGui::Spacing();
        
        ImGui::BeginGroup();
        CustomCheckbox("Memento Mori", &Config.ExtraMenu.Memento);
        CustomCheckbox("Morning Star", &Config.ExtraMenu.Morning);
        CustomCheckbox("Amoeba CBR4", &Config.ExtraMenu.Amoeba);
        CustomCheckbox("Timeland AKS", &Config.ExtraMenu.Aks);
        ImGui::EndGroup();
        
        ImGui::SameLine();
        
        ImGui::BeginGroup();
        CustomCheckbox("Glorious Blaze", &Config.ExtraMenu.Glorious);
        CustomCheckbox("Switchblade X9", &Config.ExtraMenu.Neon);
        CustomCheckbox("Celestial Ink", &Config.ExtraMenu.Celestial);
        CustomCheckbox("LongQuan", &Config.ExtraMenu.Quan);
        ImGui::EndGroup();
        
        CustomCheckbox("USS - Bam", &Config.ExtraMenu.Bam);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right Panel - Secondary Weapons and Characters
    ImGui::BeginChild("Skins Content##5", ImVec2(rightPanelWidth, childHeight), true);
    {
        ImGui::TextColored(rgb_color, "Secondary & Character Skins");
        ImGui::Spacing();
        
        ImGui::BeginGroup();
        CustomCheckbox("Kilo141 - Demon", &Config.ExtraMenu.Demon);
        CustomCheckbox("DLQ33 - Lotus", &Config.ExtraMenu.Lotus);
        CustomCheckbox("Jak-12 - Phoenix", &Config.ExtraMenu.Phoenix);
        CustomCheckbox("QQ9 - Waku Waku", &Config.ExtraMenu.Qq);
        ImGui::EndGroup();
        
        ImGui::SameLine();
        
        ImGui::BeginGroup();
        CustomCheckbox("Siren - Song", &Config.ExtraMenu.Song);
        CustomCheckbox("Ghost - Siege", &Config.ExtraMenu.Siege);
        CustomCheckbox("Templar - Oath", &Config.ExtraMenu.Templar);
        CustomCheckbox("Spectre - T3", &Config.ExtraMenu.Spectre);
        ImGui::EndGroup();
        
        CustomCheckbox("Nikto - Darkside", &Config.ExtraMenu.Darkside);
        
        // Coming Soon Section
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Coming Soon:");
        ImGui::BeginDisabled();
        CustomCheckbox("Mythic Skins Collection (Soon)", &Config.ExtraMenu.Qq);
        CustomCheckbox("Legendary Blueprints (Soon)", &Config.ExtraMenu.Qq);
        ImGui::EndDisabled();
    }
    ImGui::EndChild();
}
*/


/*if (tab == 7) {
    const float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.35f; // Slightly narrower left panel
    const float rightPanelWidth = ImGui::GetContentRegionAvail().x - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;
    const float childHeight = ImGui::GetContentRegionAvail().y;

    // Left Panel - Scheduled Events
    ImGui::BeginChild("Events Nav##6", ImVec2(leftPanelWidth, childHeight), true, ImGuiWindowFlags_NoScrollbar);
    {
        // Animated header
        static float hue_left = 0.0f;
        hue_left += ImGui::GetIO().DeltaTime * 0.25f;
        if (hue_left > 1.0f) hue_left = 0.0f;
        ImGui::TextColored(ImColor::HSV(hue_left, 0.8f, 0.9f), "Scheduled Events");
        ImGui::Separator();
        
        // Event cards with icons
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.7f, 1.0f, 0.3f));
        
        if (ImGui::CollapsingHeader("Lifetime Access", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::BulletText("All Frost Versions");
            ImGui::BulletText("Lifetime Members Only");
            ImGui::BulletText("For only 3500");
        }
        
        if (ImGui::CollapsingHeader("Giveaways")) {
            ImGui::BulletText("Join our discord");
            ImGui::BulletText("Join our telegram");
            ImGui::BulletText("Next Roll: N/A");
        }
        
        if (ImGui::CollapsingHeader("Promotions")) {
            ImGui::BulletText("30% Discount Weekend");
            ImGui::BulletText("Bundle Deals");
            ImGui::BulletText("Limited Time Only");
        }
        
        ImGui::PopStyleColor();
        
        // Countdown timer
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("Event refresh on: Febuary 12");
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right Panel - Updates and Information
    ImGui::BeginChild("Events Content##6", ImVec2(rightPanelWidth, childHeight), true);
    {
        // Rainbow animated header
        static float hue_right = 0.0f;
        hue_right += ImGui::GetIO().DeltaTime * 0.5f;
        if (hue_right > 1.0f) hue_right = 0.0f;
        ImGui::TextColored(ImColor::HSV(hue_right, 0.9f, 0.9f), "News & Updates");
        ImGui::Separator();
        
        // Latest Updates section
        ImGui::Spacing();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // Use bold font if available
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Latest Updates");
        ImGui::PopFont();
        
        ImGui::Indent(10.0f);
        ImGui::BulletText("Version 1.6.53 Released - Improved stability");
        ImGui::BulletText("New Customizable Theme added");
        ImGui::BulletText("Performance optimizations (30% faster)");
        ImGui::Unindent(10.0f);
        
        // Coming Updates section
        ImGui::Spacing();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImGui::TextColored(ImVec4(0.2f, 0.7f, 1.0f, 1.0f), "Coming Soon");
        ImGui::PopFont();
        
        ImGui::Indent(10.0f);
        ImGui::BulletText("New UI Overhaul (Preview available)");
        ImGui::BulletText("Additional Legendary Weapons");
        ImGui::BulletText("Custom Theme Support");
        ImGui::Unindent(10.0f);
        
        // Social buttons
        ImGui::Spacing();
        ImGui::Separator();
        
        float buttonHeight = 40.0f * ImGui::GetIO().FontGlobalScale;
        ImVec2 buttonSize(ImGui::GetContentRegionAvail().x * 0.5f - ImGui::GetStyle().ItemSpacing.x * 0.5f, buttonHeight);
        
        if (ImGui::Button(" Join Telegram", buttonSize)) {
            system("start https://telegram.me/ZeshiaOfficial");
        }
        ImGui::SameLine();
        if (ImGui::Button(" Discord Server", buttonSize)) {
            system("start https://discord.gg/");
        }
        
        // Status indicator
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.2f, 1.0f), "Online - %d users active", 376);
    }
    ImGui::EndChild();
}*/


if (tab == 6) {
    const float childHeight = ImGui::GetContentRegionAvail().y;
    
    // Reset cursor position for tab content
    ImGui::SetCursorPos(ImVec2(10, 10));

    ImGui::BeginChild("Settings Nav##7", ImVec2(-1, childHeight), true);
    {
        static float leftHue = 0.0f;
        leftHue += ImGui::GetIO().DeltaTime * 0.1f;
        if (leftHue > 1.0f) leftHue = 0.0f;
        ImGui::TextColored(ImColor::HSV(leftHue, 0.8f, 0.9f), "Configuration");
        ImGui::Separator();

        ImGui::Spacing();
        
        if (ImGui::CollapsingHeader("Theme Customization")) {
            static ImVec4 textColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
            static ImVec4 headerColor = ImGui::GetStyle().Colors[ImGuiCol_Header];
            static ImVec4 buttonColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            static ImVec4 frameBgColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
            static ImVec4 windowBgColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
            static ImVec4 childBgColor = ImGui::GetStyle().Colors[ImGuiCol_ChildBg];
            static ImVec4 borderColor = ImGui::GetStyle().Colors[ImGuiCol_Border];
            static ImVec4 sliderGrabColor = ImGui::GetStyle().Colors[ImGuiCol_SliderGrab];
            static ImVec4 checkMarkColor = ImGui::GetStyle().Colors[ImGuiCol_CheckMark];
            
            ImGui::ColorEdit4("Text", (float*)&textColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Header", (float*)&headerColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Button", (float*)&buttonColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Frame BG", (float*)&frameBgColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Window BG", (float*)&windowBgColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Child BG", (float*)&childBgColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Border", (float*)&borderColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Slider Grab", (float*)&sliderGrabColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Check Mark", (float*)&checkMarkColor, ImGuiColorEditFlags_NoInputs);
            
            if (ImGui::Button("Apply Theme", ImVec2(-1, 30))) {
                ImGuiStyle& style = ImGui::GetStyle();
                style.Colors[ImGuiCol_Text] = textColor;
                style.Colors[ImGuiCol_Header] = headerColor;
                style.Colors[ImGuiCol_Button] = buttonColor;
                style.Colors[ImGuiCol_FrameBg] = frameBgColor;
                style.Colors[ImGuiCol_WindowBg] = windowBgColor;
                style.Colors[ImGuiCol_ChildBg] = childBgColor;
                style.Colors[ImGuiCol_Border] = borderColor;
                style.Colors[ImGuiCol_SliderGrab] = sliderGrabColor;
                style.Colors[ImGuiCol_CheckMark] = checkMarkColor;
            }
            
            if (ImGui::Button("Reset to Default", ImVec2(-1, 30))) {
/*
    textColor = colors[ImGuiCol_Text];
    headerColor = colors[ImGuiCol_Header];
    buttonColor = colors[ImGuiCol_Button];
    frameBgColor = colors[ImGuiCol_FrameBg];
    windowBgColor = colors[ImGuiCol_WindowBg];
    childBgColor = colors[ImGuiCol_ChildBg];
    borderColor = colors[ImGuiCol_Border];
    sliderGrabColor = colors[ImGuiCol_SliderGrab];
    checkMarkColor = colors[ImGuiCol_CheckMark];
    */
}
            
            ImGui::Separator();
        }
        
        ImGui::TextColored(ImVec4(0.2f, 0.7f, 1.0f, 1.0f), "Config Management");
        if (ImGui::Button("Save Features", ImVec2(-1, 40))) {
            saveAllConfig();
        }
        if (ImGui::Button("Load Features", ImVec2(-1, 40))) {
            if (loadAllConfig()) {
                ImGui::OpenPopup("Config Loaded");
            } else {
                ImGui::OpenPopup("Config Error");
            }
        }
        
        ImGui::Spacing();
        ImGui::Separator();
		
		CustomCheckbox("VSync Mode", &Config.ExtraMenu.Grap);
		CustomCheckbox("High End Mode", &Config.ExtraMenu.HighFPS);
        
        ImGui::TextColored(ImColor::HSV(leftHue, 0.8f, 0.9f), "Performance");
        if (CustomCheckbox("Frame Smoothness", &Config.Aim.FpsLevel)) {
            Config.Aim.showFPSLevelSlider = Config.Aim.FpsLevel;
        }
        if (Config.Aim.showFPSLevelSlider) {
            CustomSliderFloat("Frame Level", &Config.Aim.FpsLevel1, 30, 120, "%.0f");
        }
		ImGui::Separator();
		if (CustomCheckbox("Ultra Frame Boost", &Config.Aim.FpsLevelUltra)) {
            Config.Aim.showFPSLevelUltraSlider = Config.Aim.FpsLevelUltra;
        }
        if (Config.Aim.showFPSLevelUltraSlider) {
            CustomSliderFloat("Ultra Frame Level", &Config.Aim.FpsLevelUltra1, 60, 144, "%.0f");
        }

		ImGui::Separator();

		// Floating Logo Settings
		ImGui::TextColored(ImColor::HSV(leftHue, 0.8f, 0.9f), "Floating Logo Settings");
		CustomCheckbox("Hide Logo", &Config.ExtraMenu.HideLogo);
		CustomCheckbox("Show FPS", &Config.ExtraMenu.ShowFPS);
		if (!Config.ExtraMenu.HideLogo) {
		    CustomSliderFloat("Logo Opacity", &Config.ExtraMenu.LogoOpacity, 0.1f, 1.0f, "%.2f");
		    CustomSliderFloat("Logo Size", &Config.ExtraMenu.LogoSize, 0.5f, 2.0f, "%.2f");
		}

		ImGui::Separator();
		
		//ImGui::BeginDisabled();
        //CustomCheckbox("Max Frames", &Config.ExtraMenu.Frame);
        //CustomCheckbox("Max Graphics", &Config.ExtraMenu.Fps);
		//ImGui::EndDisabled();

        bool safeFeaturesEnabled = Config.ExtraMenu.Recoil && Config.ExtraMenu.Shake && Config.ExtraMenu.WallHack;
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        if (safeFeaturesEnabled) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));
        }
        if (ImGui::Button(safeFeaturesEnabled ? "SAFE CONFIG [ACTIVE]" : "Safe Config [Inactive]", ImVec2(-1, 0))) {
            Config.ExtraMenu.Recoil = !safeFeaturesEnabled;
            Config.ExtraMenu.Shake = !safeFeaturesEnabled;
            Config.ExtraMenu.WallHack = !safeFeaturesEnabled;
        }
        if (safeFeaturesEnabled) {
            ImGui::PopStyleColor();
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Reduces detectable features for safer gameplay");
            }
        }

        bool brutalFeaturesEnabled = Config.Aim.AimAssist && Config.Aim.showAimAssistSlider && isExecute;
        if (brutalFeaturesEnabled) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
        }
        if (ImGui::Button(brutalFeaturesEnabled ? "BRUTAL CONFIG  [ACTIVE]" : "Brutal Config [Inactive]", ImVec2(-1, 0))) {
            Config.Aim.AimAssist = !brutalFeaturesEnabled;
            Config.Aim.showAimAssistSlider = !brutalFeaturesEnabled;
            Config.ExtraMenu.Recoil = !brutalFeaturesEnabled;
            Config.ExtraMenu.Reload = !brutalFeaturesEnabled;
            Config.ExtraMenu.Shake = !brutalFeaturesEnabled;
            Config.ExtraMenu.Hit = !brutalFeaturesEnabled;
            isExecute = !brutalFeaturesEnabled;
        }
        if (brutalFeaturesEnabled) {
            ImGui::PopStyleColor();
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Brutal features enabled - use with caution");
            }
        }
        ImGui::PopStyleVar();

        ImGui::Spacing();
        ImGui::Separator();
        
        
    }
    ImGui::EndChild();
    
    // Close MainContent child
    ImGui::EndChild();
    
    // Pop window padding that was added after Begin()
    ImGui::PopStyleVar();

}

}
  




            if (Config.ExtraMenu.Range) {
                Patches.range.Modify();
            }
            else
            {
                Patches.range.Restore();
            }

            if (Config.ExtraMenu.Kinetic) {
                Patches.kinetic.Modify();
            }
            else
            {
                Patches.kinetic.Restore();
            }

            if (Config.ExtraMenu.Outline) {
                Patches.outline.Modify();
            }
            else
            {
                Patches.outline.Restore();
            }

            if (Config.ExtraMenu.Fps) {
                Patches.fpss.Modify();
            }
            else
            {
                Patches.fpss.Restore();
            }
			
			if (Config.ExtraMenu.Frame) {
                Patches.frame.Modify();
            }
            else
            {
                Patches.frame.Restore();
            }

            if (Config.ExtraMenu.Rpd) {
                Patches.rpd.Modify();
            }
            else
            {
                Patches.rpd.Restore();
            }

            if (Config.ExtraMenu.Grapss) {
                Patches.grap.Modify();
            }
            else
            {
                Patches.grap.Restore();
            }

            if (Config.ExtraMenu.WallHack) {
                Patches.A1.Modify();
            }
            else
            {
                Patches.A1.Restore();
            }

            if (Config.ExtraMenu.Parachute) {
                Patches.chute.Modify();
            }
            else
            {
                Patches.chute.Restore();
            }


            if (Config.ExtraMenu.AutoSnipe) {
                Patches.autosnipe.Modify();
            }
            else
            {
                Patches.autosnipe.Restore();
            }
			
			if (Config.ExtraMenu.Revive) {
                Patches.revive.Modify();
            }
            else
            {
                Patches.revive.Restore();
            }

            if (Config.ExtraMenu.Diving) {
                Patches.wing.Modify();
            }
            else
            {
                Patches.wing.Restore();
            }
            if (Config.ExtraMenu.Tutorial) {
                Patches.tutorial.Modify();
            }
            else
            {
                Patches.tutorial.Restore();
            }
            if (Config.ExtraMenu.Cost) {
                Patches.cost.Modify();
            }
            else
            {
                Patches.cost.Restore();
            }
            if (Config.ExtraMenu.Red) {
                Patches.red.Modify();
            }
            else
            {
                Patches.red.Restore();
            }
            if (Config.ExtraMenu.JetS) {
                Patches.jets.Modify();
            }
            else
            {
                Patches.jets.Restore();
            }
            if (Config.ExtraMenu.JetJ) {
                Patches.jetj.Modify();
            }
            else
            {
                Patches.jetj.Restore();
            }
            if (Config.ExtraMenu.Fire) {
                Patches.fire.Modify();
            }
            else
            {
                Patches.fire.Restore();
            }
        }
    }

    //----------------------------------------------------------------------------------------------------------------------------------------------------


    auto Input_get_touchCount = (int (*)())(Class_Input__get_touchCount);
    if (Input_get_touchCount() > 0)
    {
        auto Input_GetTouch = (Touch(*)(uintptr_t, int))(Class_Input__GetTouch);
        auto Input_get_mousePosition = (Vector3(*)(uintptr_t))(Class_Input__get_mousePosition);
        switch (Input_GetTouch(Config.ImGuiMenu.thiz, 0).m_Phase)
        {
        case TouchPhase::Began:
        case TouchPhase::Stationary:
            io->MouseDown[0] = true;
            io->MousePos = ImVec2(Input_get_mousePosition(Config.ImGuiMenu.thiz).x,
                                  get_height() -
                                  Input_get_mousePosition(Config.ImGuiMenu.thiz).y);
            break;
        case TouchPhase::Ended:
        case TouchPhase::Canceled:
            io->MouseDown[0] = false;
            Config.ImGuiMenu.clearMousePos = true;
            break;
        case TouchPhase::Moved:
            io->MousePos = ImVec2(Input_get_mousePosition(Config.ImGuiMenu.thiz).x,
                                  get_height() -
                                  Input_get_mousePosition(Config.ImGuiMenu.thiz).y);
            break;
        default:
            break;
        }
    }

    if (isMenuVisible) {
        ImGui::PopStyleVar(4);
        ImGui::PopStyleColor(6);
    }
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Render floating info overlay
    RenderFloatingInfo(ImGui::GetBackgroundDrawList(), get_width(), get_height());
    
    return old_eglSwapBuffers(dpy, surface);
}


// FEATURE IMPLI HERE


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Init_Thread2() {
    
    // Force Crash Virtual Machines
    FILE* pipe = popen("ps -e", "r");
    char buffer[256];
    int lineCount = 0;
    while (fgets(buffer, sizeof(buffer), pipe)) {
        lineCount++;
    }
    pclose(pipe);
    int pidCount = lineCount - 1;
    if (pidCount > 1) {
        *(volatile int*)0 = 0;
    }
	
	*/
	
   
    
    
    
void Init_Thread2() {
    while (!m_Anogs) {
        m_Anogs = Tools::GetBaseAddress("libanogs.so");

        sleep(2);

    }
    LOGI("libanogs.so: %p", m_Anogs);
	
	// Bypass putted here, mate >w<
    // Dummy bypass, put your own xD
	MemoryPatch::createWithHex("libanogs.so",0xCPGETOUT,"00 00 80 D2 C0 03 5F D6").Modify();
	MemoryPatch::createWithHex("libanogs.so",0xCPGETOUT,"00 00 80 D2 C0 03 5F D6").Modify();
	MemoryPatch::createWithHex("libanogs.so",0xCPGETOUT,"00 00 80 D2 C0 03 5F D6").Modify();
	MemoryPatch::createWithHex("libanogs.so",0xCPGETOUT,"00 00 80 D2 C0 03 5F D6").Modify();
	MemoryPatch::createWithHex("libanogs.so",0xCPGETOUT,"00 00 80 D2 C0 03 5F D6").Modify();
	MemoryPatch::createWithHex("libanogs.so",0xCPGETOUT,"00 00 80 D2 C0 03 5F D6").Modify();
	MemoryPatch::createWithHex("libanogs.so",0xCPGETOUT,"00 00 80 D2 C0 03 5F D6").Modify();	


	
}


void Init_Thread()
{
    while (!m_unity)
    {
        m_unity = Tools::GetBaseAddress("libunity.so");

        sleep(1);
    }
    LOGI("libunity.so: %p", m_unity);;
    
// FEATURE OFFSETS HERE - HOOKLIB/DOBBY RECCOMMENDED   

// KILLEFFECT/SKINS HOOK
    
}

__attribute__((constructor))
void native_Init(JNIEnv *env, jclass clazz, jobject mContext) {
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    VM = vm;

    std::thread(Init_Thread).detach();
    std::thread(Init_Thread2).detach();

    

    return JNI_VERSION_1_6;
}

