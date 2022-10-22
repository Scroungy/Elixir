#include "scene_manager.h"

namespace SM
{
    void SceneManager::loadScene(const char *levelName) {}

    void SceneManager::unloadScene() {}

    u32 SceneManager::addActor(const Actor &actor) { return 0; }

    void SceneManager::removeActor(u32 index) {}

    u32 SceneManager::getCurrentPhysicsWriteIndex() const { return 0; }

    u32 SceneManager::getLastPhysicsWriteIndex() const { return 0; }

    u32 SceneManager::getCurrentRenderReadIndex() const { return 0; }

    void SceneManager::updateCurrentPhysicsWriteIndex(u32 index) {}

    void SceneManager::updateLastPhysicsWriteIndex(u32 index) {}

    void SceneManager::updateCurrentRenderReadIndex(u32 index) {}
}