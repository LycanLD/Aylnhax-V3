#pragma once

#define ARGB(a, r, g, b) 0 | a << 24 | r << 16 | g << 8 | b

#define WHITE               ARGB(255, 255, 255, 255)
#define RED                 ARGB(255, 0, 0, 255)
#define GREEN               ARGB(255, 000, 128, 000)
#define LIME                ARGB(255, 000, 255, 000)
#define BLUE                ARGB(255, 000, 000, 255)
#define BLACK               ARGB(255, 000, 000, 000)
#define PURPLE              ARGB(255, 125, 000, 255)
#define GREY                ARGB(255, 128, 128, 128)
#define YELLOW              ARGB(255, 255, 0, 255)
#define ORANGE              ARGB(255, 255, 125, 000)
#define DARK_GREEN          ARGB(255, 000, 100, 000)
#define PINK                ARGB(255, 255, 192, 203)
#define BROWN               ARGB(255, 210, 105, 30)
#define CYAN                ARGB(255, 000, 255, 255)

float Rainbow() {
	static float x = 0, y = 0;
	static float r = 0, g = 0, b = 0;
	if (y >= 0.0f && y < 255.0f ) {
		r = 255.0f;
		g = 0.0f;
		b = x;
	} else if (y >= 255.0f && y < 510.0f ) {
		r = 255.0f - x;
		g = 0.0f;
		b = 255.0f;
	} else if (y >= 510.0f && y < 765.0f ) {
		r = 0.0f;
		g = x;
		b = 255.0f;
	} else if (y >= 765.0f && y < 1020.0f ) {
		r = 0.0f;
		g = 255.0f;
		b = 255.0f - x;
	} else if (y >= 1020.0f && y < 1275.0f ) {
		r = x;
		g = 255.0f;
		b = 0.0f;
	} else if (y >= 1275.0f && y < 1530.0f ) {
		r = 255.0f;
		g = 255.0f - x;
		b = 0.0f;
	}
	x+= 0.25f; 
	if (x >= 255.0f )
		x = 0.0f;
	y+= 0.25f;
	if (y > 1530.0f )
		y = 0.0f;
	return ARGB(255, (int)r, (int)g, (int)b);
}

class ESP {
	public:
	
	void drawLine(ImU32 col, float thickness, Vector2 start, Vector2 end) {
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), col, thickness);
	}
	void drawLine2(Vector3 start, Vector3 end,ImU32 col, float thickness) {
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), col, thickness);
	}
	void drawText(const char *text, float X, float Y, float size, long color) {
		ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(X, Y), color, text);
	}
	
	static void drawFilledCircle(float X, float Y, float radius, long color) {
		ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(X, Y), radius, color, 0);
	}
	void drawBoundingBox(float stroke, Rect rect, ImU32 color) {
        drawLine(color,stroke,Vector2(rect.x, rect.y),Vector2(rect.x + rect.width, rect.y));
        drawLine(color,stroke,Vector2(rect.x + rect.width, rect.y),Vector2(rect.x + rect.width, rect.y + rect.height));
        drawLine(color,stroke,Vector2(rect.x + rect.width,rect.y + rect.height),Vector2(rect.x, rect.y + rect.height));
        drawLine(color,stroke,Vector2(rect.x, rect.y + rect.height), Vector2(rect.x, rect.y));
}

void drawVerticalHealthBar(Vector2 screenPos, float height, float maxHealth, float currentHealth) {
        screenPos += Vector2(8.0f, 0.0f);
        drawBoundingBox(2,Rect(screenPos.x, screenPos.y, 5.0f, height + 2), ARGB(200, 0, 0, 0));
        screenPos += Vector2(1.0f, 1.0f);
        int clr = ARGB(255, 0, 255, 0);
        float barHeight = (currentHealth * height) / maxHealth;
        if (currentHealth <= (maxHealth * 0.6)) {
            clr = ARGB(255, 255, 0, 255);
        }
        if (currentHealth < (maxHealth * 0.3)) {
            clr = ARGB(255, 255, 0, 0);
        }
        drawBoundingBox(2, Rect(screenPos.x, screenPos.y, 2.0f, barHeight), clr);
}
	
	void drawFilledBox(ImU32 color, float rounding, Rect rect) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(rect.x, rect.y), ImVec2(rect.x + rect.width, rect.y + rect.height), color, rounding);
	}
	
	
	
};
