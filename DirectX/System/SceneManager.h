#pragma once

#include "FpsCounter/IFpsGetter.h"
#include "../Device/FileOperator.h"
#include <memory>
#include <string>
#include <unordered_set>

class Renderer;
class Camera;
class EngineFunctionManager;
class GameObjectManager;
class MeshManager;
class SpriteManager;
class Physics;
class LightManager;
class MeshRenderOnTextureManager;
class DrawString;

class SceneManager
    : public FileOperator
{
public:
    SceneManager();
    ~SceneManager();
    void initialize(const IFpsGetter* fpsGetter);
    void update();
    void draw() const;

private:
    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::unique_ptr<Renderer> mRenderer;
    std::shared_ptr<Camera> mCamera;
    std::unique_ptr<EngineFunctionManager> mEngineManager;
    std::unique_ptr<GameObjectManager> mGameObjectManager;
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<SpriteManager> mSpriteManager;
    std::unique_ptr<Physics> mPhysics;
    std::unique_ptr<LightManager> mLightManager;
    std::unique_ptr<MeshRenderOnTextureManager> mMeshRenderOnTextureManager;
    DrawString* mTextDrawer;
};
