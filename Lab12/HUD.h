#include "UIComponent.h"
#include <string>

class HUD : public UIComponent
{
public:
	HUD(class Actor* owner);
	~HUD();
	void Draw(class Shader* shader) override;
	void Update(float deltaTime) override;
	void PlayerTakesDamage(float angle);
	void ShowSubtitle(std::string text);

private:
	class Font* mFont = nullptr;
	class Texture* mSubtitle = nullptr;
	class Texture* mShadow = nullptr;
	class Texture* mDamage = nullptr;
	float mDamageTime = 0.0f;
	float mDamageAngle = 0.0f;
	bool mIsDamaged = false;
};