#include "ncProfileVariable.h"
#include "ncServiceLocator.h"
#include <cstdlib> // per exit()

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

ncProfileVariable::ncProfileVariable(unsigned int uNumValues, unsigned int uRejectDelay)
	: m_uNumValues(uNumValues), m_uRejectDelay(uRejectDelay), m_uNextIndex(0),
	  m_fMin(0.0f), m_fMax(0.0f), m_fMean(0.0f), m_bFirstValue(true)
{
	if (m_uNumValues > 0)
		m_fValues = new float[m_uNumValues];
	else
	{
		ncServiceLocator::GetLogger().Write(ncILogger::LOG_FATAL, (char *)"ncProfileVariable::ncProfileVariable - Array size should be greater than zero");
		exit(-1);
	}
}

ncProfileVariable::~ncProfileVariable()
{
	if (m_fValues)
		delete[] m_fValues;
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void ncProfileVariable::AddValue(float fValue)
{	
	if (m_bFirstValue)
	{
		m_fMin = fValue;
		m_fMax = fValue;
		m_bFirstValue = false;

		if (m_uRejectDelay)
			m_timer.Start();
	}


	bool bRegisterValue = false;
	if (m_uRejectDelay)
	{
		// Timer expired
		if (m_timer.Interval() > m_uRejectDelay)
		{
			bRegisterValue = true;
			m_timer.Start();
		}
	}
	else
		bRegisterValue = true;

	// A new value has to be registered in the array
	if (bRegisterValue)
	{
		if (fValue > m_fMax)
			m_fMax = fValue;
		else if (fValue < m_fMin)
			m_fMin = fValue;

		m_fValues[m_uNextIndex] = fValue;
		m_fMean = 0.0f;
		for(int i = 0; i < 3; i++)
			m_fMean += m_fValues[(m_uNextIndex-i)%m_uNumValues];
		m_fMean /= 3.0f;

		m_uNextIndex = (m_uNextIndex+1)%m_uNumValues;
	}
}