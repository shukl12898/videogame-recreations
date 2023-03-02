#include "Effect.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "AudioSystem.h"

Effect::Effect(Game* game, Vector2 position, std::string animName, std::string soundName)
: Actor(game)
{
	mGame = game;
	mPosition = position;
	AnimatedSprite* animatedSprite = new AnimatedSprite(this);
	animatedSprite->LoadAnimations("Assets/Effects");
	animatedSprite->SetAnimation(animName);
	mLifetime = animatedSprite->GetAnimDuration(animName);
	mGame->GetAudioSystem()->PlaySound(soundName);
}

void Effect::OnUpdate(float deltaTime)
{
	mLifetime -= deltaTime;
	if (mLifetime <= 0)
	{
		SetState(ActorState::Destroy);
	}
}
