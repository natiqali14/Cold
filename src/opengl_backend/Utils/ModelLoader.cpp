#pragma once
#include "ModelLoader.h"
#include <filesystem>
#include <Logger.h>
namespace Cold {
    static ModelLoader* instance = nullptr;
    void ModelLoader::initiate()
    {
        instance = new ModelLoader;
    }

    void ModelLoader::shutdown()
    {
    }

    void ModelLoader::model_load(const std::string &file_path, u32 flags)
    {
        std::filesystem::path cwd = std::filesystem::current_path();
        cwd = cwd / file_path;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(cwd.string(), flags);
        COLD_TRACE("Sponze Root name %s", scene->mRootNode->mName.C_Str());
        auto root = scene->mRootNode;
        u32 root_child = root->mNumChildren;
        COLD_TRACE("Sponza root has childs %d", root_child);
        for(u32 i = 0; i < root_child; i++) {
            auto child = root->mChildren[i];
            u32 child_child = child->mNumChildren;
            COLD_TRACE("Sponza child %d has child %d", i, child_child);
        }

    }
}