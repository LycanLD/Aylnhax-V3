#pragma once

class Camera {
	public:
	static Camera *get_main() {
		return reinterpret_cast<Camera *(__fastcall *)()>(Class_Camera__get_main)();
	}
};

Vector3 WorldToScreenPoint(Vector3 position) {
	return reinterpret_cast<Vector3 (__fastcall *)(Camera *, Vector3)>(Class_Camera__WorldToScreenPoint)(Camera::get_main(), position);
}


class Transform {
	public:
	Vector3 get_position() {
		return reinterpret_cast<Vector3(__fastcall *)(Transform *)>(Class_Transform__get_position)(this);
	}
	
};


class Component {
	public:
	Transform *get_transform() {
		return reinterpret_cast<Transform *(__fastcall *)(Component *)>(Class_Component__get_transform)(this);
	}
	
};

class Weapon {
public:

    int get_WeaponID() {
        using GetWeaponIDFunc = int (*)(Weapon*);
        static GetWeaponIDFunc _Weapon__get_WeaponID = (GetWeaponIDFunc)(Weapon_GetWeaponId);
        return _Weapon__get_WeaponID ? _Weapon__get_WeaponID(this) : 0;
    }
    
}; 


class Pawn {
public:

    Weapon* get_CurrentWeapon() {
        using GetCurrentWeaponFunc = Weapon* (*)(Pawn*);
        static GetCurrentWeaponFunc _Pawn__get_CurrentWeapon = (GetCurrentWeaponFunc)(Pawn_get_CurrentWeapon);
        return _Pawn__get_CurrentWeapon ? _Pawn__get_CurrentWeapon(this) : nullptr;
    }
    
};

class GamePlay {
public:
    static Pawn* get_LocalPawn() {
        using GetLocalPawnFunc = Pawn*(*)();
        static GetLocalPawnFunc _GamePlay__get_LocalPawn = (GetLocalPawnFunc)(Class_Gameplay__get_LocalPawn);
        return _GamePlay__get_LocalPawn ? _GamePlay__get_LocalPawn() : nullptr;
    }

};


int get_width() {
	return reinterpret_cast<int(__fastcall *)()>(Class_Screen__get_width)();
}

int get_height() {
	return reinterpret_cast<int(__fastcall *)()>(Class_Screen__get_height)();
}

float get_density() {
	return reinterpret_cast<float(__fastcall *)()>(Class_Screen__get_density)();
}
