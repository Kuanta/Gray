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
    GrLight *ambient = createAmbientLight(glm::vec3(0.4, 0.4, 0.4), 0.05);
    GrLight *directional = createDirectionalLight(glm::vec3(0.3, 0.2, 0.4), 0.05f, glm::vec3(-0.5f, -1.0f, -1.0f));
    directional->setPosition(9.9, 25, -3.79f);
    directional->castShadow = true;
    directional->farPlane = 10000.0f;
    directional->staticLight = false;

    GrLight *directional2 = createDirectionalLight(glm::vec3(0.3, 0.2, 0.4), 0.1f, glm::vec3(-0.5f, -1.0f, -1.0f));
    directional2->setPosition(-100, 25, -3.79f);
    directional2->castShadow = true;
    directional2->farPlane = 10000.0f;
    directional2->staticLight = false;

    Model *model = new Model();
    cout<<"Starting to load"<<endl;
    Object* demoScene = model->loadModel(gm, "./assets/demoScene2.fbx");
    demoScene->setScale(glm::vec3(0.01,0.01,0.01));
    demoScene->setPosition(glm::vec3(0,0,0));
    demoScene->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    scene->add(demoScene);
    scene->add(ambient);
    scene->add(directional);
    scene->add(directional2);

    //Point lights
    vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(17,6,-22));
    lightPositions.push_back(glm::vec3(4,6,22));
    lightPositions.push_back(glm::vec3(-9.5,6,-22));
    lightPositions.push_back(glm::vec3(-5.5,6,-22));
    lightPositions.push_back(glm::vec3(-15,6,-22));
    lightPositions.push_back(glm::vec3(-20, 6, -9));
    lightPositions.push_back(glm::vec3(-22,6,8));
    lightPositions.push_back(glm::vec3(-22,6,23));
    lightPositions.push_back(glm::vec3(-5, 6, 24));
    lightPositions.push_back(glm::vec3(-17, 6, 25));
    lightPositions.push_back(glm::vec3(4.69, 6, -3));

    for (int i = 0; i < lightPositions.size(); i++)
    {
        GrLight *light = createPointLight(glm::vec3(0.5, 0.6f, 0.3), 0.8f, lightPositions.at(i), 1.0f, 0.07f * 0.001, 0.07f * 0.01);
        light->farPlane = 500.0f;
        light->castShadow = true;
        light->isStatic = true;
        light->attenuationFactor = 1.8;
        scene->add(light);
    }

    //Add some boxes
    BoxGeometry *bgClone = new BoxGeometry(3, 8, 3);
    Material *matClone = new Material(glm::vec3(0.4, 0.2, 0.4), 0, false, 0.1);
    for(int i=0;i<10;i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Object* boxClone = new Object();
            boxClone->gm = gm;
            GrMesh* meshClone = new GrMesh(bgClone, matClone);
            meshClone->addToObject(boxClone);
            boxClone->setPosition(glm::vec3(i * 10 - 150, 2, j * 10 - 0));
            boxClone->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
            scene->add(boxClone);
            if(i%3 == 0 && j%3 == 0)
            {
                cout<<"Creating a light "<<i<<" "<<j<<endl;
                GrLight *lightClone = createPointLight(glm::vec3(0.8, 0.57, 0.6), 0.4f, glm::vec3(i * 10 - 150 + 5, 4, j * 10 +5), 1.0f, 0.07f * 0.001, 0.07f * 0.01);
                lightClone->farPlane = 500.0f;
                lightClone->castShadow = true;
                lightClone->isStatic = true;
                lightClone->attenuationFactor = 0.5;
                scene->add(lightClone);
            }
        }
    }

    //Add a plane under the boxes
    Object *plane = new Object();
    Plane *pg = new Plane(10.0f, 10.0f, 3.5f);
    Material *mat_plane = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true, 1);
    mat_plane->setTexture("assets/wood/wood.png", DIFFUSE_TEXTURE);
    mat_plane->setTexture("assets/wood/woodNormal.png", NORMAL_TEXTURE);
    mat_plane->shininess = 32.0f;
    mat_plane->rougness = 0.1f;
    mat_plane->metalness = true;
    GrMesh *plane_mesh = new GrMesh(pg, mat_plane);
    plane->setRotation(glm::vec3(-90, 0, 0));
    plane->setPosition(glm::vec3(-100, 0, 50));
    plane->setScale(10, 10, 10);
    plane->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    plane->addComponent(plane_mesh);
    scene->add(plane);

    GrLight *followLight = createPointLight(glm::vec3(0.4, 0.2, 0.5), 0.5f, glm::vec3(0, 5, 0), 1.0f, 0.014f * 0.001, 0.07f * 0.01);
    followLight->farPlane = 50.0f;
    followLight->castShadow = true;
    followLight->staticLight = false;
    followLight->attenuationFactor = 0.5f;
    scene->add(followLight);

    //Follow light box
    BoxGeometry* bg = new BoxGeometry(1,1,1);
    Material* mat = new Material(glm::vec3(0.9,0.9,0.9), 0, false, 0.1);
    Object* box = new Object();
    box->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    GrMesh *mesh = new GrMesh(bg, mat);
    mesh->addToObject(box);
    box->gm = gm;
    box->castShadow = false;
    box->receiveShadow = false;
    box->setPosition(glm::vec3(0,0,0));
    scene->add(box);
    //Ybot
    Model* modelBot = new Model();
    ybot = modelBot->loadModel(gm, "./assets/ybot.fbx");
    modelBot->importAnimations("./assets/torchIdle.fbx", "torchIdle");
    modelBot->importAnimations("./assets/torchWalk.fbx", "torchWalk");
    modelBot->importAnimations("./assets/torchWalkBack.fbx", "torchWalkBack");
    ybot->setPosition(glm::vec3(5,0,3));
    ybot->setScale(glm::vec3(0.04,0.04,0.04));
    ybot->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    ybot->name = "Ybot";
    animMan = (GrAnimManager *) ybot->getComponentByType(ComponentType::ANIMATION_MANAGER);
    idle = animMan->getAnimation("torchIdle");
    walk = animMan->getAnimation("torchWalk");
    walkBack = animMan->getAnimation("torchWalkBack");

    animMan->addToActive(idle, 1.0f, true);
    animMan->addToActive(walk, 0.0f, true);
    animMan->addToActive(walkBack, 0.0f, true);

    animMan->addToActive(animMan->getAnimation("torchWalk"), 0.0f, true);
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

    // scene->add(box);
    // scene->add(plane);
    scene->add(skybox);
    OrbitCamera *oc = new OrbitCamera(&(scene->camera));
    scene->camera.controls = oc;
    scene->camera.setTarget(glm::vec3(0, 0, -10));

    bool first;
    //Walk dynamics
    while (!glfwWindowShouldClose(gm->window))
    {
        float deltaTime = glfwGetTime();
        followLight->setPosition(followLight->getPosition() + glm::vec3(moveRate*deltaTime*0.01, 0, turnRate*deltaTime*0.01));
        box->setPosition(followLight->getPosition());
        followLight->requiresUpdate = true;
    
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
        walk->weight = 1.0f;
        idle->weight = 0.0f;
        walkBack->weight = 0.0f;
        animMan->startTransition(0.5f);
        moveRate = 1;
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        walk->weight = 0.0f;
        idle->weight = 1.0f;
        walkBack->weight = 0.0f;
        animMan->startTransition(0.5f);
        moveRate = 0;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS && moveRate == 0)
    {
        walk->weight = 0.0f;
        idle->weight = 0.0f;
        walkBack->weight = 1.0f;
        animMan->startTransition(0.5f);
        moveRate = -1;
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        walk->weight = 0.0f;
        idle->weight = 1.0f;
        walkBack->weight = 0.0f;
        animMan->startTransition(0.5f);
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
