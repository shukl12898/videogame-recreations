#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	void Update(float deltaTime) override;
	Vector3 Ideal();
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; };
	float GetPitchSpeed() const { return mPitchSpeed; };
	float GetPitchAngle() const { return mPitchAngle; };
	void ResetPitchAngle() { mPitchAngle = 0; };
	void SnapToIdeal();
	static const int HDIST = 60;
	static const int VDIST = 70;
	static const int TARGET_DIST = 50;
	const float SPRING = 256.0f;
	const float DAMP = 2.0 * sqrt(SPRING);

private:
	Vector3 mCameraPos;
	Vector3 mCameraVelocity;
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;
};