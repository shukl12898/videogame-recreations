#include "Actor.h"
#include <unordered_map>

class Frog : public Actor
{
public:
	Frog(class Game* game);
	void OnProcessInput(const Uint8* keyState) override;

protected:
	class SpriteComponent* mSpriteComponent;
	std::unordered_map<int, bool> mLastKeyFrame;
	class Game* mGame;
};