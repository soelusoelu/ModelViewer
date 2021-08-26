#include "SceneManager.h"
#include "Game.h"
#include "GlobalFunction.h"
#include "Texture/MeshRenderOnTextureManager.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Component/Engine/Text/TextBase.h"
#include "../Device/DrawString.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../Engine/EngineFunctionManager.h"
#include "../Engine/DebugManager/DebugManager.h"
#include "../Engine/DebugManager/DebugLayer/DebugLayer.h"
#include "../Engine/DebugManager/DebugLayer/Hierarchy.h"
#include "../Engine/DebugManager/DebugLayer/Inspector/ImGuiInspector.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../Light/LightManager.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../Utility/JsonHelper.h"
#include <vector>

SceneManager::SceneManager()
    : FileOperator("SceneManager")
    , mRenderer(std::make_unique<Renderer>())
    , mCamera(nullptr)
    , mEngineManager(std::make_unique<EngineFunctionManager>())
    , mGameObjectManager(std::make_unique<GameObjectManager>())
    , mMeshManager(std::make_unique<MeshManager>())
    , mSpriteManager(std::make_unique<SpriteManager>())
    , mPhysics(std::make_unique<Physics>())
    , mLightManager(std::make_unique<LightManager>())
    , mMeshRenderOnTextureManager(std::make_unique<MeshRenderOnTextureManager>())
    , mTextDrawer(new DrawString())
{
}

SceneManager::~SceneManager() {
    safeDelete(mTextDrawer);

    TextBase::setDrawString(nullptr);
}

void SceneManager::initialize(const IFpsGetter* fpsGetter) {
    //デフォルトカメラを作成する
    auto cam = GameObjectCreater::create("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    mRenderer->initialize();
    mEngineManager->initialize(mGameObjectManager.get(), fpsGetter);
    mMeshManager->initialize();
    mLightManager->initialize();
    mTextDrawer->initialize();

    TextBase::setDrawString(mTextDrawer);

    //デフォルトのインスペクターの対象に設定する
    mEngineManager->debug().getDebugLayer().inspector()->setTarget(cam);

    mLightManager->createDirectionalLight();
}

void SceneManager::update() {
    //アップデート前処理
    mEngineManager->preUpdateProcess();
    //デバッグ
    mEngineManager->update();

    //スプライトはいつでも更新する
    mSpriteManager->update();
    mMeshRenderOnTextureManager->update();
}

void SceneManager::draw() const {
    const auto& view = mCamera->getView();
    const auto& proj = mCamera->getProjection();

    //メッシュ描画準備
    mRenderer->renderMesh();

    mEngineManager->draw3D(*mRenderer, *mCamera, mLightManager->getDirectionalLight());

    //スプライト描画準備
    mRenderer->renderSprite();
    //3Dスプライト
    mRenderer->renderSprite3D();

    //2Dスプライト
    auto proj2D = Matrix4::identity;
    mRenderer->renderSprite2D(proj2D);

    //レンダリング領域をデバッグに変更
    mRenderer->renderSprite();
    mRenderer->renderSprite2D(proj2D);
    mRenderer->renderToDebug(proj2D);
    mSpriteManager->draw(proj2D);
    mEngineManager->drawDebug2D(proj2D);
}

void SceneManager::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mEngineManager->writeAndRead(inObj, alloc, mode);
    mMeshManager->saveAndLoad(inObj, alloc, mode);
    mLightManager->writeAndRead(inObj, alloc, mode);
    mTextDrawer->writeAndRead(inObj, alloc, mode);
}
