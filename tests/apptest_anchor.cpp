#include <ncine/imgui.h>

#include "apptest_anchor.h"
#include <ncine/Application.h>
#include <ncine/Texture.h>
#include <ncine/Sprite.h>
#include <ncine/MeshSprite.h>
#include <ncine/ParticleSystem.h>
#include <ncine/ParticleInitializer.h>
#include <ncine/IFile.h> // for dataPath()
#include "apptest_datapath.h"

namespace {

#ifdef __ANDROID__
const char *Texture1File = "texture1_ETC2.ktx";
const char *Texture2File = "texture2_ETC2.ktx";
const char *Texture3File = "texture3_ETC2.ktx";
const char *Texture4File = "texture4_ETC2.ktx";
#else
const char *Texture1File = "texture1.png";
const char *Texture2File = "texture2.png";
const char *Texture3File = "texture3.png";
const char *Texture4File = "texture4.png";
#endif

const int NumTexelPoints[MyEventHandler::NumTextures] = { 3, 4, 5, 4 };
const nc::Vector2f TexelPoints[] = {
	// clang-format off
	{4.0f, 2.0f}, {124.0f, 2.0f}, {64.0f, 122.0f},
	{13.0f, 13.0f}, {115.0f, 13.0f}, {13.0f, 115.0f}, {115.0f, 115.0f},
	{3.0f, 79.0f}, {26.0f, 2.0f}, {64.0f, 125.0f}, {102.0f, 2.0f}, {125.0f, 79.0f},
	{20.0f, 20.0f}, {107.0f, 20.0f}, {20.0f, 107.0f}, {107.0f, 107.0f},
	// clang-format on
};

const char *TypeLables[] = { "Sprites", "Mesh Sprites", "Particle Systems" };
const char *AnchorPoints[] = { "Center", "Bottom Left", "Top Left", "Bottom Right", "Top Right" };
const char *BlendingPresets[] = { "Disabled", "Alpha", "Pre-multiplied Alpha", "Additive", "Multiply" };

}

nc::IAppEventHandler *createAppEventHandler()
{
	return new MyEventHandler;
}

MyEventHandler::MyEventHandler()
    : background_(nc::Colorf::Black),
      currentType_(0), anchorPoint_(0.5f, 0.5f),
      position_(0.0f, 0.0f), angle_(0.0f),
      scale_(1.0f, 1.0f), lockScale_(true),
      flippedX_(false), flippedY_(false),
      blendingPreset_(nc::DrawableNode::BlendingPreset::ALPHA)
{
}

void MyEventHandler::onPreInit(nc::AppConfiguration &config)
{
	setDataPath(config);
}

void MyEventHandler::onInit()
{
	nc::SceneNode &rootNode = nc::theApplication().rootNode();

	textures_[0] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture1File).data());
	textures_[1] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture2File).data());
	textures_[2] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture3File).data());
	textures_[3] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture4File).data());

	const float width = nc::theApplication().width();
	const float height = nc::theApplication().height();
	int texelStartIndex = 0;
	for (unsigned int i = 0; i < NumSprites; i++)
	{
		nc::Texture *texture = textures_[i % NumTextures].get();
		const nc::Vector2f position(width / (NumSprites + 3) * (i + 2), height * 0.5f);

		sprites_[i] = nctl::makeUnique<nc::Sprite>(&rootNode, texture, position);
		sprites_[i]->setEnabled(false);

		meshSprites_[i] = nctl::makeUnique<nc::MeshSprite>(&rootNode, texture, position);
		meshSprites_[i]->createVerticesFromTexels(NumTexelPoints[i % NumTextures], &TexelPoints[texelStartIndex]);
		texelStartIndex += NumTexelPoints[i % NumTextures];
		if (texelStartIndex >= static_cast<int>(sizeof(TexelPoints) / sizeof(*TexelPoints)))
			texelStartIndex = 0;

		particleSystems_[i] = nctl::makeUnique<nc::ParticleSystem>(&rootNode, 16, texture, texture->rect());
		particleSystems_[i]->setPosition(position);

		nctl::UniquePtr<nc::ColorAffector> colAffector = nctl::makeUnique<nc::ColorAffector>();
		colAffector->addColorStep(0.0f, nc::Colorf(1.0f, 1.0f, 1.0f, 1.0f));
		colAffector->addColorStep(1.0f, nc::Colorf(1.0f, 1.0f, 1.0f, 0.0f));
		particleSystems_[i]->addAffector(nctl::move(colAffector));
		nctl::UniquePtr<nc::SizeAffector> sizeAffector = nctl::makeUnique<nc::SizeAffector>(1.0f);
		sizeAffectors_[i] = sizeAffector.get();
		particleSystems_[i]->addAffector(nctl::move(sizeAffector));
		nctl::UniquePtr<nc::RotationAffector> rotationAffector = nctl::makeUnique<nc::RotationAffector>();
		rotationAffector->addRotationStep(0.0f, 0.0f);
		rotationAffector->addRotationStep(1.0f, 0.0f);
		rotationAffectors_[i] = rotationAffector.get();
		particleSystems_[i]->addAffector(nctl::move(rotationAffector));
	}
}

void MyEventHandler::onFrameStart()
{
	nc::Application::RenderingSettings &settings = nc::theApplication().renderingSettings();

	const float width = nc::theApplication().width();
	const float height = nc::theApplication().height();

	ImGui::Begin("apptest_anchor");
	if (ImGui::Combo("Type", &currentType_, TypeLables, IM_ARRAYSIZE(TypeLables)))
	{
		if (currentType_ == Type::PARTICLE_SYSTEM)
			lastEmissionTime_ = nc::TimeStamp::now();
	}
	ImGui::SliderFloat2("Anchor", anchorPoint_.data(), 0.0f, 1.0f);
	static int currentAnchorSelection = 0;
	if (ImGui::Combo("Presets", &currentAnchorSelection, AnchorPoints, IM_ARRAYSIZE(AnchorPoints)))
	{
		switch (currentAnchorSelection)
		{
			case 0:
				anchorPoint_ = nc::BaseSprite::AnchorCenter;
				break;
			case 1:
				anchorPoint_ = nc::BaseSprite::AnchorBottomLeft;
				break;
			case 2:
				anchorPoint_ = nc::BaseSprite::AnchorTopLeft;
				break;
			case 3:
				anchorPoint_ = nc::BaseSprite::AnchorBottomRight;
				break;
			case 4:
				anchorPoint_ = nc::BaseSprite::AnchorTopRight;
				break;
		}
	}
	ImGui::SliderFloat("Position X", &position_.x, -width * 0.75f, width * 0.75f);
	ImGui::SameLine();
	if (ImGui::Button("Reset##Position"))
		position_.set(0.0f, 0.0f);
	ImGui::SliderFloat("Position Y", &position_.y, -height * 0.75f, height * 0.75f);
	ImGui::SliderFloat("Rotation", &angle_, 0.0f, 360.0f);
	ImGui::SameLine();
	if (ImGui::Button("Reset##Rotation"))
		angle_ = 0.0f;
	if (lockScale_)
	{
		ImGui::SliderFloat("Scale", &scale_.x, 0.5f, 2.0f);
		scale_.y = scale_.x;
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &lockScale_);
	}
	else
	{
		ImGui::SliderFloat("Scale X", &scale_.x, 0.5f, 2.0f);
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &lockScale_);
		ImGui::SliderFloat("Scale Y", &scale_.y, 0.5f, 2.0f);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset##Scale"))
		scale_.set(1.0f, 1.0f);
	ImGui::Checkbox("Flipped X", &flippedX_);
	ImGui::SameLine();
	ImGui::Checkbox("Flipped Y", &flippedY_);
	ImGui::SameLine();
	if (ImGui::Button("Reset##Flip"))
	{
		flippedX_ = false;
		flippedY_ = false;
	}
	static int currentBlendingSelection = 1;
	if (ImGui::Combo("Blending", &currentBlendingSelection, BlendingPresets, IM_ARRAYSIZE(BlendingPresets)))
		blendingPreset_ = static_cast<nc::DrawableNode::BlendingPreset>(currentBlendingSelection);

	ImGui::ColorEdit4("Background Color", background_.data(), ImGuiColorEditFlags_NoAlpha);
	nc::theApplication().gfxDevice().setClearColor(background_);
	ImGui::Checkbox("Culling", &settings.cullingEnabled);
	ImGui::SameLine();
	ImGui::Checkbox("Batching", &settings.batchingEnabled);
	ImGui::End();

	for (unsigned int i = 0; i < NumSprites; i++)
	{
		if (currentType_ == Type::SPRITE)
		{
			sprites_[i]->setAnchorPoint(anchorPoint_);
			sprites_[i]->x = position_.x + width / (NumSprites + 3) * (i + 2);
			sprites_[i]->y = position_.y + (height * 0.5f);
			sprites_[i]->setRotation(angle_);
			sprites_[i]->setScale(scale_);
			sprites_[i]->setFlippedX(flippedX_);
			sprites_[i]->setFlippedY(flippedY_);
			sprites_[i]->setBlendingPreset(blendingPreset_);
		}
		else if (currentType_ == Type::MESH_SPRITE)
		{
			meshSprites_[i]->setAnchorPoint(anchorPoint_);
			meshSprites_[i]->x = position_.x + width / (NumSprites + 3) * (i + 2);
			meshSprites_[i]->y = position_.y + (height * 0.5f);
			meshSprites_[i]->setRotation(angle_);
			meshSprites_[i]->setScale(scale_);
			meshSprites_[i]->setFlippedX(flippedX_);
			meshSprites_[i]->setFlippedY(flippedY_);
			meshSprites_[i]->setBlendingPreset(blendingPreset_);
		}
		else if (currentType_ == Type::PARTICLE_SYSTEM)
		{
			particleSystems_[i]->setAnchorPoint(anchorPoint_);
			particleSystems_[i]->x = position_.x + width / (NumSprites + 3) * (i + 2);
			particleSystems_[i]->y = position_.y + (height * 0.5f);
			rotationAffectors_[i]->steps()[1].angle = angle_;
			sizeAffectors_[i]->setBaseScale(scale_);
			particleSystems_[i]->setFlippedX(flippedX_);
			particleSystems_[i]->setFlippedY(flippedY_);
			particleSystems_[i]->setBlendingPreset(blendingPreset_);
		}

		sprites_[i]->setEnabled(currentType_ == Type::SPRITE);
		meshSprites_[i]->setEnabled(currentType_ == Type::MESH_SPRITE);
		particleSystems_[i]->setEnabled(currentType_ == Type::PARTICLE_SYSTEM);
	}

	if (currentType_ == Type::PARTICLE_SYSTEM && lastEmissionTime_.secondsSince() > 0.1f)
	{
		nc::ParticleInitializer init;
		init.setAmount(1);
		init.setLife(0.5f, 0.75f);
		init.setPositionAndRadius(nc::Vector2f::Zero, 10.0f);
		init.setVelocityAndScale(nc::Vector2f(0.0f, 250.0f), 0.8f, 1.0f);

		lastEmissionTime_ = nc::TimeStamp::now();
		for (unsigned int i = 0; i < NumSprites; i++)
			particleSystems_[i]->emitParticles(init);
	}
}

void MyEventHandler::onKeyReleased(const nc::KeyboardEvent &event)
{
	if (event.sym == nc::KeySym::ESCAPE || event.sym == nc::KeySym::Q)
		nc::theApplication().quit();
}
