#ifndef CLASS_NCINE_PARTICLESYSTEM
#define CLASS_NCINE_PARTICLESYSTEM

#include <ctime>
#include <cstdlib>
#include "Rect.h"
#include "SceneNode.h"
#include "ParticleAffectors.h"
#include "BaseSprite.h"

namespace ncine {

class Texture;
class Particle;
struct ParticleInitializer;

/// The class representing a particle system
class DLL_PUBLIC ParticleSystem : public SceneNode
{
  public:
	/// Constructs a particle system with the specified maximum amount of particles
	ParticleSystem(SceneNode *parent, unsigned int count, Texture *texture);
	/// Constructs a particle system with the specified maximum amount of particles and the specified texture rectangle
	ParticleSystem(SceneNode *parent, unsigned int count, Texture *texture, Recti texRect);
	~ParticleSystem() override;

	/// Default move constructor
	ParticleSystem(ParticleSystem &&);
	/// Default move assignment operator
	ParticleSystem &operator=(ParticleSystem &&);

	/// Returns a copy of this object
	inline ParticleSystem clone() const { return ParticleSystem(*this); }

	/// Adds a particle affector
	inline void addAffector(nctl::UniquePtr<ParticleAffector> affector) { affectors_.pushBack(nctl::move(affector)); }
	/// Deletes all particle affectors
	void clearAffectors();
	/// Emits particles with the specified initialization parameters
	void emitParticles(const ParticleInitializer &init);
	/// Kills all alive particles
	void killParticles();

	/// Returns the local space flag of the system
	inline bool inLocalSpace(void) const { return inLocalSpace_; }
	/// Sets the local space flag of the system
	inline void setInLocalSpace(bool inLocalSpace) { inLocalSpace_ = inLocalSpace; }

	/// Returns the total number of particles in the system
	inline unsigned int numParticles() const { return particleArray_.size(); }
	/// Returns the number of particles currently alive
	inline unsigned int numAliveParticles() const { return particleArray_.size() - poolTop_ - 1; }

	/// Sets the texture object for every particle
	void setTexture(Texture *texture);
	/// Sets the texture source rectangle for every particle
	void setTexRect(const Recti &rect);

	/// Sets the transformation anchor point for every particle
	void setAnchorPoint(float xx, float yy);
	/// Sets the transformation anchor point for every particle with a `Vector2f`
	void setAnchorPoint(const Vector2f &point);

	/// Flips the texture rect horizontally for every particle
	void setFlippedX(bool flippedX);
	/// Flips the texture rect vertically for every particle
	void setFlippedY(bool flippedY);

	/// Sets the blending factors preset for every particle
	void setBlendingPreset(DrawableNode::BlendingPreset blendingPreset);
	/// Sets the source and destination blending factors for every particle
	void setBlendingFactors(DrawableNode::BlendingFactor srcBlendingFactor, DrawableNode::BlendingFactor destBlendingFactor);

	/// Sets the rendering layer for every particle
	void setLayer(unsigned short layer);

	void update(float interval) override;

	inline static ObjectType sType() { return ObjectType::PARTICLE_SYSTEM; }

  protected:
	/// Protected copy constructor used to clone objects
	ParticleSystem(const ParticleSystem &other);

  private:
	/// The particle pool size
	unsigned int poolSize_;
	/// The index of the next free particle in the pool
	int poolTop_;
	/// The pool containing available particles (only dead ones)
	nctl::Array<Particle *> particlePool_;
	/// The array containing every particle (dead or alive)
	nctl::Array<nctl::UniquePtr<Particle>> particleArray_;

	/// The array of particle affectors
	nctl::Array<nctl::UniquePtr<ParticleAffector>> affectors_;

	/// A flag indicating whether the system should be simulated in local space
	bool inLocalSpace_;

	/// Deleted assignment operator
	ParticleSystem &operator=(const ParticleSystem &) = delete;
};

}

#endif
