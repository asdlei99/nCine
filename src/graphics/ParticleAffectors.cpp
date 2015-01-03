#include "ParticleAffectors.h"
#include "Color.h"
#include "Particle.h"
#include "Array.h"

namespace ncine {

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void AccelerationAffector::affect(Particle *particle)
{
	particle->velocity_ += acceleration_;
}

void ColorAffector::addColorStep(float time, Color color)
{
	if (colorSteps_.isEmpty() || time > colorSteps_[colorSteps_.size() - 1]->time)
	{
		colorSteps_.insertBack(new ColorStep(time, color));
	}
	else
	{
		LOGW("Out of order step not added");
	}
}

void ColorAffector::affect(Particle* particle)
{
	unsigned int index;
	float normalizedLife = particle->life_ / particle->startingLife;

	for (index = 0; index < colorSteps_.size() - 1; index++)
	{
		if (colorSteps_[index]->time > normalizedLife)
		{
			break;
		}
	}

//	assert(colorSteps_[index - 1]->time <= normalizedLife);

	float prevTime = colorSteps_[index - 1]->time;
	float thisTime = colorSteps_[index]->time;
	Color &prevColor = colorSteps_[index - 1]->color;
	Color &thisColor = colorSteps_[index]->color;

	float factor = (normalizedLife - prevTime) / (thisTime - prevTime);
	GLubyte red = static_cast<GLubyte>(prevColor.r() + (thisColor.r() - prevColor.r()) * factor);
	GLubyte green = static_cast<GLubyte>(prevColor.g() + (thisColor.g() - prevColor.g()) * factor);
	GLubyte blue = static_cast<GLubyte>(prevColor.b() + (thisColor.b() - prevColor.b()) * factor);
	GLubyte alpha = static_cast<GLubyte>(prevColor.a() + (thisColor.a() - prevColor.a()) * factor);

	particle->setColor(red, green, blue, alpha);
}

void SizeAffector::addSizeStep(float time, float scale)
{
	if (sizeSteps_.isEmpty() || time > sizeSteps_[sizeSteps_.size() - 1]->time)
	{
		sizeSteps_.insertBack(new SizeStep(time, scale));
	}
	else
	{
		LOGW("Out of order step not added");
	}
}

void SizeAffector::affect(Particle* particle)
{
	unsigned int index;
	float normalizedLife = particle->life_ / particle->startingLife;

	for (index = 0; index < sizeSteps_.size() - 1; index++)
	{
		if (sizeSteps_[index]->time > normalizedLife)
		{
			break;
		}
	}

//	assert(sizeSteps[index - 1]->time <= normalizedLife);

	float prevTime = sizeSteps_[index - 1]->time;
	float thisTime = sizeSteps_[index]->time;
	float prevScale = sizeSteps_[index - 1]->scale;
	float thisScale = sizeSteps_[index]->scale;

	float factor = (normalizedLife - prevTime) / (thisTime - prevTime);
	float newScale = prevScale + (thisScale - prevScale) * factor;

	particle->setScale(baseScale_ * newScale);
}

}