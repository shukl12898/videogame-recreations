#include "UIComponent.h"
#include <string>

class HUD : public UIComponent
{
public:
	HUD(class Actor* owner);
	~HUD();
	void Draw(class Shader* shader) override;
	void ShowSubtitle(std::string text);

private:
	class Font* mFont = nullptr;
	class Texture* mSubtitle = nullptr;
	class Texture* mShadow = nullptr;
};