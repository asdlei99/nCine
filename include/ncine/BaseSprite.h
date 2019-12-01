#ifndef CLASS_NCINE_BASESPRITE
#define CLASS_NCINE_BASESPRITE

#include "DrawableNode.h"
#include "Rect.h"

namespace ncine {

class Texture;
class GLUniformBlockCache;

/// The base class for sprites
/*! \note Users cannot create instances of this class */
class DLL_PUBLIC BaseSprite : public DrawableNode
{
  public:
	/// Sets sprite width
	inline void setWidth(float width) { width_ = width; }
	/// Sets sprite height
	inline void setHeight(float height) { height_ = height; }
	/// Sets sprite size
	void setSize(float width, float height);
	/// Sets sprite size with a `Vector2f`
	void setSize(const Vector2f &size);

	/// Gets the texture object
	inline const Texture *texture() const { return texture_; }
	/// Sets the texture object
	inline void setTexture(Texture *texture) { texture_ = texture; }
	/// Sets a flag that makes a transparent texture to be considered opaque
	/*! \note This flag comes in handy when the sprite uses an opaque region of a transparent texture atlas. */
	inline void setOpaqueTexture(bool opaqueTexture) { opaqueTexture_ = opaqueTexture; }

	/// Gets the texture source rectangle for blitting
	inline Recti texRect() const { return texRect_; }
	/// Sets the texture source rectangle for blitting
	void setTexRect(const Recti &rect);

	/// Returns `true` if the sprite is horizontally flipped
	inline bool isFlippedX() const { return flippedX_; }
	/// Flips the texture rect horizontally
	void setFlippedX(bool flippedX);
	/// Returns `true` if the sprite is vertically flipped
	inline bool isFlippedY() const { return flippedY_; }
	/// Flips the texture rect vertically
	void setFlippedY(bool flippedY);

  protected:
	/// The sprite texture
	Texture *texture_;
	/// The texture source rectangle
	Recti texRect_;
	/// The opaque texture flag
	bool opaqueTexture_;

	/// A flag indicating if the sprite is horizontally flipped
	bool flippedX_;
	/// A flag indicating if the sprite is vertically flipped
	bool flippedY_;

	GLUniformBlockCache *spriteBlock_;

	/// Protected construtor accessible only by derived sprite classes
	BaseSprite(SceneNode *parent, Texture *texture, float xx, float yy);
	/// Protected construtor accessible only by derived sprite classes
	BaseSprite(SceneNode *parent, Texture *texture, const Vector2f &position);

	void updateRenderCommand() override;
};

}

#endif
