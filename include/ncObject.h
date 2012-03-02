#ifndef CLASS_NCOBJECT
#define CLASS_NCOBJECT

#include <cstring>
#include "ncServiceLocator.h"

/// Maximum length for an object name
static const int NC_OBJNAME_LENGTH = 128;

/// Static RRTI and identification index
class ncObject
{
private:
	/// Object identification in the indexer
	unsigned int m_uId;

	/// Object name
	/** This field is currently only useful in debug,
	as there's still no string hashing based search. */
	char m_vName[NC_OBJNAME_LENGTH];

protected:
	/// The enumeration of object types
	enum eObjectType {
		BASE_TYPE = 0,
		TEXTURE_TYPE,
		SCENENODE_TYPE,
		SPRITE_TYPE,
		SPRITEBATCH_TYPE,
		PARTICLESYSTEM_TYPE,
		TEXT_TYPE,
		ANIMATEDSPRITE_TYPE,
		AUDIOBUFFER_TYPE,
		AUDIOBUFFERPLAYER_TYPE,
		AUDIOSTREAMPLAYER_TYPE
	};

	/// Object type
	eObjectType m_eType;

public:
	ncObject() : m_uId(0), m_eType(BASE_TYPE)
	{
		memset(m_vName, 0, NC_OBJNAME_LENGTH);
		m_uId = ncServiceLocator::Indexer().AddObject(this);
	}
	virtual ~ncObject() { ncServiceLocator::Indexer().RemoveObject(m_uId); }

	/// Returns the object identification number
	inline unsigned int Id() const { return m_uId; }

	/// Returns the object type (RTTI)
	eObjectType Type() const { return m_eType; }
	/// Static method to return class type
	inline static eObjectType sType() { return BASE_TYPE; }

	/// Returns object name
	char const * const Name() const { return m_vName; }
	/// Sets the object name
	void SetName(const char vName[NC_OBJNAME_LENGTH]) { strncpy(m_vName, vName, NC_OBJNAME_LENGTH); }

	/// Returns a casted pointer to the object with the specified id, if any exists
	template <class T>
	static T* FromId(unsigned int uId)
	{
		ncObject *pObject = ncServiceLocator::Indexer().Object(uId);

		if(pObject)
		{
			if (pObject->m_eType == T::sType())
				return static_cast<T *>(pObject);
			else // Cannot cast
			{
				ncServiceLocator::Logger().Write(ncILogger::LOG_FATAL, "ncObject::FromId - Object \"%s\" (%u) is of type %u instead of %u", pObject->m_vName, uId, pObject->m_eType, T::sType());
				return NULL;
			}
		}
		else
		{
			ncServiceLocator::Logger().Write(ncILogger::LOG_WARN, "ncObject::FromId - Object %u not found", uId);
			return NULL;
		}
	}
};

#endif
