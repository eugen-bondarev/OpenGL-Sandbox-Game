// #pragma once

// #include "../RenderPass.h"

// #include "Resources/ColorFBO.h"

// #include "MapRenderer.h"
// #include "Game/Map.h"

// #include <functional>

// class ColorPass : public RenderPass<ColorFBO> {
// public:
//   ColorPass(Ref<Map>& map);
//   void Execute(const Mat4& viewMatrix, const Vec2& viewPos, std::function<void()> add);

//   inline Ref<MapRenderer>& GetMapRenderer() {
//     return mapRenderer;
//   }

//   inline light_data_t& GetLightData() {
//     return light;
//   }

//   inline light_data_t& GetAdditionalLightData() {
//     return additionalLight;
//   }

//   struct {
//     unsigned int chunksRendered;
//   } info;

// private:
//   light_data_t light;
//   light_data_t additionalLight;
// 	Ref<MapRenderer> mapRenderer;
// 	Ref<Map> map;
// 	Ref<Werwel::VAO> chunkVao;
// };