#include "demoScene.h"

void run()
{
    GameManager *gm = new GameManager(1920.0f, 1080.0f);
    scene = new Scene(gm);
    scene->keyInputCallback = keyInput;
    scene->cursorEnterCallback = cursor_enter_callback;
    scene->cursorPositionCallback = cursor_position_callback;
    scene->mouseButtonCallback = mouse_button_callback;
    scene->scrollCallback = scroll_callback;
    gm->changeScene(scene);
    GrLight *ambient = createAmbientLight(glm::vec3(0.4, 0.4, 0.4), 0.5);
    GrLight *directional = createDirectionalLight(glm::vec3(0.3, 0.2, 0.4), 0.05f, glm::vec3(-0.5f, -1.0f, -1.0f));
    directional->setPosition(9.9, 25, -3.79f);
    directional->castShadow = false;
    directional->farPlane = 10000.0f;
    directional->staticLight = false;
    scene->add(ambient);
    scene->add(directional);
    ModelLoader ml;
    // gm->assetMan.loadAsset("./assets/paladin.fbx", "Ybot");
    // gm->assetMan.loadAnimation("./assets/torchIdle.fbx", "Ybot", "torchIdle");
    // gm->assetMan.loadAnimation("./assets/torchWalk.fbx", "Ybot", "torchWalk");
    // gm->assetMan.loadAnimation("./assets/torchWalkBack.fbx", "Ybot", "torchWalkBack");

    ybot = ml.loadModel("./assets/ybot2.fbx");
    gm->assetMan.addAsset("Ybot", ybot);
    gm->assetMan.loadAnimation("./assets/torchIdle.fbx", "Ybot", "torchIdle");
    gm->assetMan.loadAnimation("./assets/torchWalk.fbx", "Ybot", "torchWalk");
    gm->assetMan.loadAnimation("./assets/torchWalkBack.fbx", "Ybot", "torchWalkBack");

    ybot = gm->assetMan.cloneAsset("Ybot");
    // ybot->setPosition(glm::vec3(5,0,3));
    ybot->setScale(glm::vec3(0.1, 0.1, 0.1));
    // ybot->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    ybot->name = "Ybot";
    animMan = (GrAnimManager *)ybot->getComponentByType(ComponentType::ANIMATION_MANAGER);
    idle = animMan->getAnimation("torchIdle");
    walk = animMan->getAnimation("torchWalk");
    walkBack = animMan->getAnimation("torchWalkBack");

    animMan->addToActive(idle, 1.0f, true);
    animMan->addToActive(walk, 0.0f, true);
    animMan->addToActive(walkBack, 0.0f, true);
    ybot->isStatic = false;
    ybot->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    scene->add(ybot);

    //Skybox
    Object *skybox = new Object();
    //back, bottom, front, left, right
    vector<string> filepaths = {"./assets/Skyboxes/skybox/right.jpg",
                                "./assets/Skyboxes/skybox/left.jpg",
                                "./assets/Skyboxes/skybox/top.jpg",
                                "./assets/Skyboxes/skybox/bottom.jpg",
                                "./assets/Skyboxes/skybox/front.jpg",
                                "./assets/Skyboxes/skybox/back.jpg"};
    Skybox *skyboxComp = new Skybox(gm, filepaths);
    skybox->addComponent(skyboxComp);
    scene->add(skybox);

    OrbitCamera *oc = new OrbitCamera(&(scene->camera));
    scene->camera.controls = oc;
    scene->camera.setTarget(glm::vec3(0, 0, -10));

    //Walk dynamics
    while (!glfwWindowShouldClose(gm->window))
    {
        float deltaTime = glfwGetTime();
        //ybot->setPosition(ybot->getPosition() + glm::vec3(moveRate*deltaTime*0.05f, 0.0f, 0.0f));
        gm->update();
    }
}

void keyInput(int key, int scancode, int action, int mods)
{
    if (scene->camera.controls != nullptr)
    {
        scene->camera.controls->keyInput(key, scancode, action, mods);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS && moveRate == 0)
    {
        animMan->setActiveAnimation(walk);
        animMan->startTransition(0.2f);
        moveRate = 1;
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        animMan->setActiveAnimation(idle);
        animMan->startTransition(0.2f);
        moveRate = 0;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS && moveRate == 0)
    {
        animMan->setActiveAnimation(walkBack);
        animMan->startTransition(0.2f);
        moveRate = -1;
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        animMan->setActiveAnimation(idle);
        animMan->startTransition(0.2f);
        moveRate = 0;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS && turnRate == 0)
    {
        turnRate = 1;
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE) 
    {
       turnRate = 0;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS && turnRate == 0)
    {
        turnRate = -1;
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        turnRate = 0;
    }

}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (scene->camera.controls != nullptr)
    {
        scene->camera.controls->cursor_position_callback(window, xpos, ypos);
    }
}

void cursor_enter_callback(GLFWwindow *window, int entered)
{
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (scene->camera.controls != nullptr)
    {
        scene->camera.controls->mouse_button_callback(window, button, action, mods);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (scene->camera.controls != nullptr)
    {
        scene->camera.controls->scroll_callback(window, xoffset, yoffset);
    }
}
