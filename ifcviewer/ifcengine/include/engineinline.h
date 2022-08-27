#ifndef __RDF_LTD__ENGINE_INLINE_H
#define __RDF_LTD__ENGINE_INLINE_H


#include	<assert.h>

#include	"engine.h"


static	inline	char	* GetAssertionFile(
								)
{
	char	* fileName = nullptr;
	GetAssertionFile(
			&fileName
		);
	return	fileName;
}

static	inline	wchar_t	* GetAssertionFileW(
								)
{
	wchar_t	* fileName = nullptr;
	GetAssertionFileW(
			&fileName
		);
	return	fileName;
}

static	inline	char	* GetNameOfClass(
									int64_t				owlClass
								)
{
	char	* name = nullptr;
	GetNameOfClass(
			owlClass,
			&name
		);
	return	name;
}

static	inline	wchar_t	* GetNameOfClassW(
									int64_t				owlClass
								)
{
	wchar_t	* name = nullptr;
	GetNameOfClassW(
			owlClass,
			&name
		);
	return	name;
}

static	inline	char	* GetNameOfClassEx(
									int64_t				model,
									int64_t				owlClass
								)
{
	char	* name = nullptr;
	GetNameOfClassEx(
			model,
			owlClass,
			&name
		);
	return	name;
}

static	inline	wchar_t	* GetNameOfClassWEx(
									int64_t				model,
									int64_t				owlClass
								)
{
	wchar_t	* name = nullptr;
	GetNameOfClassWEx(
			model,
			owlClass,
			&name
		);
	return	name;
}

static	inline	char	* GetNameOfProperty(
									int64_t				rdfProperty
								)
{
	char	* name = nullptr;
	GetNameOfProperty(
			rdfProperty,
			&name
		);
	return	name;
}

static	inline	wchar_t	* GetNameOfPropertyW(
									int64_t				rdfProperty
								)
{
	wchar_t	* name = nullptr;
	GetNameOfPropertyW(
			rdfProperty,
			&name
		);
	return	name;
}

static	inline	char	* GetNameOfPropertyEx(
									int64_t				model,
									int64_t				rdfProperty
								)
{
	char	* name = nullptr;
	GetNameOfPropertyEx(
			model,
			rdfProperty,
			&name
		);
	return	name;
}

static	inline	wchar_t	* GetNameOfPropertyWEx(
									int64_t				model,
									int64_t				rdfProperty
								)
{
	wchar_t	* name = nullptr;
	GetNameOfPropertyWEx(
			model,
			rdfProperty,
			&name
		);
	return	name;
}

static	inline	int64_t	CreateInstance(
									int64_t				owlClass
								)
{
	const char	* name = nullptr;
	return		CreateInstance(
					owlClass,
					name
				);
}

static	inline	int64_t	CreateInstanceW(
									int64_t				owlClass
								)
{
	const wchar_t	* name = nullptr;
	return		CreateInstanceW(
					owlClass,
					name
				);
}

static	inline	int64_t	CreateInstanceEx(
									int64_t				model,
									int64_t				owlClass
								)
{
	const char	* name = nullptr;
	return		CreateInstanceEx(
					model,
					owlClass,
					name
				);
}

static	inline	int64_t	CreateInstanceWEx(
									int64_t				model,
									int64_t				owlClass
								)
{
	const wchar_t	* name = nullptr;
	return		CreateInstanceWEx(
					model,
					owlClass,
					name
				);
}

static	inline	char	* GetNameOfInstance(
									int64_t				owlInstance
								)
{
	char	* name = nullptr;
	GetNameOfInstance(
			owlInstance,
			&name
		);
	return	name;
}

static	inline	wchar_t	* GetNameOfInstanceW(
									int64_t				owlInstance
								)
{
	wchar_t	* name = nullptr;
	GetNameOfInstanceW(
			owlInstance,
			&name
		);
	return	name;
}

static	inline	char	* GetNameOfInstanceEx(
									int64_t				model,
									int64_t				owlInstance
								)
{
	char	* name = nullptr;
	GetNameOfInstanceEx(
			model,
			owlInstance,
			&name
		);
	return	name;
}

static	inline	wchar_t	* GetNameOfInstanceWEx(
									int64_t				model,
									int64_t				owlInstance
								)
{
	wchar_t	* name = nullptr;
	GetNameOfInstanceWEx(
			model,
			owlInstance,
			&name
		);
	return	name;
}

static	inline	int64_t	SetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									bool				value
								)
{
	assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					rdfProperty,
					(const void*) &value,
					card
				);
}

static	inline	int64_t	SetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const char			* value
								)
{
	assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_CHAR);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					rdfProperty,
					(const void*) &value,
					card
				);
}

static	inline	int64_t	SetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const wchar_t		* value
								)
{
	assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_CHAR);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					rdfProperty,
					(const void*) &value,
					card
				);
}

static	inline	int64_t	SetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									int64_t				value
								)
{
	assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_INTEGER);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					rdfProperty,
					(const void*) &value,
					card
				);
}

static	inline	int64_t	SetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									double				value
								)
{
	assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					rdfProperty,
					(const void*) &value,
					card
				);
}

static	inline	int64_t	SetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									unsigned char		value
								)
{
	assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BYTE);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					rdfProperty,
					(const void*) &value,
					card
				);
}

static	inline	int64_t	SetObjectProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									int64_t				value
								)
{
	assert(GetPropertyType(rdfProperty) == OBJECTPROPERTY_TYPE);
	const int64_t	card = 1;
	return	SetObjectProperty(
					owlInstance,
					rdfProperty,
					&value,
					card
				);
}

static	inline	double	GetArea(
									int64_t				owlInstance
								)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return		GetArea(
					owlInstance,
					vertexBuffer,
					indexBuffer
				);
}

static	inline	double	GetVolume(
									int64_t				owlInstance
								)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return		GetVolume(
					owlInstance,
					vertexBuffer,
					indexBuffer
				);
}

static	inline	double	* GetCenter(
									int64_t				owlInstance,
									double				* center
								)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return		GetCenter(
					owlInstance,
					vertexBuffer,
					indexBuffer,
					center
				);
}

static	inline	double	GetCentroid(
									int64_t				owlInstance,
									double				* centroid
								)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return		GetCentroid(
					owlInstance,
					vertexBuffer,
					indexBuffer,
					centroid
				);
}

static	inline	double	GetConceptualFaceArea(
									int64_t				conceptualFace
								)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return		GetConceptualFaceArea(
					conceptualFace,
					vertexBuffer,
					indexBuffer
				);
}


#endif
