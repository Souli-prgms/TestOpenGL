#include "Renderer.h"

Renderer::Renderer()
{
	this->initWindow();
	
	m_testShader = new Shader("res/shaders/Basic.shader");

	m_sphere = new Sphere();
	m_sphere->init();
	m_sphere->setTransformationMatrix(m_sphere->getTransformationMatrix() * Eigen::Translation3f(0.0f, 1.5f, 0.0f));
	
	//m_test = new Mesh();
	//m_test->load("res/models/sportsCar.obj");
	//m_test->init();
	
	this->initGround();	
	this->initCamera();
	this->initLights();
	this->initShadows();
}


Renderer::~Renderer()
{
	glfwTerminate();
}


void Renderer::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		this->updateScene();
	}
	glfwDestroyWindow(m_window);
}


void Renderer::updateScene()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glCullFace(GL_FRONT);
	this->renderShadow();
	//glCullFace(GL_BACK);

	this->renderShape(m_sphere, m_testShader);
	this->renderGround(m_groundShader);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

Eigen::Matrix3f Renderer::normalMatrixForShape(Shape* shape)
{
	Eigen::Matrix3f N = (m_cam->computeViewMatrix().matrix() * shape->getTransformationMatrix().matrix()).topLeftCorner<3, 3>();
	return (N.inverse()).transpose();
}

void Renderer::renderShape(Shape* shape, Shader* shader)
{
	shader->bind();
	this->passLights(shader);
	shader->setMat4("projection_matrix", m_cam->computeProjectionMatrix());
	shader->setMat4("light_space_matrix", m_lightSpaceMatrix);
	shader->setMat4("view_matrix", m_cam->computeViewMatrix());
	shader->setMat4("model_matrix", shape->getTransformationMatrix().matrix());
	shader->setMat3("normal_matrix", this->normalMatrixForShape(shape));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	shader->setInt("shadow_map", 2);
	shape->display(shader);
	shader->unbind();
}

void Renderer::renderGround(Shader* shader)
{
	shader->bind();
	this->passLights(shader);
	shader->setInt("ground_texture", 0);
	shader->setInt("normal_map", 1);
	shader->setInt("shadow_map", 2);
	shader->setMat4("light_space_matrix", m_lightSpaceMatrix);
	shader->setMat4("projection_matrix", m_cam->computeProjectionMatrix());
	shader->setMat4("view_matrix", m_cam->computeViewMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_groundTexture->getId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_groundNormals->getId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);

	for (int i = 0; i < m_ground.size(); i++)
	{
		shader->setMat4("model_matrix", m_ground[i]->getTransformationMatrix().matrix());
		shader->setMat3("normal_matrix", this->normalMatrixForShape(m_ground[i]));
		m_ground[i]->display(shader);
	}

	shader->unbind();
}

void Renderer::renderShadow()
{
	m_shadowShader->bind();
	m_shadowShader->setMat4("light_space_matrix", m_lightSpaceMatrix);
	
	glViewport(0, 0, m_shadowWidth, m_shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shadowShader->setMat4("model_matrix", m_sphere->getTransformationMatrix().matrix());
	m_sphere->display(m_shadowShader);

	for (int i = 0; i < m_ground.size(); i++)
	{
		m_shadowShader->setMat4("model_matrix", m_ground[i]->getTransformationMatrix().matrix());
		m_ground[i]->display(m_shadowShader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_shadowShader->unbind();

	glViewport(0, 0, 640, 480);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::initGround()
{
	m_groundShader = new Shader("res/shaders/Ground.shader");
	m_groundTexture = new Texture("res/textures/PavingStones36_col.jpg");
	m_groundNormals = new Texture("res/textures/PavingStones36_nrm.jpg");
	int nb = 0;
	for (int i = -2; i < 3; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			m_ground.push_back(new Ground());
			m_ground[nb]->init();
			m_ground[nb]->setWorldPos(i, j);
			nb++;
		}
	}
}

void Renderer::initLights()
{
	m_dirLight = new DirectionalLight;
	m_pointLight = new PointLight;
}

void Renderer::initShadows()
{
	m_shadowShader = new Shader("res/shaders/Shadow.shader");
	m_shadowHeight = 1024;
	m_shadowWidth = 1024;

	glGenFramebuffers(1, &m_depthMapFBO);

	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Eigen::Matrix4f lightProjection, lightView;
	lightProjection = perspectiveProjection(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	std::cout << lightProjection << std::endl;
	//lightProjection = m_cam->computeProjectionMatrix();
	lightView = m_cam->lookAt(m_pointLight->position, Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 1.0f, 0.0f));
	std::cout << lightView << std::endl;
	m_lightSpaceMatrix = lightProjection * lightView;
}

void Renderer::passLights(Shader* shader)
{
	shader->setVec4("directional_light.direction", m_cam->computeViewMatrix() * Eigen::Vector4f(m_dirLight->direction(0), m_dirLight->direction(1), m_dirLight->direction(2), 1.0f));
	shader->setVec3("directional_light.ambient", m_dirLight->ambient);
	shader->setVec3("directional_light.diffuse", m_dirLight->diffuse);
	shader->setVec3("directional_light.specular", m_dirLight->specular);

	shader->setVec4("point_light.position", m_cam->computeViewMatrix() * Eigen::Vector4f(m_pointLight->position(0), m_pointLight->position(1), m_pointLight->position(2), 1.0f));
	shader->setVec3("point_light.ambient", m_pointLight->ambient);
	shader->setVec3("point_light.diffuse", m_pointLight->diffuse);
	shader->setVec3("point_light.specular", m_pointLight->specular);
	shader->setFloat("point_light.constant", m_pointLight->constant);
	shader->setFloat("point_light.linear", m_pointLight->linear);
	shader->setFloat("point_light.quadratic", m_pointLight->quadratic);
}

/************************* EVENTS *************************/
void Renderer::charPressed(int key)
{
	
}

void Renderer::keyPressed(int key, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_UP:
		//m_test->setTransformationMatrix(m_test->getTransformationMatrix() * Eigen::Translation3f(0.0f, 0.0f, 0.1f));
		break;

	case GLFW_KEY_DOWN:
		//m_test->setTransformationMatrix(m_test->getTransformationMatrix() * Eigen::Translation3f(0.0f, 0.0f, -0.1f));
		break;
	
	default:
		break;
	}

}


void Renderer::mouseScrolled(double x, double y)
{
	m_cam->zoom((y > 0) ? 1.1 : 1. / 1.1);
}


void Renderer::mousePressed(GLFWwindow* window, int button, int action)
{
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_cam->startRotation(m_lastMousePos);
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			m_cam->startTranslation(m_lastMousePos);
		}
		m_button = button;
	}
	else if (action == GLFW_RELEASE) {
		if (m_button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_cam->endRotation();
		}
		else if (m_button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			m_cam->endTranslation();
		}
		m_button = -1;
	}
}


void Renderer::mouseMoved(double x, double y)
{
	if (m_button == GLFW_MOUSE_BUTTON_LEFT)
	{
		m_cam->dragRotate(Eigen::Vector2f(x, y));
	}
	else if (m_button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		m_cam->dragTranslate(Eigen::Vector2f(x, y));
	}
	m_lastMousePos = Eigen::Vector2f(x, y);
}


void Renderer::reshapeWindow(int width, int height)
{

}

/************************* CAMERA INITIALIZATION *************************/
void Renderer::initCamera()
{
	m_cam = new Camera();
	m_cam->setSceneCenter(m_ground[12]->boundingBox().center());
	m_cam->setSceneRadius(m_ground[12]->boundingBox().sizes().maxCoeff());
	m_cam->setSceneDistance(m_cam->sceneRadius() * 3.f);
	m_cam->setMinNear(0.1f);
	m_cam->setNearFarOffsets(-m_cam->sceneRadius() * 100.f, m_cam->sceneRadius() * 100.f);
	m_cam->setScreenViewport(Eigen::AlignedBox2f(Eigen::Vector2f(0.0, 0.0), Eigen::Vector2f(640, 480)));
	
	glViewport(0, 0, 640, 480);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}


/************************* WINDOW INITIALIZATION *************************/
void Renderer::setCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);

	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int key)
	{
		Renderer* viewer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		viewer->charPressed(key);
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* viewer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		viewer->keyPressed(key, action, mods);
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x, double y)
	{
		Renderer* viewer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		viewer->mouseScrolled(x, y);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		Renderer* viewer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		viewer->mousePressed(window, button, action);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
	{
		Renderer* viewer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		viewer->mouseMoved(x, y);
	});

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		Renderer* viewer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		viewer->reshapeWindow(width, height);
	});
}

void Renderer::initWindow()
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	m_window = glfwCreateWindow(640, 480, "Soulard", NULL, NULL);

	if (!m_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window);
	this->setCallbacks();

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
}
