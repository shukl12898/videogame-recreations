#include "UIComponent.h"
#include "HUD.h"
#include "UIComponent.h"
#include "Shader.h"
#include "Font.h"
#include "Texture.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "HealthComponent.h"

HUD::HUD(Actor* owner)
: UIComponent(owner)
{
	mFont = new Font();
	mFont->Load("Assets/Inconsolata-Regular.ttf");
	mDamage =
		mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Textures/UI/DamageIndicator.png");
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

	if (mDamageTime > 0)
	{
		DrawTexture(shader, mDamage, Vector2::Zero, 1.0f, mDamageAngle);
	}

	if (mOwner->GetComponent<HealthComponent>()->IsDead())
	{
		DrawTexture(shader, mOwner->GetGame()->GetRenderer()->GetTexture(
								"Assets/Textures/UI/DamageOverlay.png"));
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

void HUD::PlayerTakesDamage(float angle)
{
	mDamageAngle = angle;
	mDamageTime = 1.5f;
	mIsDamaged = true;
}

void HUD::Update(float deltaTime)
{
	if (mIsDamaged)
	{
		mDamageTime -= deltaTime;
	}
	if (mDamageTime <= 0)
	{
		mIsDamaged = false;
	}
}