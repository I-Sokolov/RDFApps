//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: IFC Engine Library
//
//  Be aware a license fee for use of this library when used commercially is required
//  For more info on commercial use please contact:  peter.bonsma@rdf.bg / contact@rdf.bg
//

#ifndef __RDF_LTD__IFCENGINE_H
#define __RDF_LTD__IFCENGINE_H


#include	"engdef.h"

#include	<assert.h>
#include	<string.h>

#if !defined _WINDOWS
#include	<stdint.h>
#endif


#ifdef _WINDOWS
	#ifdef _USRDLL
		#define DECL //__declspec(dllexport)
		//	The use of the Microsoft-specific
		//	__declspec(dllexport)
		//	is needed only when
		//	not using a .def file.
	#else
		#define DECL __declspec(dllimport) 
	#endif
	#define STDC __stdcall
#else
	#define DECL /*nothing*/
	#define STDC /*nothing*/
#endif


#define		sdaiADB					1
#define		sdaiAGGR				sdaiADB + 1
#define		sdaiBINARY				sdaiAGGR + 1
#define		sdaiBOOLEAN				sdaiBINARY + 1
#define		sdaiENUM				sdaiBOOLEAN + 1
#define		sdaiINSTANCE			sdaiENUM + 1
#define		sdaiINTEGER				sdaiINSTANCE + 1
#define		sdaiLOGICAL				sdaiINTEGER + 1
#define		sdaiREAL				sdaiLOGICAL + 1
#define		sdaiSTRING				sdaiREAL + 1
#define		sdaiUNICODE				sdaiSTRING + 1
#define		sdaiEXPRESSSTRING		sdaiUNICODE + 1
#define		engiGLOBALID			sdaiEXPRESSSTRING + 1
#define		sdaiNUMBER				engiGLOBALID + 1

#define		engiTypeFlagAggr		128
#define		engiTypeFlagAggrOption	32


typedef void(*LOGCB)(const char *);

/*	Note on sdaiSTRING and sdaiUNICODE


	sdaiUNICODE
		this will convert all internal strings from/too unicode, the internal representation and what is written to the IFC file is mapped
			"\" will be converted into "\\" to enable basic parses to still interpret file paths
			"'" will be converted to \X1\hh\X0\ or \X2\00hh\X0\ to prevent basic interpreters to read files with strings containing these characters

	sdaiSTRING
		this will leave all information as is in the IFC file, the rules are that char's ( int ) 32 to 126 (inclusive) will be kept
		all other strings will be converted to \X1\hh\X0\ or \X2\00hh\X0\		*/


#define		sdaiARRAY				1
#define		sdaiLIST				2
#define		sdaiSET					3
#define		sdaiBAG					4

typedef	void			* SchemaTypeIterator;
typedef	int_t			SchemaDecl;
typedef SchemaDecl		SchemaTypeDecl;		//	ENTITY or TYPE declaration
typedef SchemaDecl		ExpressScript;		//	script declarations include where rules and derived attributes expressions, EXPRESS functions, procedures, schema rules
typedef void			* UniqueRule;

typedef	int_t			SdaiPrimitiveType;
typedef	int_t			SdaiInteger;
typedef	double			SdaiReal;
typedef	unsigned char	SdaiBoolean;
typedef	SchemaTypeDecl	SdaiEntity;
typedef	int_t			SdaiInstance;
typedef	int_t			SdaiModel;
typedef	int_t			SdaiRep;
typedef	const char		* SdaiString;
typedef	void			* SdaiADB;
typedef	void			* SdaiNPL;
typedef	int_t			* SdaiAggr;
typedef	SdaiAggr		SdaiArray;
typedef	void			* SdaiIterator;
typedef	int_t			SdaiAggrIndex;
typedef	void			* SdaiAttr;
typedef	void			* SchemaAggr;

typedef	int64_t			ExpressID;

typedef	void			* ValidationResults;
typedef	void			* ValidationIssue;
typedef	int_t			ValidationIssueLevel;

#define	sdaiFALSE		((SdaiInteger) 0)
#define	sdaiTRUE		((SdaiInteger) 1)
#define	sdaiUNKNOWN		((SdaiInteger) 2)


#ifdef __cplusplus
enum class enum_string_encoding : unsigned char
{
	IGNORE_DEFAULT				= (0 + 0 * 2 + 0 * 4 + 0 * 8 + 0 * 16 + 0 * 32),	//		 0   0   0   0   0   0
	WINDOWS_1250				= (0 + 0 * 2 + 1 * 4 + 0 * 8 + 0 * 16 + 0 * 32),	//		 0   0   1   0   0   0
	WINDOWS_1251				= (0 + 0 * 2 + 0 * 4 + 1 * 8 + 0 * 16 + 0 * 32),	//		 0   0   0   1   0   0
	WINDOWS_1252				= (0 + 0 * 2 + 1 * 4 + 1 * 8 + 0 * 16 + 0 * 32),	//		 0   0   1   1   0   0
	WINDOWS_1253				= (0 + 0 * 2 + 0 * 4 + 0 * 8 + 1 * 16 + 0 * 32),	//		 0   0   0   0   1   0
	WINDOWS_1254				= (0 + 0 * 2 + 1 * 4 + 0 * 8 + 1 * 16 + 0 * 32),	//		 0   0   1   0   1   0
	WINDOWS_1255				= (0 + 0 * 2 + 0 * 4 + 1 * 8 + 1 * 16 + 0 * 32),	//		 0   0   0   1   1   0
	WINDOWS_1256				= (0 + 0 * 2 + 1 * 4 + 1 * 8 + 1 * 16 + 0 * 32),	//		 0   0   1   1   1   0
	WINDOWS_1257				= (0 + 0 * 2 + 0 * 4 + 0 * 8 + 0 * 16 + 1 * 32),	//		 0   0   0   0   0   1
	WINDOWS_1258				= (0 + 0 * 2 + 1 * 4 + 0 * 8 + 0 * 16 + 1 * 32),	//		 0   0   1   0   0   1
	ISO8859_1					= (1 + 0 * 2 + 0 * 4 + 0 * 8 + 0 * 16 + 0 * 32),	//		 1   0   0   0   0   0
	ISO8859_2					= (1 + 0 * 2 + 1 * 4 + 0 * 8 + 0 * 16 + 0 * 32),	//		 1   0   1   0   0   0
	ISO8859_3					= (1 + 0 * 2 + 0 * 4 + 1 * 8 + 0 * 16 + 0 * 32),	//		 1   0   0   1   0   0
	ISO8859_4					= (1 + 0 * 2 + 1 * 4 + 1 * 8 + 0 * 16 + 0 * 32),	//		 1   0   1   1   0   0
	ISO8859_5					= (1 + 0 * 2 + 0 * 4 + 0 * 8 + 1 * 16 + 0 * 32),	//		 1   0   0   0   1   0
	ISO8859_6					= (1 + 0 * 2 + 1 * 4 + 0 * 8 + 1 * 16 + 0 * 32),	//		 1   0   1   0   1   0
	ISO8859_7					= (1 + 0 * 2 + 0 * 4 + 1 * 8 + 1 * 16 + 0 * 32),	//		 1   0   0   1   1   0
	ISO8859_8					= (1 + 0 * 2 + 1 * 4 + 1 * 8 + 1 * 16 + 0 * 32),	//		 1   0   1   1   1   0
	ISO8859_9					= (1 + 0 * 2 + 0 * 4 + 0 * 8 + 0 * 16 + 1 * 32),	//		 1   0   0   0   0   1
	ISO8859_10					= (1 + 0 * 2 + 1 * 4 + 0 * 8 + 0 * 16 + 1 * 32),	//		 1   0   1   0   0   1
	ISO8859_11					= (1 + 0 * 2 + 0 * 4 + 1 * 8 + 0 * 16 + 1 * 32),	//		 1   0   0   1   0   1
	ISO8859_13					= (1 + 0 * 2 + 0 * 4 + 0 * 8 + 1 * 16 + 1 * 32),	//		 1   0   0   0   1   1
	ISO8859_14					= (1 + 0 * 2 + 1 * 4 + 0 * 8 + 1 * 16 + 1 * 32),	//		 1   0   1   0   1   1
	ISO8859_15					= (1 + 0 * 2 + 0 * 4 + 1 * 8 + 1 * 16 + 1 * 32),	//		 1   0   0   1   1   1
	ISO8859_16					= (1 + 0 * 2 + 1 * 4 + 1 * 8 + 1 * 16 + 1 * 32),	//		 1   0   1   1   1   1
	MACINTOSH_CENTRAL_EUROPEAN	= (0 + 1 * 2 + 0 * 4 + 0 * 8 + 0 * 16 + 0 * 32),	//		 0   1   0   0   0   0
	SHIFT_JIS_X_213				= (1 + 1 * 2 + 0 * 4 + 0 * 8 + 0 * 16 + 0 * 32),    //		 1   1   0   0   0   0
	UTF8						= (1 + 1 * 2 + 0 * 4 + 0 * 8 + 0 * 16 + 1 * 32)     //		 1   1   0   0   0   1
};

enum class enum_express_declaration : unsigned char
{
	__NONE						= 0,
	__ENTITY					= 1,
	__ENUM						= 2,
	__SELECT					= 3,
	__DEFINED_TYPE				= 4,
	__FUNCTION					= 5,
	__PROCEDURE					= 6,
	__GLOBAL_RULE				= 7,
	__WHERE_RULE				= 8
};

enum class enum_express_attr_type : unsigned char
{
	__NONE						= 0,					//	attribute type is unknown here but it may be defined by referenced domain entity
	__BINARY					= 1,
	__BINARY_32					= 2,
	__BOOLEAN					= 3,
	__ENUMERATION				= 4,
	__INTEGER					= 5,
	__LOGICAL					= 6,
	__NUMBER					= 7,
	__REAL						= 8,
	__SELECT					= 9,
	__STRING					= 10,
	__GENERIC					= 11
};

enum class enum_express_aggr : unsigned char
{
	__NONE						= 0,
	__ARRAY						= 1,
	__BAG						= 2,
	__LIST						= 3,
	__SET						= 4,
	__AGGREGATE					= 5						//	generic aggregate
};

enum class enum_validation_type : uint64_t
{
	__NONE						= 0,
	__KNOWN_ENTITY				= 1 << 0,				//  entity is defined in the schema
	__NO_OF_ARGUMENTS			= 1 << 1,				//	number of arguments
	__ARGUMENT_EXPRESS_TYPE		= 1 << 2,				//	argument value is correct entity, defined type or enumeration value
	__ARGUMENT_PRIM_TYPE		= 1 << 3,				//	argument value has correct primitive type
	__REQUIRED_ARGUMENTS		= 1 << 4,				//	non-optional arguments values are provided
	__ARRGEGATION_EXPECTED		= 1 << 5,				//	aggregation is provided when expected
	__AGGREGATION_NOT_EXPECTED	= 1 << 6,   			//	aggregation is not used when not expected
	__AGGREGATION_SIZE			= 1 << 7,   			//	aggregation size
	__AGGREGATION_UNIQUE		= 1 << 8,				//	elements in aggregations are unique when required
	__COMPLEX_INSTANCE			= 1 << 9,				//	complex instances contains full parent chains
	__REFERENCE_EXISTS			= 1 << 10,				//	referenced instance exists
	__ABSTRACT_ENTITY			= 1 << 11,  			//	abstract entity should not instantiate
	__WHERE_RULE				= 1 << 12,  			//	where-rule check
	__UNIQUE_RULE				= 1 << 13,				//	unique-rule check
	__STAR_USAGE				= 1 << 14,  			//	* is used only for derived arguments
	__CALL_ARGUMENT				= 1 << 15,  			//	validateModel / validateInstance function argument should be model / instance
	__INVALID_TEXT_LITERAL		= 1 << 16,				//	invalid text literal string
	__INTERNAL_ERROR			= UINT64_C(1) << 63   	//	unspecified error
};

enum class enum_validation_status : unsigned char
{
	__NONE						= 0,
	__COMPLETE_ALL				= 1,					//	all issues proceed
	__COMPLETE_NOT_ALL			= 2,					//	completed but some issues were excluded by option settings
	__TIME_EXCEED				= 3,					//	validation was finished because of reach time limit
	__COUNT_EXCEED				= 4						//	validation was finished because of reach of issue's numbers limit
};
#else	//	__cplusplus
	typedef unsigned char bool;
	typedef unsigned char enum_string_encoding;
	typedef unsigned char enum_express_declaration;
	typedef unsigned char enum_express_attr_type;
	typedef unsigned char enum_express_aggr;
	typedef uint64_t	  enum_validation_type;
	typedef unsigned char enum_validation_status;
#endif	//	__cplusplus


//
//  Instance Header API Calls
//

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetSPFFHeader                                           (http://rdf.bg/ifcdoc/CP64/SetSPFFHeader.html)
//				SdaiModel				model								IN
//				const char				* description						IN
//				const char				* implementationLevel				IN
//				const char				* name								IN
//				const char				* timeStamp							IN
//				const char				* author							IN
//				const char				* organization						IN
//				const char				* preprocessorVersion				IN
//				const char				* originatingSystem					IN
//				const char				* authorization						IN
//				const char				* fileSchema						IN
//
//				void					returns
//
//	This call is an aggregate of several SetSPFFHeaderItem calls. In several cases the header can be set easily with this call. In case an argument is zero, this argument will not be updated, i.e. it will not be filled with 0.
//
void			DECL STDC	SetSPFFHeader(
									SdaiModel				model,
									const char				* description,
									const char				* implementationLevel,
									const char				* name,
									const char				* timeStamp,
									const char				* author,
									const char				* organization,
									const char				* preprocessorVersion,
									const char				* originatingSystem,
									const char				* authorization,
									const char				* fileSchema
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	SetSPFFHeader(
								SdaiModel				model,
								char					* description,
								char					* implementationLevel,
								char					* name,
								char					* timeStamp,
								char					* author,
								char					* organization,
								char					* preprocessorVersion,
								char					* originatingSystem,
								char					* authorization,
								char					* fileSchema
							)
{
	return	SetSPFFHeader(
					model,
					(const char*) description,
					(const char*) implementationLevel,
					(const char*) name,
					(const char*) timeStamp,
					(const char*) author,
					(const char*) organization,
					(const char*) preprocessorVersion,
					(const char*) originatingSystem,
					(const char*) authorization,
					(const char*) fileSchema
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		SetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CP64/SetSPFFHeaderItem.html)
//				SdaiModel				model								IN
//				int_t					itemIndex							IN
//				int_t					itemSubIndex						IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				int_t					returns								OUT
//
//	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t			DECL STDC	SetSPFFHeaderItem(
									SdaiModel				model,
									int_t					itemIndex,
									int_t					itemSubIndex,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	SetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								const char				* value
							)
{
	return	SetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void*) value
				);
}

//
//
static	inline	int_t	SetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								char					* value
							)
{
	return	SetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void*) value
				);
}

//
//
static	inline	int_t	SetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								const wchar_t			* value
							)
{
	return	SetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void*) value
				);
}

//
//
static	inline	int_t	SetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								wchar_t					* value
							)
{
	return	SetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void*) value
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		GetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CP64/GetSPFFHeaderItem.html)
//				SdaiModel				model								IN
//				int_t					itemIndex							IN
//				int_t					itemSubIndex						IN
//				SdaiPrimitiveType		valueType							IN
//				const void				** value							IN / OUT
//
//				int_t					returns								OUT
//
//	This call can be used to read a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t			DECL STDC	GetSPFFHeaderItem(
									SdaiModel				model,
									int_t					itemIndex,
									int_t					itemSubIndex,
									SdaiPrimitiveType		valueType,
									const void				** value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	GetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								const char				** value
							)
{
	return	GetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void**) value
				);
}

//
//
static	inline	int_t	GetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								char					** value
							)
{
	return	GetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void**) value
				);
}

//
//
static	inline	int_t	GetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								const wchar_t			** value
							)
{
	return	GetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void**) value
				);
}

//
//
static	inline	int_t	GetSPFFHeaderItem(
								SdaiModel				model,
								int_t					itemIndex,
								int_t					itemSubIndex,
								SdaiPrimitiveType		valueType,
								wchar_t					** value
							)
{
	return	GetSPFFHeaderItem(
					model,
					itemIndex,
					itemSubIndex,
					valueType,
					(const void**) value
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		GetDateTime                                             (http://rdf.bg/ifcdoc/CP64/GetDateTime.html)
//				SdaiModel				model								IN
//				const char				** dateTimeStamp					IN / OUT
//
//				const char				* returns							OUT
//
//	Returns an current date and time according to ISO 8601 without time zone, i.e. formatted as '2099-12-31T23:59:59'.
//
const char		DECL * STDC	GetDateTime(
									SdaiModel				model,
									const char				** dateTimeStamp
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* GetDateTime(
									SdaiModel				model,
									char					** dateTimeStamp
								)
{
	return	GetDateTime(
					model,
					(const char**) dateTimeStamp
				);
}

//
//
static	inline	const char	* GetDateTime(
									SdaiModel				model
								)
{
	return	GetDateTime(
					model,
					(const char**) nullptr				//	dateTimeStamp
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		GetLibraryIdentifier                                    (http://rdf.bg/ifcdoc/CP64/GetLibraryIdentifier.html)
//				const char				** libraryIdentifier				IN / OUT
//
//				const char				* returns							OUT
//
//	Returns an identifier for the current instance of this library including date stamp and revision number.
//
const char		DECL * STDC	GetLibraryIdentifier(
									const char				** libraryIdentifier
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* GetLibraryIdentifier(
									char					** libraryIdentifier
								)
{
	return	GetLibraryIdentifier(
					(const char**) libraryIdentifier
				);
}

//
//
static	inline	const char	* GetLibraryIdentifier(
								)
{
	return	GetLibraryIdentifier(
					(const char**) nullptr				//	libraryIdentifier
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		GetSchemaName                                           (http://rdf.bg/ifcdoc/CP64/GetSchemaName.html)
//				SdaiModel				model								IN
//				const char				** schemaName						IN / OUT
//
//				const char				* returns							OUT
//
//	Returns the value as defined by SCHEMA in the loaded EXPRESS schema.
//
const char		DECL * STDC	GetSchemaName(
									SdaiModel				model,
									const char				** schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* GetSchemaName(
									SdaiModel				model,
									char					** schemaName
								)
{
	return	GetSchemaName(
					model,
					(const char**) schemaName
				);
}

//
//
static	inline	const char	* GetSchemaName(
									SdaiModel				model
								)
{
	return	GetSchemaName(
					model,
					(const char**) nullptr				//	schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiSetMappingSupport                                   (http://rdf.bg/ifcdoc/CP64/engiSetMappingSupport.html)
//				SdaiEntity				entity								IN
//				bool					enable								IN
//
//				bool					returns								OUT
//
//	...
//
bool			DECL STDC	engiSetMappingSupport(
									SdaiEntity				entity,
									bool					enable
								);

//
//		engiGetMappingSupport                                   (http://rdf.bg/ifcdoc/CP64/engiGetMappingSupport.html)
//				SdaiEntity				entity								IN
//
//				bool					returns								OUT
//
//	...
//
bool			DECL STDC	engiGetMappingSupport(
									SdaiEntity				entity
								);

//
//  File IO API Calls
//

//
//		sdaiCreateModelBN                                       (http://rdf.bg/ifcdoc/CP64/sdaiCreateModelBN.html)
//				SdaiRep					repository							IN
//				const char				* fileName							IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function creates and empty model (we expect with a schema file given).
//	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiCreateModelBN(
									SdaiRep					repository,
									const char				* fileName,
									const char				* schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiModel	sdaiCreateModelBN(
									SdaiRep					repository,
									char					* fileName,
									char					* schemaName
								)
{
	return	sdaiCreateModelBN(
					repository,
					(const char*) fileName,
					(const char*) schemaName
				);
}

//
//
static	inline	SdaiModel	sdaiCreateModelBN(
									const char				* schemaName
								)
{
	SdaiModel	model = sdaiCreateModelBN(
								0,
								nullptr,
								schemaName
							);

	//	HEADER;
	//	FILE_DESCRIPTION(('ViewDefinition [ReferenceView]'), '2;1');
	//	FILE_NAME('Header example.ifc', '2099-12-31T23:59:59', ('Peter Bonsma'), ('RDF Ltd.'), 'IFC Engine Library, revision 9999, 2099-12-31T23:59:59', 'Company - Application - 1.0.0.0', 'none');
	//	FILE_SCHEMA(('IFC4X3_ADD2'));
	//	ENDSEC;

	//  set Description
	//SetSPFFHeaderItem(model, 0, 0, sdaiSTRING, "ViewDefinition [ReferenceView]");

	//  set Implementation Level
	SetSPFFHeaderItem(model, 1, 0, sdaiSTRING, "2;1");

	//  set Name
	//SetSPFFHeaderItem(model, 2, 0, sdaiSTRING, "Header example.ifc");

	//  set Time Stamp
	SetSPFFHeaderItem(model, 3, 0, sdaiSTRING, GetDateTime(model));			//	'2099-12-31T23:59:59'

	//  set Author
	//SetSPFFHeaderItem(model, 4, 0, sdaiSTRING, "Peter Bonsma");

	//  set Organization
	//SetSPFFHeaderItem(model, 5, 0, sdaiSTRING, "RDF Ltd.");

	//	set Preprocessor Version
	SetSPFFHeaderItem(model, 6, 0, sdaiSTRING, GetLibraryIdentifier());		//	'IFC Engine Library, revision 9999, 2099-12-31T23:59:59'

	//  set Originating System
	//SetSPFFHeaderItem(model, 7, 0, sdaiSTRING, "Company - Application - 1.0.0.0");

	//  set Authorization
	SetSPFFHeaderItem(model, 8, 0, sdaiSTRING, "none");

	//	set File Schema
	SetSPFFHeaderItem(model, 9, 0, sdaiSTRING, GetSchemaName(model));		//	'IFC4X3_ADD2'

	return	model;
}

//
//
static	inline	SdaiModel	sdaiCreateModelBN(
									char					* schemaName
								)
{
	return	sdaiCreateModelBN(
					(const char*) schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiCreateModelBNUnicode                                (http://rdf.bg/ifcdoc/CP64/sdaiCreateModelBNUnicode.html)
//				SdaiRep					repository							IN
//				const wchar_t			* fileName							IN
//				const wchar_t			* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function creates and empty model (we expect with a schema file given).
//	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiCreateModelBNUnicode(
									SdaiRep					repository,
									const wchar_t			* fileName,
									const wchar_t			* schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiModel	sdaiCreateModelBNUnicode(
									SdaiRep					repository,
									wchar_t					* fileName,
									wchar_t					* schemaName
								)
{
	return	sdaiCreateModelBNUnicode(
					repository,
					(const wchar_t*) fileName,
					(const wchar_t*) schemaName
				);
}

//
//
static	inline	SdaiModel	sdaiCreateModelBNUnicode(
									const wchar_t			* schemaName
								)
{
	SdaiModel	model = sdaiCreateModelBNUnicode(
								0,
								nullptr,
								schemaName
							);

	//	HEADER;
	//	FILE_DESCRIPTION(('ViewDefinition [ReferenceView]'), '2;1');
	//	FILE_NAME('Header example.ifc', '2099-12-31T23:59:59', ('Peter Bonsma'), ('RDF Ltd.'), 'IFC Engine Library, revision 9999, 2099-12-31T23:59:59', 'Company - Application - 1.0.0.0', 'none');
	//	FILE_SCHEMA(('IFC4X3_ADD2'));
	//	ENDSEC;

	//  set Description
	//SetSPFFHeaderItem(model, 0, 0, sdaiSTRING, "ViewDefinition [ReferenceView]");

	//  set Implementation Level
	SetSPFFHeaderItem(model, 1, 0, sdaiUNICODE, L"2;1");

	//  set Name
	//SetSPFFHeaderItem(model, 2, 0, sdaiUNICODE, L"Header example.ifc");

	//  set Time Stamp
	SetSPFFHeaderItem(model, 3, 0, sdaiSTRING, GetDateTime(model));			//	'2099-12-31T23:59:59'

	//  set Author
	//SetSPFFHeaderItem(model, 4, 0, sdaiUNICODE, L"Peter Bonsma");

	//  set Organization
	//SetSPFFHeaderItem(model, 5, 0, sdaiUNICODE, L"RDF Ltd.");

	//	set Preprocessor Version
	SetSPFFHeaderItem(model, 6, 0, sdaiSTRING, GetLibraryIdentifier());		//	'IFC Engine Library, revision 9999, 2099-12-31T23:59:59'

	//  set Originating System
	//SetSPFFHeaderItem(model, 7, 0, sdaiUNICODE, L"Company - Application - 1.0.0.0");

	//  set Authorization
	SetSPFFHeaderItem(model, 8, 0, sdaiUNICODE, L"none");

	//	set File Schema
	SetSPFFHeaderItem(model, 9, 0, sdaiSTRING, GetSchemaName(model));		//	'IFC4X3_ADD2'

	return	model;
}

//
//
static	inline	SdaiModel	sdaiCreateModelBNUnicode(
									wchar_t					* schemaName
								)
{
	return	sdaiCreateModelBNUnicode(
					(const wchar_t*) schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiOpenModelBN                                         (http://rdf.bg/ifcdoc/CP64/sdaiOpenModelBN.html)
//				SdaiRep					repository							IN
//				const char				* fileName							IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model on location fileName.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiOpenModelBN(
									SdaiRep					repository,
									const char				* fileName,
									const char				* schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiModel	sdaiOpenModelBN(
									SdaiRep					repository,
									char					* fileName,
									char					* schemaName
								)
{
	return	sdaiOpenModelBN(
					repository,
					(const char*) fileName,
					(const char*) schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiOpenModelBNUnicode                                  (http://rdf.bg/ifcdoc/CP64/sdaiOpenModelBNUnicode.html)
//				SdaiRep					repository							IN
//				const wchar_t			* fileName							IN
//				const wchar_t			* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model on location fileName.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiOpenModelBNUnicode(
									SdaiRep					repository,
									const wchar_t			* fileName,
									const wchar_t			* schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiModel	sdaiOpenModelBNUnicode(
									SdaiRep					repository,
									wchar_t					* fileName,
									wchar_t					* schemaName
								)
{
	return	sdaiOpenModelBNUnicode(
					repository,
					(const wchar_t*) fileName,
					(const wchar_t*) schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiOpenModelByStream                                   (http://rdf.bg/ifcdoc/CP64/engiOpenModelByStream.html)
//				SdaiRep					repository							IN
//				const void				* callback							IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model via a stream.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	engiOpenModelByStream(
									SdaiRep					repository,
									const void				* callback,
									const char				* schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiModel	engiOpenModelByStream(
									SdaiRep					repository,
									const void				* callback,
									char					* schemaName
								)
{
	return	engiOpenModelByStream(
					repository,
					callback,
					(const char*) schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiOpenModelByArray                                    (http://rdf.bg/ifcdoc/CP64/engiOpenModelByArray.html)
//				SdaiRep					repository							IN
//				const unsigned char		* content							IN
//				int_t					size								IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model via an array.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	engiOpenModelByArray(
									SdaiRep					repository,
									const unsigned char		* content,
									int_t					size,
									const char				* schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiModel	engiOpenModelByArray(
									SdaiRep					repository,
									const unsigned char		* content,
									int_t					size,
									char					* schemaName
								)
{
	return	engiOpenModelByArray(
					repository,
					content,
					size,
					(const char*) schemaName
				);
}

//
//
static	inline	SdaiModel	engiOpenModelByArray(
									SdaiRep					repository,
									const unsigned char		* content,
									const char				* schemaName
								)
{
	return	engiOpenModelByArray(
					repository,
					content,
					strlen((const char*) content),	//	size
					schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiSaveModelBN                                         (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model (char file name).
//
void			DECL STDC	sdaiSaveModelBN(
									SdaiModel				model,
									const char				* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelBN(
								SdaiModel				model,
								char					* fileName
							)
{
	return	sdaiSaveModelBN(
					model,
					(const char*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiSaveModelBNUnicode                                  (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelBNUnicode(
								SdaiModel				model,
								wchar_t					* fileName
							)
{
	return	sdaiSaveModelBNUnicode(
					model,
					(const wchar_t*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiSaveModelByStream                                   (http://rdf.bg/ifcdoc/CP64/engiSaveModelByStream.html)
//				SdaiModel				model								IN
//				const void				* callback							IN
//				int_t					size								IN
//
//				void					returns
//
//	This function saves the model as a stream.
//
void			DECL STDC	engiSaveModelByStream(
									SdaiModel				model,
									const void				* callback,
									int_t					size
								);

//
//		engiSaveModelByArray                                    (http://rdf.bg/ifcdoc/CP64/engiSaveModelByArray.html)
//				SdaiModel				model								IN
//				unsigned char			* content							IN / OUT
//				int_t					* size								IN / OUT
//
//				void					returns
//
//	This function saves the model as an array.
//
void			DECL STDC	engiSaveModelByArray(
									SdaiModel				model,
									unsigned char			* content,
									int_t					* size
								);

//
//		sdaiSaveModelAsXmlBN                                    (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsXmlBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC2x3's way of XML serialization (char file name).
//
void			DECL STDC	sdaiSaveModelAsXmlBN(
									SdaiModel				model,
									const char				* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelAsXmlBN(
								SdaiModel				model,
								char					* fileName
							)
{
	return	sdaiSaveModelAsXmlBN(
					model,
					(const char*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiSaveModelAsXmlBNUnicode                             (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsXmlBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC2x3's way of XML serialization (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelAsXmlBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelAsXmlBNUnicode(
								SdaiModel				model,
								wchar_t					* fileName
							)
{
	return	sdaiSaveModelAsXmlBNUnicode(
					model,
					(const wchar_t*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiSaveModelAsSimpleXmlBN                              (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsSimpleXmlBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC4's way of XML serialization (char file name).
//
void			DECL STDC	sdaiSaveModelAsSimpleXmlBN(
									SdaiModel				model,
									const char				* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelAsSimpleXmlBN(
								SdaiModel				model,
								char					* fileName
							)
{
	return	sdaiSaveModelAsSimpleXmlBN(
					model,
					(const char*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiSaveModelAsSimpleXmlBNUnicode                       (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsSimpleXmlBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC4's way of XML serialization (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelAsSimpleXmlBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelAsSimpleXmlBNUnicode(
								SdaiModel				model,
								wchar_t					* fileName
							)
{
	return	sdaiSaveModelAsSimpleXmlBNUnicode(
					model,
					(const wchar_t*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiSaveModelAsJsonBN                                   (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsJsonBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model as JSON according to IFC4's way of JSON serialization (char file name).
//
void			DECL STDC	sdaiSaveModelAsJsonBN(
									SdaiModel				model,
									const char				* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelAsJsonBN(
								SdaiModel				model,
								char					* fileName
							)
{
	return	sdaiSaveModelAsJsonBN(
					model,
					(const char*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiSaveModelAsJsonBNUnicode                            (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsJsonBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model as JSON according to IFC4's way of JSON serialization (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelAsJsonBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiSaveModelAsJsonBNUnicode(
								SdaiModel				model,
								wchar_t					* fileName
							)
{
	return	sdaiSaveModelAsJsonBNUnicode(
					model,
					(const wchar_t*) fileName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiSaveSchemaBN                                        (http://rdf.bg/ifcdoc/CP64/engiSaveSchemaBN.html)
//				SdaiModel				model								IN
//				const char				* filePath							IN
//
//				bool					returns								OUT
//
//	This function saves the schema.
//
bool			DECL STDC	engiSaveSchemaBN(
									SdaiModel				model,
									const char				* filePath
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	bool	engiSaveSchemaBN(
								SdaiModel				model,
								char					* filePath
							)
{
	return	engiSaveSchemaBN(
					model,
					(const char*) filePath
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiSaveSchemaBNUnicode                                 (http://rdf.bg/ifcdoc/CP64/engiSaveSchemaBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* filePath							IN
//
//				bool					returns								OUT
//
//	This function saves the schema (wchar, i.e. Unicode file name).
//
bool			DECL STDC	engiSaveSchemaBNUnicode(
									SdaiModel				model,
									const wchar_t			* filePath
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	bool	engiSaveSchemaBNUnicode(
								SdaiModel				model,
								wchar_t					* filePath
							)
{
	return	engiSaveSchemaBNUnicode(
					model,
					(const wchar_t*) filePath
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiCloseModel                                          (http://rdf.bg/ifcdoc/CP64/sdaiCloseModel.html)
//				SdaiModel				model								IN
//
//				void					returns
//
//	This function closes the model. After this call no instance handles will be available including all
//	handles referencing the geometry of this specific file, in default compilation the model itself will
//	be known in the kernel, however known to be disabled. Calls containing the model reference will be
//	protected from crashing when called.
//
void			DECL STDC	sdaiCloseModel(
									SdaiModel				model
								);

//
//		setPrecisionDoubleExport                                (http://rdf.bg/ifcdoc/CP64/setPrecisionDoubleExport.html)
//				SdaiModel				model								IN
//				int_t					precisionCap						IN
//				int_t					precisionRound						IN
//				bool					clean								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setPrecisionDoubleExport(
									SdaiModel				model,
									int_t					precisionCap,
									int_t					precisionRound,
									bool					clean
								);

//
//  Schema Reading API Calls
//

//
//		engiGetNextTypeDeclarationIterator                      (http://rdf.bg/ifcdoc/CP64/engiGetNextTypeDeclarationIterator.html)
//				SdaiModel				model								IN
//				SchemaTypeIterator		iterator							IN
//
//				SchemaTypeIterator		returns								OUT
//
//	This call returns next iterator of EXPRESS schema declarations for entities and types.
//	If the input iterator is NULL it returns first iterator.
//	If the input iterator is last it returns NULL.
//	The declaration can be ENTITY, TYPE ENUM, TYPE SELECT, or defined TYPE.
//	Use engiGetDeclarationFromIterator to access the further information.
//
SchemaTypeIterator	DECL STDC	engiGetNextTypeDeclarationIterator(
									SdaiModel				model,
									SchemaTypeIterator		iterator
								);

//
//		engiGetTypeDeclarationFromIterator                      (http://rdf.bg/ifcdoc/CP64/engiGetTypeDeclarationFromIterator.html)
//				SdaiModel				model								IN
//				SchemaTypeIterator		iterator							IN
//
//				SchemaTypeDecl			returns								OUT
//
//	This call returns handle to the EXPRESS schema declaration from iterator.
//	The declaration can be ENTITY, TYPE ENUM, TYPE SELECT, or defined TYPE.
//	Use engiGetDeclarationType to access the further information.
//	Use engiGetNextTypeDeclarationIterator to iterate declarations.
//
SchemaTypeDecl	DECL STDC	engiGetTypeDeclarationFromIterator(
									SdaiModel				model,
									SchemaTypeIterator		iterator
								);

//
//		engiGetSchemaScriptDeclarationByIterator                (http://rdf.bg/ifcdoc/CP64/engiGetSchemaScriptDeclarationByIterator.html)
//				SdaiModel				model								IN
//				ExpressScript			prev								IN
//
//				ExpressScript			returns								OUT
//
//	This call iterates EXPRESS schema declarations of FUNCTION, PROCEDURE or RULE.
//	If prev is NULL it returns first declaration of above kinds.
//	If prev is the last declaration it returns NULL.
//	Use engiGetDeclarationType to access the further information.
//
ExpressScript	DECL STDC	engiGetSchemaScriptDeclarationByIterator(
									SdaiModel				model,
									ExpressScript			prev
								);

//
//		engiGetDeclarationType                                  (http://rdf.bg/ifcdoc/CP64/engiGetDeclarationType.html)
//				SchemaDecl				declaration							IN
//
//				enum_express_declaration	returns								OUT
//
//	This call returns a type of the EXPRESS schema declarations from its handle.
//
//	The following functions can be used to get further information
//		ENTITY: this SchemaDecl can be casted to SdaiEntity and used in engiGetEntityName and any other entity inquiry function
//		TYPE ENUM: engiGetEnumerationElement
//		TYPE SELECT: engiGetSelectElement
//		DEFINED_TYPE: engiGetDefinedType
//		FUNCTION, PROCEDURE, RULE, WHERE_RULE: engiGetScriptText
//
//	Use engiGetTypeDeclarationFromIterator or engiGetSchemaScriptDeclarationByIterator to obtain declaration handle
//
enum_express_declaration	DECL STDC	engiGetDeclarationType(
									SchemaDecl				declaration
								);

//
//		engiGetEnumerationElement                               (http://rdf.bg/ifcdoc/CP64/engiGetEnumerationElement.html)
//				SchemaDecl				enumeration							IN
//				SdaiInteger				index								IN
//
//				const char				* returns							OUT
//
//	This call returns a name of the enumaration element with the given index (zero based)
//	It returns NULL if the index out of range
//
const char		DECL * STDC	engiGetEnumerationElement(
									SchemaDecl				enumeration,
									SdaiInteger				index
								);

//
//		engiGetSelectElement                                    (http://rdf.bg/ifcdoc/CP64/engiGetSelectElement.html)
//				SchemaDecl				select								IN
//				SdaiInteger				index								IN
//
//				SchemaDecl				returns								OUT
//
//	This call returns a declaration handle of the select element with the given index (zero based)
//	It returns 0 if the index out of range
//
SchemaDecl		DECL STDC	engiGetSelectElement(
									SchemaDecl				select,
									SdaiInteger				index
								);

//
//		engiGetDefinedType                                      (http://rdf.bg/ifcdoc/CP64/engiGetDefinedType.html)
//				SchemaDecl				definedType							IN
//				SchemaDecl				* referencedDeclaration				IN / OUT
//				SchemaAggr				* aggregationDefinition				IN / OUT
//
//				enum_express_attr_type	returns								OUT
//
//	This call returns a simple type for defined type handle and can inquire referenced type, if any
//
enum_express_attr_type	DECL STDC	engiGetDefinedType(
									SchemaDecl				definedType,
									SchemaDecl				* referencedDeclaration,
									SchemaAggr				* aggregationDefinition
								);

//
//		engiGetScriptText                                       (http://rdf.bg/ifcdoc/CP64/engiGetScriptText.html)
//				ExpressScript			declaration							IN
//				SdaiString				* label								IN / OUT
//				SdaiString				* text								IN / OUT
//
//				void					returns
//
//	This call returns name and body test for entity where rule, schema rule, function, procedure or
//
void			DECL STDC	engiGetScriptText(
									ExpressScript			declaration,
									SdaiString				* label,
									SdaiString				* text
								);

//
//		engiEvaluateScriptExpression                            (http://rdf.bg/ifcdoc/CP64/engiEvaluateScriptExpression.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				ExpressScript			expression							IN
//				SdaiPrimitiveType		valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	This function can evaluate EXPRESS expression for entity where rule or derived attribute.
//	valueType, value and return type work similary to sdaiGetAttr
//
void			DECL * STDC	engiEvaluateScriptExpression(
									SdaiModel				model,
									SdaiInstance			instance,
									ExpressScript			expression,
									SdaiPrimitiveType		valueType,
									void					* value
								);

//
//		sdaiGetEntity                                           (http://rdf.bg/ifcdoc/CP64/sdaiGetEntity.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiEntity				returns								OUT
//
//	This call retrieves a handle to an entity based on a given entity name.
//
SdaiEntity		DECL STDC	sdaiGetEntity(
									SdaiModel				model,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiEntity	sdaiGetEntity(
									SdaiModel				model,
									char					* entityName
								)
{
	return	sdaiGetEntity(
					model,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityModel                                      (http://rdf.bg/ifcdoc/CP64/engiGetEntityModel.html)
//				SdaiEntity				entity								IN
//
//				SdaiModel				returns								OUT
//
//	This call retrieves a model based on a given entity handle.
//
SdaiModel		DECL STDC	engiGetEntityModel(
									SdaiEntity				entity
								);

//
//		engiGetEntityAttributeIndex                             (http://rdf.bg/ifcdoc/CP64/engiGetEntityAttributeIndex.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiGetAttrIndexBN
//
int_t			DECL STDC	engiGetEntityAttributeIndex(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetEntityAttributeIndex(
								SdaiEntity				entity,
								char					* attributeName
							)
{
	return	engiGetEntityAttributeIndex(
					entity,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrIndexBN                                      (http://rdf.bg/ifcdoc/CP64/engiGetAttrIndexBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	engiGetAttrIndexBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetAttrIndexBN(
								SdaiEntity				entity,
								char					* attributeName
							)
{
	return	engiGetAttrIndexBN(
					entity,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityAttributeIndexEx                           (http://rdf.bg/ifcdoc/CP64/engiGetEntityAttributeIndexEx.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//				bool					countedWithParents					IN
//				bool					countedWithInverse					IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiGetAttrIndexExBN
//
int_t			DECL STDC	engiGetEntityAttributeIndexEx(
									SdaiEntity				entity,
									SdaiString				attributeName,
									bool					countedWithParents,
									bool					countedWithInverse
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetEntityAttributeIndexEx(
								SdaiEntity				entity,
								char					* attributeName,
								bool					countedWithParents,
								bool					countedWithInverse
							)
{
	return	engiGetEntityAttributeIndexEx(
					entity,
					(const char*) attributeName,
					countedWithParents,
					countedWithInverse
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrIndexExBN                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttrIndexExBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//				bool					countedWithParents					IN
//				bool					countedWithInverse					IN
//
//				int_t					returns								OUT
//
//	..
//
int_t			DECL STDC	engiGetAttrIndexExBN(
									SdaiEntity				entity,
									SdaiString				attributeName,
									bool					countedWithParents,
									bool					countedWithInverse
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetAttrIndexExBN(
								SdaiEntity				entity,
								char					* attributeName,
								bool					countedWithParents,
								bool					countedWithInverse
							)
{
	return	engiGetAttrIndexExBN(
					entity,
					(const char*) attributeName,
					countedWithParents,
					countedWithInverse
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityArgumentName                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentName.html)
//				SdaiEntity				entity								IN
//				SdaiInteger				index								IN
//				SdaiPrimitiveType		valueType							IN
//				SdaiString				* attributeName						IN / OUT
//
//				SdaiString				returns								OUT
//
//	DEPRECATED USE engiGetAttrNameByIndex
//
SdaiString		DECL STDC	engiGetEntityArgumentName(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType,
									SdaiString				* attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiString	engiGetEntityArgumentName(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType,
									char					** attributeName
								)
{
	return	engiGetEntityArgumentName(
					entity,
					index,
					valueType,
					(SdaiString*) attributeName
				);
}

//
//
static	inline	SdaiString	engiGetEntityArgumentName(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType
								)
{
	return	engiGetEntityArgumentName(
					entity,
					index,
					valueType,
					(SdaiString*) nullptr				//	attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrNameByIndex                                  (http://rdf.bg/ifcdoc/CP64/engiGetAttrNameByIndex.html)
//				SdaiEntity				entity								IN
//				SdaiInteger				index								IN
//				SdaiPrimitiveType		valueType							IN
//				SdaiString				* attributeName						IN / OUT
//
//				SdaiString				returns								OUT
//
//	This call can be used to retrieve the name of the n-th argument of the given entity. Arguments of parent entities are included in the index. Both explicit and inverse attributes are included.
//
SdaiString		DECL STDC	engiGetAttrNameByIndex(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType,
									SdaiString				* attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* engiGetAttrNameByIndex(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType,
									char					** argumentName
								)
{
	return	engiGetAttrNameByIndex(
					entity,
					index,
					valueType,
					(SdaiString*) argumentName
				);
}

//
//
static	inline	const char	* engiGetAttrNameByIndex(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType
								)
{
	return	engiGetAttrNameByIndex(
					entity,
					index,
					valueType,
					(SdaiString*) nullptr				//	attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityArgumentType                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentType.html)
//				SdaiEntity				entity								IN
//				SdaiInteger				index								IN
//				SdaiPrimitiveType		* attributeType						IN / OUT
//
//				void					returns
//
//	DEPRECATED USE engiGetAttrTypeByIndex
//
void			DECL STDC	engiGetEntityArgumentType(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		* attributeType
								);

//
//		engiGetAttrTypeByIndex                         (http://rdf.bg/ifcdoc/CP64/engiGetAttrTypeByIndex.html)
//				SdaiEntity				entity								IN
//				SdaiInteger				index								IN
//				SdaiPrimitiveType		* attributeType						IN / OUT
//
//				void					returns
//
//	This call can be used to retrieve the type of the n-th argument of the given entity. In case of a select argument no relevant information is given by this call as it depends on the instance. Arguments of parent entities are included in the index. Both explicit and inverse attributes are included.
//
void			DECL STDC	engiGetAttrTypeByIndex(
									SdaiEntity				entity,
									SdaiInteger				index,
									SdaiPrimitiveType		* attributeType
								);

//
//		engiGetEntityCount                                      (http://rdf.bg/ifcdoc/CP64/engiGetEntityCount.html)
//				SdaiModel				model								IN
//
//				SdaiInteger				returns								OUT
//
//	Returns the total number of entities within the loaded schema.
//
SdaiInteger		DECL STDC	engiGetEntityCount(
									SdaiModel				model
								);

//
//		engiGetEntityElement                                    (http://rdf.bg/ifcdoc/CP64/engiGetEntityElement.html)
//				SdaiModel				model								IN
//				SdaiInteger				index								IN
//
//				SdaiEntity				returns								OUT
//
//	This call returns a specific entity based on an index, the index needs to be 0 or higher but lower then the number of entities in the loaded schema.
//
SdaiEntity		DECL STDC	engiGetEntityElement(
									SdaiModel				model,
									SdaiInteger				index
								);

//
//		sdaiGetEntityExtent                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetEntityExtent.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//
//				SdaiAggr				returns								OUT
//
//	This call retrieves an aggregation that contains all instances of the entity given.
//
SdaiAggr		DECL STDC	sdaiGetEntityExtent(
									SdaiModel				model,
									SdaiEntity				entity
								);

//
//		sdaiGetEntityExtentBN                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetEntityExtentBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiAggr				returns								OUT
//
//	This call retrieves an aggregation that contains all instances of the entity given.
//
//	Technically sdaiGetEntityExtentBN will transform into the following call
//		sdaiGetEntityExtent(
//				model,
//				sdaiGetEntity(
//						model,
//						entityName
//					)
//			);
//
SdaiAggr		DECL STDC	sdaiGetEntityExtentBN(
									SdaiModel				model,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiAggr	sdaiGetEntityExtentBN(
									SdaiModel				model,
									char					* entityName
								)
{
	return	sdaiGetEntityExtentBN(
					model,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityName                                       (http://rdf.bg/ifcdoc/CP64/engiGetEntityName.html)
//				SdaiEntity				entity								IN
//				SdaiPrimitiveType		valueType							IN
//				const char				** entityName						IN / OUT
//
//				const char				* returns							OUT
//
//	This call can be used to get the name of the given entity.
//
const char		DECL * STDC	engiGetEntityName(
									SdaiEntity				entity,
									SdaiPrimitiveType		valueType,
									const char				** entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* engiGetEntityName(
									SdaiEntity				entity,
									SdaiPrimitiveType		valueType,
									char					** entityName
								)
{
	return	engiGetEntityName(
					entity,
					valueType,
					(const char**) entityName
				);
}

//
//
static	inline	const char	* engiGetEntityName(
									SdaiEntity				entity,
									SdaiPrimitiveType		valueType
								)
{
	return	engiGetEntityName(
					entity,
					valueType,
					(const char**) nullptr				//	entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityNoAttributes                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoAttributes.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	This call returns the number of arguments, this includes the arguments of its (nested) parents and inverse argumnets.
//
int_t			DECL STDC	engiGetEntityNoAttributes(
									SdaiEntity				entity
								);

//
//		engiGetEntityNoAttributesEx                             (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoAttributesEx.html)
//				SdaiEntity				entity								IN
//				bool					includeParent						IN
//				bool					includeInverse						IN
//
//				int_t					returns								OUT
//
//	This call returns the number of attributes, inclusion of parents and inverse depeds on includeParent and includeInverse values
//
int_t			DECL STDC	engiGetEntityNoAttributesEx(
									SdaiEntity				entity,
									bool					includeParent,
									bool					includeInverse
								);

//
//		engiGetEntityParent                                     (http://rdf.bg/ifcdoc/CP64/engiGetEntityParent.html)
//				SdaiEntity				entity								IN
//
//				SdaiEntity				returns								OUT
//
//	Returns the first parent entity, for example the parent of IfcObject is IfcObjectDefinition, of IfcObjectDefinition is IfcRoot and of IfcRoot is 0.
//
SdaiEntity		DECL STDC	engiGetEntityParent(
									SdaiEntity				entity
								);

//
//		engiGetEntityNoParents                                  (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoParents.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	Returns number of parent entities
//
int_t			DECL STDC	engiGetEntityNoParents(
									SdaiEntity				entity
								);

//
//		engiGetEntityParentEx                                   (http://rdf.bg/ifcdoc/CP64/engiGetEntityParentEx.html)
//				SdaiEntity				entity								IN
//				SdaiInteger				index								IN
//
//				SdaiEntity				returns								OUT
//
//	Returns the N-th parent of entity or NULL if index exceeds number of parents
//
SdaiEntity		DECL STDC	engiGetEntityParentEx(
									SdaiEntity				entity,
									SdaiInteger				index
								);

//
//		engiGetAttrDerived                                      (http://rdf.bg/ifcdoc/CP64/engiGetAttrDerived.html)
//				SdaiEntity				entity								IN
//				const SdaiAttr			attribute							IN
//
//				ExpressScript			returns								OUT
//
//	This call can be used to check if an attribute is defined schema wise in the context of a certain entity
//
ExpressScript	DECL STDC	engiGetAttrDerived(
									SdaiEntity				entity,
									const SdaiAttr			attribute
								);

//
//		engiGetAttrDerivedBN                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttrDerivedBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				ExpressScript			returns								OUT
//
//	This call can be used to check if an attribute is defined schema wise in the context of a certain entity.
//
//	Technically engiGetAttrDerivedBN will transform into the following call
//		engiGetAttrDerived(
//				entity,
//				sdaiGetAttrDefinition(
//						entity,
//						attributeName
//					)
//			);
//
ExpressScript	DECL STDC	engiGetAttrDerivedBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetAttrDerivedBN(
								SdaiEntity				entity,
								char					* attributeName
							)
{
	return	engiGetAttrDerivedBN(
					entity,
					(SdaiString) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrOptional                                     (http://rdf.bg/ifcdoc/CP64/engiGetAttrOptional.html)
//				const SdaiAttr			attribute							IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiIsAttrOptional
//
int_t			DECL STDC	engiGetAttrOptional(
									const SdaiAttr			attribute
								);

//
//		engiGetAttrOptionalBN                                   (http://rdf.bg/ifcdoc/CP64/engiGetAttrOptionalBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiIsAttrOptionalBN
//
int_t			DECL STDC	engiGetAttrOptionalBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

//
//		engiGetAttrInverse                                      (http://rdf.bg/ifcdoc/CP64/engiGetAttrInverse.html)
//				const SdaiAttr			attribute							IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiIsAttrInverse
//
int_t			DECL STDC	engiGetAttrInverse(
									const SdaiAttr			attribute
								);

//
//		engiGetAttrInverseBN                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttrInverseBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiIsAttrInverseBN
//
int_t			DECL STDC	engiGetAttrInverseBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

//
//		engiAttrIsInverse                                       (http://rdf.bg/ifcdoc/CP64/engiAttrIsInverse.html)
//				const SdaiAttr			attribute							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call engiIsAttrInverse instead.
//
int_t			DECL STDC	engiAttrIsInverse(
									const SdaiAttr			attribute
								);

//
//		engiIsAttrInverse                                       (http://rdf.bg/ifcdoc/CP64/engiIsAttrInverse.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiBoolean				returns								OUT
//
//	This call can be used to check if an attribute is an inverse relation
//
SdaiBoolean		DECL STDC	engiIsAttrInverse(
									const SdaiAttr			attribute
								);

//
//		engiIsAttrInverseBN                                     (http://rdf.bg/ifcdoc/CP64/engiIsAttrInverseBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				SdaiBoolean				returns								OUT
//
//	This call can be used to check if an attribute is an inverse relation.
//
//	Technically engiIsAttrInverseBN will transform into the following call
//		engiIsAttrInverse(
//				sdaiGetAttrDefinition(
//						entity,
//						attributeName
//					)
//			);
//
SdaiBoolean		DECL STDC	engiIsAttrInverseBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiIsAttrInverseBN(
								SdaiEntity				entity,
								char					* attributeName
							)
{
	return	engiIsAttrInverseBN(
					entity,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiIsAttrOptional                                      (http://rdf.bg/ifcdoc/CP64/engiIsAttrOptional.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiBoolean				returns								OUT
//
//	This call can be used to check if an attribute is optional
//
SdaiBoolean		DECL STDC	engiIsAttrOptional(
									const SdaiAttr			attribute
								);

//
//		engiIsAttrOptionalBN                                    (http://rdf.bg/ifcdoc/CP64/engiIsAttrOptionalBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				SdaiBoolean				returns								OUT
//
//	This call can be used to check if an attribute is optional.
//
//	Technically engiIsAttrOptionalBN will transform into the following call
//		engiIsAttrOptional(
//				sdaiGetAttrDefinition(
//						entity,
//						attributeName
//					)
//			);
//
SdaiBoolean		DECL STDC	engiIsAttrOptionalBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiBoolean	engiIsAttrOptionalBN(
								SdaiEntity				entity,
								char					* attributeName
							)
{
	return	engiIsAttrOptionalBN(
					entity,
					(SdaiString) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrDomain                                       (http://rdf.bg/ifcdoc/CP64/engiGetAttrDomain.html)
//				const SdaiAttr			attribute							IN
//				const char				** domainName						IN / OUT
//
//				const char				* returns							OUT
//
//	DEPRECATED USE engiGetAttrDomainName
//
const char		DECL * STDC	engiGetAttrDomain(
									const SdaiAttr			attribute,
									const char				** domainName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* engiGetAttrDomain(
									const SdaiAttr			attribute,
									char					** domainName
								)
{
	return	engiGetAttrDomain(
					attribute,
					(const char**) domainName
				);
}

//
//
static	inline	const char	* engiGetAttrDomain(
									const SdaiAttr			attribute
								)
{
	return	engiGetAttrDomain(
					attribute,
					(const char**) nullptr				//	domainName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrDomainName                                   (http://rdf.bg/ifcdoc/CP64/engiGetAttrDomainName.html)
//				const SdaiAttr			attribute							IN
//				SdaiString				* domainName						IN / OUT
//
//				SdaiString				returns								OUT
//
//	This call can be used to get the domain of an attribute
//
SdaiString		DECL STDC	engiGetAttrDomainName(
									const SdaiAttr			attribute,
									SdaiString				* domainName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiString engiGetAttrDomainName(
									const SdaiAttr			attribute,
									char					** domainName
								)
{
	return	engiGetAttrDomainName(
					attribute,
					(SdaiString*) domainName
				);
}

//
//
static	inline	SdaiString engiGetAttrDomainName(
									const SdaiAttr			attribute
								)
{
	return	engiGetAttrDomainName(
					attribute,
					(SdaiString*) nullptr				//	domainName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrDomainBN                                     (http://rdf.bg/ifcdoc/CP64/engiGetAttrDomainBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//				SdaiString				* domainName						IN / OUT
//
//				SdaiString				returns								OUT
//
//	DEPRECATED USE engiGetAttrDomainNameBN
//
SdaiString		DECL STDC	engiGetAttrDomainBN(
									SdaiEntity				entity,
									SdaiString				attributeName,
									SdaiString				* domainName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiString	engiGetAttrDomainBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								)
{
	return	engiGetAttrDomainBN(
					entity,
					attributeName,
					(SdaiString*) nullptr				//	domainName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrDomainNameBN                                 (http://rdf.bg/ifcdoc/CP64/engiGetAttrDomainNameBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//				SdaiString				* domainName						IN / OUT
//
//				SdaiString				returns								OUT
//
//	This call can be used to get the domain of an attribute.
//
//	Technically engiGetAttrDomainNameBN will transform into the following call
//		engiGetAttrDomainName(
//				sdaiGetAttrDefinition(
//						entity,
//						attributeName
//					),
//				domainName
//			);
//
SdaiString		DECL STDC	engiGetAttrDomainNameBN(
									SdaiEntity				entity,
									SdaiString				attributeName,
									SdaiString				* domainName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* engiGetAttrDomainNameBN(
									SdaiEntity				entity,
									char					* attributeName,
									char					** domainName
								)
{
	return	engiGetAttrDomainNameBN(
					entity,
					(const char*) attributeName,
					(const char**) domainName
				);
}

//
//
static	inline	const char	* engiGetAttrDomainNameBN(
									SdaiEntity				entity,
									const char				* attributeName
								)
{
	return	engiGetAttrDomainNameBN(
					entity,
					attributeName,
					(const char**) nullptr				//	domainName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityIsAbstract                                 (http://rdf.bg/ifcdoc/CP64/engiGetEntityIsAbstract.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiIsEntityAbstract
//
int_t			DECL STDC	engiGetEntityIsAbstract(
									SdaiEntity				entity
								);

//
//		engiIsEntityAbstract                                    (http://rdf.bg/ifcdoc/CP64/engiIsEntityAbstract.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an entity is abstract
//
int_t			DECL STDC	engiIsEntityAbstract(
									SdaiEntity				entity
								);

//
//		engiGetEntityIsAbstractBN                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityIsAbstractBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				int_t					returns								OUT
//
//	DEPRECATED USE engiIsEntityAbstractbn
//
int_t			DECL STDC	engiGetEntityIsAbstractBN(
									SdaiModel				model,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetEntityIsAbstractBN(
								SdaiModel				model,
								char					* entityName
							)
{
	return	engiGetEntityIsAbstractBN(
					model,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiIsEntityAbstractBN                                  (http://rdf.bg/ifcdoc/CP64/engiIsEntityAbstractBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an entity is abstract.
//
//	Technically engiIsEntityAbstractBN will transform into the following call
//		engiIsEntityAbstract(
//				sdaiGetEntity(
//						model,
//						entityName
//					)
//			);
//
int_t			DECL STDC	engiIsEntityAbstractBN(
									SdaiModel				model,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiIsEntityAbstractBN(
								SdaiModel				model,
								char					* entityName
							)
{
	return	engiIsEntityAbstractBN(
					model,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEnumerationValue                                 (http://rdf.bg/ifcdoc/CP64/engiGetEnumerationValue.html)
//				const SdaiAttr			attribute							IN
//				SdaiInteger				index								IN
//				SdaiPrimitiveType		valueType							IN
//				const char				** enumerationValue					IN / OUT
//
//				const char				* returns							OUT
//
//	Allows to retrieve enumeration values of an attribute by index.
//
const char		DECL * STDC	engiGetEnumerationValue(
									const SdaiAttr			attribute,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType,
									const char				** enumerationValue
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* engiGetEnumerationValue(
									const SdaiAttr			attribute,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType,
									char					** enumerationValue
								)
{
	return	engiGetEnumerationValue(
					attribute,
					index,
					valueType,
					(const char**) enumerationValue
				);
}

//
//
static	inline	const char	* engiGetEnumerationValue(
									const SdaiAttr			attribute,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType
								)
{
	return	engiGetEnumerationValue(
					attribute,
					index,
					valueType,
					(const char**) nullptr				//	enumerationValue
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetEntityAttributeByIterator                        (http://rdf.bg/ifcdoc/CP64/engiGetEntityAttributeByIterator.html)
//				SdaiEntity				entity								IN
//				SdaiAttr				prev								IN
//
//				SdaiAttr				returns								OUT
//
//	Iterates attribute definition of the entity.
//	Includes explicit, inverse and derived attributes defined by this or parent entities.
//	If a explicit attribute is also known as derived it's reported onces as explicit.
//	Returns first attribute if prev is NULL.
//	Returns NULL when prev is the last attribute.
//
SdaiAttr		DECL STDC	engiGetEntityAttributeByIterator(
									SdaiEntity				entity,
									SdaiAttr				prev
								);

//
//		engiGetEntityAttributeByIndex                           (http://rdf.bg/ifcdoc/CP64/engiGetEntityAttributeByIndex.html)
//				SdaiEntity				entity								IN
//				SdaiAggrIndex			index								IN
//				bool					countedWithParents					IN
//				bool					countedWithInverse					IN
//
//				SdaiAttr				returns								OUT
//
//	Return attribute definition from attribute index
//
SdaiAttr		DECL STDC	engiGetEntityAttributeByIndex(
									SdaiEntity				entity,
									SdaiAggrIndex			index,
									bool					countedWithParents,
									bool					countedWithInverse
								);

//
//		engiGetAggregationDefinition                            (http://rdf.bg/ifcdoc/CP64/engiGetAggregationDefinition.html)
//				SchemaAggr				aggregationDefinition				IN
//				enum_express_aggr		* aggregationType					IN / OUT
//				int_t					* cardinalityMin					IN / OUT
//				int_t					* cardinalityMax					IN / OUT
//				bool					* optional							IN / OUT
//				bool					* unique							IN / OUT
//				SchemaAggr				* nextAggregationLevel				IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggregationDefinition(
									SchemaAggr				aggregationDefinition,
									enum_express_aggr		* aggregationType,
									int_t					* cardinalityMin,
									int_t					* cardinalityMax,
									bool					* optional,
									bool					* unique,
									SchemaAggr				* nextAggregationLevel
								);

//
//		engiGetEntityUniqueRuleByIterator                       (http://rdf.bg/ifcdoc/CP64/engiGetEntityUniqueRuleByIterator.html)
//				SdaiEntity				entity								IN
//				UniqueRule				prev								IN
//				SdaiString				* label								IN / OUT
//
//				UniqueRule				returns								OUT
//
//	Iterates unique rules of the entity.
//	Includes this but not parent entities.
//	Returns first rule if prev is NULL.
//	Returns NULL when prev is the last rule.
//
UniqueRule		DECL STDC	engiGetEntityUniqueRuleByIterator(
									SdaiEntity				entity,
									UniqueRule				prev,
									SdaiString				* label
								);

//
//		engiGetEntityUniqueRuleAttributeByIterator              (http://rdf.bg/ifcdoc/CP64/engiGetEntityUniqueRuleAttributeByIterator.html)
//				UniqueRule				rule								IN
//				SdaiString				prev								IN
//				SdaiString				* domain							IN / OUT
//
//				SdaiString				returns								OUT
//
//	Iterates attributes of unique rule.
//	Returns first attribute name if prev is NULL.
//	Returns NULL when prev is the name of the last attribute.
//
SdaiString		DECL STDC	engiGetEntityUniqueRuleAttributeByIterator(
									UniqueRule				rule,
									SdaiString				prev,
									SdaiString				* domain
								);

//
//		engiGetEntityWhereRuleByIterator                        (http://rdf.bg/ifcdoc/CP64/engiGetEntityWhereRuleByIterator.html)
//				SchemaDecl				declaration							IN
//				ExpressScript			prev								IN
//				SdaiString				* label								IN / OUT
//
//				ExpressScript			returns								OUT
//
//	Iterates where rules of the entity or defined type.
//	Declaration can be ENTITY or DEFINED_TYPE.
//	Includes this but not parent entities or types.
//	Returns first rule if prev is NULL.
//	Returns NULL when prev is the last rule.
//	Use engiGetScriptText to get further information.
//
ExpressScript	DECL STDC	engiGetEntityWhereRuleByIterator(
									SchemaDecl				declaration,
									ExpressScript			prev,
									SdaiString				* label
								);

//
//  Instance Reading API Calls
//

//
//		sdaiGetADBType                                          (http://rdf.bg/ifcdoc/CP64/sdaiGetADBType.html)
//				const SdaiADB			ADB									IN
//
//				SdaiPrimitiveType		returns								OUT
//
//	This call can be used to get the used type within this ADB type.
//
SdaiPrimitiveType	DECL STDC	sdaiGetADBType(
									const SdaiADB			ADB
								);

//
//		sdaiGetADBTypePath                                      (http://rdf.bg/ifcdoc/CP64/sdaiGetADBTypePath.html)
//				const SdaiADB			ADB									IN
//				int_t					typeNameNumber						IN
//
//				const char				* returns							OUT
//
//	This call can be used to get the path of an ADB type.
//
const char		DECL * STDC	sdaiGetADBTypePath(
									const SdaiADB			ADB,
									int_t					typeNameNumber
								);

//
//		sdaiGetADBValue                                         (http://rdf.bg/ifcdoc/CP64/sdaiGetADBValue.html)
//				const SdaiADB			ADB									IN
//				SdaiPrimitiveType		valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	valueType argument to specify what type of data caller wants to get and
//	value argument where the caller should provide a buffer, and the function will write the result to.
//
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetADBValue, and it works similarly for all get-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//	The Table 2 shows what valueType can be fulfilled depending on actual model data.
//	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
//	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
//	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
//	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetADBValue but valid for all get-functions)
//
//	valueType				C/C++												C#
//
//	sdaiINTEGER				int_t val;											int_t val;
//							sdaiGetADBValue (ADB, sdaiINTEGER, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiINTEGER, out val);
//
//	sdaiREAL or sdaiNUMBER	double val;											double val;
//							sdaiGetADBValue (ADB, sdaiREAL, &val);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiREAL, out val);
//
//	sdaiBOOLEAN				SdaiBoolean val;									bool val;
//							sdaiGetADBValue (ADB, sdaiBOOLEAN, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiBOOLEAN, out val);
//
//	sdaiLOGICAL				const TCHAR* val;									string val;
//							sdaiGetADBValue (ADB, sdaiLOGICAL, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiLOGICAL, out val);
//
//	sdaiENUM				const TCHAR* val;									string val;
//							sdaiGetADBValue (ADB, sdaiENUM, &val);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiENUM, out val);
//
//	sdaiBINARY				const TCHAR* val;									string val;
//							sdaiGetADBValue (ADB, sdaiBINARY, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiBINARY, out val);
//
//	sdaiSTRING				const char* val;									string val;
//							sdaiGetADBValue (ADB, sdaiSTRING, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiSTRING, out val);
//
//	sdaiUNICODE				const wchar_t* val;									string val;
//							sdaiGetADBValue (ADB, sdaiUNICODE, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiUNICODE, out val);
//
//	sdaiEXPRESSSTRING		const char* val;									string val;
//							sdaiGetADBValue (ADB, sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiEXPRESSSTRING, out val);
//
//	sdaiINSTANCE			SdaiInstance val;									int_t val;
//							sdaiGetADBValue (ADB, sdaiINSTANCE, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiINSTANCE, out val);
//
//	sdaiAGGR				SdaiAggr aggr;										int_t aggr;
//							sdaiGetADBValue (ADB, sdaiAGGR, &aggr);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiAGGR, out aggr);
//
//	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();					int_t adb = 0;	//	it is important to initialize
//							sdaiGetADBValue (ADB, sdaiADB, adb);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiADB, out adb);		
//							sdaiDeleteADB (adb);
//
//							SdaiADB adb = nullptr;	//	it is important to initialize
//							sdaiGetADBValue (ADB, sdaiADB, &adb);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
//		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
//
void			DECL * STDC	sdaiGetADBValue(
									const SdaiADB			ADB,
									SdaiPrimitiveType		valueType,
									void					* value
								);

//
//		sdaiCreateEmptyADB                                      (http://rdf.bg/ifcdoc/CP64/sdaiCreateEmptyADB.html)
//				void					* returns							OUT
//
//	Creates an empty ADB (Attribute Data Block).
//
void			DECL * STDC	sdaiCreateEmptyADB(
								);

//
//		sdaiDeleteADB                                           (http://rdf.bg/ifcdoc/CP64/sdaiDeleteADB.html)
//				const SdaiADB			ADB									IN
//
//				void					returns
//
//	Deletes an ADB (Attribute Data Block).
//
void			DECL STDC	sdaiDeleteADB(
									const SdaiADB			ADB
								);

//
//		sdaiGetAggrByIndex                                      (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrByIndex.html)
//				const SdaiAggr			aggregate							IN
//				SdaiAggrIndex			index								IN
//				SdaiPrimitiveType		valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	valueType argument to specify what type of data caller wants to get and
//	value argument where the caller should provide a buffer, and the function will write the result to.
//
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetAggrByIndex, and it works similarly for all get-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//	The Table 2 shows what valueType can be fulfilled depending on actual model data.
//	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
//	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
//	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
//	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetAggrByIndex but valid for all get-functions)
//
//	valueType				C/C++																C#
//
//	sdaiINTEGER				int_t val;															int_t val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiINTEGER, &val);			ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiINTEGER, out val);
//
//	sdaiREAL or sdaiNUMBER	double val;															double val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiREAL, &val);				ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiREAL, out val);
//
//	sdaiBOOLEAN				SdaiBoolean val;													bool val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiBOOLEAN, &val);			ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiBOOLEAN, out val);
//
//	sdaiLOGICAL				const TCHAR* val;													string val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiLOGICAL, &val);			ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiLOGICAL, out val);
//
//	sdaiENUM				const TCHAR* val;													string val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiENUM, &val);				ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiENUM, out val);
//
//	sdaiBINARY				const TCHAR* val;													string val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiBINARY, &val);			ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiBINARY, out val);
//
//	sdaiSTRING				const char* val;													string val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiSTRING, &val);			ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiSTRING, out val);
//
//	sdaiUNICODE				const wchar_t* val;													string val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiUNICODE, &val);			ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiUNICODE, out val);
//
//	sdaiEXPRESSSTRING		const char* val;													string val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiEXPRESSSTRING, out val);
//
//	sdaiINSTANCE			SdaiInstance val;													int_t val;
//							sdaiGetAggrByIndex (aggregate, index, sdaiINSTANCE, &val);			ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiINSTANCE, out val);
//
//	sdaiAGGR				SdaiAggr aggr;														int_t aggr;
//							sdaiGetAggrByIndex (aggregate, index, sdaiAGGR, &aggr);				ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiAGGR, out aggr);
//
//	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();									int_t adb = 0;	//	it is important to initialize
//							sdaiGetAggrByIndex (aggregate, index, sdaiADB, adb);				ifcengine.sdaiGetAggrByIndex (aggregate, index, ifcengine.sdaiADB, out adb);		
//							sdaiDeleteADB (adb);
//
//							SdaiADB adb = nullptr;	//	it is important to initialize
//							sdaiGetAggrByIndex (aggregate, index, sdaiADB, &adb);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
//		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
//
void			DECL * STDC	sdaiGetAggrByIndex(
									const SdaiAggr			aggregate,
									SdaiAggrIndex			index,
									SdaiPrimitiveType		valueType,
									void					* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static inline SdaiInstance	sdaiGetAggrByIndex(
									const SdaiAggr			aggregate,
									SdaiAggrIndex			index,
									SdaiInstance			* sdaiInstance
								)
{
	return (SdaiInstance) sdaiGetAggrByIndex(aggregate, index, sdaiINSTANCE, (void*) sdaiInstance);
}

//
//
static inline SdaiInstance	sdaiGetAggrByIndex(
									const SdaiAggr			aggregate,
									SdaiAggrIndex			index
								)
{
	SdaiInstance sdaiInstance = 0;
	return sdaiGetAggrByIndex(aggregate, index, &sdaiInstance);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiPutAggrByIndex                                      (http://rdf.bg/ifcdoc/CP64/sdaiPutAggrByIndex.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPutAggrByIndex, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiPutAggrByIndex but valid for all put-functions)
//
//	valueType				C/C++															C#
//
//	sdaiINTEGER				int_t val = 123;												int_t val = 123;
//							sdaiPutAggrByIndex (aggregate, index, sdaiINTEGER, &val);		ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;											double val = 123.456;
//							sdaiPutAggrByIndex (aggregate, index, sdaiREAL, &val);			ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;										bool val = true;
//							sdaiPutAggrByIndex (aggregate, index, sdaiBOOLEAN, &val);		ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";											string val = "U";
//							sdaiPutAggrByIndex (aggregate, index, sdaiLOGICAL, val);		ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";								string val = "NOTDEFINED";
//							sdaiPutAggrByIndex (aggregate, index, sdaiENUM, val);			ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";								string val = "0123456ABC";
//							sdaiPutAggrByIndex (aggregate, index, sdaiBINARY, val);			ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";							string val = "My Simple String";
//							sdaiPutAggrByIndex (aggregate, index, sdaiSTRING, val);			ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";						string val = "Any Unicode String";
//							sdaiPutAggrByIndex (aggregate, index, sdaiUNICODE, val);		ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";		string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiPutAggrByIndex (aggregate, index, sdaiEXPRESSSTRING, val);	ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");		int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiPutAggrByIndex (aggregate, index, sdaiINSTANCE, val);		ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);						int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);							ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiPutAggrByIndex (aggregate, index, sdaiAGGR, val);			ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;										int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);		int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiPutAggrByIndex (aggregate, index, sdaiADB, val);			ifcengine.sdaiPutAggrByIndex (aggregate, index, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);											ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiPutAggrByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static inline void	sdaiPutAggrByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index,
									SdaiPrimitiveType		valueType,
									SdaiInstance			sdaiInstance
								)
{
	assert(valueType == sdaiINSTANCE);
	sdaiPutAggrByIndex(
			aggregate,
			index,
			valueType,
			(const void*) sdaiInstance
		);
}

//
//
static inline void	sdaiPutAggrByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index,
									SdaiInstance			sdaiInstance
								)
{
	sdaiPutAggrByIndex(
			aggregate,
			index,
			sdaiINSTANCE,
			(const void*) sdaiInstance
		);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAggrType                                         (http://rdf.bg/ifcdoc/CP64/engiGetAggrType.html)
//				const SdaiAggr			aggregate							IN
//				SdaiPrimitiveType		* aggregateType						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggrType(
									const SdaiAggr			aggregate,
									SdaiPrimitiveType		* aggregateType
								);

//
//		engiGetAggrTypex                                        (http://rdf.bg/ifcdoc/CP64/engiGetAggrTypex.html)
//				const SdaiAggr			aggregate							IN
//				SdaiPrimitiveType		* aggregateType						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggrTypex(
									const SdaiAggr			aggregate,
									SdaiPrimitiveType		* aggregateType
								);

//
//		sdaiGetAttr                                             (http://rdf.bg/ifcdoc/CP64/sdaiGetAttr.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//				SdaiPrimitiveType		valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	valueType argument to specify what type of data caller wants to get and
//	value argument where the caller should provide a buffer, and the function will write the result to.
//
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetAttr, and it works similarly for all get-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//	The Table 2 shows what valueType can be fulfilled depending on actual model data.
//	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
//	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
//	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
//	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetAttr but valid for all get-functions)
//
//	valueType				C/C++															C#
//
//	sdaiINTEGER				int_t val;														int_t val;
//							sdaiGetAttr (instance, attribute, sdaiINTEGER, &val);			ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiINTEGER, out val);
//
//	sdaiREAL or sdaiNUMBER	double val;														double val;
//							sdaiGetAttr (instance, attribute, sdaiREAL, &val);				ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiREAL, out val);
//
//	sdaiBOOLEAN				SdaiBoolean val;												bool val;
//							sdaiGetAttr (instance, attribute, sdaiBOOLEAN, &val);			ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiBOOLEAN, out val);
//
//	sdaiLOGICAL				const TCHAR* val;												string val;
//							sdaiGetAttr (instance, attribute, sdaiLOGICAL, &val);			ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiLOGICAL, out val);
//
//	sdaiENUM				const TCHAR* val;												string val;
//							sdaiGetAttr (instance, attribute, sdaiENUM, &val);				ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiENUM, out val);
//
//	sdaiBINARY				const TCHAR* val;												string val;
//							sdaiGetAttr (instance, attribute, sdaiBINARY, &val);			ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiBINARY, out val);
//
//	sdaiSTRING				const char* val;												string val;
//							sdaiGetAttr (instance, attribute, sdaiSTRING, &val);			ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiSTRING, out val);
//
//	sdaiUNICODE				const wchar_t* val;												string val;
//							sdaiGetAttr (instance, attribute, sdaiUNICODE, &val);			ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiUNICODE, out val);
//
//	sdaiEXPRESSSTRING		const char* val;												string val;
//							sdaiGetAttr (instance, attribute, sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiEXPRESSSTRING, out val);
//
//	sdaiINSTANCE			SdaiInstance val;												int_t val;
//							sdaiGetAttr (instance, attribute, sdaiINSTANCE, &val);			ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiINSTANCE, out val);
//
//	sdaiAGGR				SdaiAggr aggr;													int_t aggr;
//							sdaiGetAttr (instance, attribute, sdaiAGGR, &aggr);				ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiAGGR, out aggr);
//
//	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();								int_t adb = 0;	//	it is important to initialize
//							sdaiGetAttr (instance, attribute, sdaiADB, adb);				ifcengine.sdaiGetAttr (instance, attribute, ifcengine.sdaiADB, out adb);		
//							sdaiDeleteADB (adb);
//
//							SdaiADB adb = nullptr;	//	it is important to initialize
//							sdaiGetAttr (instance, attribute, sdaiADB, &adb);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
//		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
//
void			DECL * STDC	sdaiGetAttr(
									SdaiInstance			instance,
									const SdaiAttr			attribute,
									SdaiPrimitiveType		valueType,
									void					* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static inline SdaiInstance	sdaiGetAttr(
									SdaiInstance			instance,
									const SdaiAttr			attribute,
									SdaiInstance			* sdaiInstance
								)
{
	return (SdaiInstance) sdaiGetAttr(instance, attribute, sdaiINSTANCE, (void*) sdaiInstance);
}

//
//
static inline SdaiInstance	sdaiGetAttr(
									SdaiInstance			instance,
									const SdaiAttr			attribute
								)
{
	SdaiInstance	sdaiInstance = 0;
	return sdaiGetAttr(instance, attribute, &sdaiInstance);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetAttrBN                                           (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//				SdaiPrimitiveType		valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	valueType argument to specify what type of data caller wants to get and
//	value argument where the caller should provide a buffer, and the function will write the result to.
//
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetAttrBN, and it works similarly for all get-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//	The Table 2 shows what valueType can be fulfilled depending on actual model data.
//	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
//	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
//	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
//	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetAttrBN but valid for all get-functions)
//
//	valueType				C/C++																C#
//
//	sdaiINTEGER				int_t val;															int_t val;
//							sdaiGetAttrBN (instance, "attrName", sdaiINTEGER, &val);			ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiINTEGER, out val);
//
//	sdaiREAL or sdaiNUMBER	double val;															double val;
//							sdaiGetAttrBN (instance, "attrName", sdaiREAL, &val);				ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiREAL, out val);
//
//	sdaiBOOLEAN				SdaiBoolean val;													bool val;
//							sdaiGetAttrBN (instance, "attrName", sdaiBOOLEAN, &val);			ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiBOOLEAN, out val);
//
//	sdaiLOGICAL				const TCHAR* val;													string val;
//							sdaiGetAttrBN (instance, "attrName", sdaiLOGICAL, &val);			ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiLOGICAL, out val);
//
//	sdaiENUM				const TCHAR* val;													string val;
//							sdaiGetAttrBN (instance, "attrName", sdaiENUM, &val);				ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiENUM, out val);
//
//	sdaiBINARY				const TCHAR* val;													string val;
//							sdaiGetAttrBN (instance, "attrName", sdaiBINARY, &val);				ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiBINARY, out val);
//
//	sdaiSTRING				const char* val;													string val;
//							sdaiGetAttrBN (instance, "attrName", sdaiSTRING, &val);				ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiSTRING, out val);
//
//	sdaiUNICODE				const wchar_t* val;													string val;
//							sdaiGetAttrBN (instance, "attrName", sdaiUNICODE, &val);			ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiUNICODE, out val);
//
//	sdaiEXPRESSSTRING		const char* val;													string val;
//							sdaiGetAttrBN (instance, "attrName", sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiEXPRESSSTRING, out val);
//
//	sdaiINSTANCE			SdaiInstance val;													int_t val;
//							sdaiGetAttrBN (instance, "attrName", sdaiINSTANCE, &val);			ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiINSTANCE, out val);
//
//	sdaiAGGR				SdaiAggr aggr;														int_t aggr;
//							sdaiGetAttrBN (instance, "attrName", sdaiAGGR, &aggr);				ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiAGGR, out aggr);
//
//	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();									int_t adb = 0;	//	it is important to initialize
//							sdaiGetAttrBN (instance, "attrName", sdaiADB, adb);					ifcengine.sdaiGetAttrBN (instance, "attrName", ifcengine.sdaiADB, out adb);		
//							sdaiDeleteADB (adb);
//
//							SdaiADB adb = nullptr;	//	it is important to initialize
//							sdaiGetAttrBN (instance, "attrName", sdaiADB, &adb);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
//		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
//
//	Technically sdaiGetAttrBN will transform into the following call
//		sdaiGetAttr(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					),
//				valueType,
//				value
//			);
//
void			DECL * STDC	sdaiGetAttrBN(
									SdaiInstance			instance,
									SdaiString				attributeName,
									SdaiPrimitiveType		valueType,
									void					* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	* sdaiGetAttrBN(
								SdaiInstance			instance,
								char					* attributeName,
								SdaiPrimitiveType		valueType,
								void					* value
							)
{
	return	sdaiGetAttrBN(
					instance,
					(const char*) attributeName,
					valueType,
					value
				);
}

//
//
static	inline	SdaiInstance	sdaiGetAttrBN(
										SdaiInstance			instance,
										const char				* attributeName,
										SdaiInstance			* sdaiInstance
									)
{
	return	(SdaiInstance) sdaiGetAttrBN(
					instance,
					attributeName,
					sdaiINSTANCE,
					sdaiInstance
				);
}

//
//
static	inline	SdaiInstance	sdaiGetAttrBN(
										SdaiInstance			instance,
										char					* attributeName,
										SdaiInstance			* sdaiInstance
									)
{
	return	sdaiGetAttrBN(
					instance,
					(const char*) attributeName,
					sdaiInstance
				);
}

//
//
static	inline	SdaiInstance	sdaiGetAttrBN(
										SdaiInstance			instance,
										const char				* attributeName
									)
{
	SdaiInstance	sdaiInstance = 0;
	return	sdaiGetAttrBN(
					instance,
					attributeName,
					&sdaiInstance
				);
}

//
//
static	inline	SdaiInstance	sdaiGetAttrBN(
										SdaiInstance			instance,
										char					* attributeName
									)
{
	return	sdaiGetAttrBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetAttrBNUnicode                                    (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrBNUnicode.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//				unsigned char			* buffer							IN / OUT
//				int_t					bufferLength						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiGetAttrBNUnicode(
									SdaiInstance			instance,
									SdaiString				attributeName,
									unsigned char			* buffer,
									int_t					bufferLength
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	sdaiGetAttrBNUnicode(
								SdaiInstance			instance,
								char					* attributeName,
								unsigned char			* buffer,
								int_t					bufferLength
							)
{
	return	sdaiGetAttrBNUnicode(
					instance,
					(const char*) attributeName,
					buffer,
					bufferLength
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetStringAttrBN                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetStringAttrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				char					* returns							OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiSTRING.
//	This call can be useful in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
//	Technically sdaiGetStringAttrBN will transform into the following call
//		char	* rValue = 0;
//		sdaiGetAttr(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					),
//				sdaiSTRING,
//				&rValue
//			);
//		return	rValue;
//
char			DECL * STDC	sdaiGetStringAttrBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	char	* sdaiGetStringAttrBN(
								SdaiInstance			instance,
								char					* attributeName
							)
{
	return	sdaiGetStringAttrBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetInstanceAttrBN                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceAttrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				SdaiInstance			returns								OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiINSTANCE.
//	This call can be useful in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
//	Technically sdaiGetInstanceAttrBN will transform into the following call
//		SdaiInstance	inst = 0;
//		sdaiGetAttr(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					),
//				sdaiINSTANCE,
//				&inst
//			);
//		return	inst;
//
SdaiInstance	DECL STDC	sdaiGetInstanceAttrBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiInstance	sdaiGetInstanceAttrBN(
										SdaiInstance			instance,
										char					* attributeName
									)
{
	return	sdaiGetInstanceAttrBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetAggregationAttrBN                                (http://rdf.bg/ifcdoc/CP64/sdaiGetAggregationAttrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				SdaiAggr				returns								OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiAGGR.
//	This call can be useful in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
//	Technically sdaiGetAggregationAttrBN will transform into the following call
//		SdaiAggr	aggr = 0;
//		sdaiGetAttr(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					),
//				sdaiAGGR,
//				&aggr
//			);
//		return	aggr;
//
SdaiAggr		DECL STDC	sdaiGetAggregationAttrBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiAggr	sdaiGetAggregationAttrBN(
									SdaiInstance			instance,
									char					* attributeName
								)
{
	return	sdaiGetAggregationAttrBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetAttrDefinition                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrDefinition.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				SdaiAttr				returns								OUT
//
//	...
//
SdaiAttr		DECL STDC	sdaiGetAttrDefinition(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiAttr	sdaiGetAttrDefinition(
									SdaiEntity				entity,
									char					* attributeName
								)
{
	return	sdaiGetAttrDefinition(
					entity,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrTraits                                       (http://rdf.bg/ifcdoc/CP64/engiGetAttrTraits.html)
//				const SdaiAttr			attribute							IN
//				SdaiString				* name								IN / OUT
//				SdaiEntity				* definingEntity					IN / OUT
//				SdaiBoolean				* isExplicit						IN / OUT
//				SdaiBoolean				* isInverse							IN / OUT
//				enum_express_attr_type	* attrType							IN / OUT
//				SdaiEntity				* domainEntity						IN / OUT
//				SchemaAggr				* aggregationDefinition				IN / OUT
//				SdaiBoolean				* isOptional						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAttrTraits(
									const SdaiAttr			attribute,
									SdaiString				* name,
									SdaiEntity				* definingEntity,
									SdaiBoolean				* isExplicit,
									SdaiBoolean				* isInverse,
									enum_express_attr_type	* attrType,
									SdaiEntity				* domainEntity,
									SchemaAggr				* aggregationDefinition,
									SdaiBoolean				* isOptional
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	engiGetAttrTraits(
								const SdaiAttr			attribute,
								char					** name,
								SdaiEntity				* definingEntity,
								SdaiBoolean				* isExplicit,
								SdaiBoolean				* isInverse,
								enum_express_attr_type	* attrType,
								SdaiEntity				* domainEntity,
								SchemaAggr				* aggregationDefinition,
								SdaiBoolean				* isOptional
							)
{
	return	engiGetAttrTraits(
					attribute,
					(SdaiString*) name,
					definingEntity,
					isExplicit,
					isInverse,
					attrType,
					domainEntity,
					aggregationDefinition,
					isOptional
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttributeTraits                                  (http://rdf.bg/ifcdoc/CP64/engiGetAttributeTraits.html)
//				const SdaiAttr			attribute							IN
//				const char				** name								IN / OUT
//				SdaiEntity				* definingEntity					IN / OUT
//				bool					* isExplicit						IN / OUT
//				bool					* isInverse							IN / OUT
//				enum_express_attr_type	* attrType							IN / OUT
//				SdaiEntity				* domainEntity						IN / OUT
//				SchemaAggr				* aggregationDefinition				IN / OUT
//				bool					* isOptional						IN / OUT
//
//				void					returns
//
//	DEPRECATED USE engiGetAttrTraits
//
void			DECL STDC	engiGetAttributeTraits(
									const SdaiAttr			attribute,
									const char				** name,
									SdaiEntity				* definingEntity,
									bool					* isExplicit,
									bool					* isInverse,
									enum_express_attr_type	* attrType,
									SdaiEntity				* domainEntity,
									SchemaAggr				* aggregationDefinition,
									bool					* isOptional
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	engiGetAttributeTraits(
								const SdaiAttr			attribute,
								char					** name,
								SdaiEntity				* definingEntity,
								bool					* isExplicit,
								bool					* isInverse,
								enum_express_attr_type	* attrType,
								SdaiEntity				* domainEntity,
								SchemaAggr				* aggregationDefinition,
								bool					* isOptional
							)
{
	return	engiGetAttributeTraits(
					attribute,
					(SdaiString*) name,
					definingEntity,
					isExplicit,
					isInverse,
					attrType,
					domainEntity,
					aggregationDefinition,
					isOptional
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrName                                         (http://rdf.bg/ifcdoc/CP64/engiGetAttrName.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiString				returns								OUT
//
//	...
//
SdaiString		DECL STDC	engiGetAttrName(
									const SdaiAttr			attribute
								);

//
//		engiGetAttrDefiningEntity                               (http://rdf.bg/ifcdoc/CP64/engiGetAttrDefiningEntity.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiEntity				returns								OUT
//
//	...
//
SdaiEntity		DECL STDC	engiGetAttrDefiningEntity(
									const SdaiAttr			attribute
								);

//
//		engiIsAttrExplicit                                        (http://rdf.bg/ifcdoc/CP64/engiIsAttrExplicit.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiBoolean				returns								OUT
//
//	...
//
SdaiBoolean		DECL STDC	engiIsAttrExplicit(
									const SdaiAttr			attribute
								);

//
//		engiIsAttrExplicitBN                                      (http://rdf.bg/ifcdoc/CP64/engiIsAttrExplicittBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				SdaiBoolean				returns								OUT
//
//	...
//
SdaiBoolean		DECL STDC	engiIsAttrExplicitBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

//
//		sdaiGetInstanceModel                                    (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceModel.html)
//				SdaiInstance			instance							IN
//
//				SdaiModel				returns								OUT
//
//	Returns the model based on an instance.
//
SdaiModel		DECL STDC	sdaiGetInstanceModel(
									SdaiInstance			instance
								);

//
//		sdaiGetInstanceType                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceType.html)
//				SdaiInstance			instance							IN
//
//				SdaiEntity				returns								OUT
//
//	Returns the entity based on an instance.
//
SdaiEntity		DECL STDC	sdaiGetInstanceType(
									SdaiInstance			instance
								);

//
//		sdaiGetMemberCount                                      (http://rdf.bg/ifcdoc/CP64/sdaiGetMemberCount.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiInteger				returns								OUT
//
//	...
//
SdaiInteger		DECL STDC	sdaiGetMemberCount(
									SdaiAggr				aggregate
								);

//
//		sdaiIsKindOf                                            (http://rdf.bg/ifcdoc/CP64/sdaiIsKindOf.html)
//				SdaiInstance			instance							IN
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	This call checks if an instance is a type of a certain given entity.
//
int_t			DECL STDC	sdaiIsKindOf(
									SdaiInstance			instance,
									SdaiEntity				entity
								);

//
//		sdaiIsKindOfBN                                          (http://rdf.bg/ifcdoc/CP64/sdaiIsKindOfBN.html)
//				SdaiInstance			instance							IN
//				const char				* entityName						IN
//
//				int_t					returns								OUT
//
//	This call checks if an instance is a type of a certain given entity.
//
//	Technically sdaiIsKindOfBN will transform into the following call
//		sdaiIsKindOf(
//				instance,
//				sdaiGetEntity(
//						engiGetEntityModel(
//								sdaiGetInstanceType(
//										instance
//									)
//							),
//						entityName
//			);
//
int_t			DECL STDC	sdaiIsKindOfBN(
									SdaiInstance			instance,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	sdaiIsKindOfBN(
								SdaiInstance			instance,
								char					* entityName
							)
{
	return	sdaiIsKindOfBN(
					instance,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAttrType                                         (http://rdf.bg/ifcdoc/CP64/engiGetAttrType.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiPrimitiveType		returns								OUT
//
//	Returns primitive SDAI data type for the attribute according to schema, e.g. sdaiINTEGER
//
//	In case of aggregation if will return base primitive type combined with engiTypeFlagAggr, e.g. sdaiINTEGER|engiTypeFlagAggr
//
//	For SELECT it will return sdaiINSTANCE if all options are instances or aggegation of instances, either sdaiADB
//	In case of SELECT and sdaiINSTANCE, return value will be combined with engiTypeFlagAggrOption if some options are aggregation
//	or engiTypeFlagAggr if all options are aggregations of instances
//
//	It works for explicit and inverse attributes
//
SdaiPrimitiveType	DECL STDC	engiGetAttrType(
									const SdaiAttr			attribute
								);

//
//		engiGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CP64/engiGetAttrTypeBN.html)
//				SdaiEntity				entity								IN
//				SdaiString				attributeName						IN
//
//				SdaiPrimitiveType		returns								OUT
//
//	Combines sdaiGetAttrDefinition and engiGetAttrType.
//
//	Technically engiGetAttrTypeBN will transform into the following call
//		engiGetAttrType(
//				sdaiGetAttrDefinition(
//						entity,
//						attributeName
//					)
//			);
//
SdaiPrimitiveType	DECL STDC	engiGetAttrTypeBN(
									SdaiEntity				entity,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiPrimitiveType	engiGetAttrTypeBN(
											SdaiEntity				entity,
											char					* attributeName
										)
{
	return	engiGetAttrTypeBN(
					entity,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetInstanceAttrType                                 (http://rdf.bg/ifcdoc/CP64/engiGetInstanceAttrType.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//
//				SdaiPrimitiveType		returns								OUT
//
//	Returns SDAI type for actual data stored in the instance for the attribute
//	It may be primitive type, sdaiAGGR or sdaiADB
//	Returns 0 for $ and * 
//
SdaiPrimitiveType	DECL STDC	engiGetInstanceAttrType(
									SdaiInstance			instance,
									const SdaiAttr			attribute
								);

//
//		engiGetInstanceAttrTypeBN                               (http://rdf.bg/ifcdoc/CP64/engiGetInstanceAttrTypeBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				SdaiPrimitiveType		returns								OUT
//
//	Combines sdaiGetAttrDefinition and engiGetInstanceAttrType.
//
//	Technically engiGetInstanceAttrTypeBN will transform into the following call
//		engiGetInstanceAttrType(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					)
//			);
//
SdaiPrimitiveType	DECL STDC	engiGetInstanceAttrTypeBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiPrimitiveType	engiGetInstanceAttrTypeBN(
											SdaiInstance			instance,
											char					* attributeName
										)
{
	return	engiGetInstanceAttrTypeBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiIsInstanceOf                                        (http://rdf.bg/ifcdoc/CP64/sdaiIsInstanceOf.html)
//				SdaiInstance			instance							IN
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	This call checks if an instance is an exact instance of a given entity.
//
int_t			DECL STDC	sdaiIsInstanceOf(
									SdaiInstance			instance,
									SdaiEntity				entity
								);

//
//		sdaiIsInstanceOfBN                                      (http://rdf.bg/ifcdoc/CP64/sdaiIsInstanceOfBN.html)
//				SdaiInstance			instance							IN
//				const char				* entityName						IN
//
//				int_t					returns								OUT
//
//	This call checks if an instance is an exact instance of a given entity.
//
//	Technically sdaiIsInstanceOfBN will transform into the following call
//		sdaiIsInstanceOf(
//				instance,
//				sdaiGetEntity(
//						engiGetEntityModel(
//								sdaiGetInstanceType(
//										instance
//									)
//							),
//						entityName
//					)
//			);
//
int_t			DECL STDC	sdaiIsInstanceOfBN(
									SdaiInstance			instance,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	sdaiIsInstanceOfBN(
								SdaiInstance			instance,
								char					* entityName
							)
{
	return	sdaiIsInstanceOfBN(
					instance,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiIsEqual                                             (http://rdf.bg/ifcdoc/CP64/sdaiIsEqual.html)
//				SdaiInstance			instanceI							IN
//				SdaiInstance			instanceII							IN
//
//				bool					returns								OUT
//
//	...
//
bool			DECL STDC	sdaiIsEqual(
									SdaiInstance			instanceI,
									SdaiInstance			instanceII
								);

//
//		sdaiValidateAttribute                                   (http://rdf.bg/ifcdoc/CP64/sdaiValidateAttribute.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiValidateAttribute(
									SdaiInstance			instance,
									const SdaiAttr			attribute
								);

//
//		sdaiValidateAttributeBN                                 (http://rdf.bg/ifcdoc/CP64/sdaiValidateAttributeBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				int_t					returns								OUT
//
//	Technically it will transform into the following call
//		sdaiValidateAttribute(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					)
//			);
//
int_t			DECL STDC	sdaiValidateAttributeBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	sdaiValidateAttributeBN(
								SdaiInstance			instance,
								char					* attributeName
							)
{
	return	sdaiValidateAttributeBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetInstanceClassInfo                                (http://rdf.bg/ifcdoc/CP64/engiGetInstanceClassInfo.html)
//				SdaiInstance			instance							IN
//
//				char					* returns							OUT
//
//	...
//
char			DECL * STDC	engiGetInstanceClassInfo(
									SdaiInstance			instance
								);

//
//		engiGetInstanceClassInfoUC                              (http://rdf.bg/ifcdoc/CP64/engiGetInstanceClassInfoUC.html)
//				SdaiInstance			instance							IN
//
//				char					* returns							OUT
//
//	...
//
char			DECL * STDC	engiGetInstanceClassInfoUC(
									SdaiInstance			instance
								);

//
//		engiGetInstanceMetaInfo                                 (http://rdf.bg/ifcdoc/CP64/engiGetInstanceMetaInfo.html)
//				SdaiInstance			instance							IN
//				int_t					* localId							IN / OUT
//				const char				** entityName						IN / OUT
//				const char				** entityNameUC						IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	engiGetInstanceMetaInfo(
									SdaiInstance			instance,
									int_t					* localId,
									const char				** entityName,
									const char				** entityNameUC
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetInstanceMetaInfo(
								SdaiInstance			instance,
								int_t					* localId,
								char					** entityName,
								char					** entityNameUC
							)
{
	return	engiGetInstanceMetaInfo(
					instance,
					localId,
					(const char**) entityName,
					(const char**) entityNameUC
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiFindInstanceUsers                                   (http://rdf.bg/ifcdoc/CP64/sdaiFindInstanceUsers.html)
//				SdaiInstance			instance							IN
//				int_t					domain								IN
//				int_t					resultList							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiFindInstanceUsers(
									SdaiInstance			instance,
									int_t					domain,
									int_t					resultList
								);

//
//		sdaiFindInstanceUsedInBN                                (http://rdf.bg/ifcdoc/CP64/sdaiFindInstanceUsedInBN.html)
//				SdaiInstance			instance							IN
//				const char				* roleName							IN
//				int_t					domain								IN
//				int_t					resultList							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiFindInstanceUsedInBN(
									SdaiInstance			instance,
									const char				* roleName,
									int_t					domain,
									int_t					resultList
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	sdaiFindInstanceUsedInBN(
								SdaiInstance			instance,
								char					* roleName,
								int_t					domain,
								int_t					resultList
							)
{
	return	sdaiFindInstanceUsedInBN(
					instance,
					(const char*) roleName,
					domain,
					resultList
				);
}

//
//  Instance Writing API Calls
//

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiPrepend                                             (http://rdf.bg/ifcdoc/CP64/sdaiPrepend.html)
//				const SdaiAggr			aggregate							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPrepend, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiPrepend but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiPrepend (aggregate, sdaiINTEGER, &val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiPrepend (aggregate, sdaiREAL, &val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiPrepend (aggregate, sdaiBOOLEAN, &val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiPrepend (aggregate, sdaiLOGICAL, val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiPrepend (aggregate, sdaiENUM, val);						ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiPrepend (aggregate, sdaiBINARY, val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiPrepend (aggregate, sdaiSTRING, val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiPrepend (aggregate, sdaiUNICODE, val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiPrepend (aggregate, sdaiEXPRESSSTRING, val);			ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiPrepend (aggregate, sdaiINSTANCE, val);					ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiPrepend (aggregate, sdaiAGGR, val);						ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");					ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiPrepend (aggregate, sdaiADB, val);						ifcengine.sdaiPrepend (aggregate, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiPrepend(
									const SdaiAggr			aggregate,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiPrepend(
								const SdaiAggr			aggregate,
								SdaiPrimitiveType		valueType,
								SdaiInstance			sdaiInstance
							)
{
	assert(valueType == sdaiINSTANCE);
	return	sdaiPrepend(
					aggregate,
					valueType,
					(const void*) sdaiInstance
				);
}

//
//
static	inline	void	sdaiPrepend(
								const SdaiAggr			aggregate,
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiPrepend(
					aggregate,
					sdaiINSTANCE,
					(const void*) sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiAppend                                              (http://rdf.bg/ifcdoc/CP64/sdaiAppend.html)
//				const SdaiAggr			aggregate							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiAppend, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiAppend but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiAppend (aggregate, sdaiINTEGER, &val);					ifcengine.sdaiAppend (aggregate, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiAppend (aggregate, sdaiREAL, &val);						ifcengine.sdaiAppend (aggregate, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiAppend (aggregate, sdaiBOOLEAN, &val);					ifcengine.sdaiAppend (aggregate, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiAppend (aggregate, sdaiLOGICAL, val);					ifcengine.sdaiAppend (aggregate, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiAppend (aggregate, sdaiENUM, val);						ifcengine.sdaiAppend (aggregate, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiAppend (aggregate, sdaiBINARY, val);					ifcengine.sdaiAppend (aggregate, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiAppend (aggregate, sdaiSTRING, val);					ifcengine.sdaiAppend (aggregate, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiAppend (aggregate, sdaiUNICODE, val);					ifcengine.sdaiAppend (aggregate, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiAppend (aggregate, sdaiEXPRESSSTRING, val);				ifcengine.sdaiAppend (aggregate, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiAppend (aggregate, sdaiINSTANCE, val);					ifcengine.sdaiAppend (aggregate, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiAppend (aggregate, sdaiAGGR, val);						ifcengine.sdaiAppend (aggregate, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");					ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiAppend (aggregate, sdaiADB, val);						ifcengine.sdaiAppend (aggregate, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiAppend(
									const SdaiAggr			aggregate,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiAppend(
								const SdaiAggr			aggregate,
								SdaiPrimitiveType		valueType,
								SdaiInstance			sdaiInstance
							)
{
	assert(sdaiInstance == sdaiINSTANCE);
	return	sdaiAppend(
					aggregate,
					valueType,
					(const void*) sdaiInstance
				);
}

//
//
static	inline	void	sdaiAppend(
								const SdaiAggr			aggregate,
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiAppend(
					aggregate,
					sdaiINSTANCE,
					(const void*) sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiAdd                                                 (http://rdf.bg/ifcdoc/CP64/sdaiAdd.html)
//				const SdaiAggr			aggregate							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiAdd, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiAdd but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiAdd (aggregate, sdaiINTEGER, &val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiAdd (aggregate, sdaiREAL, &val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiAdd (aggregate, sdaiBOOLEAN, &val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiAdd (aggregate, sdaiLOGICAL, val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiAdd (aggregate, sdaiENUM, val);							ifcengine.sdaiAdd (aggregate, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiAdd (aggregate, sdaiBINARY, val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiAdd (aggregate, sdaiSTRING, val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiAdd (aggregate, sdaiUNICODE, val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiAdd (aggregate, sdaiEXPRESSSTRING, val);				ifcengine.sdaiAdd (aggregate, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiAdd (aggregate, sdaiINSTANCE, val);						ifcengine.sdaiAdd (aggregate, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiAdd (aggregate, sdaiAGGR, val);							ifcengine.sdaiAdd (aggregate, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");					ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiAdd (aggregate, sdaiADB, val);							ifcengine.sdaiAdd (aggregate, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiAdd(
									const SdaiAggr			aggregate,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiAdd(
								const SdaiAggr			aggregate,
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiAdd(
					aggregate,
					sdaiINSTANCE,
					(const void*) sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiInsertByIndex                                       (http://rdf.bg/ifcdoc/CP64/sdaiInsertByIndex.html)
//				const SdaiAggr			aggregate							IN
//				SdaiAggrIndex			index								IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiInsertByIndex, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiInsertByIndex but valid for all put-functions)
//
//	valueType				C/C++															C#
//
//	sdaiINTEGER				int_t val = 123;												int_t val = 123;
//							sdaiInsertByIndex (aggregate, index, sdaiINTEGER, &val);		ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;											double val = 123.456;
//							sdaiInsertByIndex (aggregate, index, sdaiREAL, &val);			ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;										bool val = true;
//							sdaiInsertByIndex (aggregate, index, sdaiBOOLEAN, &val);		ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";											string val = "U";
//							sdaiInsertByIndex (aggregate, index, sdaiLOGICAL, val);			ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";								string val = "NOTDEFINED";
//							sdaiInsertByIndex (aggregate, index, sdaiENUM, val);			ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";								string val = "0123456ABC";
//							sdaiInsertByIndex (aggregate, index, sdaiBINARY, val);			ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";							string val = "My Simple String";
//							sdaiInsertByIndex (aggregate, index, sdaiSTRING, val);			ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";						string val = "Any Unicode String";
//							sdaiInsertByIndex (aggregate, index, sdaiUNICODE, val);			ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";		string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiInsertByIndex (aggregate, index, sdaiEXPRESSSTRING, val);	ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");		int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiInsertByIndex (aggregate, index, sdaiINSTANCE, val);		ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);						int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);							ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiInsertByIndex (aggregate, index, sdaiAGGR, val);			ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;										int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);		int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiInsertByIndex (aggregate, index, sdaiADB, val);				ifcengine.sdaiInsertByIndex (aggregate, index, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);											ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiInsertByIndex(
									const SdaiAggr			aggregate,
									SdaiAggrIndex			index,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiInsertByIndex(
								const SdaiAggr			aggregate,
								SdaiAggrIndex			index,
								SdaiPrimitiveType		valueType,
								SdaiInstance			sdaIInstance
							)
{
	assert(valueType == sdaiINSTANCE);
	return	sdaiInsertByIndex(
					aggregate,
					index,
					valueType,
					(const void*) sdaIInstance
				);
}

//
//
static	inline	void	sdaiInsertByIndex(
								const SdaiAggr			aggregate,
								SdaiAggrIndex			index,
								SdaiInstance			sdaIInstance
							)
{
	return	sdaiInsertByIndex(
					aggregate,
					index,
					sdaiINSTANCE,
					(const void*) sdaIInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiInsertBefore                                        (http://rdf.bg/ifcdoc/CP64/sdaiInsertBefore.html)
//				const SdaiIterator		iterator							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiInsertBefore, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiInsertBefore but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiInsertBefore (iterator, sdaiINTEGER, &val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiInsertBefore (iterator, sdaiREAL, &val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiInsertBefore (iterator, sdaiBOOLEAN, &val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiInsertBefore (iterator, sdaiLOGICAL, val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiInsertBefore (iterator, sdaiENUM, val);					ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiInsertBefore (iterator, sdaiBINARY, val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiInsertBefore (iterator, sdaiSTRING, val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiInsertBefore (iterator, sdaiUNICODE, val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiInsertBefore (iterator, sdaiEXPRESSSTRING, val);		ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiInsertBefore (iterator, sdaiINSTANCE, val);				ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiInsertBefore (iterator, sdaiAGGR, val);					ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");					ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiInsertBefore (iterator, sdaiADB, val);					ifcengine.sdaiInsertBefore (iterator, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiInsertBefore(
									const SdaiIterator		iterator,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static inline void	sdaiInsertBefore(
									SdaiIterator			iterator,
									SdaiPrimitiveType		valueType,
									SdaiInstance			sdaiInstance
								)
{
	assert(valueType == sdaiINSTANCE);
	sdaiInsertBefore(iterator, valueType, (const void*) sdaiInstance);
}

//
//
static inline void	sdaiInsertBefore(
									SdaiIterator			iterator,
									SdaiInstance			sdaiInstance
								)
{
	sdaiInsertBefore(iterator, sdaiINSTANCE, (const void*) sdaiInstance);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif


//
//		sdaiInsertAfter                                         (http://rdf.bg/ifcdoc/CP64/sdaiInsertAfter.html)
//				const SdaiIterator		iterator							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiInsertAfter, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiInsertAfter but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiInsertAfter (iterator, sdaiINTEGER, &val);				ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiInsertAfter (iterator, sdaiREAL, &val);					ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiInsertAfter (iterator, sdaiBOOLEAN, &val);				ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiInsertAfter (iterator, sdaiLOGICAL, val);				ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiInsertAfter (iterator, sdaiENUM, val);					ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiInsertAfter (iterator, sdaiBINARY, val);				ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiInsertAfter (iterator, sdaiSTRING, val);				ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiInsertAfter (iterator, sdaiUNICODE, val);				ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiInsertAfter (iterator, sdaiEXPRESSSTRING, val);			ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiInsertAfter (iterator, sdaiINSTANCE, val);				ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiInsertAfter (iterator, sdaiAGGR, val);					ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");					ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiInsertAfter (iterator, sdaiADB, val);					ifcengine.sdaiInsertAfter (iterator, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiInsertAfter(
									const SdaiIterator		iterator,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static inline void	sdaiInsertAfter(
									const SdaiIterator		iterator,
									SdaiPrimitiveType		valueType,
									SdaiInstance			sdaiInstance
								)
{
	assert(valueType == sdaiINSTANCE);
	sdaiInsertAfter(iterator, valueType, (const void*) sdaiInstance);
}

//
//
static inline void	sdaiInsertAfter(
									const SdaiIterator		iterator,
									SdaiInstance			sdaiInstance
								)
{
	sdaiInsertAfter(iterator, sdaiINSTANCE, (const void*) sdaiInstance);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif


//
//		sdaiCreateADB                                           (http://rdf.bg/ifcdoc/CP64/sdaiCreateADB.html)
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				SdaiADB					returns								OUT
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiCreateADB, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiCreateADB but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							SdaiADB adb = sdaiCreateADB (sdaiINTEGER, &val);			int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							SdaiADB adb = sdaiCreateADB (sdaiREAL, &val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							SdaiADB adb = sdaiCreateADB (sdaiBOOLEAN, &val);			int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							SdaiADB adb = sdaiCreateADB (sdaiLOGICAL, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							SdaiADB adb = sdaiCreateADB (sdaiENUM, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							SdaiADB adb = sdaiCreateADB (sdaiBINARY, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							SdaiADB adb = sdaiCreateADB (sdaiSTRING, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							SdaiADB adb = sdaiCreateADB (sdaiUNICODE, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							SdaiADB adb = sdaiCreateADB (sdaiEXPRESSSTRING, val);		int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							SdaiADB adb = sdaiCreateADB (sdaiINSTANCE, val);			int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							SdaiADB adb = sdaiCreateADB (sdaiAGGR, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiAGGR, val);
//
//	sdaiADB					not applicable
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
SdaiADB			DECL STDC	sdaiCreateADB(
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiADB	sdaiCreateADB(
								SdaiPrimitiveType		valueType,
								SdaiInstance			sdaiInstance
							)
{
	assert(valueType == sdaiINSTANCE);
	return	sdaiCreateADB(
					valueType,
					(const void*) sdaiInstance
				);
}

//
//
static	inline	SdaiADB	sdaiCreateADB(
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiCreateADB(
					sdaiINSTANCE,
					(const void*) sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiCreateAggr                                          (http://rdf.bg/ifcdoc/CP64/sdaiCreateAggr.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//
//				SdaiAggr				returns								OUT
//
//	This call creates an aggregation.
//	The instance has to be present,
//	the attribute argument can be empty (0) in case the aggregation is an nested aggregation for this specific instance,
//	preferred use would be use of sdaiCreateNestedAggr in such a case.
//
SdaiAggr		DECL STDC	sdaiCreateAggr(
									SdaiInstance			instance,
									const SdaiAttr			attribute
								);

//
//		sdaiCreateAggrBN                                        (http://rdf.bg/ifcdoc/CP64/sdaiCreateAggrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				SdaiAggr				returns								OUT
//
//	This call creates an aggregation.
//	The instance has to be present,
//	the attributeName argument can be NULL (0) in case the aggregation is an nested aggregation for this specific instance,
//	preferred use would be use of sdaiCreateNestedAggr in such a case.
//
//	Technically sdaiCreateAggrBN will transform into the following call
//		(attributeName) ?
//			sdaiCreateAggr(
//					instance,
//					sdaiGetAttrDefinition(
//							sdaiGetInstanceType(
//									instance
//								),
//							attributeName
//						)
//				) :
//			sdaiCreateAggr(
//					instance,
//					nullptr
//				);
//
SdaiAggr		DECL STDC	sdaiCreateAggrBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiAggr	sdaiCreateAggrBN(
									SdaiInstance			instance,
									char					* attributeName
								)
{
	return	sdaiCreateAggrBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiCreateNPL                                           (http://rdf.bg/ifcdoc/CP64/sdaiCreateNPL.html)
//				SdaiNPL					returns								OUT
//
//	...
//
SdaiNPL			DECL STDC	sdaiCreateNPL(
								);

//
//		sdaiDeleteNPL                                           (http://rdf.bg/ifcdoc/CP64/sdaiDeleteNPL.html)
//				SdaiNPL					list								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiDeleteNPL(
									SdaiNPL					list
								);

//
//		sdaiCreateNestedAggr                                    (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggr.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiAggr				returns								OUT
//
//	This call creates an aggregation within an aggregation.
//
SdaiAggr		DECL STDC	sdaiCreateNestedAggr(
									SdaiAggr				aggregate
								);

//
//		sdaiCreateNestedAggrByIndex                             (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggrByIndex.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance and replaces the existing member of the specified ordered aggregate instance
//	referenced by the specified index.
//
SdaiAggr		DECL STDC	sdaiCreateNestedAggrByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index
								);

//
//		sdaiInsertNestedAggrByIndex                             (http://rdf.bg/ifcdoc/CP64/sdaiInsertNestedAggrByIndex.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance as a member of the specified ordered aggregate instance.
//	The newly created aggregate is inserted into the aggregate at the position referenced by the specified index.
//
SdaiAggr		DECL STDC	sdaiInsertNestedAggrByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index
								);

//
//		sdaiCreateNestedAggrByItr                               (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggrByItr.html)
//				SdaiIterator			iterator							IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance replacing the current member of the aggregate instance
//	referenced by the specified iterator.
//
SdaiAggr		DECL STDC	sdaiCreateNestedAggrByItr(
									SdaiIterator			iterator
								);

//
//		sdaiInsertNestedAggrBefore                              (http://rdf.bg/ifcdoc/CP64/sdaiInsertNestedAggrBefore.html)
//				SdaiIterator			iterator							IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance as a member of a list instance.
//	The newly created aggregate is inserted into the list instance before the member referenced by the specified iterator.
//
SdaiAggr		DECL STDC	sdaiInsertNestedAggrBefore(
									SdaiIterator			iterator
								);

//
//		sdaiInsertNestedAggrAfter                               (http://rdf.bg/ifcdoc/CP64/sdaiInsertNestedAggrAfter.html)
//				SdaiIterator			iterator							IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance as a member of a list instance.
//	The newly created aggregate is inserted into the list instance after the member referenced by the specified iterator.
//
SdaiAggr		DECL STDC	sdaiInsertNestedAggrAfter(
									SdaiIterator			iterator
								);

//
//		sdaiCreateNestedAggrADB                                 (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggrADB.html)
//				SdaiAggr				aggregate							IN
//				SdaiADB					selaggrInstance						IN
//
//				SdaiAggr				returns								OUT
//
//	The CreateNestedAggrABD function creates an aggregate instance as a member of (an unordered)
//	aggregate instance in the case where the type of the aggregate to create is a SELECT TYPE and
//	ambiguous.
//	Input ADB is expected to have type path.
//	The function sets the value of the ADB with the identifier of the newly created aggregate instance.
//
SdaiAggr		DECL STDC	sdaiCreateNestedAggrADB(
									SdaiAggr				aggregate,
									SdaiADB					selaggrInstance
								);

//
//		sdaiCreateNestedAggrByIndexADB                          (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggrByIndexADB.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//				SdaiADB					selaggrInstance						IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance and replaces the existing member of the specified ordered aggregate instance 
//	referenced by the specified index.
//	Input ADB is expected to have type path.
//	The function sets the value of the ADB with the identifier of the newly created aggregate instance.
//
SdaiAggr		DECL STDC	sdaiCreateNestedAggrByIndexADB(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index,
									SdaiADB					selaggrInstance
								);

//
//		sdaiInsertNestedAggrByIndexADB                          (http://rdf.bg/ifcdoc/CP64/sdaiInsertNestedAggrByIndexADB.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//				SdaiADB					selaggrInstance						IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance as member of the specified ordered aggregate instance. 
//	The newly created aggregate is inserted into the aggregate at the position referenced by the specified index.
//	Input ADB is expected to have type path.
//	The function sets the value of the ADB with the identifier of the newly created aggregate instance.
//
SdaiAggr		DECL STDC	sdaiInsertNestedAggrByIndexADB(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index,
									SdaiADB					selaggrInstance
								);

//
//		sdaiCreateNestedAggrByItrADB                            (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggrByItrADB.html)
//				SdaiIterator			iterator							IN
//				SdaiADB					selaggrInstance						IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance replacing the current member of the aggregate instance 
//	referenced by the specified iterator where the type of the aggregate to create is a SELECT TYPE and ambiguous,
//	Input ADB is expected to have type path.
//	The function sets the value of the ADB with the identifier of the newly created aggregate instance.
//
SdaiAggr		DECL STDC	sdaiCreateNestedAggrByItrADB(
									SdaiIterator			iterator,
									SdaiADB					selaggrInstance
								);

//
//		sdaiInsertNestedAggrBeforeADB                           (http://rdf.bg/ifcdoc/CP64/sdaiInsertNestedAggrBeforeADB.html)
//				SdaiIterator			iterator							IN
//				SdaiADB					selaggrInstance						IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance as a member of a list instance where the type of the aggregate to create is a SELECT TYPE and ambiguous.
//	The newly created aggregate is inserted into the list instance before the member referenced by the specified iterator.
//	Input ADB is expected to have type path.
//	The function sets the value of the ADB with the identifier of the newly created aggregate instance.
//
SdaiAggr		DECL STDC	sdaiInsertNestedAggrBeforeADB(
									SdaiIterator			iterator,
									SdaiADB					selaggrInstance
								);

//
//		sdaiInsertNestedAggrAfterADB                            (http://rdf.bg/ifcdoc/CP64/sdaiInsertNestedAggrAfterADB.html)
//				SdaiIterator			iterator							IN
//				SdaiADB					selaggrInstance						IN
//
//				SdaiAggr				returns								OUT
//
//	The function creates an aggregate instance as a member of a list instance where the type of the aggregate to create is a SELECT TYPE and ambiguous.
//	The newly created aggregate is inserted into the list instance after the member referenced by the specified iterator.
//	Input ADB is expected to have type path.
//	The function sets the value of the ADB with the identifier of the newly created aggregate instance.
//
SdaiAggr		DECL STDC	sdaiInsertNestedAggrAfterADB(
									SdaiIterator			iterator,
									SdaiADB					selaggrInstance
								);

//
//		sdaiRemoveByIndex                                       (http://rdf.bg/ifcdoc/CP64/sdaiRemoveByIndex.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//
//				void					returns
//
//	The function removes the member of the specified list referenced by the specified index
//
void			DECL STDC	sdaiRemoveByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index
								);

//
//		sdaiRemoveByIterator                                    (http://rdf.bg/ifcdoc/CP64/sdaiRemoveByIterator.html)
//				SdaiIterator			iterator							IN
//
//				void					returns
//
//	The function removes the current member of an aggregate instance, that is not an array, referenced by the specified iterator.
//	After executing the function, the iterator position set as if the sdaiNext function had been invoked before the member was removed.
//
void			DECL STDC	sdaiRemoveByIterator(
									SdaiIterator			iterator
								);

//
//		sdaiRemove                                              (http://rdf.bg/ifcdoc/CP64/sdaiRemove.html)
//				SdaiAggr				aggregate							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	The function removes one occurrence of the specified value from the specified unordered aggregate instance.
//
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiRemove, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiRemove but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiRemove (aggregate, sdaiINTEGER, &val);					ifcengine.sdaiRemove (aggregate, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiRemove (aggregate, sdaiREAL, &val);						ifcengine.sdaiRemove (aggregate, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiRemove (aggregate, sdaiBOOLEAN, &val);					ifcengine.sdaiRemove (aggregate, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiRemove (aggregate, sdaiLOGICAL, val);					ifcengine.sdaiRemove (aggregate, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiRemove (aggregate, sdaiENUM, val);						ifcengine.sdaiRemove (aggregate, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiRemove (aggregate, sdaiBINARY, val);					ifcengine.sdaiRemove (aggregate, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiRemove (aggregate, sdaiSTRING, val);					ifcengine.sdaiRemove (aggregate, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiRemove (aggregate, sdaiUNICODE, val);					ifcengine.sdaiRemove (aggregate, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiRemove (aggregate, sdaiEXPRESSSTRING, val);				ifcengine.sdaiRemove (aggregate, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = ...										int_t val = ...
//							sdaiRemove (aggregate, sdaiINSTANCE, val);					ifcengine.sdaiRemove (aggregate, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = ...											int_t val = ...
//							sdaiRemove (aggregate, sdaiAGGR, val);						ifcengine.sdaiRemove (aggregate, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					SdaiADB val = ...											int_t val = ...
//							sdaiRemove (aggregate, sdaiADB, val);						ifcengine.sdaiRemove (aggregate, ifcengine.sdaiADB, val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiRemove(
									SdaiAggr				aggregate,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static inline void	sdaiRemove(
									SdaiAggr				aggregate,
									SdaiPrimitiveType		valueType,
									SdaiInstance			sdaiInstance
								)
{
	assert(valueType == sdaiINSTANCE);
	sdaiRemove(aggregate, valueType, (const void*) sdaiInstance);
}

//
//
static inline void	sdaiRemove(
									SdaiAggr				aggregate,
									SdaiInstance			sdaiInstance
								)
{
	sdaiRemove(aggregate, sdaiINSTANCE, (const void*) sdaiInstance);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiTestArrayByIndex                                    (http://rdf.bg/ifcdoc/CP64/sdaiTestArrayByIndex.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//
//				SdaiBoolean				returns								OUT
//
//	The function tests whether the member of the specified array referenced by the specified index position has a value.
//
SdaiBoolean		DECL STDC	sdaiTestArrayByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index
								);

//
//		sdaiTestArrayByItr                                      (http://rdf.bg/ifcdoc/CP64/sdaiTestArrayByItr.html)
//				SdaiIterator			iterator							IN
//
//				SdaiBoolean				returns								OUT
//
//	The function tests whether the member of the specified array referenced by the specified index position has a value.
//
SdaiBoolean		DECL STDC	sdaiTestArrayByItr(
									SdaiIterator			iterator
								);

//
//		sdaiCreateInstance                                      (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstance.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//
//				SdaiInstance			returns								OUT
//
//	This call creates an instance of the given entity
//
SdaiInstance	DECL STDC	sdaiCreateInstance(
									SdaiModel				model,
									SdaiEntity				entity
								);

//
//		sdaiCreateInstanceBN                                    (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiInstance			returns								OUT
//
//	This call creates an instance of the given entity.
//
//	Technically it will transform into the following call
//		sdaiCreateInstance(
//				model,
//				sdaiGetEntity(
//						model,
//						entityName
//					)
//			);
//
SdaiInstance	DECL STDC	sdaiCreateInstanceBN(
									SdaiModel				model,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiInstance	sdaiCreateInstanceBN(
										SdaiModel				model,
										char					* entityName
									)
{
	return	sdaiCreateInstanceBN(
					model,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiDeleteInstance                                      (http://rdf.bg/ifcdoc/CP64/sdaiDeleteInstance.html)
//				SdaiInstance			instance							IN
//
//				void					returns
//
//	This call will delete an existing instance
//
void			DECL STDC	sdaiDeleteInstance(
									SdaiInstance			instance
								);

//
//		sdaiPutADBTypePath                                      (http://rdf.bg/ifcdoc/CP64/sdaiPutADBTypePath.html)
//				const SdaiADB			ADB									IN
//				int_t					pathCount							IN
//				const char				* path								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiPutADBTypePath(
									const SdaiADB			ADB,
									int_t					pathCount,
									const char				* path
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiPutADBTypePath(
								const SdaiADB			ADB,
								int_t					pathCount,
								char					* path
							)
{
	return	sdaiPutADBTypePath(
					ADB,
					pathCount,
					(const char*) path
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiPutAttr                                             (http://rdf.bg/ifcdoc/CP64/sdaiPutAttr.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPutAttr, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiPutAttr but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiPutAttr (instance, attribute, sdaiINTEGER, &val);		ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiPutAttr (instance, attribute, sdaiREAL, &val);			ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiPutAttr (instance, attribute, sdaiBOOLEAN, &val);		ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiPutAttr (instance, attribute, sdaiLOGICAL, val);		ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiPutAttr (instance, attribute, sdaiENUM, val);			ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiPutAttr (instance, attribute, sdaiBINARY, val);			ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiPutAttr (instance, attribute, sdaiSTRING, val);			ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiPutAttr (instance, attribute, sdaiUNICODE, val);		ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiPutAttr (instance, attribute, sdaiEXPRESSSTRING, val);	ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiPutAttr (instance, attribute, sdaiINSTANCE, val);		ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiPutAttr (instance, attribute, sdaiAGGR, val);			ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");					ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiPutAttr (instance, attribute, sdaiADB, val);			ifcengine.sdaiPutAttr (instance, attribute, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiPutAttr(
									SdaiInstance			instance,
									const SdaiAttr			attribute,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiPutAttr(
								SdaiInstance			instance,
								const SdaiAttr			attribute,
								SdaiPrimitiveType		valueType,
								SdaiInstance			sdaiInstance
							)
{
	assert(valueType == sdaiINSTANCE);
	return	sdaiPutAttr(
					instance,
					attribute,
					valueType,
					(const void*) sdaiInstance
				);
}

//
//
static	inline	void	sdaiPutAttr(
								SdaiInstance			instance,
								const SdaiAttr			attribute,
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiPutAttr(
					instance,
					attribute,
					sdaiINSTANCE,
					(const void*) sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiPutAttrBN                                           (http://rdf.bg/ifcdoc/CP64/sdaiPutAttrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPutAttrBN, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiPutAttrBN but valid for all put-functions)
//
//	valueType				C/C++															C#
//
//	sdaiINTEGER				int_t val = 123;												int_t val = 123;
//							sdaiPutAttrBN (instance, "attrName", sdaiINTEGER, &val);		ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;											double val = 123.456;
//							sdaiPutAttrBN (instance, "attrName", sdaiREAL, &val);			ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;										bool val = true;
//							sdaiPutAttrBN (instance, "attrName", sdaiBOOLEAN, &val);		ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";											string val = "U";
//							sdaiPutAttrBN (instance, "attrName", sdaiLOGICAL, val);			ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";								string val = "NOTDEFINED";
//							sdaiPutAttrBN (instance, "attrName", sdaiENUM, val);			ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";								string val = "0123456ABC";
//							sdaiPutAttrBN (instance, "attrName", sdaiBINARY, val);			ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";							string val = "My Simple String";
//							sdaiPutAttrBN (instance, "attrName", sdaiSTRING, val);			ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";						string val = "Any Unicode String";
//							sdaiPutAttrBN (instance, "attrName", sdaiUNICODE, val);			ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";		string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiPutAttrBN (instance, "attrName", sdaiEXPRESSSTRING, val);	ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");		int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiPutAttrBN (instance, "attrName", sdaiINSTANCE, val);		ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);						int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);							ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiPutAttrBN (instance, "attrName", sdaiAGGR, val);			ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;										int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);		int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiPutAttrBN (instance, "attrName", sdaiADB, val);				ifcengine.sdaiPutAttrBN (instance, "attrName", ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);											ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
//	Technically sdaiPutAttrBN will transform into the following call
//		sdaiPutAttr(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					),
//				valueType,
//				value
//			);
//
void			DECL STDC	sdaiPutAttrBN(
									SdaiInstance			instance,
									SdaiString				attributeName,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiPutAttrBN(
								SdaiInstance			instance,
								char					* attributeName,
								SdaiPrimitiveType		valueType,
								const void				* value
							)
{
	return	sdaiPutAttrBN(
					instance,
					(const char*) attributeName,
					valueType,
					value
				);
}

//
//
static	inline	void	sdaiPutAttrBN(
								SdaiInstance			instance,
								const char				* attributeName,
								SdaiPrimitiveType		valueType,
								SdaiInstance			sdaiInstance
							)
{
	assert(valueType == sdaiINSTANCE);
	return	sdaiPutAttrBN(
					instance,
					attributeName,
					valueType,
					(const void*) sdaiInstance
				);
}

//
//
static	inline	void	sdaiPutAttrBN(
								SdaiInstance			instance,
								const char				* attributeName,
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiPutAttrBN(
					instance,
					attributeName,
					sdaiINSTANCE,
					(const void*) sdaiInstance
				);
}

//
//
static	inline	void	sdaiPutAttrBN(
								SdaiInstance			instance,
								char					* attributeName,
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiPutAttrBN(
					instance,
					(const char*) attributeName,
					sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiUnsetAttr                                           (http://rdf.bg/ifcdoc/CP64/sdaiUnsetAttr.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//
//				void					returns
//
//	This call removes all data from a specific attribute for the given instance
//
void			DECL STDC	sdaiUnsetAttr(
									SdaiInstance			instance,
									const SdaiAttr			attribute
								);

//
//		sdaiUnsetAttrBN                                         (http://rdf.bg/ifcdoc/CP64/sdaiUnsetAttrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				void					returns
//
//	This call removes all data from a specific attribute for the given instance
//
//	Technically it will transform into the following call
//		sdaiUnsetAttr(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					)
//			);
//
void			DECL STDC	sdaiUnsetAttrBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiUnsetAttrBN(
								SdaiInstance			instance,
								char					* attributeName
							)
{
	return	sdaiUnsetAttrBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiSetComment                                          (http://rdf.bg/ifcdoc/CP64/engiSetComment.html)
//				SdaiInstance			instance							IN
//				const char				* comment							IN
//
//				void					returns
//
//	This call can be used to add a comment to an instance when exporting the content. The comment is available in the exported/saved IFC file.
//
void			DECL STDC	engiSetComment(
									SdaiInstance			instance,
									const char				* comment
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	engiSetComment(
								SdaiInstance			instance,
								char					* comment
							)
{
	return	engiSetComment(
					instance,
					(const char*) comment
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetInstanceLocalId                                  (http://rdf.bg/ifcdoc/CP64/engiGetInstanceLocalId.html)
//				SdaiInstance			instance							IN
//
//				ExpressID				returns								OUT
//
//	...
//
ExpressID		DECL STDC	engiGetInstanceLocalId(
									SdaiInstance			instance
								);

//
//		sdaiTestAttr                                            (http://rdf.bg/ifcdoc/CP64/sdaiTestAttr.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiTestAttr(
									SdaiInstance			instance,
									const SdaiAttr			attribute
								);

//
//		sdaiTestAttrBN                                          (http://rdf.bg/ifcdoc/CP64/sdaiTestAttrBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//
//				int_t					returns								OUT
//
//	Technically it will transform into the following call
//		sdaiGetAttrDefinition(
//				sdaiGetInstanceType(
//						instance
//					),
//				attributeName
//			);
//
int_t			DECL STDC	sdaiTestAttrBN(
									SdaiInstance			instance,
									SdaiString				attributeName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	sdaiTestAttrBN(
								SdaiInstance			instance,
								char					* attributeName
							)
{
	return	sdaiTestAttrBN(
					instance,
					(const char*) attributeName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiCreateInstanceEI                                    (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceEI.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//				ExpressID				expressID							IN
//
//				SdaiInstance			returns								OUT
//
//	This call creates an instance at a specific given express ID, the instance is only created if the express ID was not used yet
//
SdaiInstance	DECL STDC	sdaiCreateInstanceEI(
									SdaiModel				model,
									SdaiEntity				entity,
									ExpressID				expressID
								);

//
//		sdaiCreateInstanceBNEI                                  (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceBNEI.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//				ExpressID				expressID							IN
//
//				SdaiInstance			returns								OUT
//
//	This call creates an instance at a specific given express ID, the instance is only created if the express ID was not used yet
//
SdaiInstance	DECL STDC	sdaiCreateInstanceBNEI(
									SdaiModel				model,
									const char				* entityName,
									ExpressID				expressID
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiInstance	sdaiCreateInstanceBNEI(
										SdaiModel				model,
										char					* entityName,
										ExpressID				expressID
									)
{
	return	sdaiCreateInstanceBNEI(
					model,
					(const char*) entityName,
					expressID
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		setSegmentation                                         (http://rdf.bg/ifcdoc/CP64/setSegmentation.html)
//				SdaiModel				model								IN
//				int_t					segmentationParts					IN
//				double					segmentationLength					IN
//
//				void					returns
//
//	This call sets the segmentation for any curved part of an object in case it is defined by a circle, ellipse, nurbs etc.
//
//	If segmentationParts is set to 0 it will fallback on the default setting (i.e. 36),
//	it makes sense to change the segmentation depending on the entity type that is visualized.
//
//	in case segmentationLength is non-zero, this is the maximum length (in file length unit definition) of a segment
//	For example a slightly curved wall with large size will get much more precise segmentation as the segmentLength
//	will force the segmentation for the wall to increase.
//
void			DECL STDC	setSegmentation(
									SdaiModel				model,
									int_t					segmentationParts,
									double					segmentationLength
								);

//
//		getSegmentation                                         (http://rdf.bg/ifcdoc/CP64/getSegmentation.html)
//				SdaiModel				model								IN
//				int_t					* segmentationParts					IN / OUT
//				double					* segmentationLength				IN / OUT
//
//				void					returns
//
//	This returns the set values for segmentationParts and segmentationLength. Both attributes are optional.
//	The values can be changed through the API call setSegmentation().
//	The default values are
//		segmentationParts  = 36
//		segmentationLength = 0.
//
void			DECL STDC	getSegmentation(
									SdaiModel				model,
									int_t					* segmentationParts,
									double					* segmentationLength
								);

//
//		setEpsilon                                              (http://rdf.bg/ifcdoc/CP64/setEpsilon.html)
//				SdaiModel				model								IN
//				int_t					mask								IN
//				double					absoluteEpsilon						IN
//				double					relativeEpsilon						IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setEpsilon(
									SdaiModel				model,
									int_t					mask,
									double					absoluteEpsilon,
									double					relativeEpsilon
								);

//
//		getEpsilon                                              (http://rdf.bg/ifcdoc/CP64/getEpsilon.html)
//				SdaiModel				model								IN
//				int_t					mask								IN
//				double					* absoluteEpsilon					IN / OUT
//				double					* relativeEpsilon					IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getEpsilon(
									SdaiModel				model,
									int_t					mask,
									double					* absoluteEpsilon,
									double					* relativeEpsilon
								);

//
//  Controling API Calls
//

//
//		circleSegments                                          (http://rdf.bg/ifcdoc/CP64/circleSegments.html)
//				int_t					circles								IN
//				int_t					smallCircles						IN
//
//				void					returns
//
//	Please use the setSegmentation call, note it is now a call that is model dependent.
//
//	The circleSegments(circles, smallCircles) can be replaced with
//		double	segmentationLength = 0.;
//		getSegmentation(model, nullptr, &segmentationLength);
//		setSegmentation(model, circles, segmentationLength);
//
void			DECL STDC	circleSegments(
									int_t					circles,
									int_t					smallCircles
								);

//
//		setMaximumSegmentationLength                            (http://rdf.bg/ifcdoc/CP64/setMaximumSegmentationLength.html)
//				SdaiModel				model								IN
//				double					length								IN
//
//				void					returns
//
//	Please use setSegmentation call
//
//	The call setMaximumSegmentationLength(model, length) can be replaced with
//		int_t segmentationParts = 0;
//		getSegmentation(model, &segmentationParts, nullptr);
//		setSegmentation(model, segmentationParts, length);
//
void			DECL STDC	setMaximumSegmentationLength(
									SdaiModel				model,
									double					length
								);

//
//		getProjectUnitConversionFactor                          (http://rdf.bg/ifcdoc/CP64/getProjectUnitConversionFactor.html)
//				SdaiModel				model								IN
//				const char				* unitType							IN
//				const char				** unitPrefix						IN / OUT
//				const char				** unitName							IN / OUT
//				const char				** SIUnitName						IN / OUT
//
//				double					returns								OUT
//
//	...
//
double			DECL STDC	getProjectUnitConversionFactor(
									SdaiModel				model,
									const char				* unitType,
									const char				** unitPrefix,
									const char				** unitName,
									const char				** SIUnitName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	double	getProjectUnitConversionFactor(
								SdaiModel				model,
								char					* unitType,
								char					** unitPrefix,
								char					** unitName,
								char					** SIUnitName
							)
{
	return	getProjectUnitConversionFactor(
					model,
					(const char*) unitType,
					(const char**) unitPrefix,
					(const char**) unitName,
					(const char**) SIUnitName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		getUnitInstanceConversionFactor                         (http://rdf.bg/ifcdoc/CP64/getUnitInstanceConversionFactor.html)
//				SdaiInstance			unitInstance						IN
//				const char				** unitPrefix						IN / OUT
//				const char				** unitName							IN / OUT
//				const char				** SIUnitName						IN / OUT
//
//				double					returns								OUT
//
//	...
//
double			DECL STDC	getUnitInstanceConversionFactor(
									SdaiInstance			unitInstance,
									const char				** unitPrefix,
									const char				** unitName,
									const char				** SIUnitName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	double	getUnitInstanceConversionFactor(
								SdaiInstance			unitInstance,
								char					** unitPrefix,
								char					** unitName,
								char					** SIUnitName
							)
{
	return	getUnitInstanceConversionFactor(
					unitInstance,
					(const char**) unitPrefix,
					(const char**) unitName,
					(const char**) SIUnitName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		setBRepProperties                                       (http://rdf.bg/ifcdoc/CP64/setBRepProperties.html)
//				SdaiModel				model								IN
//				int64_t					consistencyCheck					IN
//				double					fraction							IN
//				double					epsilon								IN
//				int_t					maxVerticesSize						IN
//
//				void					returns
//
//	This call can be used to optimize Boundary Representation geometries
//
//		consistencyCheck
//			bit0  (1)		merge elements in the vertex array are duplicated (epsilon used as distance)
//			bit1  (2)		remove elements in the vertex array that are not referenced by elements in the index array (interpreted as SET if flags are defined)
//			bit2  (4)		merge polygons placed in the same plane and sharing at least one edge
//			bit3  (8)		merge polygons advanced (check of polygons have the opposite direction and are overlapping, but don't share points)
//			bit4  (16)		check if faces are wrongly turned opposite from each other
//			bit5  (32)		check if faces are inside-out
//			bit6  (64)		check if faces result in solid, if not generate both sided faces
//			bit7  (128)		invert direction of the faces / normal's
//			bit8  (256)		export all faces as one conceptual face
//			bit9  (512)		remove irrelevant intermediate points on lines
//			bit10 (1024)	check and repair faces that are not defined in a perfect plane
//
//		fraction
//			To compare adjacent faces, they will be defined as being part of the same conceptual face if the fraction
//			value is larger then the dot product of the normal vector's of the individual faces.
//
//		epsilon
//			This value is used to compare vertex elements, if vertex elements should be merged and the distance is smaller than this epsilon value
//			then it will be defined as equal
//
//		maxVerticesSize
//			if 0 this setting is applied to BoundaryRepresentation based geometries
//			if larger than 0 it is applied to all BoundaryRepresentation based geometries with vertices size smaller or equal to the given number
//
void			DECL STDC	setBRepProperties(
									SdaiModel				model,
									int64_t					consistencyCheck,
									double					fraction,
									double					epsilon,
									int_t					maxVerticesSize
								);

//
//		cleanMemory                                             (http://rdf.bg/ifcdoc/CP64/cleanMemory.html)
//				SdaiModel				model								IN
//				int_t					mode								IN
//
//				void					returns
//
//	This call forces cleaning of memory allocated.
//	The following mode values are effected:
//		0	non-cached geometry tree structures
//		1	cached and non-cached geometry tree structures + resetting buffers for internally used Geometry Kernel instance
//		3	cached and non-cached geometry tree structures
//		4	clean memory allocated within a session for ADB structures and string values (including enumerations requested as wide char).
//
void			DECL STDC	cleanMemory(
									SdaiModel				model,
									int_t					mode
								);

//
//		internalGetP21Line                                      (http://rdf.bg/ifcdoc/CP64/internalGetP21Line.html)
//				SdaiInstance			instance							IN
//
//				ExpressID				returns								OUT
//
//	Returns the line STEP / Express ID of an instance
//
ExpressID		DECL STDC	internalGetP21Line(
									SdaiInstance			instance
								);

//
//		internalForceInstanceFromP21Line                        (http://rdf.bg/ifcdoc/CP64/internalForceInstanceFromP21Line.html)
//				SdaiModel				model								IN
//				ExpressID				P21Line								IN
//
//				SdaiInstance			returns								OUT
//
//	Returns an instance based on the model and STEP / Express ID (even when the instance itself might be non-existant)
//
SdaiInstance	DECL STDC	internalForceInstanceFromP21Line(
									SdaiModel				model,
									ExpressID				P21Line
								);

//
//		internalGetInstanceFromP21Line                          (http://rdf.bg/ifcdoc/CP64/internalGetInstanceFromP21Line.html)
//				SdaiModel				model								IN
//				ExpressID				P21Line								IN
//
//				SdaiInstance			returns								OUT
//
//	Returns an instance based on the model and STEP / Express ID
//
SdaiInstance	DECL STDC	internalGetInstanceFromP21Line(
									SdaiModel				model,
									ExpressID				P21Line
								);

//
//		internalGetXMLID                                        (http://rdf.bg/ifcdoc/CP64/internalGetXMLID.html)
//				SdaiInstance			instance							IN
//				const char				** XMLID							IN / OUT
//
//				const char				* returns							OUT
//
//	In case an XML file is loaded the XML ID values are kept in memory and can be retrieved through this API call
//
const char		DECL * STDC	internalGetXMLID(
									SdaiInstance			instance,
									const char				** XMLID
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* internalGetXMLID(
									SdaiInstance			instance,
									char					** XMLID
								)
{
	return	internalGetXMLID(
					instance,
					(const char**) XMLID
				);
}

//
//
static	inline	const char	* internalGetXMLID(
									SdaiInstance			instance
								)
{
	return	internalGetXMLID(
					instance,
					(const char**) nullptr				//	XMLID
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		setStringUnicode                                        (http://rdf.bg/ifcdoc/CP64/setStringUnicode.html)
//				int_t					unicode								IN
//
//				int_t					returns								OUT
//
//	Set mode of interpretation for arguments of type char*
//		0 - char* (default)
//		1 - wchar_t*
//		2 - char16_t*
//		4 - char32_t*
//
int_t			DECL STDC	setStringUnicode(
									int_t					unicode
								);

//
//		getStringUnicode                                        (http://rdf.bg/ifcdoc/CP64/getStringUnicode.html)
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getStringUnicode(
								);

//
//		engiSetStringEncoding                                   (http://rdf.bg/ifcdoc/CP64/engiSetStringEncoding.html)
//				SdaiModel				model								IN
//				enum_string_encoding	encoding							IN
//
//				int_t					returns								OUT
//
//	sets encoding for sdaiSTRING data type in put and get functions
//	if model is NULL it will set codepage for models, created after the call or for contexts when model is not known
//	returns 1 when successfull of 0 when fails
//
int_t			DECL STDC	engiSetStringEncoding(
									SdaiModel				model,
									enum_string_encoding	encoding
								);

//
//		setFilter                                               (http://rdf.bg/ifcdoc/CP64/setFilter.html)
//				SdaiModel				model								IN
//				int_t					setting								IN
//				int_t					mask								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setFilter(
									SdaiModel				model,
									int_t					setting,
									int_t					mask
								);

//
//		getFilter                                               (http://rdf.bg/ifcdoc/CP64/getFilter.html)
//				SdaiModel				model								IN
//				int_t					mask								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getFilter(
									SdaiModel				model,
									int_t					mask
								);

//
//  Uncategorized API Calls
//

//
//		xxxxGetEntityAndSubTypesExtent                          (http://rdf.bg/ifcdoc/CP64/xxxxGetEntityAndSubTypesExtent.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//
//				SdaiAggr				returns								OUT
//
//	model input parameter is irrelevant, but is required for backwards compatibility
//
SdaiAggr		DECL STDC	xxxxGetEntityAndSubTypesExtent(
									SdaiModel				model,
									SdaiEntity				entity
								);

//
//		xxxxGetEntityAndSubTypesExtentBN                        (http://rdf.bg/ifcdoc/CP64/xxxxGetEntityAndSubTypesExtentBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiAggr				returns								OUT
//
//	Technically xxxxGetEntityAndSubTypesExtentBN will transform into the following call
//		xxxxGetEntityAndSubTypesExtent(
//				model,
//				sdaiGetEntity(
//						model,
//						entityName
//					)
//			);
//
SdaiAggr		DECL STDC	xxxxGetEntityAndSubTypesExtentBN(
									SdaiModel				model,
									const char				* entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiAggr	xxxxGetEntityAndSubTypesExtentBN(
									SdaiModel				model,
									char					* entityName
								)
{
	return	xxxxGetEntityAndSubTypesExtentBN(
					model,
					(const char*) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		xxxxGetAllInstances                                     (http://rdf.bg/ifcdoc/CP64/xxxxGetAllInstances.html)
//				SdaiModel				model								IN
//
//				SdaiAggr				returns								OUT
//
//	This call returns an aggregation containing all instances.
//
SdaiAggr		DECL STDC	xxxxGetAllInstances(
									SdaiModel				model
								);

//
//		xxxxGetInstancesUsing                                   (http://rdf.bg/ifcdoc/CP64/xxxxGetInstancesUsing.html)
//				SdaiInstance			instance							IN
//
//				SdaiAggr				returns								OUT
//
//	This call returns an aggregation containing all instances referencing the given instance.
//
//	note: this is independent from if there are inverse relations defining such an aggregation or parts of it.
//
SdaiAggr		DECL STDC	xxxxGetInstancesUsing(
									SdaiInstance			instance
								);

//
//		xxxxDeleteFromAggregation                               (http://rdf.bg/ifcdoc/CP64/xxxxDeleteFromAggregation.html)
//				SdaiInstance			instance							IN
//				const SdaiAggr			aggregate							IN
//				int_t					elementIndex						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	xxxxDeleteFromAggregation(
									SdaiInstance			instance,
									const SdaiAggr			aggregate,
									int_t					elementIndex
								);

//
//		xxxxGetAttrDefinitionByValue                            (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrDefinitionByValue.html)
//				SdaiInstance			instance							IN
//				const void				* value								IN
//
//				SdaiAttr				returns								OUT
//
//	...
//
SdaiAttr		DECL STDC	xxxxGetAttrDefinitionByValue(
									SdaiInstance			instance,
									const void				* value
								);

//
//		xxxxGetAttrNameByIndex                                  (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrNameByIndex.html)
//				SdaiInstance			instance							IN
//				SdaiInteger				index								IN
//				const char				** name								IN / OUT
//
//				const char				* returns							OUT
//
//	...
//
const char		DECL * STDC	xxxxGetAttrNameByIndex(
									SdaiInstance			instance,
									SdaiInteger				index,
									const char				** name
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* xxxxGetAttrNameByIndex(
									SdaiInstance			instance,
									SdaiInteger				index,
									char					** name
								)
{
	return	xxxxGetAttrNameByIndex(
					instance,
					index,
					(const char**) name
				);
}

//
//
static	inline	const char	* xxxxGetAttrNameByIndex(
									SdaiInstance			instance,
									SdaiInteger				index
								)
{
	return	xxxxGetAttrNameByIndex(
					instance,
					index,
					(const char**) nullptr				//	name
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		iterateOverInstances                                    (http://rdf.bg/ifcdoc/CP64/iterateOverInstances.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				SdaiEntity				* entity							IN / OUT
//				const char				** entityName						IN / OUT
//
//				SdaiInstance			returns								OUT
//
//	This function iterates over all available instances loaded in memory, it is the fastest way to find all instances.
//	Argument entity and entityName are both optional and if non-zero are filled with respectively the entity handle and entity name as char array.
//
SdaiInstance	DECL STDC	iterateOverInstances(
									SdaiModel				model,
									SdaiInstance			instance,
									SdaiEntity				* entity,
									const char				** entityName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiInstance	iterateOverInstances(
										SdaiModel				model,
										SdaiInstance			instance,
										SdaiEntity				* entity,
										char					** entityName
									)
{
	return	iterateOverInstances(
					model,
					instance,
					entity,
					(const char**) entityName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		iterateOverProperties                                   (http://rdf.bg/ifcdoc/CP64/iterateOverProperties.html)
//				SdaiEntity				entity								IN
//				SdaiInteger				index								IN
//
//				int_t					returns								OUT
//
//	This function iterated over all available attributes of a specific given entity.
//	This call is typically used in combination with iterateOverInstances(..).
//
int_t			DECL STDC	iterateOverProperties(
									SdaiEntity				entity,
									SdaiInteger				index
								);

//
//		sdaiGetAggrByIterator                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrByIterator.html)
//				SdaiIterator			iterator							IN
//				SdaiPrimitiveType		valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	valueType argument to specify what type of data caller wants to get and
//	value argument where the caller should provide a buffer, and the function will write the result to.
//
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetAggrByIterator, and it works similarly for all get-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//	The Table 2 shows what valueType can be fulfilled depending on actual model data.
//	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
//	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
//	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
//	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetAggrByIterator but valid for all get-functions)
//
//	valueType				C/C++															C#
//
//	sdaiINTEGER				int_t val;														int_t val;
//							sdaiGetAggrByIterator (iterator, sdaiINTEGER, &val);			ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiINTEGER, out val);
//
//	sdaiREAL or sdaiNUMBER	double val;														double val;
//							sdaiGetAggrByIterator (iterator, sdaiREAL, &val);				ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiREAL, out val);
//
//	sdaiBOOLEAN				SdaiBoolean val;												bool val;
//							sdaiGetAggrByIterator (iterator, sdaiBOOLEAN, &val);			ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiBOOLEAN, out val);
//
//	sdaiLOGICAL				const TCHAR* val;												string val;
//							sdaiGetAggrByIterator (iterator, sdaiLOGICAL, &val);			ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiLOGICAL, out val);
//
//	sdaiENUM				const TCHAR* val;												string val;
//							sdaiGetAggrByIterator (iterator, sdaiENUM, &val);				ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiENUM, out val);
//
//	sdaiBINARY				const TCHAR* val;												string val;
//							sdaiGetAggrByIterator (iterator, sdaiBINARY, &val);				ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiBINARY, out val);
//
//	sdaiSTRING				const char* val;												string val;
//							sdaiGetAggrByIterator (iterator, sdaiSTRING, &val);				ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiSTRING, out val);
//
//	sdaiUNICODE				const wchar_t* val;												string val;
//							sdaiGetAggrByIterator (iterator, sdaiUNICODE, &val);			ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiUNICODE, out val);
//
//	sdaiEXPRESSSTRING		const char* val;												string val;
//							sdaiGetAggrByIterator (iterator, sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiEXPRESSSTRING, out val);
//
//	sdaiINSTANCE			SdaiInstance val;												int_t val;
//							sdaiGetAggrByIterator (iterator, sdaiINSTANCE, &val);			ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiINSTANCE, out val);
//
//	sdaiAGGR				SdaiAggr aggr;													int_t aggr;
//							sdaiGetAggrByIterator (iterator, sdaiAGGR, &aggr);				ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiAGGR, out aggr);
//
//	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();								int_t adb = 0;	//	it is important to initialize
//							sdaiGetAggrByIterator (iterator, sdaiADB, adb);					ifcengine.sdaiGetAggrByIterator (iterator, ifcengine.sdaiADB, out adb);		
//							sdaiDeleteADB (adb);
//
//							SdaiADB adb = nullptr;	//	it is important to initialize
//							sdaiGetAggrByIterator (iterator, sdaiADB, &adb);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
//		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
//
void			DECL * STDC	sdaiGetAggrByIterator(
									SdaiIterator			iterator,
									SdaiPrimitiveType		valueType,
									void					* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiInstance sdaiGetAggrByIterator(
									SdaiIterator			iterator,
									SdaiInstance			* sdaiInstance
								)
{
	return	(SdaiInstance) sdaiGetAggrByIterator(
					iterator,
					sdaiINSTANCE,
					(void*) sdaiInstance
				);
}

//
//
static	inline	SdaiInstance sdaiGetAggrByIterator(
									SdaiIterator			iterator
								)
{
	SdaiInstance sdaiInstance = 0;
	return	sdaiGetAggrByIterator(
					iterator,
					&sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiPutAggrByIterator                                   (http://rdf.bg/ifcdoc/CP64/sdaiPutAggrByIterator.html)
//				SdaiIterator			iterator							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	valueType argument to specify what type of data caller wants to put
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPutAggrByIterator, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiPutAggrByIterator but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiPutAggrByIterator (iterator, sdaiINTEGER, &val);		ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiPutAggrByIterator (iterator, sdaiREAL, &val);			ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiPutAggrByIterator (iterator, sdaiBOOLEAN, &val);		ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiPutAggrByIterator (iterator, sdaiLOGICAL, val);			ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiPutAggrByIterator (iterator, sdaiENUM, val);			ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiPutAggrByIterator (iterator, sdaiBINARY, val);			ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiPutAggrByIterator (iterator, sdaiSTRING, val);			ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiPutAggrByIterator (iterator, sdaiUNICODE, val);			ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiPutAggrByIterator (iterator, sdaiEXPRESSSTRING, val);	ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
//							sdaiPutAggrByIterator (iterator, sdaiINSTANCE, val);		ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
//							sdaiPutAttr (val, sdaiINSTANCE, inst);						ifcengine.sdaiPutAttr (val, ifcengine.sdaiINSTANCE, inst);
//							sdaiPutAggrByIterator (iterator, sdaiAGGR, val);			ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiAGGR, val);
//
//	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
//							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
//							sdaiPutADBTypePath (val, 1, "IFCINTEGER");					ifcengine.sdaiPutADBTypePath (val, 1, "IFCINTEGER");
//							sdaiPutAggrByIterator (iterator, sdaiADB, val);				ifcengine.sdaiPutAggrByIterator (iterator, ifcengine.sdaiADB, val);	
//							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
void			DECL STDC	sdaiPutAggrByIterator(
									SdaiIterator			iterator,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	sdaiPutAggrByIterator(
								SdaiIterator			iterator,
								SdaiPrimitiveType		valueType,
								SdaiInstance			sdaiInstance
							)
{
	assert(valueType == sdaiINSTANCE);
	return	sdaiPutAggrByIterator(
					iterator,
					valueType,
					(const void*) sdaiInstance
				);
}

//
//
static	inline	void	sdaiPutAggrByIterator(
								SdaiIterator			iterator,
								SdaiInstance			sdaiInstance
							)
{
	return	sdaiPutAggrByIterator(
					iterator,
					sdaiINSTANCE,
					(const void*) sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		internalSetLink                                         (http://rdf.bg/ifcdoc/CP64/internalSetLink.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//				int_t					linked_id							IN
//
//				void					returns
//
//	...
//
void			DECL STDC	internalSetLink(
									SdaiInstance			instance,
									SdaiString				attributeName,
									int_t					linked_id
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	internalSetLink(
								SdaiInstance			instance,
								char					* attributeName,
								int_t					linked_id
							)
{
	return	internalSetLink(
					instance,
					(const char*) attributeName,
					linked_id
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		internalAddAggrLink                                     (http://rdf.bg/ifcdoc/CP64/internalAddAggrLink.html)
//				const SdaiAggr			aggregate							IN
//				int_t					linked_id							IN
//
//				void					returns
//
//	...
//
void			DECL STDC	internalAddAggrLink(
									const SdaiAggr			aggregate,
									int_t					linked_id
								);

//
//		engiGetNotReferedAggr                                   (http://rdf.bg/ifcdoc/CP64/engiGetNotReferedAggr.html)
//				SdaiModel				model								IN
//				int_t					* value								IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetNotReferedAggr(
									SdaiModel				model,
									int_t					* value
								);

//
//		engiGetAttributeAggr                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttributeAggr.html)
//				SdaiInstance			instance							IN
//				int_t					* value								IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAttributeAggr(
									SdaiInstance			instance,
									int_t					* value
								);

//
//		engiGetAggrUnknownElement                               (http://rdf.bg/ifcdoc/CP64/engiGetAggrUnknownElement.html)
//				const SdaiAggr			aggregate							IN
//				int_t					elementIndex						IN
//				SdaiPrimitiveType		* valueType							IN / OUT
//				void					* value								IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggrUnknownElement(
									const SdaiAggr			aggregate,
									int_t					elementIndex,
									SdaiPrimitiveType		* valueType,
									void					* value
								);

//
//		sdaiErrorQuery                                          (http://rdf.bg/ifcdoc/CP64/sdaiErrorQuery.html)
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiErrorQuery(
								);

//
//  Geometry Kernel related API Calls
//

//
//		owlGetModel                                             (http://rdf.bg/ifcdoc/CP64/owlGetModel.html)
//				SdaiModel				model								IN
//				int64_t					* owlModel							IN / OUT
//
//				void					returns
//
//	Returns a handle to the model within the Geometry Kernel.
//
//	Note: the STEP Engine uses one or more models within the Geometry Kernel to generate design trees
//		  within the Geometry Kernel. All Geometry Kernel calls can be called with the STEP model handle also,
//		  however most correct would be to get and use the Geometry Kernel handle.
//
void			DECL STDC	owlGetModel(
									SdaiModel				model,
									int64_t					* owlModel
								);

//
//		owlGetInstance                                          (http://rdf.bg/ifcdoc/CP64/owlGetInstance.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	Returns a handle to the instance representing the head of design tree within the Geometry Kernel.
//
//	Note: the STEP Engine uses one or more models within the Geometry Kernel to generate design trees
//		  within the Geometry Kernel. All Geometry Kernel calls can be called with the STEP instance handle also,
//		  however most correct would be to get and use the Geometry Kernel handle.
//
void			DECL STDC	owlGetInstance(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstance
								);

//
//		owlMaterialInstance                                     (http://rdf.bg/ifcdoc/CP64/owlMaterialInstance.html)
//				SdaiInstance			instanceBase						IN
//				SdaiInstance			instanceContext						IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlMaterialInstance(
									SdaiInstance			instanceBase,
									SdaiInstance			instanceContext,
									int64_t					* owlInstance
								);

//
//		owlBuildInstance                                        (http://rdf.bg/ifcdoc/CP64/owlBuildInstance.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	Returns a handle to the instance representing the head of design tree within the Geometry Kernel.
//	If no design tree is created yet it will be created on-the-fly.
//
//	Note: the STEP Engine uses one or more models within the Geometry Kernel to generate design trees
//		  within the Geometry Kernel. All Geometry Kernel calls can be called with the STEP instance handle also,
//		  however most correct would be to get and use the Geometry Kernel handle.
//
void			DECL STDC	owlBuildInstance(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstance
								);

//
//		owlBuildInstanceInContext                               (http://rdf.bg/ifcdoc/CP64/owlBuildInstanceInContext.html)
//				SdaiInstance			instanceBase						IN
//				SdaiInstance			instanceContext						IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	Returns a handle to the instance representing the head of design tree within the Geometry Kernel.
//	If no design tree is created yet it will be created on-the-fly.
//
//	Note: the STEP Engine uses one or more models within the Geometry Kernel to generate design trees
//		  within the Geometry Kernel. All Geometry Kernel calls can be called with the STEP instance handle also,
//		  however most correct would be to get and use the Geometry Kernel handle.
//
void			DECL STDC	owlBuildInstanceInContext(
									SdaiInstance			instanceBase,
									SdaiInstance			instanceContext,
									int64_t					* owlInstance
								);

//
//		engiInstanceUsesSegmentation                            (http://rdf.bg/ifcdoc/CP64/engiInstanceUsesSegmentation.html)
//				SdaiInstance			instance							IN
//
//				bool					returns								OUT
//
//	...
//
bool			DECL STDC	engiInstanceUsesSegmentation(
									SdaiInstance			instance
								);

//
//		owlBuildInstances                                       (http://rdf.bg/ifcdoc/CP64/owlBuildInstances.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstanceComplete				IN / OUT
//				int64_t					* owlInstanceSolids					IN / OUT
//				int64_t					* owlInstanceVoids					IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlBuildInstances(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstanceComplete,
									int64_t					* owlInstanceSolids,
									int64_t					* owlInstanceVoids
								);

//
//		owlGetMappedItem                                        (http://rdf.bg/ifcdoc/CP64/owlGetMappedItem.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//				double					* transformationMatrix				IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlGetMappedItem(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstance,
									double					* transformationMatrix
								);

//
//		getInstanceDerivedPropertiesInModelling                 (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedPropertiesInModelling.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* height							IN / OUT
//				double					* width								IN / OUT
//				double					* thickness							IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceDerivedPropertiesInModelling(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* height,
									double					* width,
									double					* thickness
								);

//
//		getInstanceDerivedBoundingBox                           (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedBoundingBox.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* Ox								IN / OUT
//				double					* Oy								IN / OUT
//				double					* Oz								IN / OUT
//				double					* Vx								IN / OUT
//				double					* Vy								IN / OUT
//				double					* Vz								IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceDerivedBoundingBox(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* Ox,
									double					* Oy,
									double					* Oz,
									double					* Vx,
									double					* Vy,
									double					* Vz
								);

//
//		getInstanceTransformationMatrix                         (http://rdf.bg/ifcdoc/CP64/getInstanceTransformationMatrix.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* _11								IN / OUT
//				double					* _12								IN / OUT
//				double					* _13								IN / OUT
//				double					* _14								IN / OUT
//				double					* _21								IN / OUT
//				double					* _22								IN / OUT
//				double					* _23								IN / OUT
//				double					* _24								IN / OUT
//				double					* _31								IN / OUT
//				double					* _32								IN / OUT
//				double					* _33								IN / OUT
//				double					* _34								IN / OUT
//				double					* _41								IN / OUT
//				double					* _42								IN / OUT
//				double					* _43								IN / OUT
//				double					* _44								IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceTransformationMatrix(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* _11,
									double					* _12,
									double					* _13,
									double					* _14,
									double					* _21,
									double					* _22,
									double					* _23,
									double					* _24,
									double					* _31,
									double					* _32,
									double					* _33,
									double					* _34,
									double					* _41,
									double					* _42,
									double					* _43,
									double					* _44
								);

//
//		getInstanceDerivedTransformationMatrix                  (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedTransformationMatrix.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* _11								IN / OUT
//				double					* _12								IN / OUT
//				double					* _13								IN / OUT
//				double					* _14								IN / OUT
//				double					* _21								IN / OUT
//				double					* _22								IN / OUT
//				double					* _23								IN / OUT
//				double					* _24								IN / OUT
//				double					* _31								IN / OUT
//				double					* _32								IN / OUT
//				double					* _33								IN / OUT
//				double					* _34								IN / OUT
//				double					* _41								IN / OUT
//				double					* _42								IN / OUT
//				double					* _43								IN / OUT
//				double					* _44								IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceDerivedTransformationMatrix(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* _11,
									double					* _12,
									double					* _13,
									double					* _14,
									double					* _21,
									double					* _22,
									double					* _23,
									double					* _24,
									double					* _31,
									double					* _32,
									double					* _33,
									double					* _34,
									double					* _41,
									double					* _42,
									double					* _43,
									double					* _44
								);

//
//		internalGetBoundingBox                                  (http://rdf.bg/ifcdoc/CP64/internalGetBoundingBox.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	internalGetBoundingBox(
									SdaiModel				model,
									SdaiInstance			instance
								);

//
//		internalGetCenter                                       (http://rdf.bg/ifcdoc/CP64/internalGetCenter.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	internalGetCenter(
									SdaiModel				model,
									SdaiInstance			instance
								);

//
//		getRootAxis2Placement                                   (http://rdf.bg/ifcdoc/CP64/getRootAxis2Placement.html)
//				SdaiModel				model								IN
//				bool					exclusiveIfHasGeometry				IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getRootAxis2Placement(
									SdaiModel				model,
									bool					exclusiveIfHasGeometry
								);

//
//		getGlobalPlacement                                      (http://rdf.bg/ifcdoc/CP64/getGlobalPlacement.html)
//				SdaiModel				model								IN
//				double					* origin							IN / OUT
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	getGlobalPlacement(
									SdaiModel				model,
									double					* origin
								);

//
//		setGlobalPlacement                                      (http://rdf.bg/ifcdoc/CP64/setGlobalPlacement.html)
//				SdaiModel				model								IN
//				const double			* origin							IN
//				bool					includeRotation						IN
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	setGlobalPlacement(
									SdaiModel				model,
									const double			* origin,
									bool					includeRotation
								);

//
//		getTimeStamp                                            (http://rdf.bg/ifcdoc/CP64/getTimeStamp.html)
//				SdaiModel				model								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getTimeStamp(
									SdaiModel				model
								);

//
//		setInstanceReference                                    (http://rdf.bg/ifcdoc/CP64/setInstanceReference.html)
//				SdaiInstance			instance							IN
//				int_t					value								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	setInstanceReference(
									SdaiInstance			instance,
									int_t					value
								);

//
//		getInstanceReference                                    (http://rdf.bg/ifcdoc/CP64/getInstanceReference.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceReference(
									SdaiInstance			instance
								);

//
//		inferenceInstance                                       (http://rdf.bg/ifcdoc/CP64/inferenceInstance.html)
//				SdaiInstance			instance							IN
//
//				SdaiInstance			returns								OUT
//
//	This call allows certain constructs to complete implicitely already available data
//	Specifically for IFC4.3 and higher calls using the instances of the following entities are supported:
//		IfcAlignment	   => in case business logic is defined and not geometricaly representation is available yet
//							  the geometrical representation will be constructed on the fly, i.e.
//							  an IfcCompositeCurve with IfcCurveSegment instances for the horizontal alignment 
//							  an IfcGradientCurve with IfcCurveSegment instances for the vertical alignment 
//							  an IfcSegmentedReferenceCurve with IfcCurveSegment instances for the cant alignment
//		IfcLinearPlacement => in case CartesianPosition is empty the internally calculated matrix will be
//							  represented as an IfcAxis2Placement
//
SdaiInstance	DECL STDC	inferenceInstance(
									SdaiInstance			instance
								);

//
//		sdaiValidateSchemaInstance                              (http://rdf.bg/ifcdoc/CP64/sdaiValidateSchemaInstance.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiValidateSchemaInstance(
									SdaiInstance			instance
								);

//
//  Deprecated API Calls (GENERIC)
//

//
//		engiGetEntityNoArguments                                (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoArguments.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	DEPRECATED use engiGetEntityNoAttributes
//
int_t			DECL STDC	engiGetEntityNoArguments(
									SdaiEntity				entity
								);

//
//		engiGetArgumentType                                     (http://rdf.bg/ifcdoc/CP64/engiGetArgumentType.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiPrimitiveType		returns								OUT
//
//	DEPR4ECATED use engiGetAttrType
//
SdaiPrimitiveType	DECL STDC	engiGetArgumentType(
									const SdaiAttr			attribute
								);

//
//		engiGetAttributeType                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttributeType.html)
//				const SdaiAttr			attribute							IN
//
//				SdaiPrimitiveType		returns								OUT
//
//	DEPRECATED use engiGetAttrType
//
SdaiPrimitiveType	DECL STDC	engiGetAttributeType(
									const SdaiAttr			attribute
								);

//
//		engiGetEntityArgumentIndex                              (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentIndex.html)
//				SdaiEntity				entity								IN
//				const char				* argumentName						IN
//
//				int_t					returns								OUT
//
//	DEPRECATED use engiGetEntityAttributeIndex
//
int_t			DECL STDC	engiGetEntityArgumentIndex(
									SdaiEntity				entity,
									const char				* argumentName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	engiGetEntityArgumentIndex(
								SdaiEntity				entity,
								char					* argumentName
							)
{
	return	engiGetEntityArgumentIndex(
					entity,
					(const char*) argumentName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		engiGetAggrElement                                      (http://rdf.bg/ifcdoc/CP64/engiGetAggrElement.html)
//				const SdaiAggr			aggregate							IN
//				SdaiInteger				index								IN
//				SdaiPrimitiveType		valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	This call is deprecated, please use call sdaiGetAggrByIndex instead.
//
void			DECL * STDC	engiGetAggrElement(
									const SdaiAggr			aggregate,
									SdaiInteger				index,
									SdaiPrimitiveType		valueType,
									void					* value
								);

//
//		engiGetEntityArgument                                   (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgument.html)
//				SdaiEntity				entity								IN
//				const char				* argumentName						IN
//
//				SdaiAttr				returns								OUT
//
//	Deprecated, please use the API call sdaiGetAttrDefinition() instead
//
SdaiAttr		DECL STDC	engiGetEntityArgument(
									SdaiEntity				entity,
									const char				* argumentName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiAttr	engiGetEntityArgument(
									SdaiEntity				entity,
									char					* argumentName
								)
{
	return	engiGetEntityArgument(
					entity,
					(const char*) argumentName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetADBTypePathx                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetADBTypePathx.html)
//				const SdaiADB			ADB									IN
//				int_t					typeNameNumber						IN
//				const char				** path								IN / OUT
//
//				const char				* returns							OUT
//
//	This call is deprecated, please use call sdaiGetADBTypePath instead.
//
const char		DECL * STDC	sdaiGetADBTypePathx(
									const SdaiADB			ADB,
									int_t					typeNameNumber,
									const char				** path
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	const char	* sdaiGetADBTypePathx(
									const SdaiADB			ADB,
									int_t					typeNameNumber,
									char					** path
								)
{
	return	sdaiGetADBTypePathx(
					ADB,
					typeNameNumber,
					(const char**) path
				);
}

//
//
static	inline	const char	* sdaiGetADBTypePathx(
									const SdaiADB			ADB,
									int_t					typeNameNumber
								)
{
	return	sdaiGetADBTypePathx(
					ADB,
					typeNameNumber,
					(const char**) nullptr				//	path
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		xxxxOpenModelByStream                                   (http://rdf.bg/ifcdoc/CP64/xxxxOpenModelByStream.html)
//				int_t					repository							IN
//				const void				* callback							IN
//				const char				* schemaName						IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call engiOpenModelByStream instead.
//
int_t			DECL STDC	xxxxOpenModelByStream(
									int_t					repository,
									const void				* callback,
									const char				* schemaName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	xxxxOpenModelByStream(
								int_t					repository,
								const void				* callback,
								char					* schemaName
							)
{
	return	xxxxOpenModelByStream(
					repository,
					callback,
					(const char*) schemaName
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiCreateIterator                                      (http://rdf.bg/ifcdoc/CP64/sdaiCreateIterator.html)
//				const SdaiAggr			aggregate							IN
//
//				SdaiIterator			returns								OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
//
SdaiIterator	DECL STDC	sdaiCreateIterator(
									const SdaiAggr			aggregate
								);

//
//		sdaiDeleteIterator                                      (http://rdf.bg/ifcdoc/CP64/sdaiDeleteIterator.html)
//				SdaiIterator			iterator							IN
//
//				void					returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
//
void			DECL STDC	sdaiDeleteIterator(
									SdaiIterator			iterator
								);

//
//		sdaiBeginning                                           (http://rdf.bg/ifcdoc/CP64/sdaiBeginning.html)
//				SdaiIterator			iterator							IN
//
//				void					returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
//
void			DECL STDC	sdaiBeginning(
									SdaiIterator			iterator
								);

//
//		sdaiNext                                                (http://rdf.bg/ifcdoc/CP64/sdaiNext.html)
//				SdaiIterator			iterator							IN
//
//				SdaiBoolean				returns								OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
//
SdaiBoolean		DECL STDC	sdaiNext(
									SdaiIterator			iterator
								);

//
//		sdaiPrevious                                            (http://rdf.bg/ifcdoc/CP64/sdaiPrevious.html)
//				SdaiIterator			iterator							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
//
int_t			DECL STDC	sdaiPrevious(
									SdaiIterator			iterator
								);

//
//		sdaiEnd                                                 (http://rdf.bg/ifcdoc/CP64/sdaiEnd.html)
//				SdaiIterator			iterator							IN
//
//				void					returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
//
void			DECL STDC	sdaiEnd(
									SdaiIterator			iterator
								);

//
//		sdaiIsMember                                            (http://rdf.bg/ifcdoc/CP64/sdaiIsMember.html)
//				SdaiAggr				aggregate							IN
//				SdaiPrimitiveType		valueType							IN
//				const void				* value								IN
//
//				SdaiBoolean				returns								OUT
//
//	The function determines whether the specified primitive or instance value is contained
//	in the aggregate. In the case of aggregate members represented by ADBs, both the data value and data
//	type are compared.
//
//	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiIsMember, and it works similarly for all put-functions.
//	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
//		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
//
//
//	Table 1 – Required value buffer depending on valueType (on the example of sdaiIsMember but valid for all put-functions)
//
//	valueType				C/C++														C#
//
//	sdaiINTEGER				int_t val = 123;											int_t val = 123;
//							sdaiIsMember (sdaiINTEGER, &val);							ifcengine.sdaiIsMember (ifcengine.sdaiINTEGER, ref val);
//
//	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
//							sdaiIsMember (sdaiREAL, &val);								ifcengine.sdaiIsMember (ifcengine.sdaiREAL, ref val);
//
//	sdaiBOOLEAN				SdaiBoolean val = sdaiTRUE;									bool val = true;
//							sdaiIsMember (sdaiBOOLEAN, &val);							ifcengine.sdaiIsMember (ifcengine.sdaiBOOLEAN, ref val);
//
//	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
//							sdaiIsMember (sdaiLOGICAL, val);							ifcengine.sdaiIsMember (ifcengine.sdaiLOGICAL, val);
//
//	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
//							sdaiIsMember (sdaiENUM, val);								ifcengine.sdaiIsMember (ifcengine.sdaiENUM, val);
//
//	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
//							sdaiIsMember (sdaiBINARY, val);								ifcengine.sdaiIsMember (ifcengine.sdaiBINARY, val);
//
//	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
//							sdaiIsMember (sdaiSTRING, val);								ifcengine.sdaiIsMember (ifcengine.sdaiSTRING, val);
//
//	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
//							sdaiIsMember (sdaiUNICODE, val);							ifcengine.sdaiIsMember (ifcengine.sdaiUNICODE, val);
//
//	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
//							sdaiIsMember (sdaiEXPRESSSTRING, val);						ifcengine.sdaiIsMember (ifcengine.sdaiEXPRESSSTRING, val);
//
//	sdaiINSTANCE			SdaiInstance val = ...										int_t val = ...
//							sdaiIsMember (sdaiINSTANCE, val);							ifcengine.sdaiIsMember (ifcengine.sdaiINSTANCE, val);
//
//	sdaiAGGR				SdaiAggr val = ...											int_t val = ...
//							sdaiIsMember (sdaiAGGR, val);								ifcengine.sdaiIsMember (ifcengine.sdaiAGGR, val);
//
//	sdaiADB					SdaiADB val = ...											int_t val = ...
//							sdaiIsMember (sdaiADB, val);								ifcengine.sdaiIsMember (ifcengine.sdaiADB, val);
//
//	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
//	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t.
//
//
//	Table 2 - valueType can be requested depending on actual model data.
//
//	valueType		Works for following values in the model
//				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
//	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
//	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
//	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
//	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
//	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
//	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
//	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
//	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
//	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
//
SdaiBoolean		DECL STDC	sdaiIsMember(
									SdaiAggr				aggregate,
									SdaiPrimitiveType		valueType,
									const void				* value
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	SdaiBoolean	sdaiIsMember(
									SdaiAggr			aggregate,
									SdaiPrimitiveType	valueType,
									SdaiInstance		sdaiInstance
							)
{
	assert(valueType == sdaiINSTANCE);
	return	sdaiIsMember(
					aggregate,
					valueType,
					(void*) sdaiInstance
				);
}

//
//
static	inline	SdaiBoolean	sdaiIsMember(
									SdaiAggr			aggregate,
									SdaiInstance		sdaiInstance
							)
{
	return	sdaiIsMember(
					aggregate,
					sdaiINSTANCE,
					(void*) sdaiInstance
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		sdaiGetAggrElementBoundByItr                            (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrElementBoundByItr.html)
//				SdaiIterator			iterator							IN
//
//				SdaiInteger				returns								OUT
//
//	The function returns the current value of the real precision, the string width, or the binary width
//	for the current member referenced by the specified iterator.
//
SdaiInteger		DECL STDC	sdaiGetAggrElementBoundByItr(
									SdaiIterator			iterator
								);

//
//		sdaiGetAggrElementBoundByIndex                          (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrElementBoundByIndex.html)
//				SdaiAggr				aggregate							IN
//				SdaiAggrIndex			index								IN
//
//				SdaiInteger				returns								OUT
//
//	The function returns the current value of the real precision, the string width, or the binary width 
//	of the aggregate element at the specified index position in the specified ordered aggregate instance.
//
SdaiInteger		DECL STDC	sdaiGetAggrElementBoundByIndex(
									SdaiAggr				aggregate,
									SdaiAggrIndex			index
								);

//
//		sdaiGetLowerBound                                       (http://rdf.bg/ifcdoc/CP64/sdaiGetLowerBound.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiInteger				returns								OUT
//
//	The function returns the current value of the lower bound, or index, of the specified aggregate instance.
//
SdaiInteger		DECL STDC	sdaiGetLowerBound(
									SdaiAggr				aggregate
								);

//
//		sdaiGetUpperBound                                       (http://rdf.bg/ifcdoc/CP64/sdaiGetUpperBound.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiInteger				returns								OUT
//
//	The function returns the current value of the upper bound, or index, of the specified aggregate instance.
//
SdaiInteger		DECL STDC	sdaiGetUpperBound(
									SdaiAggr				aggregate
								);

//
//		sdaiGetLowerIndex                                       (http://rdf.bg/ifcdoc/CP64/sdaiGetLowerIndex.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiInteger				returns								OUT
//
//	The function returns the value of the lower index of the specified array instance when it was created.
//
SdaiInteger		DECL STDC	sdaiGetLowerIndex(
									SdaiAggr				aggregate
								);

//
//		sdaiGetUpperIndex                                       (http://rdf.bg/ifcdoc/CP64/sdaiGetUpperIndex.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiInteger				returns								OUT
//
//	The function returns the value of the upper index of the specified array instance when it was created.
//
SdaiInteger		DECL STDC	sdaiGetUpperIndex(
									SdaiAggr				aggregate
								);

//
//		sdaiUnsetArrayByIndex                                   (http://rdf.bg/ifcdoc/CP64/sdaiUnsetArrayByIndex.html)
//				SdaiArray				array								IN
//				SdaiAggrIndex			index								IN
//
//				void					returns
//
//	The function restores the unset (not assigned a value) status of the member
//	of the specified array at the specified index position.
//
void			DECL STDC	sdaiUnsetArrayByIndex(
									SdaiArray				array,
									SdaiAggrIndex			index
								);

//
//		sdaiUnsetArrayByItr                                     (http://rdf.bg/ifcdoc/CP64/sdaiUnsetArrayByItr.html)
//				SdaiIterator			iterator							IN
//
//				void					returns
//
//	The function restores the unset (not assigned a value) status of a member at the
//	position identified by the iterator in the array associated with the iterator.
//
void			DECL STDC	sdaiUnsetArrayByItr(
									SdaiIterator			iterator
								);

//
//		sdaiReindexArray                                        (http://rdf.bg/ifcdoc/CP64/sdaiReindexArray.html)
//				SdaiArray				array								IN
//
//				void					returns
//
//	The function resizes the specified array instance setting the lower, or upper index,
//	or both, based upon the current population of the application schema.
//
void			DECL STDC	sdaiReindexArray(
									SdaiArray				array
								);

//
//		sdaiResetArrayIndex                                     (http://rdf.bg/ifcdoc/CP64/sdaiResetArrayIndex.html)
//				SdaiArray				array								IN
//				SdaiAggrIndex			lower								IN
//				SdaiAggrIndex			upper								IN
//
//				void					returns
//
//	The function shall resizes the specified array instance setting the lower and upper
//	index with the specified values.
//
void			DECL STDC	sdaiResetArrayIndex(
									SdaiArray				array,
									SdaiAggrIndex			lower,
									SdaiAggrIndex			upper
								);

//
//		sdaiplusGetAggregationType                              (http://rdf.bg/ifcdoc/CP64/sdaiplusGetAggregationType.html)
//				SdaiInstance			instance							IN
//				const SdaiAggr			aggregate							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call .... instead.
//
int_t			DECL STDC	sdaiplusGetAggregationType(
									SdaiInstance			instance,
									const SdaiAggr			aggregate
								);

//
//		engiGetComplexInstanceNextPart                          (http://rdf.bg/ifcdoc/CP64/engiGetComplexInstanceNextPart.html)
//				SdaiInstance			instance							IN
//
//				SdaiInstance			returns								OUT
//
//	The function returns next part of complex instance or NULL.
//
SdaiInstance	DECL STDC	engiGetComplexInstanceNextPart(
									SdaiInstance			instance
								);

//
//		engiEnableDerivedAttributes                                 (http://rdf.bg/ifcdoc/CP64/engiEnableDerivedAttributes.html)
//				SdaiModel				model								IN
//				SdaiBoolean				enable								IN
//
//				SdaiBoolean				returns								OUT
//
//	The function enables calculation of derived attributes for sdaiGetAttr(BN) and other get value functions and dynamic aggregation indexes.
//	Returns success flag.
//
SdaiBoolean		DECL STDC	engiEnableDerivedAttributes(
									SdaiModel				model,
									SdaiBoolean				enable
								);

//
//		engiEvaluateAllDerivedAttributes                        (http://rdf.bg/ifcdoc/CP64/engiEvaluateAllDerivedAttributes.html)
//				SdaiModel				model								IN
//				SdaiBoolean				includeNullValues					IN
//
//				void					returns
//
//	The function evaluates and replaces all * with values, optionally can handle $ values as derived attributes.
//
void			DECL STDC	engiEvaluateAllDerivedAttributes(
									SdaiModel				model,
									SdaiBoolean				includeNullValues
								);

//
//		xxxxGetAttrType                                         (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrType.html)
//				SdaiInstance			instance							IN
//				const SdaiAttr			attribute							IN
//				const char				** attributeType					IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	xxxxGetAttrType(
									SdaiInstance			instance,
									const SdaiAttr			attribute,
									const char				** attributeType
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	xxxxGetAttrType(
								SdaiInstance			instance,
								const SdaiAttr			attribute,
								char					** attributeType
							)
{
	return	xxxxGetAttrType(
					instance,
					attribute,
					(const char**) attributeType
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		xxxxGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrTypeBN.html)
//				SdaiInstance			instance							IN
//				SdaiString				attributeName						IN
//				const char				** attributeType					IN / OUT
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use calls engiGetAttrTypeBN(..) instead.
//
//	Technically it will transform into the following call
//		xxxxGetAttrType(
//				instance,
//				sdaiGetAttrDefinition(
//						sdaiGetInstanceType(
//								instance
//							),
//						attributeName
//					),
//				attributeType
//			);
//
int_t			DECL STDC	xxxxGetAttrTypeBN(
									SdaiInstance			instance,
									SdaiString				attributeName,
									const char				** attributeType
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	int_t	xxxxGetAttrTypeBN(
								SdaiInstance			instance,
								char					* attributeName,
								char					** attributeType
							)
{
	return	xxxxGetAttrTypeBN(
					instance,
					(SdaiString) attributeName,
					(const char**) attributeType
				);
}

//}} End C++ polymorphic versions
	extern "C" {
#endif

//
//		GetSPFFHeaderItemUnicode                                (http://rdf.bg/ifcdoc/CP64/GetSPFFHeaderItemUnicode.html)
//				SdaiModel				model								IN
//				int_t					itemIndex							IN
//				int_t					itemSubIndex						IN
//				unsigned char			* buffer							IN / OUT
//				int_t					bufferLength						IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call GetSPFFHeaderItem instead
//
int_t			DECL STDC	GetSPFFHeaderItemUnicode(
									SdaiModel				model,
									int_t					itemIndex,
									int_t					itemSubIndex,
									unsigned char			* buffer,
									int_t					bufferLength
								);

//
//  Validation
//

//
//		validateSetOptions                                      (http://rdf.bg/ifcdoc/CP64/validateSetOptions.html)
//				int_t					timeLimitSeconds					IN
//				int_t					issueCntLimit						IN
//				bool					showEachIssueOnce					IN
//				uint64_t				issueTypes							IN
//				uint64_t				mask								IN
//
//				void					returns
//
//	Allows to set a time limit in seconds, setting to 0 means no time limit.
//	Allows to set a count limit, setting to 0 means no count limit.
//	Allows to hide redundant issues.
//
//		bit 0:	(__KNOWN_ENTITY)					entity is defined in the schema
//		bit 1:	(__NO_OF_ARGUMENTS)					number of arguments
//		bit 2:	(__ARGUMENT_EXPRESS_TYPE)			argument value is correct entity, defined type or enumeration value
//		bit 3:	(__ARGUMENT_PRIM_TYPE)				argument value has correct primitive type
//		bit 4:	(__REQUIRED_ARGUMENTS)				non-optional arguments values are provided
//		bit 5:	(__ARRGEGATION_EXPECTED)			aggregation is provided when expected
//		bit 6:	(__AGGREGATION_NOT_EXPECTED)		aggregation is not used when not expected
//		bit 7:	(__AGGREGATION_SIZE)				aggregation size
//		bit 8:	(__AGGREGATION_UNIQUE)				elements in aggregations are unique when required
//		bit 9:	(__COMPLEX_INSTANCE)				complex instances contains full parent chains
//		bit 10:	(__REFERENCE_EXISTS)				referenced instance exists
//		bit 11:	(__ABSTRACT_ENTITY)					abstract entity should not instantiate
//		bit 12:	(__WHERE_RULE)						where-rule check
//		bit 13:	(__UNIQUE_RULE)						unique-rule check
//		bit 14:	(__STAR_USAGE)						* is used only for derived arguments
//		bit 15:	(__CALL_ARGUMENT)					validateModel / validateInstance function argument should be model / instance
//		bit 63:	(__INTERNAL_ERROR)					unspecified error
//
void			DECL STDC	validateSetOptions(
									int_t					timeLimitSeconds,
									int_t					issueCntLimit,
									bool					showEachIssueOnce,
									uint64_t				issueTypes,
									uint64_t				mask
								);

//
//		validateGetOptions                                      (http://rdf.bg/ifcdoc/CP64/validateGetOptions.html)
//				int_t					* timeLimitSeconds					IN / OUT
//				int_t					* issueCntLimit						IN / OUT
//				bool					* showEachIssueOnce					IN / OUT
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	Allows to get the time limit in seconds, value 0 means no time limit, input can be left to NULL if not relevant.
//	Allows to get the count limit, value 0 means no count limit, input can be left to NULL if not relevant.
//	Allows to get hide redundant issues, input can be left to NULL if not relevant.
//	Return value is the issueTypes enabled according to the mask given.
//
//		bit 0:	(__KNOWN_ENTITY)					entity is defined in the schema
//		bit 1:	(__NO_OF_ARGUMENTS)					number of arguments
//		bit 2:	(__ARGUMENT_EXPRESS_TYPE)			argument value is correct entity, defined type or enumeration value
//		bit 3:	(__ARGUMENT_PRIM_TYPE)				argument value has correct primitive type
//		bit 4:	(__REQUIRED_ARGUMENTS)				non-optional arguments values are provided
//		bit 5:	(__ARRGEGATION_EXPECTED)			aggregation is provided when expected
//		bit 6:	(__AGGREGATION_NOT_EXPECTED)		aggregation is not used when not expected
//		bit 7:	(__AGGREGATION_SIZE)				aggregation size
//		bit 8:	(__AGGREGATION_UNIQUE)				elements in aggregations are unique when required
//		bit 9:	(__COMPLEX_INSTANCE)				complex instances contains full parent chains
//		bit 10:	(__REFERENCE_EXISTS)				referenced instance exists
//		bit 11:	(__ABSTRACT_ENTITY)					abstract entity should not instantiate
//		bit 12:	(__WHERE_RULE)						where-rule check
//		bit 13:	(__UNIQUE_RULE)						unique-rule check
//		bit 14:	(__STAR_USAGE)						* is used only for derived arguments
//		bit 15:	(__CALL_ARGUMENT)					validateModel / validateInstance function argument should be model / instance
//		bit 63:	(__INTERNAL_ERROR)					unspecified error
//
uint64_t		DECL STDC	validateGetOptions(
									int_t					* timeLimitSeconds,
									int_t					* issueCntLimit,
									bool					* showEachIssueOnce,
									uint64_t				mask
								);

//
//		validateModel                                           (http://rdf.bg/ifcdoc/CP64/validateModel.html)
//				SdaiModel				model								IN
//
//				ValidationResults		returns								OUT
//
//	Apply validation of a model
//
ValidationResults	DECL STDC	validateModel(
									SdaiModel				model
								);

//
//		validateInstance                                        (http://rdf.bg/ifcdoc/CP64/validateInstance.html)
//				SdaiInstance			instance							IN
//
//				ValidationResults		returns								OUT
//
//	Apply validation of an instance
//
ValidationResults	DECL STDC	validateInstance(
									SdaiInstance			instance
								);

//
//		validateFreeResults                                     (http://rdf.bg/ifcdoc/CP64/validateFreeResults.html)
//				ValidationResults		results								IN
//
//				void					returns
//
//	Clean validation results
//
void			DECL STDC	validateFreeResults(
									ValidationResults		results
								);

//
//		validateGetFirstIssue                                   (http://rdf.bg/ifcdoc/CP64/validateGetFirstIssue.html)
//				ValidationResults		results								IN
//
//				ValidationIssue			returns								OUT
//
//	Get first issue from validation results.
//	If no issues inside validation results or validation results is NULL it will return NULL.
//
ValidationIssue	DECL STDC	validateGetFirstIssue(
									ValidationResults		results
								);

//
//		validateGetNextIssue                                    (http://rdf.bg/ifcdoc/CP64/validateGetNextIssue.html)
//				ValidationIssue			issue								IN
//
//				ValidationIssue			returns								OUT
//
//	Get next issue based on a given issue.
//	If no issues left or validation issue is NULL it will return NULL.
//
ValidationIssue	DECL STDC	validateGetNextIssue(
									ValidationIssue			issue
								);

//
//		validateGetStatus                                       (http://rdf.bg/ifcdoc/CP64/validateGetStatus.html)
//				ValidationResults		results								IN
//
//				enum_validation_status	returns								OUT
//
//	Return value is the issueStatus (enum_validation_status):
//
//		value 0:	(__NONE)						no status set
//		value 1:	(__COMPLETE_ALL)				all issues proceed
//		value 2:	(__COMPLETE_NOT_ALL)			completed but some issues were excluded by option settings
//		value 3:	(__TIME_EXCEED)					validation was finished because of reach time limit
//		value 4:	(__COUNT_EXCEED)				validation was finished because of reach of issue's numbers limit
//
enum_validation_status	DECL STDC	validateGetStatus(
									ValidationResults		results
								);

//
//		validateGetIssueType                                    (http://rdf.bg/ifcdoc/CP64/validateGetIssueType.html)
//				ValidationIssue			issue								IN
//
//				enum_validation_type	returns								OUT
//
//	Return value is the issueType (enum_validation_type):
//
//		bit 0:	(__KNOWN_ENTITY)					entity is defined in the schema
//		bit 1:	(__NO_OF_ARGUMENTS)					number of arguments
//		bit 2:	(__ARGUMENT_EXPRESS_TYPE)			argument value is correct entity, defined type or enumeration value
//		bit 3:	(__ARGUMENT_PRIM_TYPE)				argument value has correct primitive type
//		bit 4:	(__REQUIRED_ARGUMENTS)				non-optional arguments values are provided
//		bit 5:	(__ARRGEGATION_EXPECTED)			aggregation is provided when expected
//		bit 6:	(__AGGREGATION_NOT_EXPECTED)		aggregation is not used when not expected
//		bit 7:	(__AGGREGATION_SIZE)				aggregation size
//		bit 8:	(__AGGREGATION_UNIQUE)				elements in aggregations are unique when required
//		bit 9:	(__COMPLEX_INSTANCE)				complex instances contains full parent chains
//		bit 10:	(__REFERENCE_EXISTS)				referenced instance exists
//		bit 11:	(__ABSTRACT_ENTITY)					abstract entity should not instantiate
//		bit 12:	(__WHERE_RULE)						where-rule check
//		bit 13:	(__UNIQUE_RULE)						unique-rule check
//		bit 14:	(__STAR_USAGE)						* is used only for derived arguments
//		bit 15:	(__CALL_ARGUMENT)					validateModel / validateInstance function argument should be model / instance
//		bit 63:	(__INTERNAL_ERROR)					unspecified error
//
enum_validation_type	DECL STDC	validateGetIssueType(
									ValidationIssue			issue
								);

//
//		validateGetInstance                                     (http://rdf.bg/ifcdoc/CP64/validateGetInstance.html)
//				ValidationIssue			issue								IN
//
//				SdaiInstance			returns								OUT
//
//	Returns the (first) instance related to the given issue
//
SdaiInstance	DECL STDC	validateGetInstance(
									ValidationIssue			issue
								);

//
//		validateGetInstanceRelated                              (http://rdf.bg/ifcdoc/CP64/validateGetInstanceRelated.html)
//				ValidationIssue			issue								IN
//
//				SdaiInstance			returns								OUT
//
//	Returns the second instance related to the given issue (if relevant)
//
SdaiInstance	DECL STDC	validateGetInstanceRelated(
									ValidationIssue			issue
								);

//
//		validateGetEntity                                       (http://rdf.bg/ifcdoc/CP64/validateGetEntity.html)
//				ValidationIssue			issue								IN
//
//				SdaiEntity				returns								OUT
//
//	Returns the entity handle related to the given issue (if relevant)
//
SdaiEntity		DECL STDC	validateGetEntity(
									ValidationIssue			issue
								);

//
//		validateGetAttr                                         (http://rdf.bg/ifcdoc/CP64/validateGetAttr.html)
//				ValidationIssue			issue								IN
//
//				SdaiAttr				returns								OUT
//
//	Returns the attribute handle related to the given issue (if relevant)
//
SdaiAttr		DECL STDC	validateGetAttr(
									ValidationIssue			issue
								);

//
//		validateGetAggrLevel                                    (http://rdf.bg/ifcdoc/CP64/validateGetAggrLevel.html)
//				ValidationIssue			issue								IN
//
//				ValidationIssueLevel	returns								OUT
//
//	specifies nesting level of aggregation or 0
//
ValidationIssueLevel	DECL STDC	validateGetAggrLevel(
									ValidationIssue			issue
								);

//
//		validateGetAggrIndArray                                 (http://rdf.bg/ifcdoc/CP64/validateGetAggrIndArray.html)
//				ValidationIssue			issue								IN
//
//				const int_t				* returns							OUT
//
//	array of indices for each aggregation size is aggrLevel
//
const int_t		DECL * STDC	validateGetAggrIndArray(
									ValidationIssue			issue
								);

//
//		validateGetIssueLevel                                   (http://rdf.bg/ifcdoc/CP64/validateGetIssueLevel.html)
//				ValidationIssue			issue								IN
//
//				int_t					returns								OUT
//
//	Returns the issue level (i.e. severity of the issue) of the issue given as input
//
int_t			DECL STDC	validateGetIssueLevel(
									ValidationIssue			issue
								);

//
//		validateGetDescription                                  (http://rdf.bg/ifcdoc/CP64/validateGetDescription.html)
//				ValidationIssue			issue								IN
//
//				const char				* returns							OUT
//
//	Returns the description text of the issue given as input
//
const char		DECL * STDC	validateGetDescription(
									ValidationIssue			issue
								);

//
//  Deprecated API Calls (GEOMETRY)
//

//
//		initializeModellingInstance                             (http://rdf.bg/ifcdoc/CP64/initializeModellingInstance.html)
//				SdaiModel				model								IN
//				int_t					* noVertices						IN / OUT
//				int_t					* noIndices							IN / OUT
//				double					scale								IN
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call CalculateInstance().
//
int_t			DECL STDC	initializeModellingInstance(
									SdaiModel				model,
									int_t					* noVertices,
									int_t					* noIndices,
									double					scale,
									SdaiInstance			instance
								);

//
//		finalizeModelling                                       (http://rdf.bg/ifcdoc/CP64/finalizeModelling.html)
//				SdaiModel				model								IN
//				float					* vertices							IN / OUT
//				int_t					* indices							IN / OUT
//				int_t					FVF									IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call UpdateInstanceVertexBuffer() and UpdateInstanceIndexBuffer().
//
int_t			DECL STDC	finalizeModelling(
									SdaiModel				model,
									float					* vertices,
									int_t					* indices,
									int_t					FVF
								);

//
//		getInstanceInModelling                                  (http://rdf.bg/ifcdoc/CP64/getInstanceInModelling.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int_t					mode								IN
//				int_t					* startVertex						IN / OUT
//				int_t					* startIndex						IN / OUT
//				int_t					* primitiveCount					IN / OUT
//
//				int_t					returns								OUT
//
//	This call is deprecated, there is no direct / easy replacement although the functionality is present. If you still use this call please contact RDF to find a solution together.
//
int_t			DECL STDC	getInstanceInModelling(
									SdaiModel				model,
									SdaiInstance			instance,
									int_t					mode,
									int_t					* startVertex,
									int_t					* startIndex,
									int_t					* primitiveCount
								);

//
//		setVertexOffset                                         (http://rdf.bg/ifcdoc/CP64/setVertexOffset.html)
//				SdaiModel				model								IN
//				double					x									IN
//				double					y									IN
//				double					z									IN
//
//				void					returns
//
//	This call is deprecated, please use call SetVertexBufferOffset().
//
void			DECL STDC	setVertexOffset(
									SdaiModel				model,
									double					x,
									double					y,
									double					z
								);

//
//		setFormat                                               (http://rdf.bg/ifcdoc/CP64/setFormat.html)
//				SdaiModel				model								IN
//				int_t					setting								IN
//				int_t					mask								IN
//
//				void					returns
//
//	This call is deprecated, please use call SetFormat().
//
void			DECL STDC	setFormat(
									SdaiModel				model,
									int_t					setting,
									int_t					mask
								);

//
//		getConceptualFaceCnt                                    (http://rdf.bg/ifcdoc/CP64/getConceptualFaceCnt.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call GetConceptualFaceCnt().
//
int_t			DECL STDC	getConceptualFaceCnt(
									SdaiInstance			instance
								);

//
//		getConceptualFaceEx                                     (http://rdf.bg/ifcdoc/CP64/getConceptualFaceEx.html)
//				SdaiInstance			instance							IN
//				int_t					index								IN
//				int_t					* startIndexTriangles				IN / OUT
//				int_t					* noIndicesTriangles				IN / OUT
//				int_t					* startIndexLines					IN / OUT
//				int_t					* noIndicesLines					IN / OUT
//				int_t					* startIndexPoints					IN / OUT
//				int_t					* noIndicesPoints					IN / OUT
//				int_t					* startIndexFacePolygons			IN / OUT
//				int_t					* noIndicesFacePolygons				IN / OUT
//				int_t					* startIndexConceptualFacePolygons	IN / OUT
//				int_t					* noIndicesConceptualFacePolygons	IN / OUT
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call GetConceptualFaceEx().
//
int_t			DECL STDC	getConceptualFaceEx(
									SdaiInstance			instance,
									int_t					index,
									int_t					* startIndexTriangles,
									int_t					* noIndicesTriangles,
									int_t					* startIndexLines,
									int_t					* noIndicesLines,
									int_t					* startIndexPoints,
									int_t					* noIndicesPoints,
									int_t					* startIndexFacePolygons,
									int_t					* noIndicesFacePolygons,
									int_t					* startIndexConceptualFacePolygons,
									int_t					* noIndicesConceptualFacePolygons
								);

//
//		createGeometryConversion                                (http://rdf.bg/ifcdoc/CP64/createGeometryConversion.html)
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	This call is deprecated, please use call owlBuildInstance.
//
void			DECL STDC	createGeometryConversion(
									SdaiInstance			instance,
									int64_t					* owlInstance
								);

//
//		convertInstance                                         (http://rdf.bg/ifcdoc/CP64/convertInstance.html)
//				SdaiInstance			instance							IN
//
//				void					returns
//
//	This call is deprecated, please use call owlBuildInstance.
//
void			DECL STDC	convertInstance(
									SdaiInstance			instance
								);

//
//		initializeModellingInstanceEx                           (http://rdf.bg/ifcdoc/CP64/initializeModellingInstanceEx.html)
//				SdaiModel				model								IN
//				int_t					* noVertices						IN / OUT
//				int_t					* noIndices							IN / OUT
//				double					scale								IN
//				SdaiInstance			instance							IN
//				int_t					instanceList						IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call CalculateInstance().
//
int_t			DECL STDC	initializeModellingInstanceEx(
									SdaiModel				model,
									int_t					* noVertices,
									int_t					* noIndices,
									double					scale,
									SdaiInstance			instance,
									int_t					instanceList
								);

//
//		exportModellingAsOWL                                    (http://rdf.bg/ifcdoc/CP64/exportModellingAsOWL.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This call is deprecated, please contact us if you use this call.
//
void			DECL STDC	exportModellingAsOWL(
									SdaiModel				model,
									const char				* fileName
								);

#ifdef __cplusplus
	}
//{{ Begin C++ polymorphic versions

//
//
static	inline	void	exportModellingAsOWL(
								SdaiModel				model,
								char					* fileName
							)
{
	return	exportModellingAsOWL(
					model,
					(const char*) fileName
				);
}

//}} End C++ polymorphic versions
#endif


#undef DECL
#undef STDC
#endif
