#pragma once
#include <includes.h>
#include <initializer_list>
#include <VertexBuffer.h>
#include <unordered_map>
#include "Geometry.h"
namespace Cold {
   

    using GeometryId = u32;
    using GeometrySPtr = std::shared_ptr<Geometry>;
    class GeometrySystem {
    public:
        static void initiate();
        static void shutdown();
        template<typename T, typename S>
       // static GeometryId create_geometry(const GeometryConfig<T,S>& config); 
        static void delete_geometry(GeometryId id);
    private: 
       // std::unordered_map<GeometryId, >
    };
}