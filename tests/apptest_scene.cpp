#include "apptest_scene.h"
#include <ncine/Application.h>
#include <ncine/Texture.h>
#include <ncine/Sprite.h>
#include <ncine/IFile.h> // for dataPath()
#include "apptest_datapath.h"

#include <ncine/config.h>
#if NCINE_WITH_NUKLEAR
	#define NK_IMPLEMENTATION
	#include <ncine/NuklearContext.h>
	#include <ncine/nuklear.h>
#endif

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

}

nc::IAppEventHandler *createAppEventHandler()
{
	return new MyEventHandler;
}

void MyEventHandler::onPreInit(nc::AppConfiguration &config)
{
	setDataPath(config);
}

void MyEventHandler::onInit()
{
	pause_ = false;
	angle_ = 0.0f;

	nc::SceneNode &rootNode = nc::theApplication().rootNode();

	textures_[0] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture1File).data());
	textures_[1] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture2File).data());
	textures_[2] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture3File).data());
	textures_[3] = nctl::makeUnique<nc::Texture>((nc::IFile::dataPath() + "textures/" + Texture4File).data());

	const float width = nc::theApplication().width();
	for (unsigned int i = 0; i < NumSprites; i++)
	{
		sprites_[i] = nctl::makeUnique<nc::Sprite>(&rootNode, textures_[i % NumTextures].get(), width * 0.15f + width * 0.1f * i, 0.0f);
		sprites_[i]->setScale(0.5f);
	}
}

void MyEventHandler::onFrameStart()
{
	const float height = nc::theApplication().height();

	if (pause_ == false)
	{
		angle_ += 1.0f * nc::theApplication().interval();
		if (angle_ > 360.0f)
			angle_ -= 360.0f;
	}

	for (unsigned int i = 0; i < NumSprites; i++)
	{
		sprites_[i]->y = height * 0.3f + fabsf(sinf(angle_ + 5.0f * i)) * (height * (0.25f + 0.02f * i));
		sprites_[i]->setRotation(angle_ * 20);
	}

#if NCINE_WITH_NUKLEAR
	nk_context *ctx = nc::NuklearContext::context();

	enum
	{
		SLOW,
		FAST
	};
	static int speed = SLOW;
	static float spriteScale = 0.5f;
	static nk_colorf bg{ 0.10f, 0.18f, 0.24f, 1.0f };

	if (nk_begin(ctx, "Nuklear Demo", nk_rect(50, 50, 230, 250),
	             NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		nk_layout_row_static(ctx, 30, 80, 2);
		if (nk_button_label(ctx, "Quit"))
			nc::theApplication().quit();

		nk_label(ctx, pause_ ? "Paused" : "Not Paused", NK_TEXT_CENTERED);

		nk_layout_row_dynamic(ctx, 30, 2);
		if (nk_option_label(ctx, "Slow", speed == SLOW))
			speed = SLOW;
		if (nk_option_label(ctx, "Fast", speed == FAST))
			speed = FAST;

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_property_float(ctx, "Sprite Scale:", 0.25f, &spriteScale, 2.0f, 0.1f, 0.01f);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "Background:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400)))
		{
			nk_layout_row_dynamic(ctx, 120, 1);
			bg = nk_color_picker(ctx, bg, NK_RGB);
			nk_layout_row_dynamic(ctx, 25, 1);
			bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
			bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
			bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
			nk_combo_end(ctx);
		}
	}
	nk_end(ctx);

	if (pause_ == false && speed == FAST)
		angle_ += 1.0f * nc::theApplication().interval();

	for (unsigned int i = 0; i < NumSprites; i++)
		sprites_[i]->setScale(spriteScale);

	nc::theApplication().gfxDevice().setClearColor(bg.r, bg.g, bg.b, bg.a);
#endif
}

#ifdef __ANDROID__
void MyEventHandler::onTouchDown(const nc::TouchEvent &event)
{
	pause_ = true;
}

void MyEventHandler::onTouchUp(const nc::TouchEvent &event)
{
	pause_ = false;
}
#endif

void MyEventHandler::onMouseButtonPressed(const nc::MouseEvent &event)
{
	if (event.isLeftButton())
		pause_ = true;
}

void MyEventHandler::onMouseButtonReleased(const nc::MouseEvent &event)
{
	if (event.isLeftButton())
		pause_ = false;
}

void MyEventHandler::onKeyReleased(const nc::KeyboardEvent &event)
{
	if (event.sym == nc::KeySym::ESCAPE || event.sym == nc::KeySym::Q)
		nc::theApplication().quit();
	else if (event.sym == nc::KeySym::SPACE)
	{
		const bool isSuspended = nc::theApplication().isSuspended();
		nc::theApplication().setSuspended(!isSuspended);
	}
}
