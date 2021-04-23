#include "test.h"

void run()
{
	GameManager* gm = new GameManager(1920.0f, 1080.0f);
	scene = new Scene(gm);
	scene->keyInputCallback = keyInput;
	scene->cursorEnterCallback = cursor_enter_callback;
	scene->cursorPositionCallback = cursor_position_callback;
	scene->mouseButtonCallback = mouse_button_callback;
	scene->scrollCallback = scroll_callback;
	gm->changeScene(scene);
	GrLight* ambient = createAmbientLight(glm::vec3(0.8, 0.6, 0.1), 0.3);
	GrLight* point = createPointLight(glm::vec3(1.0, 1.0,0.9), 0.9, glm::vec3(2, 10, 30), 1.0f, 0.014f * 0.001, 0.07f * 0.01);
	GrLight* directional = createDirectionalLight(glm::vec3(0.5, 0.5,0.5), 0.1f, glm::vec3(0.5f,-1.0f,1.0f));
	directional->castShadow = 1;
	directional->setPosition(-10.0f, 50.0f,-20.0f);
	scene->add(ambient);
	scene->add(point);
	scene->add(directional);

	//Arthas

	Model* model = new Model();
	player = model->loadModel(gm, "./assets/arthas/arthas.fbx");
	player->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	model->importAnimations("./assets/arthas/arthas_Walk_1.fbx");
	model->importAnimations("./assets/arthas/arthas_Stand_0.fbx");
	model->importAnimations("./assets/arthas/arthas_Attack1H_82.fbx"); //"Attack1H [82]
	model->importAnimations("./assets/arthas/arthas_EmoteKiss_49.fbx");

	animMan = (GrAnimManager*)player->getComponentByType(ComponentType::ANIMATION_MANAGER);

	stand = animMan->getAnimation("Stand [0]");
	walk = animMan->getAnimation("Walk [1]");
	//animMan->addToActive("EmoteKiss [49]", 1.0f, true);
	animMan->addToActive(stand, 1.0f, true);
	animMan->addToActive(walk, 0.0f, true);

	if (player != nullptr)
	{
		player->setScale(glm::vec3(0.25, 0.25, 0.25));
		player->setRotationY(-90);
		player->setPositionY(0);
		player->setPositionZ(10);
		scene->em.addElement(player);
		player->gm = gm;
	}
	//TODO: If this code doesn't slow down things, then it means that we have solved skeletal animation optimization issue
	// for(int r=0;r<10;r++)
	// {
	// 	for(int i=0; i<5;i++)
	// 	{
	// 		Object* clone = player->clone();
	// 		clone->setPosition(r*25*cos(i*3.14*30/180), 3, r*30*sin(i*3.14*30/180));
	// 		clone->setScale(0.2f, 0.2f, 0.2f);
	// 		clone->setRotationY(-90);
	// 		clone->gm = this->gm;
	// 		this->add(clone);
	// 	}
	// }

	//Brick
	Object* box = new Object();
	BoxGeometry* bg = new BoxGeometry(3.0f, 7.0f, 3.0f);
	Material* mat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true,1);
	mat->rougness = 100000.0f;
	mat->metalness = true;
	mat->setTexture("assets/brick.png", DIFFUSE_TEXTURE);
	GrMesh* mesh = new GrMesh(bg, mat);
	box->name = string("Brick");
	box->gm = gm;
	mesh->material->gm = gm;
	box->setPosition(-5, 0, 20);
	mesh->addToObject(box);
	box->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	//Plane
	Object* plane = new Object();
	Plane* pg = new Plane(10.0f, 10.0f, 3.5f);
	Material* mat_plane = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true, 1);
	mat_plane->setTexture("./assets/wood.png", DIFFUSE_TEXTURE);
	mat_plane->shininess = 32.0f;
	mat_plane->rougness = 0.1f;
	mat_plane->metalness = true;
	GrMesh* plane_mesh = new GrMesh(pg, mat_plane);
	plane->setRotation(glm::vec3(-90,0,0));
	plane->setPosition(glm::vec3(0, 0, 20));
	plane->setScale(5,5,5);
	plane->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	plane->addComponent(plane_mesh);

	//Sphere
	// Model* modelSphere = new Model();
	// Object* sphere = modelSphere->loadModel(gm, "./assets/sphere.fbx");
	// sphere->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	// sphere->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
	// sphere->setPositionZ(15.0f);
	// sphere->setPositionX(7.0f);
	// sphere->setPositionY(15.0f);
	// scene->em.addElement(sphere);
	// sphere->gm = gm;

	// //Blender box
	// Model* modelBox = new Model();
	// Object* boxBlend = modelBox->loadModel(gm, "./assets/box.fbx");
	// boxBlend->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	// boxBlend->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	// boxBlend->setPositionZ(15.0f);
	// boxBlend->setPositionX(-7.0f);
	// boxBlend->setPositionY(15.0f);
	// scene->em.addElement(boxBlend);
	// boxBlend->gm = gm;

	//Skybox
	Object* skybox = new Object();
	//back, bottom, front, left, right
	vector<string> filepaths = {"./assets/Skyboxes/skybox/right.jpg", 
	"./assets/Skyboxes/skybox/left.jpg",
	"./assets/Skyboxes/skybox/top.jpg",
	"./assets/Skyboxes/skybox/bottom.jpg",
	"./assets/Skyboxes/skybox/front.jpg",
	"./assets/Skyboxes/skybox/back.jpg"};
	Skybox* skyboxComp = new Skybox(gm,filepaths);
	skybox->addComponent(skyboxComp);

	scene->add(box);
	scene->add(plane);
	scene->add(skybox);
	OrbitCamera* oc = new OrbitCamera(&(scene->camera));
	scene->camera.controls = oc;
	scene->camera.setTarget(glm::vec3(0, 0, -10));

	while (!glfwWindowShouldClose(gm->window))
	{
		point->setPositionX(20*cos(glfwGetTime() * 0.4));
		gm->update();
	}
}
	


void keyInput(int key, int scancode, int action, int mods)
{
	if (scene->camera.controls != nullptr)
	{
		scene->camera.controls->keyInput(key, scancode, action, mods);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		walk->weight = 1.0f;
		stand->weight = 0.0f;
		animMan->startTransition(0.1f);
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		walk->weight = 0.0f;
		stand->weight = 1.0f;
		animMan->startTransition(0.1f);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animMan->addToActive("Attack1H [82]",1.0f, false);
		animMan->startTransition(0.1f);
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		animMan->addToActive("EmoteKiss [49]", 100.0f, false);
		animMan->startTransition(0.1f);
	}
}

void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (scene->camera.controls != nullptr)
	{
		scene->camera.controls->cursor_position_callback(window, xpos, ypos);
	}
}

void cursor_enter_callback(GLFWwindow * window, int entered)
{
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (scene->camera.controls != nullptr)
	{
		scene->camera.controls->mouse_button_callback(window, button, action, mods);
	}
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	if (scene->camera.controls != nullptr)
	{
		scene->camera.controls->scroll_callback(window, xoffset, yoffset);
	}
}
