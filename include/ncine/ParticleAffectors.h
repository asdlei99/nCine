#ifndef CLASS_NCINE_PARTICLEAFFECTORS
#define CLASS_NCINE_PARTICLEAFFECTORS

#include "Vector2.h"
#include "Colorf.h"
#include <nctl/Array.h>

namespace ncine {

class Particle;

const unsigned int StepsInitialSize = 4;

/// Base class for particle affectors
/*! Affectors modify particle properties depending on their remaining life */
class DLL_PUBLIC ParticleAffector
{
  public:
	virtual ~ParticleAffector() {}
	/// Affects a property of the specified particle
	void affect(Particle *particle);
	/// Affects a property of the specified particle, without calculating the normalized age
	virtual void affect(Particle *particle, float normalizedAge) = 0;
};

/// Particle color affector
class DLL_PUBLIC ColorAffector : public ParticleAffector
{
  public:
	struct ColorStep
	{
		float age;
		Colorf color;

		ColorStep()
		    : age(0.0f) {}
		ColorStep(float newAge, const Colorf &newColor)
		    : age(newAge), color(newColor) {}
	};

	ColorAffector()
	    : colorSteps_(StepsInitialSize) {}

	/// Affects the color of the specified particle
	void affect(Particle *particle, float normalizedAge) override;
	void addColorStep(float age, const Colorf &color);

	inline nctl::Array<ColorStep> &steps() { return colorSteps_; }
	inline const nctl::Array<ColorStep> &steps() const { return colorSteps_; }

  private:
	nctl::Array<ColorStep> colorSteps_;
};

/// Particle size affector
class DLL_PUBLIC SizeAffector : public ParticleAffector
{
  public:
	struct SizeStep
	{
		float age;
		Vector2f scale;

		SizeStep()
		    : age(0.0f), scale(1.0f, 1.0f) {}
		SizeStep(float newAge, float newScale)
		    : age(newAge), scale(newScale, newScale) {}
		SizeStep(float newAge, float newScaleX, float newScaleY)
		    : age(newAge), scale(newScaleX, newScaleY) {}
		SizeStep(float newAge, const Vector2f &newScale)
		    : age(newAge), scale(newScale) {}
	};

	/// Constructs a size affector with a default base scale factor
	SizeAffector()
	    : sizeSteps_(StepsInitialSize), baseScale_(1.0f, 1.0f) {}
	/// Constructs a size affector with a base scale factor as a reference
	explicit SizeAffector(float baseScale)
	    : sizeSteps_(StepsInitialSize), baseScale_(baseScale, baseScale) {}
	/// Constructs a size affector with a horizontal and a vertical base scale factor as a reference
	SizeAffector(float baseScaleX, float baseScaleY)
	    : sizeSteps_(StepsInitialSize), baseScale_(baseScaleX, baseScaleY) {}
	/// Constructs a size affector with a vector base scale factor as a reference
	explicit SizeAffector(const Vector2f &baseScale)
	    : sizeSteps_(StepsInitialSize), baseScale_(baseScale) {}

	/// Affects the size of the specified particle
	void affect(Particle *particle, float normalizedAge) override;
	inline void addSizeStep(float age, float scale) { addSizeStep(age, scale, scale); }
	void addSizeStep(float age, float scaleX, float scaleY);
	inline void addSizeStep(float age, const Vector2f &scale) { addSizeStep(age, scale.x, scale.y); }

	inline nctl::Array<SizeStep> &steps() { return sizeSteps_; }
	inline const nctl::Array<SizeStep> &steps() const { return sizeSteps_; }

	inline float baseScaleX() const { return baseScale_.x; }
	inline void setBaseScaleX(float baseScaleX) { baseScale_.x = baseScaleX; }
	inline float baseScaleY() const { return baseScale_.y; }
	inline void setBaseScaleY(float baseScaleY) { baseScale_.y = baseScaleY; }

	inline const Vector2f &baseScale() const { return baseScale_; }
	inline void setBaseScale(float baseScale) { baseScale_.set(baseScale, baseScale); }
	inline void setBaseScale(const Vector2f &baseScale) { baseScale_ = baseScale; }

  private:
	nctl::Array<SizeStep> sizeSteps_;
	Vector2f baseScale_;
};

/// Particle rotation affector
class DLL_PUBLIC RotationAffector : public ParticleAffector
{
  public:
	struct RotationStep
	{
		float age;
		float angle;

		RotationStep()
		    : age(0.0f), angle(0.0f) {}
		RotationStep(float newAge, float newAngle)
		    : age(newAge), angle(newAngle) {}
	};

	RotationAffector()
	    : rotationSteps_(StepsInitialSize) {}

	/// Affects the rotation of the specified particle
	void affect(Particle *particle, float normalizedAge) override;
	void addRotationStep(float age, float angle);

	inline nctl::Array<RotationStep> &steps() { return rotationSteps_; }
	inline const nctl::Array<RotationStep> &steps() const { return rotationSteps_; }

  private:
	nctl::Array<RotationStep> rotationSteps_;
};

/// Particle position affector
class DLL_PUBLIC PositionAffector : public ParticleAffector
{
  public:
	struct PositionStep
	{
		float age;
		Vector2f position;

		PositionStep()
		    : age(0.0f), position(0.0f, 0.0f) {}
		PositionStep(float newAge, float newPositionX, float newPositionY)
		    : age(newAge), position(newPositionX, newPositionY) {}
	};

	PositionAffector()
	    : positionSteps_(StepsInitialSize) {}

	/// Affects the position of the specified particle
	void affect(Particle *particle, float normalizedAge) override;
	void addPositionStep(float age, float posX, float posY);
	inline void addPositionStep(float age, const Vector2f &position) { addPositionStep(age, position.x, position.y); }

	inline nctl::Array<PositionStep> &steps() { return positionSteps_; }
	inline const nctl::Array<PositionStep> &steps() const { return positionSteps_; }

  private:
	nctl::Array<PositionStep> positionSteps_;
};

/// Particle velocity affector
class DLL_PUBLIC VelocityAffector : public ParticleAffector
{
  public:
	struct VelocityStep
	{
		float age;
		Vector2f velocity;

		VelocityStep()
		    : age(0.0f), velocity(0.0f, 0.0f) {}
		VelocityStep(float newAge, float newVelocityX, float newVelocityY)
		    : age(newAge), velocity(newVelocityX, newVelocityY) {}
	};

	VelocityAffector()
	    : velocitySteps_(StepsInitialSize) {}

	/// Affects the velocity of the specified particle
	void affect(Particle *particle, float normalizedAge) override;
	void addVelocityStep(float age, float velX, float velY);
	inline void addVelocityStep(float age, const Vector2f &velocity) { addVelocityStep(age, velocity.x, velocity.y); }

	inline nctl::Array<VelocityStep> &steps() { return velocitySteps_; }
	inline const nctl::Array<VelocityStep> &steps() const { return velocitySteps_; }

  private:
	nctl::Array<VelocityStep> velocitySteps_;
};

}

#endif
