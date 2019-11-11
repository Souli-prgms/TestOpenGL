#pragma once

#include "Config.h"

Eigen::Matrix4f orthographicProjection(float l, float r, float b, float t, float n, float f);
Eigen::Matrix4f perspectiveProjection(float l, float r, float b, float t, float n, float f);

class Camera
{
public:
	enum State {
		Idle, Rotating, Translating
	};

	Camera();
	~Camera();

	const Eigen::Vector3f& sceneCenter() const;
	const Eigen::Quaternionf& sceneOrientation() const;
	const Eigen::AlignedBox2f& screenViewport() const;
	Eigen::Matrix4f computeViewMatrix() const;
	Eigen::Matrix4f computeProjectionMatrix() const;
	Eigen::Matrix4f computeOrthoType() const;

	void setSceneCenter(const Eigen::Vector3f& scnCenter);
	void setSceneDistance(float scnDistance);
	void setSceneRadius(float scnRadius);
	void setSceneOrientation(const Eigen::Quaternionf& scnOrientation);
	void setScreenViewport(const Eigen::AlignedBox2f& scnViewport);
	void setMinNear(float minNear);
	void setNearFarOffsets(float nearOffset, float farOffset);

	State state() const;
	bool isIdle() const;

	void rotate(const Eigen::Quaternionf& rot);
	bool isRotating() const;
	void startRotation(const Eigen::Vector2f& scrPos);
	void dragRotate(const Eigen::Vector2f& scrPos);
	void cancelRotation();
	void endRotation();

	void translate(const Eigen::Vector2f& scnVec);
	void translate(const Eigen::Vector3f& scnVec);
	bool isTranslating() const;
	void startTranslation(const Eigen::Vector2f& scrPos);
	void dragTranslate(const Eigen::Vector2f& scrPos);
	void cancelTranslation();
	void endTranslation();

	void zoom(float factor);
	void grow(float factor);
	void dollyZoom(float factor);

	float sceneDistance() const;
	float sceneRadius() const;

	float minNear() const;

	float nearOffset() const;
	float farOffset() const;

	float minScreenViewportSize() const;
	bool isOrthographic() const;
	bool isPerspective() const;

	static Eigen::Matrix4f perspective(float fovy, float aspect, float zNear, float zFar);
	static Eigen::Matrix4f lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up);

private:
	Eigen::Vector2f normFromScr(const Eigen::Vector2f& scrPos) const;

	Eigen::Quaternionf computeRotation(const Eigen::Vector2f& scrPos) const;
	Eigen::Vector3f computeTranslation(const Eigen::Vector2f& scrPos) const;

	Eigen::Vector3f m_scnCenter;
	float m_scnDistance;
	float m_scnRadius;
	Eigen::Quaternionf m_scnOrientation;
	Eigen::AlignedBox2f m_scrViewport;
	float m_minNear;
	float m_nearOffset;
	float m_farOffset;

	State m_state;
	Eigen::Vector2f m_scrMouseInit;
	Eigen::Quaternionf m_scnOrientInit;
	Eigen::Vector3f m_scnCenterInit;
};


