#pragma once

#include "mgcwel/mgcwel.h"

#include "assets/text_asset.h"
#include "../color_pass/color_pass.h"
#include "../light_pass/light_pass.h"

class CompositionPass
{
public:
	CompositionPass();
	void Perform(const Ref<ColorPass> &colorPass, const Ref<LightPass> &lightPass);

	Ref<mw::Shader> shader;
	Ref<mw::VAO> canvas;

private:
	struct
	{
		Ref<mw::Shader> shader;
	} sky;
};