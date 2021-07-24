#include "demoScene.h"
#include "GrAnimGroup.h"

void run()
{
    GameManager *gm = new GameManager(1920, 1080);
    scene = new Scene(gm);
    scene->keyInputCallback = keyInput;
    scene->cursorEnterCallback = cursor_enter_callback;
    scene->cursorPositionCallback = cursor_position_callback;
    scene->mouseButtonCallback = mouse_button_callback;
    scene->scrollCallback = scroll_callback;
    gm->changeScene(scene);
    GrLight *ambient = createAmbientLight(glm::vec3(0.9, 0.9, 0.9), 0.1);
    GrLight *directional = createDirectionalLight(glm::vec3(0.6, 0.6, 0.8), 2.0f, glm::vec3(-0.5f, -1.0f, -1.0f));
    directional->setPosition(10, 100, 25.0);
    glm::vec3 direction = glm::vec3(-5.0f, 0, -5.0f) - directional->getPosition();
    direction = glm::normalize(direction);
    directional->direction = direction;
    directional->castShadow = false;
    directional->farPlane = 1000.0f;
    directional->softShadowFactor = 2;
    directional->staticLight = false;
    scene->add(ambient);
    scene->add(directional);

    //Add some point lights
    vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(-25.0f, 15.0f, 5.0f));
    lightPositions.push_back(glm::vec3(-40.0f, 15.0f, 42.0f));
    lightPositions.push_back(glm::vec3(-40.0f, 15.0f, -42.0f));
    lightPositions.push_back(glm::vec3(-60.0f, 15.0f, -42.0f));
    lightPositions.push_back(glm::vec3(-60.0f, 15.0f, 42.0f));
    lightPositions.push_back(glm::vec3(-80.0f, 15.0f, -42.0f));
    lightPositions.push_back(glm::vec3(-80.0f, 15.0f, 42.0f));
    lightPositions.push_back(glm::vec3(-100.0f, 15.0f, -42.0f));
    lightPositions.push_back(glm::vec3(-100.0f, 15.0f, 42.0f));

    for(int i=0;i<lightPositions.size();++i)
    {
        GrLight *point = createPointLight(glm::vec3(0.9, 0.9, 0.9), 10.0f, lightPositions.at(i), 0,0,0);
        point->attenuationFactor = 10.0f;
        point->farPlane = 50.0f;
        point->castShadow = true;
        point->staticLight = false;
        point->softShadowFactor = 2;
        scene->add(point);
    }
    

    gm->assetMan.loadAsset("./assets/ybot.fbx", "Ybot");
    gm->assetMan.loadAnimation("./assets/idle.fbx", "Ybot", "idle");
    gm->assetMan.loadAnimation("./assets/walk.fbx", "Ybot", "walk");
    gm->assetMan.loadAnimation("./assets/walkBack.fbx", "Ybot", "walkBack");
    gm->assetMan.loadAnimation("./assets/leftStrafe.fbx", "Ybot", "leftStrafe");
    gm->assetMan.loadAnimation("./assets/rightStrafe.fbx", "Ybot", "rightStrafe");

    Object* _ybot = gm->assetMan.cloneAsset("Ybot");
    _ybot->name = "Ybot";
    _ybot->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    ybot = new Object();
    ybot->add(_ybot);
    //ybot = gm->assetMan.cloneAsset("Ybot");
    ybot->setPosition(glm::vec3(0,0,-20));
    ybot->setScale(glm::vec3(0.1, 0.1, 0.1));
    // ybot->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    ybot->name = "YbotRoot";
    animMan = (GrAnimManager *)(_ybot)->getComponentByType(ComponentType::ANIMATION_MANAGER);
    idle = animMan->getAnimation("idle");
    walk = animMan->getAnimation("walk");
    walkBack = animMan->getAnimation("walkBack");
    leftStrafe = animMan->getAnimation("leftStrafe");
    rightStrafe = animMan->getAnimation("rightStrafe");
    GrAnimationGroup* movementAnims = new GrAnimationGroup("MoveGroup");
    movementAnims->addAnimation(walk, 0.0f);
    movementAnims->addAnimation(walkBack, 0.0f);
    movementAnims->addAnimation(idle, 1.0f);
    movementAnims->addAnimation(leftStrafe, 0.0f);
    movementAnims->addAnimation(rightStrafe, 0.0f);
    animMan->setActiveAnimation(movementAnims);

    ybot->isStatic = false;
    ybot->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    scene->add(ybot);

    gm->assetMan.loadAsset("./assets/scene.glb", "Floor");
    Object* floor = gm->assetMan.cloneAsset("Floor");
    floor->castShadow = true;
    floor->isStatic = true;
    floor->setPosition(glm::vec3(0,0,0));
    floor->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    floor->setScale(glm::vec3(2,2,2));
    scene->add(floor);

    //Box
    Object *box = new Object();
    box->name = "box";
    BoxGeometry* bg = new BoxGeometry(3,3,3);
    Material* boxMat = new Material();
    boxMat->diffuse = glm::vec3(0.8, 0.3, 0.6);
    GrMesh* boxMesh = new GrMesh(bg, boxMat);
    box->addComponent(boxMesh);
    scene->add(box);
    box->setShader(gm->getShader(SHADER_TYPE::GBUFFER_SHADER));
    box->setPosition(5, 1.5, 10);
    box->isStatic = true;
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
    skybox->isStatic = true;
    scene->add(skybox);

    OrbitCamera *oc = new OrbitCamera(&(scene->camera));
    scene->camera.controls = oc;
    scene->camera.setTarget(glm::vec3(0, 0, -10));

    //Walk dynamics
    while (!glfwWindowShouldClose(gm->window))
    {
        float denum = sqrt(moveRate * moveRate + strafeRate*strafeRate);
        if(denum > 0)
        {
            float _moveRate = moveRate / denum;
            float _strafeRate = strafeRate / denum;
            glm::vec3 forward = ybot->getForward();
            glm::vec3 right = ybot->getRight();
            float moveLength = (_moveRate * gm->deltaTime * 5.0f);
            float strafeLength = (_strafeRate * gm->deltaTime * 5.0f);
            ybot->setPosition(ybot->getPosition() + forward * moveLength + right * strafeLength);
        }
        if(turnRate != 0)
        {
            glm::vec3 currRot = ybot->getRotation();
            currRot.y = currRot.y + gm->deltaTime * 50.0f * turnRate;
            ybot->setRotation(currRot);
        }
        gm->update();
    }
}

void keyInput(int key, int scancode, int action, int mods)
{
    if (scene->camera.controls != nullptr)
    {
        scene->camera.controls->keyInput(key, scancode, action, mods);
    }
    if(key==GLFW_KEY_ENTER && action ==GLFW_PRESS)
    {
        cout<<glm::to_string(ybot->getPosition())<<endl;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS && moveRate == 0)
    {
        if (moveRate != 0)
        {
            return;
        }
        idle->weight = 0.0f;
        walk->weight = 1.0f;
        animMan->startTransition(0.1f);
        moveRate = 1;
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        animMan->activeAnimation->resetAnimation();
        walk->weight = 0.0f;
        if (fabs(strafeRate) == 0)
        {
            idle->weight = 1.0f;
            animMan->activeAnimation->resetAnimation();
        }
        animMan->startTransition(0.1f);
        moveRate = 0;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS && moveRate == 0)
    {
        if(moveRate != 0)
        {
            return;
        }
        idle->weight = 0.0f;
        walkBack->weight = 1.0f;
        moveRate = -1;
        animMan->startTransition(0.4f);
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        walkBack->weight = 0.0f;
        if(fabs(strafeRate) == 0)
        {
            idle->weight = 1.0f;
            animMan->activeAnimation->resetAnimation();
        }
        animMan->startTransition(0.4f);
        moveRate = 0;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS && strafeRate == 0)
    {
        if (strafeRate != 0)
        {
            return;
        }
        idle->weight = 0.0f;
        if (moveRate < 0)
        {
            leftStrafe->weight = 1.0f;
            leftStrafe->direction = -1;
        }
        else
        {
            rightStrafe->weight = 1.0f;
            rightStrafe->direction = 1;
        }
        animMan->startTransition(0.2f);
        strafeRate = -1;
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE) 
    {
        rightStrafe->weight = 0.0f;
        leftStrafe->weight = 0.0f;
        if(fabs(moveRate) == 0)
        {
            idle->weight = 1.0f;
            animMan->activeAnimation->resetAnimation();
        }
        animMan->startTransition(0.2f);
        strafeRate = 0;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS && strafeRate == 0)
    {
        if (strafeRate != 0)
        {
            return;
        }
        idle->weight = 0.0f;
        if(moveRate < 0)
        {
            rightStrafe->weight = 1.0f;
            rightStrafe->direction = -1;
        }else{
            leftStrafe->weight = 1.0f;
            leftStrafe->direction = 1;
        }
        animMan->startTransition(0.2f);
        strafeRate = 1;
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        leftStrafe->weight = 0.0f;
        rightStrafe->weight = 0.0f;
        if (fabs(moveRate) == 0)
        {
            idle->weight = 1.0f;
            animMan->activeAnimation->resetAnimation();
        }
        animMan->startTransition(0.2f);
        strafeRate = 0;
    }
    if(key == GLFW_KEY_Q && action== GLFW_PRESS)
    {
        turnRate = 1;
    }
    else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
    {
        turnRate = 0;
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        turnRate = -1;
    }
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
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
