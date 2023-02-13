#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (!mAnims[mAnimName].empty())
	{
		mAnimTimer += mAnimFPS * deltaTime;

		while (mAnimTimer >= mAnims[mAnimName].size())
		{
			mAnimTimer -= mAnims.size();
		}

		SetTexture(mAnims[mAnimName].at(static_cast<unsigned>(std::floor(mAnimTimer))));
	}
}

void AnimatedSprite::AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
