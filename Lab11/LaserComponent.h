#include "MeshComponent.h"
#include "SegmentCast.h"
#include <vector>

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner);
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	void SetIgnoreActor(class Actor* ignore) { mIgnoreActor = ignore; }

private:
	Matrix4 LaserTransform(LineSegment segment);
	std::vector<LineSegment> mLineSegments;
	Actor* mIgnoreActor;
};