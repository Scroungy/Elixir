#pragma once

#include "defines.h"
#include <fixed_array.h>
#include <array.h>

namespace SM
{
    // Renderer uses this
    struct RenderMesh
    {
        u64 vertexCount = 0;        // Amount of vertices
        std::Array<f32> position{}; // Array of geometry vertices (x, y, z, w)
        std::Array<f32> normal{};   // Array of normal vertices (x, y, z)
        std::Array<f32> color{};    // Array of color vertices (x, y, z)
        std::Array<f32> texture{};  // Array of texture vertices (x, y, z)
    };

    // Used for scene management ONLY
    struct RenderMeshSIMD
    {
        std::Array<RenderMesh *> renderMesh{}; // Array of render mesh pointers (reference)
    };

    // Physics uses this
    struct PhysicsMesh
    {
        u64 vertexCount = 0;        // Amount of vertices
        std::Array<f32> position{}; // Array of geometry vertices (x, y, z, w)
    };

    // Used for scene management ONLY
    struct PhysicsMeshSIMD
    {
        std::Array<PhysicsMesh *> physicsMesh{}; // Array of physics mesh pointers (references)
    };

    // Used for scene management ONLY
    struct BoundingBoxSIMD
    {
        std::Array<f32> xLen{}; // Array of x len
        std::Array<f32> yLen{}; // Array of y len
        std::Array<f32> zLen{}; // Array of z len
    };

    struct BoundingBox
    {
        f32 xLen{};
        f32 yLen{};
        f32 zLen{};
    };

    struct TranslationSIMD
    {
        std::Array<f32> posX{}; // Array of posX
        std::Array<f32> posY{}; // Array of posY
        std::Array<f32> posZ{}; // Array of posZ
        std::Array<f32> rotX{}; // Array of rotX
        std::Array<f32> rotY{}; // Array of rotY
        std::Array<f32> rotZ{}; // Array of rotZ
        std::Array<f32> scaX{}; // Array of scaX
        std::Array<f32> scaY{}; // Array of scaY
        std::Array<f32> scaZ{}; // Array of scaZ
    };

    struct Translation
    {
        f32 posX{};
        f32 posY{};
        f32 posZ{};
        f32 rotX{};
        f32 rotY{};
        f32 rotZ{};
        f32 scaX{};
        f32 scaY{};
        f32 scaZ{};
    };

    struct ObjectFlagsSIMD
    {
        std::Array<bool> synced{};    // Array of multiplayer sync flags
        std::Array<bool> collision{}; // Array of collision flags
    };

    struct ObjectFlags
    {
        bool synced = true;
        bool collision = true;
    };

    // Used for scene management ONLY
    struct WorldObjectSIMD
    {
        u64 count = 0;                                      // Amount of elements
        std::FixedArray<TranslationSIMD, 3> translations{}; // Physics writes to one buffer, rendering reads from last buffer, third buffer allows switching physics without it waiting for render
        ObjectFlagsSIMD flags{};
        std::Array<void *> userData{}; // Array of custom data structs (references)
    };

    struct WorldObject
    {
        Translation translation{};
        ObjectFlags flags{};
        void *userData{};
    };

    // Used for scene management ONLY
    struct ActorSIMD : WorldObjectSIMD
    {
        std::FixedArray<RenderMeshSIMD, 3> renderMeshes{};   // Physics writes to one buffer, rendering reads from last buffer, third buffer allows switching physics without it waiting for render
        std::FixedArray<PhysicsMeshSIMD, 3> physicsMeshes{}; // Physics writes to one buffer, rendering reads from last buffer, third buffer allows switching physics without it waiting for render
        std::FixedArray<BoundingBoxSIMD, 3> boundingBoxes{}; // Physics writes to one buffer, rendering reads from last buffer, third buffer allows switching physics without it waiting for render
    };

    struct Actor : WorldObject
    {
        RenderMesh *renderMesh{};
        PhysicsMesh *physicsMesh{};
        BoundingBox boundingBox{};
    };

    struct GlobalData
    {
        const char *levelName{};
        void *userData{};
    };

    class SceneManager
    {
        GlobalData globalData{};
        ActorSIMD actors{};
        u32 currentPhysicsWriteIndex = 0;
        u32 lastPhysicsWriteIndex = 0;
        u32 currentRenderReadIndex = 0;

    public:
        SceneManager() {}
        ~SceneManager() {}

        void loadScene(const char *levelName);          // TODO: Load scene from scene directory
        void unloadScene();                             // TODO: Clean up all scene obj and global data
        u32 addActor(const Actor &actor);               // TODO: Atomically add actor
        void removeActor(u32 index);                    // TODO: Atomically remove actor
        u32 getCurrentPhysicsWriteIndex() const;        // TODO: Atomically get current physics write index
        u32 getLastPhysicsWriteIndex() const;           // TODO: Atomically get last physics write index
        u32 getCurrentRenderReadIndex() const;          // TODO: Atomically get current render read index
        void updateCurrentPhysicsWriteIndex(u32 index); // TODO: Atomically update current physics write index
        void updateLastPhysicsWriteIndex(u32 index);    // TODO: Atomically update last physics write index
        void updateCurrentRenderReadIndex(u32 index);   // TODO: Atomically update current render read index
    };
}