#pragma once

uintptr_t m_Il2Cpp;
uintptr_t m_Anogs;
void *m_EGL = nullptr;
int32_t sWidth = 0;
int32_t sHeight = 0;
void *BaseWorld_Instance = 0;

bool SlideTackle = false;
float SlideRange;
bool SlideShow;
bool SnowBshow;
bool isExecute;
bool RedWallhack;
bool showSpeedHackSlider;

enum TouchPhase {
    Began = 0,
    Moved = 1,
    Stationary = 2,
    Ended = 3,
    Canceled = 4,
};

enum TouchType {
    Direct = 0,
    Indirect = 1,
    Stylus = 2,
};

struct Touch {
    int m_FingerId;
    Vector2 m_Position;
    Vector2 m_RawPosition;
    Vector2 m_PositionDelta;
    float m_TimeDelta;
    int m_TapCount;
    TouchPhase m_Phase;
    TouchType m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_RadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};

enum LineTarget {
    Top = 0,
    Center = 1,
    Bottom = 2
};

enum CrosshairTarget {
    Circle = 0,
    Plus = 1,
    Ekis = 2
};

enum ChamsTarget {
     none = 0,
     enableWallhack = 1,
     enableWallhackW = 2,
     enableWallhackG = 3,
     enableWallhackO = 4
};

enum EAim {
  Distance = 0,
  Crosshair = 1
};

enum EAimTarget {
    Heads = 0,
    Chests = 1,
    Body = 2
};

enum EAimTrigger {
    None = 0,
    Shooting = 1,
    Scoping = 2
};

enum EAimRecoil {
    None1 = 0,
    Zero2 = 1,
    Less3 = 2
};

enum EAimReload {
    None4 = 0,
    Zero5 = 1,
    Quick6 = 2
};

enum EAimScope {
    None7 = 0,
    Zero8 = 1,
    Quick9 = 2
};

enum EAimSwitch {
    None10 = 0,
    Zero11 = 1,
    Quick12 = 2
};

enum ESkinCharacter {
    Character = 0,
    Character1 = 1,
    Character2 = 2,
    Character3 = 3,
    Character4 = 4,
    Character5 = 5,
    Character6 = 6,
    Character7 = 7,
    Character8 = 8,
    Character9 = 9,
    Character10 = 10,
    Character11 = 11,
    Character12 = 12,
    Character13 = 13
};

enum EAntenna {
    Red = 0,
    Pink = 1,
    Blue = 2,
    Purple = 3,
    Yellow = 4,
};

enum EAntenna1 {
    Cyan = 0,
    Black = 1,
    White = 2,
    Green = 3,
    Orange = 4,
};

enum ELight {
    Red1 = 0,
    Pink1 = 1,
    Blue1 = 2,
    Cyan1 = 3,
    Green1 = 4,
};
enum EBulletTrack {
    Bt = 0,
    Bt1 = 1,
    Bt2 = 2,
};

struct sConfig {
    struct sInitImGui {
        bool g_Initialized;
        bool clearMousePos = true;
        uintptr_t thiz;
    };
    sInitImGui ImGuiMenu{0};
    
    struct sWeaponAim {
        bool Aimbot360;
        bool AimLock;
        bool AimAssist;
        bool Jump;
        bool Slide;
        bool Infinite;

        // Frame Rate Controls
        bool FpsLevel;
        float FpsLevel1;
        bool showFPSLevelSlider;

        bool FpsLevelUltra;              // New: toggle for Ultra FPS
        float FpsLevelUltra1 = 240.0f;   // Default Ultra FPS value
        bool showFPSLevelUltraSlider;    // New: show Ultra FPS slider

        // Aim & Movement Settings
        bool showAimAssistSlider;
        bool showHighJumpSlider;
        bool showSlideSpeedSlider;
        float AimAssistSize;
        float Jumpsize;
        float Infinitesize;

        // Silent Aim
        bool AimSilent;
        bool AimSilentTools;
        bool AimSilentSettings;

        // Targeting & Triggering
        EAimTarget Target;
        EAimTrigger Trigger;
        EAim By;
        float size;

        // Bullet Track
        bool BulletTrack;
        bool BulletTrackSettings;
        EBulletTrack Options;
    };
    sWeaponAim Aim{0};
    
    struct sESPMenuLineScale {
        bool ScaleLine;
        bool showLineSlider;
        float lineSize;
    };
    sESPMenuLineScale sESPMenuLineScale{0};
    
    struct sESPMenuNameScale {
        bool ScaleName;
        bool showNameSlider;
        float NameSize;
    };
    sESPMenuNameScale sESPMenuNameScale{0};
    
    struct sESPMenuDistanceScale {
        bool ScaleDistance;
        bool showDistanceSlider;
        float DistanceSize;
    };
    sESPMenuDistanceScale sESPMenuDistanceScale{0};
    
    struct sMAINMenu {
        bool LOBBY;
    };
    sMAINMenu sMAINMenu{0};
    
    struct sESPMenu {
        bool Esp;
        bool Alert;
        bool Count;
        bool Name;
        bool isPlayerLine;
        LineTarget Target;
        ChamsTarget Type;
        CrosshairTarget CrosshairType;
        bool Box;
        bool Health;
        bool Distance;
        bool Skeleton;
        bool Vehicles;
        bool Crosshair;
        bool Aimline;
    };
    sESPMenu ESPMenu{0};
    
    struct sColorsESPPLAYER {
        float *LinePLAYER;
        float *BoxPLAYER;
        float *NamePLAYER;
        float *HealthPLAYER;
        float *DistancePLAYER;
        float *SkeletonPLAYER;
    };
    sColorsESPPLAYER sColorsESPPLAYER{0};
    
    struct sColorsESPBOT {
        float *LineBOT;
        float *BoxBOT;
        float *NameBOT;
        float *HealthBOT;
        float *DistanceBOT;
        float *SkeletonBOT;
    };
    sColorsESPBOT sColorsESPBOT{0};
    
    struct sColorsESPOTHERS {
        float *PovOTHERS;
    };
    sColorsESPOTHERS sColorsESPOTHERS{0};
    
    struct sExtraMenu {
        bool Infinite;
        bool Spread;
        bool Spread1;
        bool ClearDisplay;
        bool Cost;
        bool Fire;
        bool Diving;
        bool Bullet;
        bool Blacksky;
        bool Test;
        bool Red;
        bool Pink;
        bool Blue;
        bool Purple;
        bool Yellow;
        bool Cyan;
        bool Black;
        bool White;
        bool Green;
        bool Orange;
        bool Fps;
        bool Grap;
        bool Frame;
        bool Tutorial;
        bool Recoil;
        bool Reload;
        bool Reload1;
        bool Shake;
        bool Scope;
        bool Scope1;
        bool Switch;
        bool Switch1;
        bool Small;
        bool Over;
        bool Flash;
        bool Hit;
        bool Auto;
        bool Wallhack;
        bool Rpd;
        bool Grapss;
        bool Radar1;
        bool Radar2;
        bool Parachute;
        bool Slide;
        bool Speed;
        bool Fpss;
        bool NoSpread;
        bool NoSmoke;
        bool RapidFire;
        bool FastSwitch;
        bool NoParachute;
        bool RedLight;
        bool GravityFall;
        bool Hitbox;
        bool WallHack;
        bool DarkMode;
        bool SpectatorTag;
        bool WeaponRange;
        bool NoOverheat;
        bool MaxFramerate;
        bool LockFPS;
        bool Smoke;
        bool Memento;
        bool Radiance;
        bool Neon;
        bool Glorious;
        bool Morning;
        bool Amoeba;
        bool Celestial;
        bool Bam;
        bool Demon;
        bool Lotus;
        bool Phoenix;
        bool Qq;
        bool Spectre;
        bool Templar;
        bool Darkside;
        bool Song;
        bool Siege;
        bool JetS;
        bool JetJ;
        bool Spectate;
        bool Outline;
        bool Range;
        bool Kinetic;
        bool AutoSnipe;
        bool Aks;
        bool Quan;
        bool HighFPS; // Added Ultra Frame Rate toggle
		bool HideLogo; // Hide logo when menu is collapsed
		float LogoOpacity = 1.0f; // Logo opacity (0.0 - 1.0)
		float LogoSize = 1.0f; // Logo size multiplier (0.5 - 2.0)
		bool ShowFPS = true; // Show FPS on floating logo
		float LogoPosX = 10.0f; // Saved logo X position
		float LogoPosY = 10.0f; // Saved logo Y position
		bool Revive;
		bool Medicine;
    };
    sExtraMenu ExtraMenu;
    
    struct sExtraMenu1 {
        bool Red1;
        bool Pink1;
        bool Blue1;
        bool Cyan1;
        bool Green1;
        bool Jump;
        bool showJumpSlider;
        float jumpHeight;
        bool Swim;
        bool showSwimSlider;
        float swimSpeed;
        bool Slide;
        bool showSlideSlider;
        float slideSpeed;
        bool Flash;
        bool showFlashSlider;
        float movementSpeed;
        bool Crouch;
        bool showCrouchSlider;
        float crouchSpeed;
        bool Prone;
        bool showProneSlider;
        float proneSpeed;
    };
    sExtraMenu1 ExtraMenu1{0};
    
    struct sExtraMenu2 {
        bool Skin1;
        bool Skin2;
        bool Skin3;
        bool Skin4;
        bool Skin5;
        bool Skin6;
        bool Skin7;
        bool Skin8;
        bool Skin9;
        bool Skin10;
        bool Skin11;
        bool Skin12;
        bool Skin13;
        bool Skin14;
        bool Skin15;
        bool Skin16;
        bool Skin17;
        bool Skin18;
        bool Skin19;
        bool Skin20;
        
        ESkinCharacter Operators;
    };
    sExtraMenu2 ExtraMenu2{0};
};

sConfig Config{0};

void (*orig_SetUltraFrameRateDeviceInfo)(void* instance, bool enableUltraFrameRate, int ultraFrameRate, int ultraFrameRateBR, int ultraFrameRateQualityLimit, bool customizedFrameRate);
void hooked_SetUltraFrameRateDeviceInfo(void* instance, bool enableUltraFrameRate, int ultraFrameRate, int ultraFrameRateBR, int ultraFrameRateQualityLimit, bool customizedFrameRate) {
    if (instance != NULL) {
        if (Config.Aim.FpsLevel1) {  // Using the config option we added earlier
            enableUltraFrameRate = true;
            ultraFrameRate = (int)Config.Aim.FpsLevelUltra1; // Using the configurable value
            ultraFrameRateBR = 144;
            ultraFrameRateQualityLimit = 240;
            customizedFrameRate = true;
        }
    }
    orig_SetUltraFrameRateDeviceInfo(instance, enableUltraFrameRate, ultraFrameRate, ultraFrameRateBR, ultraFrameRateQualityLimit, customizedFrameRate);
}

