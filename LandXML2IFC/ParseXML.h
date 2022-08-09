#pragma once


struct	STRUCT_ELEMENT_LIST;

struct	STRUCT_TAG
{
	char					* name;

	STRUCT_ELEMENT_LIST		* elements;
};

struct	STRUCT_ATTRIBUTE
{
	char					* key;
	char					* value;
};

struct	STRUCT_ATTRIBUTE_LIST
{
	STRUCT_ATTRIBUTE		* attribute;

	STRUCT_ATTRIBUTE_LIST	* next;
};

struct	STRUCT_ELEMENT
{
	STRUCT_TAG				* tag;

	STRUCT_ATTRIBUTE_LIST	* attributes;
	STRUCT_ELEMENT_LIST		* elements;
};

struct	STRUCT_ELEMENT_LIST
{
	char					* value;
	STRUCT_ELEMENT			* element;

	STRUCT_ELEMENT_LIST		* next;
};

struct	__VECTOR2
{
	double					x, y;
};


static	double	Sqr(
						double		value
					)
{
	return	value * value;
}

static	double	Vec2DistanceSqr(
						const __VECTOR2				* pV1,
						const __VECTOR2				* pV2
					)
{
	return	Sqr(pV1->x - pV2->x) + Sqr(pV1->y - pV2->y);
}

static	double	Vec2Distance(
						const __VECTOR2				* pV1,
						const __VECTOR2				* pV2
					)
{
	return	sqrt(
					Vec2DistanceSqr(
							pV1,
							pV2
						)
				);
}

STRUCT_ATTRIBUTE	* newATTRIBUTE(char * key);

STRUCT_ELEMENT_LIST	* ParseXML(
							char	* fileName
						);

bool	equals(char * txtI, char * txtII);
