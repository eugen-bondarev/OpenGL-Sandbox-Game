#pragma once

#include "werwel/werwel.h"

#include "assets/text_asset.h"
#include "../color_pass/color_pass.h"
#include "../light_pass/light_pass.h"

class CompositionPass
{
public:
	CompositionPass();
	void Perform(const Ref<ColorPass> &colorPass, const Ref<LightPass> &lightPass);

	Ref<ww::Shader> shader;
	Ref<ww::VAO> canvas;

private:
	struct
	{
		Ref<ww::Shader> shader;
	} sky;
};