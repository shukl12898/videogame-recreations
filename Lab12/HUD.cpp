#include "UIComponent.h"
#include "HUD.h"
#include "UIComponent.h"
#include "Shader.h"
#include "Font.h"
#include "Texture.h"

HUD::HUD(Actor* owner)
: UIComponent(owner)
{
	mFont = new Font();
	mFont->Load("Assets/Inconsolata-Regular.ttf");
}

HUD::~HUD()
{
	mFont->Unload();
	delete mFont;
}

void HUD::Draw(Shader* shader)
{
	if (mSubtitle != nullptr)
	{
		float yPos = -325.0f + mSubtitle->GetHeight() / 2.0f;
		Vector2 position(0.0f, yPos);
		Vector2 offset(2.0f, -2.0f);
		DrawTexture(shader, mShadow, position + offset);
		DrawTexture(shader, mSubtitle, position);
	}
}

void HUD::ShowSubtitle(std::string text)
{
	if (mSubtitle != nullptr)
	{
		mSubtitle->Unload();
		delete mSubtitle;
		mSubtitle = nullptr;
		mShadow->Unload();
		delete mShadow;
		mShadow = nullptr;
	}

	if (!text.empty())
	{
		text = "GLaDOS: " + text;
		mSubtitle = mFont->RenderText(text, Color::LightGreen);
		mShadow = mFont->RenderText(text, Color::Black);
	}
}