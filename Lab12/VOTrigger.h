#include "Actor.h"
#include <string>

class VOTrigger : public Actor
{
public:
	VOTrigger(class Game* game);
	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState, Uint32 mouseButtons,
						const Vector2& relativeMouse) override;
	void SetDoorName(std::string name) { mDoorName = name; }
	void SetNextLevel(std::string level) { mNextLevel = level; }
	void SetSounds(std::vector<std::string> sounds) { mSounds = sounds; }
	void SetSubtitles(std::vector<std::string> subtitles) { mSubtitles = subtitles; }
	bool PlayNextSound();

private:
	std::string mDoorName;
	std::string mNextLevel;
	int mCurrentSound = 0;
	std::vector<std::string> mSounds;
	std::vector<std::string> mSubtitles;
	class CollisionComponent* mCollisionComponent = nullptr;
	bool mActivated = false;
	class AudioSystem* mAudio = nullptr;
	int mSoundHandle = 0;
	bool mLastFrameF = false;
};