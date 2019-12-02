#define NCINE_INCLUDE_LUA
#include "common_headers.h"

#include "LuaDrawableNode.h"
#include "LuaSceneNode.h"
#include "LuaClassWrapper.h"
#include "LuaUtils.h"
#include "LuaVector2Utils.h"
#include "DrawableNode.h"

namespace ncine {

namespace LuaNames {
namespace DrawableNode {
	static const char *DISABLED = "DISABLED";
	static const char *ALPHA = "ALPHA";
	static const char *PREMULTIPLIED_ALPHA = "PREMULTIPLIED_ALPHA";
	static const char *ADDITIVE = "ADDITIVE";
	static const char *MULTIPLY = "MULTIPLY";
	static const char *BlendingPreset = "blending_preset";

	static const char *ZERO = "ZERO";
	static const char *ONE = "ONE";
	static const char *SRC_COLOR = "SRC_COLOR";
	static const char *ONE_MINUS_SRC_COLOR = "ONE_MINUS_SRC_COLOR";
	static const char *DST_COLOR = "DST_COLOR";
	static const char *ONE_MINUS_DST_COLOR = "ONE_MINUS_DST_COLOR";
	static const char *SRC_ALPHA = "SRC_ALPHA";
	static const char *ONE_MINUS_SRC_ALPHA = "ONE_MINUS_SRC_ALPHA";
	static const char *DST_ALPHA = "DST_ALPHA";
	static const char *ONE_MINUS_DST_ALPHA = "ONE_MINUS_DST_ALPHA";
	static const char *CONSTANT_COLOR = "CONSTANT_COLOR";
	static const char *ONE_MINUS_CONSTANT_COLOR = "ONE_MINUS_CONSTANT_COLOR";
	static const char *CONSTANT_ALPHA = "CONSTANT_ALPHA";
	static const char *ONE_MINUS_CONSTANT_ALPHA = "ONE_MINUS_CONSTANT_ALPHA";
	static const char *SRC_ALPHA_SATURATE = "SRC_ALPHA_SATURATE";
	static const char *BlendingFactor = "blending_factor";

	static const char *LOWEST = "LOWEST";
	static const char *SCENE = "SCENE";
	static const char *HUD = "HUD";
	static const char *HIGHEST = "HIGHEST";
	static const char *LayerBase = "layer_base";

	static const char *width = "get_width";
	static const char *height = "get_height";
	static const char *size = "get_size";
	static const char *anchorPoint = "get_anchor_point";
	static const char *setAnchorPoint = "set_anchor_point";

	static const char *isBlendingEnabled = "is_blending_enabled";
	static const char *setBlendingEnabled = "set_blending_enabled";
	static const char *srcBlendingFactor = "get_src_blending_factor";
	static const char *destBlendingFactor = "get_dest_blending_factor";
	static const char *setBlendingPreset = "set_blending_preset";
	static const char *setBlendingFactors = "set_blending_factors";

	static const char *layer = "get_layer";
	static const char *setLayer = "set_layer";
}}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void LuaDrawableNode::exposeConstants(lua_State *L)
{
	lua_createtable(L, 5, 0);

	LuaUtils::pushField(L, LuaNames::DrawableNode::DISABLED, static_cast<int64_t>(DrawableNode::BlendingPreset::DISABLED));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ALPHA, static_cast<int64_t>(DrawableNode::BlendingPreset::ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::PREMULTIPLIED_ALPHA, static_cast<int64_t>(DrawableNode::BlendingPreset::PREMULTIPLIED_ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ADDITIVE, static_cast<int64_t>(DrawableNode::BlendingPreset::ADDITIVE));
	LuaUtils::pushField(L, LuaNames::DrawableNode::MULTIPLY, static_cast<int64_t>(DrawableNode::BlendingPreset::MULTIPLY));

	lua_setfield(L, -2, LuaNames::DrawableNode::BlendingPreset);

	lua_createtable(L, 15, 0);

	LuaUtils::pushField(L, LuaNames::DrawableNode::ZERO, static_cast<int64_t>(DrawableNode::BlendingFactor::ZERO));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ONE, static_cast<int64_t>(DrawableNode::BlendingFactor::ONE));
	LuaUtils::pushField(L, LuaNames::DrawableNode::SRC_COLOR, static_cast<int64_t>(DrawableNode::BlendingFactor::SRC_COLOR));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ONE_MINUS_SRC_COLOR, static_cast<int64_t>(DrawableNode::BlendingFactor::ONE_MINUS_SRC_COLOR));
	LuaUtils::pushField(L, LuaNames::DrawableNode::DST_COLOR, static_cast<int64_t>(DrawableNode::BlendingFactor::DST_COLOR));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ONE_MINUS_DST_COLOR, static_cast<int64_t>(DrawableNode::BlendingFactor::ONE_MINUS_DST_COLOR));
	LuaUtils::pushField(L, LuaNames::DrawableNode::SRC_ALPHA, static_cast<int64_t>(DrawableNode::BlendingFactor::SRC_ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ONE_MINUS_SRC_ALPHA, static_cast<int64_t>(DrawableNode::BlendingFactor::ONE_MINUS_SRC_ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::DST_ALPHA, static_cast<int64_t>(DrawableNode::BlendingFactor::DST_ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ONE_MINUS_DST_ALPHA, static_cast<int64_t>(DrawableNode::BlendingFactor::ONE_MINUS_DST_ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::CONSTANT_COLOR, static_cast<int64_t>(DrawableNode::BlendingFactor::CONSTANT_COLOR));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ONE_MINUS_CONSTANT_COLOR, static_cast<int64_t>(DrawableNode::BlendingFactor::ONE_MINUS_CONSTANT_COLOR));
	LuaUtils::pushField(L, LuaNames::DrawableNode::CONSTANT_ALPHA, static_cast<int64_t>(DrawableNode::BlendingFactor::CONSTANT_ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::ONE_MINUS_CONSTANT_ALPHA, static_cast<int64_t>(DrawableNode::BlendingFactor::ONE_MINUS_CONSTANT_ALPHA));
	LuaUtils::pushField(L, LuaNames::DrawableNode::SRC_ALPHA_SATURATE, static_cast<int64_t>(DrawableNode::BlendingFactor::SRC_ALPHA_SATURATE));

	lua_setfield(L, -2, LuaNames::DrawableNode::BlendingFactor);

	lua_createtable(L, 4, 0);

	LuaUtils::pushField(L, LuaNames::DrawableNode::LOWEST, static_cast<int64_t>(DrawableNode::LayerBase::LOWEST));
	LuaUtils::pushField(L, LuaNames::DrawableNode::SCENE, static_cast<int64_t>(DrawableNode::LayerBase::SCENE));
	LuaUtils::pushField(L, LuaNames::DrawableNode::HUD, static_cast<int64_t>(DrawableNode::LayerBase::HUD));
	LuaUtils::pushField(L, LuaNames::DrawableNode::HIGHEST, static_cast<int64_t>(DrawableNode::LayerBase::HIGHEST));

	lua_setfield(L, -2, LuaNames::DrawableNode::LayerBase);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void LuaDrawableNode::exposeFunctions(lua_State *L)
{
	LuaSceneNode::exposeFunctions(L);

	LuaUtils::addFunction(L, LuaNames::DrawableNode::width, width);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::height, height);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::size, size);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::anchorPoint, anchorPoint);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::setAnchorPoint, setAnchorPoint);

	LuaUtils::addFunction(L, LuaNames::DrawableNode::isBlendingEnabled, isBlendingEnabled);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::setBlendingEnabled, setBlendingEnabled);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::srcBlendingFactor, srcBlendingFactor);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::destBlendingFactor, destBlendingFactor);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::setBlendingPreset, setBlendingPreset);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::setBlendingFactors, setBlendingFactors);

	LuaUtils::addFunction(L, LuaNames::DrawableNode::layer, layer);
	LuaUtils::addFunction(L, LuaNames::DrawableNode::setLayer, setLayer);
}

int LuaDrawableNode::width(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const float width = node->width();
	LuaUtils::push(L, width);

	return 1;
}

int LuaDrawableNode::height(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const float height = node->height();
	LuaUtils::push(L, height);

	return 1;
}

int LuaDrawableNode::size(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const Vector2f size = node->size();
	LuaVector2fUtils::push(L, size);

	return 1;
}

int LuaDrawableNode::anchorPoint(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const Vector2f &anchorPoint = node->anchorPoint();
	LuaVector2fUtils::push(L, anchorPoint);

	return 1;
}

int LuaDrawableNode::setAnchorPoint(lua_State *L)
{
	int vectorIndex = 0;
	const Vector2f &anchorPoint = LuaVector2fUtils::retrieve(L, -1, vectorIndex);
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, vectorIndex - 1);

	node->setAnchorPoint(anchorPoint);

	return 0;
}

int LuaDrawableNode::isBlendingEnabled(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const bool enabled = node->isBlendingEnabled();
	LuaUtils::push(L, enabled);

	return 1;
}

int LuaDrawableNode::setBlendingEnabled(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -2);
	const bool enabled = LuaUtils::retrieve<bool>(L, -1);

	node->setBlendingEnabled(enabled);

	return 0;
}

int LuaDrawableNode::srcBlendingFactor(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const DrawableNode::BlendingFactor srcFactor = node->srcBlendingFactor();
	LuaUtils::push(L, static_cast<int64_t>(srcFactor));

	return 1;
}

int LuaDrawableNode::destBlendingFactor(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const DrawableNode::BlendingFactor destFactor = node->destBlendingFactor();
	LuaUtils::push(L, static_cast<int64_t>(destFactor));

	return 1;
}

int LuaDrawableNode::setBlendingPreset(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -2);
	const DrawableNode::BlendingPreset blendingPreset = static_cast<DrawableNode::BlendingPreset>(LuaUtils::retrieve<int64_t>(L, -1));

	node->setBlendingPreset(blendingPreset);

	return 0;
}

int LuaDrawableNode::setBlendingFactors(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -3);
	const DrawableNode::BlendingFactor srcBlendingFactor = static_cast<DrawableNode::BlendingFactor>(LuaUtils::retrieve<int64_t>(L, -2));
	const DrawableNode::BlendingFactor destBlendingFactor = static_cast<DrawableNode::BlendingFactor>(LuaUtils::retrieve<int64_t>(L, -1));

	node->setBlendingFactors(srcBlendingFactor, destBlendingFactor);

	return 0;
}

int LuaDrawableNode::layer(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -1);

	const unsigned int layer = node->layer();
	LuaUtils::push(L, layer);

	return 1;
}

int LuaDrawableNode::setLayer(lua_State *L)
{
	DrawableNode *node = LuaClassWrapper<DrawableNode>::unwrapUserData(L, -2);
	const uint32_t layer = LuaUtils::retrieve<uint32_t>(L, -1);

	node->setLayer(layer);

	return 0;
}

}
