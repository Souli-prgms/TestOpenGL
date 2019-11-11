#pragma once

#include "Config.h"
#include "Shader.h"
#include "Camera.h"
#include "Sphere.h"
#include "Cube.h"
#include "Mesh.h"
#include "Ground.h"
#include "Texture.h"
#include "Light.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	/* MAIN API */
	void run();
	
private:
	/* UI */
	GLFWwindow* m_window;
	Camera* m_cam;

	/* TEST MESH */
	Mesh* m_test;
	Shader* m_testShader;
	Sphere* m_sphere;
	
	/* GROUND */
	Shader* m_groundShader;
	std::vector<Ground*> m_ground;
	Texture* m_groundTexture;
	Texture* m_groundNormals;

	/* LIGHTS */
	DirectionalLight* m_dirLight;
	PointLight* m_pointLight;

	/* SHADOWS */
	Shader* m_shadowShader;
	unsigned int m_depthMap;
	unsigned int m_depthMapFBO;
	unsigned int m_shadowWidth;
	unsigned int m_shadowHeight;
	Eigen::Matrix4f m_lightSpaceMatrix;

	/* INITIALIZATIONS */
	void initCamera();
	void initWindow();
	void initGround();
	void initLights();
	void initShadows();
	void setCallbacks();

	/* RENDERING */
	void updateScene();
	void renderShape(Shape* shape, Shader* shader);
	void renderGround(Shader* shader);
	void renderShadow();
	void passLights(Shader* shader);

	/* EVENTS */
	void charPressed(int key);
	void keyPressed(int key, int action, int mods);
	void mouseScrolled(double x, double y);
	void mousePressed(GLFWwindow* window, int button, int action);
	void mouseMoved(double x, double y);
	void reshapeWindow(int width, int height);

	/* EVENTS ATTRIBUTES*/
	Eigen::Vector2f m_lastMousePos;
	int m_button = -1;

	/* UTILS */
	Eigen::Matrix3f normalMatrixForShape(Shape* shape);
};
