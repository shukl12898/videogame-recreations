#include "MeshComponent.h"
#include "SegmentCast.h"
#include <vector>

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner);
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	void Disable() { mDisabled = true; }
	void SetIgnoreActor(class Actor* ignore) { mIgnoreActor = ignore; }
	class Actor* GetLastHitActor() { return mLastHitActor; }

private:
	Matrix4 LaserTransform(LineSegment segment);
	std::vector<LineSegment> mLineSegments;
	class Actor* mIgnoreActor = nullptr;
	class Actor* mLastHitActor = nullptr;
	bool mDisabled = false;
};