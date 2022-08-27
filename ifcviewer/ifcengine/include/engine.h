//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: Geometry Kernel
//
//  Be aware of license fee for use of this library when used commercially
//  More info for commercial use:  peter.bonsma@rdf.bg / contact@rdf.bg
//

#ifndef __RDF_LTD__ENGINE_H
#define __RDF_LTD__ENGINE_H


#include	"engdef.h"

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


#ifdef __cplusplus
	extern "C" {
#endif


#define		OBJECTPROPERTY_TYPE					1
#define		OBJECTTYPEPROPERTY_TYPE				1
#define		DATATYPEPROPERTY_TYPE_BOOLEAN 		2
#define		DATATYPEPROPERTY_TYPE_CHAR			3
#define		DATATYPEPROPERTY_TYPE_INTEGER		4
#define		DATATYPEPROPERTY_TYPE_DOUBLE		5
#define		DATATYPEPROPERTY_TYPE_BYTE			6

//
// Flags can be used in bitwise combination for settings and masks to SetFormat, GetFormat and other functions working with settings
//

#define		FLAGBIT(n)									((int64_t) (((int64_t) 1L)<<(n)))

//control precision / data size
#define		FORMAT_SIZE_VERTEX_DOUBLE					FLAGBIT(2)		//	Vertex items returned as double (8 byte/64 bit) else - as float (4 byte/32 bit)
#define		FORMAT_SIZE_INDEX_INT64						FLAGBIT(3)		//	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode) - else as int32_t (4 byte/32 bit)

//control vertex data
#define		FORMAT_VERTEX_POINT							FLAGBIT(4)		//	Vertex contains 3D point info
#define		FORMAT_VERTEX_NORMAL						FLAGBIT(5)		//	Vertex contains normat info
#define		FORMAT_VERTEX_TEXTURE_UV					FLAGBIT(6)		//	Vertex contains first 2D texture info
#define		FORMAT_VERTEX_TEXTURE_TANGENT				FLAGBIT(28)		//	Vertex does contain tangent vector for first texture
#define		FORMAT_VERTEX_TEXTURE_BINORMAL				FLAGBIT(29)		//	Vertex does contain binormal vector for first texture
#define		FORMAT_VERTEX_TEXTURE2_UV					FLAGBIT(7)		//	Vertex contains second 2D texture info
#define		FORMAT_VERTEX_TEXTURE2_TANGENT				FLAGBIT(30)		//	Vertex does contain tangent vector for second texture (only 64 bit platform)
#define		FORMAT_VERTEX_TEXTURE2_BINORMAL				FLAGBIT(31)		//	Vertex does contain binormal vector for second texture (only 64 bit platform)
#define		FORMAT_VERTEX_COLOR_AMBIENT					FLAGBIT(24)		//	Vertex does contain Ambient color information
#define		FORMAT_VERTEX_COLOR_DIFFUSE					FLAGBIT(25)		//	Vertex does contain Diffuse color information
#define		FORMAT_VERTEX_COLOR_EMISSIVE				FLAGBIT(26)		//	Vertex does contain Emissive color information
#define		FORMAT_VERTEX_COLOR_SPECULAR				FLAGBIT(27)		//	Vertex does contain Specular color information

//control CalculateInstance behaviour
#define		FORMAT_EXPORT_TRIANGLES						FLAGBIT(8)		//	Object form triangles are exported (effective if instance contains faces and/or solids)(triangulated surface representation)
#define		FORMAT_EXPORT_LINES							FLAGBIT(9)		//	Object polygon lines are exported (effective if instance contains line representations)
#define		FORMAT_EXPORT_POINTS						FLAGBIT(10)		//	Object points are exported (effective if instance contains point representations)
#define		FORMAT_EXPORT_FACE_POLYGONS					FLAGBIT(12)		//	Object face polygon lines (dense wireframe) are exported (requires FORMAT_FLAG_CONTAINS_TRIANGLES)
#define		FORMAT_EXPORT_CONCEPTUAL_FACE_POLYGONS		FLAGBIT(13)		//	Object conceptual face polygon lines (wireframe) are exported 
#define		FORMAT_EXPORT_POLYGONS_AS_TUPLES			FLAGBIT(14)		//	Polygon lines (wireframe) exported as tuples (edges) - else as list (loop)
#define		FORMAT_EXPORT_ADVANCED_NORMALS				FLAGBIT(15)		//	Normals are exported to be in line with the original semantic form description (orthogonal to conceprual face, could be non orthogonal to the planar face or triangle) - else all normals of triangles are transformed orthogonal to the palnar face or triangle they belong to
#define		FORMAT_EXPORT_DIRECTX						FLAGBIT(16)		//	Where possible DirectX compatibility is given to exported data. Unsets FORMAT_FLAG_OPENGL, FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010
#define		FORMAT_EXPORT_OPENGL						FLAGBIT(17)		//	Where possible OpenGL compatibility is given to exported data. Unsets FORMAT_FLAG_DIRECTX. Sets FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010
#define		FORMAT_EXPORT_DOUBLE_SIDED					FLAGBIT(18)		//	Every face has exactly one opposite face (normally both index and vertex array are doubled in size)
#define		FORMAT_EXPORT_VERSION_0001					FLAGBIT(20)		//	Opposite Triangle Rotation (RHS as expected by OpenGL) - else  Standard Triangle Rotation (LHS as expected by DirectX)
#define		FORMAT_EXPORT_VERSION_0010					FLAGBIT(21)		//	X, Y, Z (nX, nY, nZ) formatted as , i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL) - else X, Y, Z (nX, nY, nZ) formatted as considering internal concepts
#define		FORMAT_EXPORT_VERSION_0100					FLAGBIT(22)
#define		FORMAT_EXPORT_VERSION_1000					FLAGBIT(23)

#define GetVertexElementsCount(setting) GetVertexDataOffset(0, setting)	// Get number of elements in verctor data array with the format settings

//
//  Color pack/unpack
//

//get color from its components in range 0..255
#define		COLOR_RGBW255(r,g,b,w)						(											\
															((int32_t)(unsigned char)(r)	<<24)	\
														|	((int32_t)(unsigned char)(g)	<<16)	\
														|	((int32_t)(unsigned char)(b)	<<8	)	\
														|	((int32_t)(unsigned char)(w)		)	\
														)
//get color from its components in range 0..1 
#define		COLOR_RGBW(r,g,b,w)							COLOR_RGBW255(r*255,g*255,b*255,w*255)

#define		COLOR_GET_R255(clr)							((unsigned char)(((int32_t)(clr)>>24)&0xFF))	//	get color red component in range 0..255
#define		COLOR_GET_G255(clr)							((unsigned char)(((int32_t)(clr)>>16)&0xFF))	//	get color green component in range 0..255
#define		COLOR_GET_B255(clr)							((unsigned char)(((int32_t)(clr)>>8 )&0xFF))	//	get color blue component in range 0..255
#define		COLOR_GET_W255(clr)							((unsigned char)(((int32_t)(clr)	)&0xFF))	//	get color transparency in range 0..255

#define		COLOR_GET_R(clr)							(COLOR_GET_R255(clr)/255.0)						//	get color red component in range 0..1
#define		COLOR_GET_G(clr)							(COLOR_GET_G255(clr)/255.0)						//	get color green component in range 0..1
#define		COLOR_GET_B(clr)							(COLOR_GET_B255(clr)/255.0)						//	get color blue component in range 0..1
#define		COLOR_GET_W(clr)							(COLOR_GET_W255(clr)/255.0)						//	get color trancparency in range 0..1


#define		COLOR_ARR_RGBW255(r)						COLOR_RGBW255(r[0],r[1],r[2],r[3])				//	get color from array of 4 components in range 0..255
#define		COLOR_ARR_RGBW(r)							COLOR_RGBW(r[0],r[1],r[2],r[3])					//	get color from array of 4 components in range 0..1

//get color comonents in range 0..255 to arry of 4 elements
#define		COLOR_GET_COMPONENTS255(r,clr)				{r[0]=COLOR_GET_R255(clr); r[1]=COLOR_GET_G255(clr); r[2]=COLOR_GET_B255(clr); r[3]=COLOR_GET_W255(clr);}

//get color comonents in range 0..1 to arry of 4 elements
#define		COLOR_GET_COMPONENTS(r,clr)					{r[0]=COLOR_GET_R(clr); r[1]=COLOR_GET_G(clr); r[2]=COLOR_GET_B(clr); r[3]=COLOR_GET_W(clr);}

//
//  Meta information API Calls
//

//
//		GetRevision                                 (http://rdf.bg/gkdoc/CP64/GetRevision.html)
//				char				** timeStamp						IN / OUT
//
//				int64_t				returns								OUT
//
//	Returns the revision number.
//	The timeStamp is generated by the SVN system used during development.
//
int64_t		DECL STDC	GetRevision(
									char				** timeStamp
								);

//
//		GetRevisionW                                (http://rdf.bg/gkdoc/CP64/GetRevisionW.html)
//				wchar_t				** timeStamp						IN / OUT
//
//				int64_t				returns								OUT
//
//	Returns the revision number.
//	The timeStamp is generated by the SVN system used during development.
//
int64_t		DECL STDC	GetRevisionW(
									wchar_t				** timeStamp
								);

//
//		GetProtection                               (http://rdf.bg/gkdoc/CP64/GetProtection.html)
//				int64_t				returns								OUT
//
//	This call is required to be called to enable the DLL to work if protection is active.
//
//	Returns the number of days (incl. this one) that this version is still active or 0 if no protection is embedded.
//	In case no days are left and protection is active this call will return -1.
//
int64_t		DECL STDC	GetProtection(
								);

//
//		GetEnvironment                              (http://rdf.bg/gkdoc/CP64/GetEnvironment.html)
//				char				** environmentVariables				IN / OUT
//				char				** developmentVariables				IN / OUT
//
//				int64_t				returns								OUT
//
//	Returns the revision number similar to the call GetRevision.
//	The environment variables will show known environment variables
//	and if they are set, for example environment variables ABC known
//	and unset and DEF as well as GHI known and set:
//		environmentVariables = "ABC:F;DEF:T;GHI:T"
//	Development variables are depending on the build environment
//	As an example in windows systems where Visual Studio is used:
//		developmentVariables = "...."
//
int64_t		DECL STDC	GetEnvironment(
									char				** environmentVariables,
									char				** developmentVariables
								);

//
//		GetEnvironmentW                             (http://rdf.bg/gkdoc/CP64/GetEnvironmentW.html)
//				wchar_t				** environmentVariables				IN / OUT
//				wchar_t				** developmentVariables				IN / OUT
//
//				int64_t				returns								OUT
//
//	Returns the revision number similar to the call GetRevision[W].
//	The environment variables will show known environment variables
//	and if they are set, for example environment variables ABC known
//	and unset and DEF as well as GHI known and set:
//		environmentVariables = "ABC:F;DEF:T;GHI:T"
//	Development variables are depending on the build environment
//	As an example in windows systems where Visual Studio is used:
//		developmentVariables = "...."
//
int64_t		DECL STDC	GetEnvironmentW(
									wchar_t				** environmentVariables,
									wchar_t				** developmentVariables
								);

//
//		SetAssertionFile                            (http://rdf.bg/gkdoc/CP64/SetAssertionFile.html)
//				const char			* fileName							IN
//
//				void				returns
//
//	This function sets the file location where internal assertions should be written to.
//	If the file name is not set (default) many internal control procedures are not executed
//	and the code will be faster.
//
void		DECL STDC	SetAssertionFile(
									const char			* fileName
								);

//
//		SetAssertionFileW                           (http://rdf.bg/gkdoc/CP64/SetAssertionFileW.html)
//				const wchar_t		* fileName							IN
//
//				void				returns
//
//	This function sets the file location where internal assertions should be written to.
//	If the file name is not set (default) many internal control procedures are not executed
//	and the code will be faster.
//
void		DECL STDC	SetAssertionFileW(
									const wchar_t		* fileName
								);

//
//		GetAssertionFile                            (http://rdf.bg/gkdoc/CP64/GetAssertionFile.html)
//				char				** fileName							IN / OUT
//
//				void				returns
//
//	This function gets the file location as stored/set internally where internal assertions should be written to.
//	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
//
void		DECL STDC	GetAssertionFile(
									char				** fileName
								);

//
//		GetAssertionFileW                           (http://rdf.bg/gkdoc/CP64/GetAssertionFileW.html)
//				wchar_t				** fileName							IN / OUT
//
//				void				returns
//
//	This function gets the file location as stored/set internally where internal assertions should be written to.
//	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
//
void		DECL STDC	GetAssertionFileW(
									wchar_t				** fileName
								);

//
//		SetCharacterSerialization                   (http://rdf.bg/gkdoc/CP64/SetCharacterSerialization.html)
//				int64_t				model								IN
//				int64_t				encoding							IN
//				int64_t				wcharBitSizeOverride				IN
//				bool				ascii								IN
//
//				int64_t				returns								OUT
//
//	This call defines how characters for names, strings will be serializaed and how
//	they are expected to be serialized. An exception are the Open / Import / Save calls,
//	these calls have a fixed way of serialization of path / file names.
//
//	If the encoding value is non-zero the following values are possible (if zero encoding is kept as defined)
//		 32 [default]	encoding ignored
//		 64				encoding Windows 1250
//		 65				encoding Windows 1251
//		 66				encoding Windows 1252
//		 67				encoding Windows 1253
//		 68				encoding Windows 1254
//		 69				encoding Windows 1255
//		 70				encoding Windows 1256
//		 71				encoding Windows 1257
//		 72				encoding Windows 1258
//		128				encoding ISO8859 1
//		129				encoding ISO8859 2
//		130				encoding ISO8859 3
//		131				encoding ISO8859 4
//		132				encoding ISO8859 5
//		133				encoding ISO8859 6
//		134				encoding ISO8859 7
//		135				encoding ISO8859 8
//		136				encoding ISO8859 9
//		137				encoding ISO8859 10
//		138				encoding ISO8859 11
//						encoding ISO8859 12 => does not exist
//		140				encoding ISO8859 13
//		141				encoding ISO8859 14
//		142				encoding ISO8859 15
//		143				encoding ISO8859 16
//		160				encoding MACINTOSH CENTRAL EUROPEAN
//		192				encoding SHIFT JIS X 213
//
//	The wcharBitSizeOverride value overrides the OS based size of wchar_t, the following values can be applied:
//		0			wcharBitSizeOverride is ignored, override is not changed
//		16			wchar_t interpreted as being 2 bytes wide (size of wchar_t in bits)
//		32			wchar_t interpreted as being 4 bytes wide (size of wchar_t in bits)
//		Any other value will reset the override and wchar_t will follow the OS based size of wchar_t
//	Note: this setting is independent from the model, this call can also be called without a model defined.
//
//	The ascii value defines
//		true [default]	8 bit serializatiom (size of char returned in bits)
//		false			16/32 bit serialization (depending on the operating system, i.e. sizeof of wchar_t returned in number of bits)
//	Note: this setting is model-dependent and requires a model present to have any effect.
//
//	The return value is the size of a single character in bits, i.e. 1 byte is 8 bits, the value for a wchar_t can be 16 or 32 depending on settings and operating system
//
int64_t		DECL STDC	SetCharacterSerialization(
									int64_t				model,
									int64_t				encoding,
									int64_t				wcharBitSizeOverride,
									bool				ascii
								);

//
//		GetCharacterSerialization                   (http://rdf.bg/gkdoc/CP64/GetCharacterSerialization.html)
//				int64_t				model								IN
//				int64_t				* encoding							IN / OUT
//				bool				* ascii								IN / OUT
//
//				int64_t				returns								OUT
//
//	This call retrieves the values as set by 
//
//	The returns the size of a single character in bits, i.e. 1 byte is 8 bits, this can be 8, 16 or 32 depending on settings and operating system
//
int64_t		DECL STDC	GetCharacterSerialization(
									int64_t				model,
									int64_t				* encoding,
									bool				* ascii
								);

//
//		SetModellingStyle                           (http://rdf.bg/gkdoc/CP64/SetModellingStyle.html)
//				int64_t				model								IN
//				int64_t				setting								IN
//				int64_t				mask								IN
//
//				void				returns
//
//	This call sets the modelling style.
//
void		DECL STDC	SetModellingStyle(
									int64_t				model,
									int64_t				setting,
									int64_t				mask
								);

//
//		GetModellingStyle                           (http://rdf.bg/gkdoc/CP64/GetModellingStyle.html)
//				int64_t				model								IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	This call gets the modelling style.
//
int64_t		DECL STDC	GetModellingStyle(
									int64_t				model,
									int64_t				mask
								);

//
//		AbortModel                                  (http://rdf.bg/gkdoc/CP64/AbortModel.html)
//				int64_t				model								IN
//				int64_t				setting								IN
//
//				int64_t				returns								OUT
//
//	This function abort running processes for a model. It can be used when a task takes more time than
//	expected / available, or in case the requested results are not relevant anymore.
//
int64_t		DECL STDC	AbortModel(
									int64_t				model,
									int64_t				setting
								);

//
//		GetSessionMetaInfo                          (http://rdf.bg/gkdoc/CP64/GetSessionMetaInfo.html)
//				int64_t				* allocatedBlocks					IN / OUT
//				int64_t				* allocatedBytes					IN / OUT
//				int64_t				* nonUsedBlocks						IN / OUT
//				int64_t				* nonUsedBytes						IN / OUT
//
//				int64_t				returns								OUT
//
//	This function is meant for debugging purposes and return statistics during processing.
//	The return value represents the number of active models within the session (or zero if the model was not recognized).
//
int64_t		DECL STDC	GetSessionMetaInfo(
									int64_t				* allocatedBlocks,
									int64_t				* allocatedBytes,
									int64_t				* nonUsedBlocks,
									int64_t				* nonUsedBytes
								);

//
//		GetModelMetaInfo                            (http://rdf.bg/gkdoc/CP64/GetModelMetaInfo.html)
//				int64_t				model								IN
//				int64_t				* activeClasses						IN / OUT
//				int64_t				* removedClasses					IN / OUT
//				int64_t				* activeProperties					IN / OUT
//				int64_t				* removedProperties					IN / OUT
//				int64_t				* activeInstances					IN / OUT
//				int64_t				* removedInstances					IN / OUT
//				int64_t				* inactiveInstances					IN / OUT
//
//				int64_t				returns								OUT
//
//	This function is meant for debugging purposes and return statistics during processing.
//	The return value represents the number of active models within the session (or zero if the model was not recognized).
//
int64_t		DECL STDC	GetModelMetaInfo(
									int64_t				model,
									int64_t				* activeClasses,
									int64_t				* removedClasses,
									int64_t				* activeProperties,
									int64_t				* removedProperties,
									int64_t				* activeInstances,
									int64_t				* removedInstances,
									int64_t				* inactiveInstances
								);

//
//		GetInstanceMetaInfo                         (http://rdf.bg/gkdoc/CP64/GetInstanceMetaInfo.html)
//				int64_t				owlInstance							IN
//				int64_t				* allocatedBlocks					IN / OUT
//				int64_t				* allocatedBytes					IN / OUT
//
//				int64_t				returns								OUT
//
//	This function is meant for debugging purposes and return statistics during processing.
//	The return value represents the number of active instances within the model (or zero if the instance was not recognized).
//
int64_t		DECL STDC	GetInstanceMetaInfo(
									int64_t				owlInstance,
									int64_t				* allocatedBlocks,
									int64_t				* allocatedBytes
								);

//
//		GetSmoothness                               (http://rdf.bg/gkdoc/CP64/GetSmoothness.html)
//				int64_t				owlInstance							IN
//				int64_t				* degree							IN / OUT
//
//				int64_t				returns								OUT
//
//	This function returns the smoothness of a line or surface.
//	In case the smoothness can be defined the degree will get assigned either
//		0 - continuous curve / surface (i.e. degree 9)
//		1 - the direction of the curve / surface is gradually changing (i.e. degree 1)
//		2 - the change of direction of the curve / surface is gradually changing (i.e. degree 2)
//	In return value of this function retuns the dimension of the found smoothness:
//		0 - smoothness could not be defined
//		1 - found the smoothness of a curve
//		2 - found the smoothness of a surface
//
int64_t		DECL STDC	GetSmoothness(
									int64_t				owlInstance,
									int64_t				* degree
								);

//
//		AddState                                    (http://rdf.bg/gkdoc/CP64/AddState.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//
//				void				returns
//
//	This call will integrate the current state information into the model.
//
//	Model should be non-zero.
//
//	If owlInstance is given the state is only applied on the owlInstance and all its children.
//	If owlInstance is zero the state is applied on all owlInstances within a model.
//
void		DECL STDC	AddState(
									int64_t				model,
									int64_t				owlInstance
								);

//
//		GetModel                                    (http://rdf.bg/gkdoc/CP64/GetModel.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//
int64_t		DECL STDC	GetModel(
									int64_t				owlInstance
								);

//
//		OrderedHandles                              (http://rdf.bg/gkdoc/CP64/OrderedHandles.html)
//				int64_t				model								IN
//				int64_t				* classCnt							IN / OUT
//				int64_t				* propertyCnt						IN / OUT
//				int64_t				* instanceCnt						IN / OUT
//				int64_t				setting								IN
//				int64_t				mask								IN
//
//				void				returns
//
//	This call can be used in two ways. The optional arguments classCnt,
//	propertyCnt and instanceCnt can be used to get the total amount of active classes,
//	properies and instances available within the model.
//
//	The setting and mask can be used to order the handles given for classes,
//	properties and instances.
//		1 - if set this will number all classes with possible values [1 .. classCnt]
//		2 - if set this will number all classes with possible values [1 .. propertyCnt]
//		4 - if set this will number all classes with possible values [1 .. instanceCnt]
//
//	Note: when enabling ordered handles be aware that classes, properties and instances
//		  can share the same handles, using the correct argument cannot be checked anymore
//		  by the library itself. This could result in crashes in case of incorrect assignments
//		  by the hosting application.
//	Note: internally there is no performance gain / loss. This is purely meant for situations
//		  where the hosting application can benefit performance wise from having an ordered list.
//	Note: use in combination with other libraries is not adviced, i.e. when combined with the
//		  IFC generation from the IFC Engine component for example
//
void		DECL STDC	OrderedHandles(
									int64_t				model,
									int64_t				* classCnt,
									int64_t				* propertyCnt,
									int64_t				* instanceCnt,
									int64_t				setting,
									int64_t				mask
								);

//
//		PeelArray                                   (http://rdf.bg/gkdoc/CP64/PeelArray.html)
//				const void			** inValue							IN
//				void				** outValue							IN / OUT
//				int64_t				elementSize							IN
//
//				void				returns
//
//	This function introduces functionality that is missing or complicated in some programming languages.
//	The attribute inValue is a reference to an array of references. The attribute outValue is a reference to the same array,
//	however a number of elements earlier or further, i.e. number of elements being attribute elementSize. Be aware that as
//	we are talking about references the offset is depending on 32 bit / 64 bit compilation.
//
void		DECL STDC	PeelArray(
									const void			** inValue,
									void				** outValue,
									int64_t				elementSize
								);

//
//		SetInternalCheck                            (http://rdf.bg/gkdoc/CP64/SetInternalCheck.html)
//				int64_t				model								IN
//				int64_t				setting								IN
//				int64_t				mask								IN
//
//				void				returns
//
//	This function allows to enable or disable several active consistency checks. Enabling the checks can 
//	introduce performance effects; it is helpfull for and meant for debugging on client side.
//	If model is zero the consistency checks are set for all open and to be created models.
//
void		DECL STDC	SetInternalCheck(
									int64_t				model,
									int64_t				setting,
									int64_t				mask
								);

//
//		GetInternalCheck                            (http://rdf.bg/gkdoc/CP64/GetInternalCheck.html)
//				int64_t				model								IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	This function returns all current enabled active consistency checks given the mask the function is 
//	called for.
//	When leaving mask and settinbg zero it will return all bits that can be set.
//
int64_t		DECL STDC	GetInternalCheck(
									int64_t				model,
									int64_t				mask
								);

//
//		GetInternalCheckIssueCnt                    (http://rdf.bg/gkdoc/CP64/GetInternalCheckIssueCnt.html)
//				int64_t				model								IN
//
//				int64_t				returns								OUT
//
//	This function returns all issues found and not retrieved by the hosting application through 
//	GetInternalCheckIssue() / GetInternalCheckIssueW().
//
int64_t		DECL STDC	GetInternalCheckIssueCnt(
									int64_t				model
								);

//
//		GetInternalCheckIssue                       (http://rdf.bg/gkdoc/CP64/GetInternalCheckIssue.html)
//				int64_t				model								IN
//				char				** name								IN / OUT
//				char				** description						IN / OUT
//				int64_t				* relatedOwlInstance				IN / OUT
//
//				void				returns
//
//	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
//	The name and description represent the issue as ASCII string, if relevant the relating owlInstance
//	will be returned through relatedOwlInstance.
//	Namer, Description and relatedOwlInstance are optional.
//
void		DECL STDC	GetInternalCheckIssue(
									int64_t				model,
									char				** name,
									char				** description,
									int64_t				* relatedOwlInstance
								);

//
//		GetInternalCheckIssueW                      (http://rdf.bg/gkdoc/CP64/GetInternalCheckIssueW.html)
//				int64_t				model								IN
//				wchar_t				** name								IN / OUT
//				wchar_t				** description						IN / OUT
//				int64_t				* relatedOwlInstance				IN / OUT
//
//				void				returns
//
//	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
//	The name and description represent the issue as Unicode string, if relevant the relating owlInstance
//	will be returned through relatedOwlInstance.
//	Namer, Description and relatedOwlInstance are optional.
//
void		DECL STDC	GetInternalCheckIssueW(
									int64_t				model,
									wchar_t				** name,
									wchar_t				** description,
									int64_t				* relatedOwlInstance
								);

//
//		CloseSession                                (http://rdf.bg/gkdoc/CP64/CloseSession.html)
//				int64_t				returns								OUT
//
//	This function closes the session, after this call the geometry kernel cannot be used anymore.
//
int64_t		DECL STDC	CloseSession(
								);

//
//		CleanMemory                                 (http://rdf.bg/gkdoc/CP64/CleanMemory.html)
//				void				returns
//
//		This function ..
//
void		DECL STDC	CleanMemory(
								);

//
//		ClearCache                                  (http://rdf.bg/gkdoc/CP64/ClearCache.html)
//				int64_t				model								IN
//
//				void				returns
//
//		This function ..
//
void		DECL STDC	ClearCache(
									int64_t				model
								);

//
//  File IO / Stream / Copy API Calls
//

//
//		CreateModel                                 (http://rdf.bg/gkdoc/CP64/CreateModel.html)
//				int64_t				returns								OUT
//
//	This function creates and empty model.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int64_t		DECL STDC	CreateModel(
								);

//
//		OpenModel                                   (http://rdf.bg/gkdoc/CP64/OpenModel.html)
//				const char			* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function opens the model on location file name.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int64_t		DECL STDC	OpenModel(
									const char			* fileName
								);

//
//		OpenModelW                                  (http://rdf.bg/gkdoc/CP64/OpenModelW.html)
//				const wchar_t		* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function opens the model on location file name.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int64_t		DECL STDC	OpenModelW(
									const wchar_t		* fileName
								);

//
//		OpenModelS                                  (http://rdf.bg/gkdoc/CP64/OpenModelS.html)
//				const void			* callback							IN
//
//				int64_t				returns								OUT
//
//	This function opens the model via a stream.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int64_t		DECL STDC	OpenModelS(
									const void			* callback
								);

//
//		OpenModelA                                  (http://rdf.bg/gkdoc/CP64/OpenModelA.html)
//				const unsigned char	* content							IN
//				int64_t				size								IN
//
//				int64_t				returns								OUT
//
//	This function opens the model via an array.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int64_t		DECL STDC	OpenModelA(
									const unsigned char	* content,
									int64_t				size
								);

//
//		ImportModel                                 (http://rdf.bg/gkdoc/CP64/ImportModel.html)
//				int64_t				model								IN
//				const char			* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function imports a design tree on location file name.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
int64_t		DECL STDC	ImportModel(
									int64_t				model,
									const char			* fileName
								);

//
//		ImportModelW                                (http://rdf.bg/gkdoc/CP64/ImportModelW.html)
//				int64_t				model								IN
//				const wchar_t		* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function imports a design tree on location file name.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
int64_t		DECL STDC	ImportModelW(
									int64_t				model,
									const wchar_t		* fileName
								);

//
//		ImportModelS                                (http://rdf.bg/gkdoc/CP64/ImportModelS.html)
//				int64_t				model								IN
//				const void			* callback							IN
//
//				int64_t				returns								OUT
//
//	This function imports a design tree via a stream.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
int64_t		DECL STDC	ImportModelS(
									int64_t				model,
									const void			* callback
								);

//
//		ImportModelA                                (http://rdf.bg/gkdoc/CP64/ImportModelA.html)
//				int64_t				model								IN
//				const unsigned char	* content							IN
//				int64_t				size								IN
//
//				int64_t				returns								OUT
//
//	This function imports a design tree via an array.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
int64_t		DECL STDC	ImportModelA(
									int64_t				model,
									const unsigned char	* content,
									int64_t				size
								);

//
//		SaveInstanceTree                            (http://rdf.bg/gkdoc/CP64/SaveInstanceTree.html)
//				int64_t				owlInstance							IN
//				const char			* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t		DECL STDC	SaveInstanceTree(
									int64_t				owlInstance,
									const char			* fileName
								);

//
//		SaveInstanceTreeW                           (http://rdf.bg/gkdoc/CP64/SaveInstanceTreeW.html)
//				int64_t				owlInstance							IN
//				const wchar_t		* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t		DECL STDC	SaveInstanceTreeW(
									int64_t				owlInstance,
									const wchar_t		* fileName
								);

//
//		SaveInstanceTreeS                           (http://rdf.bg/gkdoc/CP64/SaveInstanceTreeS.html)
//				int64_t				owlInstance							IN
//				const void			* callback							IN
//				int64_t				size								IN
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies in a stream.
//
int64_t		DECL STDC	SaveInstanceTreeS(
									int64_t				owlInstance,
									const void			* callback,
									int64_t				size
								);

//
//		SaveInstanceTreeA                           (http://rdf.bg/gkdoc/CP64/SaveInstanceTreeA.html)
//				int64_t				owlInstance							IN
//				unsigned char		* content							IN / OUT
//				int64_t				* size								IN / OUT
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies in an array.
//
int64_t		DECL STDC	SaveInstanceTreeA(
									int64_t				owlInstance,
									unsigned char		* content,
									int64_t				* size
								);

//
//		SaveInstanceNetwork                         (http://rdf.bg/gkdoc/CP64/SaveInstanceNetwork.html)
//				int64_t				owlInstance							IN
//				bool				includeInverseRelations				IN
//				const char			* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t		DECL STDC	SaveInstanceNetwork(
									int64_t				owlInstance,
									bool				includeInverseRelations,
									const char			* fileName
								);

//
//		SaveInstanceNetworkW                        (http://rdf.bg/gkdoc/CP64/SaveInstanceNetworkW.html)
//				int64_t				owlInstance							IN
//				bool				includeInverseRelations				IN
//				const wchar_t		* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t		DECL STDC	SaveInstanceNetworkW(
									int64_t				owlInstance,
									bool				includeInverseRelations,
									const wchar_t		* fileName
								);

//
//		SaveInstanceNetworkS                        (http://rdf.bg/gkdoc/CP64/SaveInstanceNetworkS.html)
//				int64_t				owlInstance							IN
//				bool				includeInverseRelations				IN
//				const void			* callback							IN
//				int64_t				size								IN
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies in a stream.
//
int64_t		DECL STDC	SaveInstanceNetworkS(
									int64_t				owlInstance,
									bool				includeInverseRelations,
									const void			* callback,
									int64_t				size
								);

//
//		SaveInstanceNetworkA                        (http://rdf.bg/gkdoc/CP64/SaveInstanceNetworkA.html)
//				int64_t				owlInstance							IN
//				bool				includeInverseRelations				IN
//				unsigned char		* content							IN / OUT
//				int64_t				* size								IN / OUT
//
//				int64_t				returns								OUT
//
//	This function saves the selected instance and its dependancies in an array.
//
int64_t		DECL STDC	SaveInstanceNetworkA(
									int64_t				owlInstance,
									bool				includeInverseRelations,
									unsigned char		* content,
									int64_t				* size
								);

//
//		SaveModel                                   (http://rdf.bg/gkdoc/CP64/SaveModel.html)
//				int64_t				model								IN
//				const char			* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function saves the current model on location file name.
//
int64_t		DECL STDC	SaveModel(
									int64_t				model,
									const char			* fileName
								);

//
//		SaveModelW                                  (http://rdf.bg/gkdoc/CP64/SaveModelW.html)
//				int64_t				model								IN
//				const wchar_t		* fileName							IN
//
//				int64_t				returns								OUT
//
//	This function saves the current model on location file name.
//
int64_t		DECL STDC	SaveModelW(
									int64_t				model,
									const wchar_t		* fileName
								);

//
//		SaveModelS                                  (http://rdf.bg/gkdoc/CP64/SaveModelS.html)
//				int64_t				model								IN
//				const void			* callback							IN
//				int64_t				size								IN
//
//				int64_t				returns								OUT
//
//	This function saves the current model in a stream.
//
int64_t		DECL STDC	SaveModelS(
									int64_t				model,
									const void			* callback,
									int64_t				size
								);

//
//		SaveModelA                                  (http://rdf.bg/gkdoc/CP64/SaveModelA.html)
//				int64_t				model								IN
//				unsigned char		* content							IN / OUT
//				int64_t				* size								IN / OUT
//
//				int64_t				returns								OUT
//
//	This function saves the current model in an array.
//
int64_t		DECL STDC	SaveModelA(
									int64_t				model,
									unsigned char		* content,
									int64_t				* size
								);

//
//		SetOverrideFileIO                           (http://rdf.bg/gkdoc/CP64/SetOverrideFileIO.html)
//				int64_t				model								IN
//				int64_t				setting								IN
//				int64_t				mask								IN
//
//				void				returns
//
//	This function overrides the type of file saved / exported independent of the extension given.
//	By default the extension of the file name will define the type saved / exported:
//		.rdf => generated RDF serialized content
//		.ttl => generated TTL serialized content
//		.bin => generated BIN/X serialized content
//
//	Available formats
//		RDF
//		TTL
//		BIN/L - readible but large BIN format
//		BIN/S - Optimized Binary, only running within given revision 
//		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
//
//	Force file type (overrides extension), works only on save (open selects correct type automatically)
//		bit0	bit1	bit2
//		  0		  0		  0		[default] unset forced file type
//		  0		  0		  1		RESERVED
//		  0		  1		  0		TTL
//		  0		  1		  1		RDF
//		  1		  0		  0		BIN/X
//		  1		  0		  1		BIN/S
//		  1		  1		  0		RESERVED
//		  1		  1		  1		BIN/L
//
//	Force exporting as Base64
//		bit4
//		  0		do not use Base64
//		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
//
void		DECL STDC	SetOverrideFileIO(
									int64_t				model,
									int64_t				setting,
									int64_t				mask
								);

//
//		GetOverrideFileIO                           (http://rdf.bg/gkdoc/CP64/GetOverrideFileIO.html)
//				int64_t				model								IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	This function get the current overrides for type of file saved / exported independent of the extension given.
//	By default the extension of the file name will define the type saved / exported:
//		.rdf => generated RDF serialized content
//		.ttl => generated TTL serialized content
//		.bin => generated BIN/X serialized content
//
//	Available formats
//		RDF
//		TTL
//		BIN/L - readible but large BIN format
//		BIN/S - Optimized Binary, only running within given revision 
//		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
//
//	Force file type (overrides extension), works only on save (open selects correct type automatically)
//		bit0	bit1	bit2
//		  0		  0		  0		[default] unset forced file type
//		  0		  0		  1		RESERVED
//		  0		  1		  0		TTL
//		  0		  1		  1		RDF
//		  1		  0		  0		BIN/X
//		  1		  0		  1		BIN/S
//		  1		  1		  0		RESERVED
//		  1		  1		  1		BIN/L
//
//	Force exporting as Base64
//		bit4
//		  0		do not use Base64
//		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
//
int64_t		DECL STDC	GetOverrideFileIO(
									int64_t				model,
									int64_t				mask
								);

//
//		CopyInstanceTree                            (http://rdf.bg/gkdoc/CP64/CopyInstanceTree.html)
//				int64_t				owlInstance							IN
//				int64_t				targetModel							IN
//
//				int64_t				returns								OUT
//
//	This function copies the instance tree towards a new model.
//	In case model is empty a new model will be created (the handle to this new model can be retrieved through
//	the call GetModel() based on the return value of this call).
//	The model can be amy opem model, it can be zero (a new model will be created on-the-fly) and it can be
//	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
//	original instance tree.
//
//	The return value is the handle to the copied owlInstance in the model of choice.
//
int64_t		DECL STDC	CopyInstanceTree(
									int64_t				owlInstance,
									int64_t				targetModel
								);

//
//		CopyInstanceNetwork                         (http://rdf.bg/gkdoc/CP64/CopyInstanceNetwork.html)
//				int64_t				owlInstance							IN
//				bool				includeInverseRelations				IN
//				int64_t				targetModel							IN
//
//				int64_t				returns								OUT
//
//	This function copies the instance network towards a new model.
//	An instance network is different from an instance tree in that it can contain 'loops', the performance
//	from this call will be slower in case the tree / network is sparse.
//	In case model is empty a new model will be created (the handle to this new model can be retrieved through
//	the call GetModel() based on the return value of this call).
//	The model can be amy opem model, it can be zero (a new model will be created on-the-fly) and it can be
//	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
//	original instance tree.
//
//	In case it is known we are talking about a tree (i.e. no internal loops) and inverse relations can be ignored
//	the call CopyInstanceTree is a better choice concerning performance.
//
//	The return value is the handle to the copied owlInstance in the model of choice.
//
int64_t		DECL STDC	CopyInstanceNetwork(
									int64_t				owlInstance,
									bool				includeInverseRelations,
									int64_t				targetModel
								);

//
//		CopyModel                                   (http://rdf.bg/gkdoc/CP64/CopyModel.html)
//				int64_t				sourceModel							IN
//				int64_t				targetModel							IN
//				int64_t				* values							IN / OUT
//				int64_t				card								IN
//
//				int64_t				returns								OUT
//
//	This function copies the complete structure of a model towards another or new model.
//	In case the targetModel is empty a new model will be created.
//	The owlInstance array (values) and it's cardinality (card) can be empty, in case they are
//	non-empty the values are expected to contain owlInstance handles referencing in the source model
//	after a successful copy the values will be adjusted into values referencing the copied owl instances
//	in the new model. the list of values does not have to be complete or even unique and can even be empty.
//
//	sourceModel is not allowed to be empty, targetModel however can be empty or even equal to the source model.
//
//	The return value is the targetModel or in case this value was empty the newly created model.
//
int64_t		DECL STDC	CopyModel(
									int64_t				sourceModel,
									int64_t				targetModel,
									int64_t				* values,
									int64_t				card
								);

//
//		CloseModel                                  (http://rdf.bg/gkdoc/CP64/CloseModel.html)
//				int64_t				model								IN
//
//				int64_t				returns								OUT
//
//	This function closes the model. After this call none of the instances and classes within the model
//	can be used anymore, also garbage collection is not allowed anymore, in default compilation the
//	model itself will be known in the kernel, however known to be disabled. Calls containing the model
//	reference will be protected from crashing when called.
//
int64_t		DECL STDC	CloseModel(
									int64_t				model
								);

//
//  Design Tree Classes API Calls
//

//
//		CreateClass                                 (http://rdf.bg/gkdoc/CP64/CreateClass.html)
//				int64_t				model								IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created class.
//	If the model input is zero or not a model handle 0 will be returned,
//
int64_t		DECL STDC	CreateClass(
									int64_t				model,
									const char			* name
								);

//
//		CreateClassW                                (http://rdf.bg/gkdoc/CP64/CreateClassW.html)
//				int64_t				model								IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created class.
//	If the model input is zero or not a model handle 0 will be returned,
//
int64_t		DECL STDC	CreateClassW(
									int64_t				model,
									const wchar_t		* name
								);

//
//		GetClassByName                              (http://rdf.bg/gkdoc/CP64/GetClassByName.html)
//				int64_t				model								IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to the class as stored inside.
//	When the class does not exist yet and the name is unique
//	the class will be created on the fly and the handle will be returned.
//	When the name is not unique and given to an instance, objectTypeProperty
//	or dataTypeProperty 0 will be returned.
//
int64_t		DECL STDC	GetClassByName(
									int64_t				model,
									const char			* name
								);

//
//		GetClassByNameW                             (http://rdf.bg/gkdoc/CP64/GetClassByNameW.html)
//				int64_t				model								IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to the class as stored inside.
//	When the class does not exist yet and the name is unique
//	the class will be created on the fly and the handle will be returned.
//	When the name is not unique and given to an instance, objectTypeProperty
//	or dataTypeProperty 0 will be returned.
//
int64_t		DECL STDC	GetClassByNameW(
									int64_t				model,
									const wchar_t		* name
								);

//
//		GetClassesByIterator                        (http://rdf.bg/gkdoc/CP64/GetClassesByIterator.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an class.
//	If input class is zero, the handle will point to the first relevant class.
//	If all classes are past (or no relevant classes are found), the function will return 0.
//
int64_t		DECL STDC	GetClassesByIterator(
									int64_t				model,
									int64_t				owlClass
								);

//
//		SetClassParent                              (http://rdf.bg/gkdoc/CP64/SetClassParent.html)
//				int64_t				owlClass							IN
//				int64_t				parentOwlClass						IN
//				int64_t				setting								IN
//
//				void				returns
//
//	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
//	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
//	object properties (relations).
//
void		DECL STDC	SetClassParent(
									int64_t				owlClass,
									int64_t				parentOwlClass,
									int64_t				setting
								);

//
//		SetClassParentEx                            (http://rdf.bg/gkdoc/CP64/SetClassParentEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				int64_t				parentOwlClass						IN
//				int64_t				setting								IN
//
//				void				returns
//
//	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
//	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
//	object properties (relations).
//
//	This call has the same behavior as SetClassParent, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
void		DECL STDC	SetClassParentEx(
									int64_t				model,
									int64_t				owlClass,
									int64_t				parentOwlClass,
									int64_t				setting
								);

//
//		GetClassParentsByIterator                   (http://rdf.bg/gkdoc/CP64/GetClassParentsByIterator.html)
//				int64_t				owlClass							IN
//				int64_t				parentOwlClass						IN
//
//				int64_t				returns								OUT
//
//	Returns the next parent of the class.
//	If input parent is zero, the handle will point to the first relevant parent.
//	If all parent are past (or no relevant parent are found), the function will return 0.
//
int64_t		DECL STDC	GetClassParentsByIterator(
									int64_t				owlClass,
									int64_t				parentOwlClass
								);

//
//		SetNameOfClass                              (http://rdf.bg/gkdoc/CP64/SetNameOfClass.html)
//				int64_t				owlClass							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the class, if no error it returns 0.
//	In case class does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfClass(
									int64_t				owlClass,
									const char			* name
								);

//
//		SetNameOfClassW                             (http://rdf.bg/gkdoc/CP64/SetNameOfClassW.html)
//				int64_t				owlClass							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the class, if no error it returns 0.
//	In case class does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfClassW(
									int64_t				owlClass,
									const wchar_t		* name
								);

//
//		SetNameOfClassEx                            (http://rdf.bg/gkdoc/CP64/SetNameOfClassEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the class, if no error it returns 0.
//	In case class does not exist it returns 1, when name cannot be updated 2.
//
//	This call has the same behavior as SetNameOfClass, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
int64_t		DECL STDC	SetNameOfClassEx(
									int64_t				model,
									int64_t				owlClass,
									const char			* name
								);

//
//		SetNameOfClassWEx                           (http://rdf.bg/gkdoc/CP64/SetNameOfClassWEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the class, if no error it returns 0.
//	In case class does not exist it returns 1, when name cannot be updated 2.
//
//	This call has the same behavior as SetNameOfClassW, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
int64_t		DECL STDC	SetNameOfClassWEx(
									int64_t				model,
									int64_t				owlClass,
									const wchar_t		* name
								);

//
//		GetNameOfClass                              (http://rdf.bg/gkdoc/CP64/GetNameOfClass.html)
//				int64_t				owlClass							IN
//				char				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfClass(
									int64_t				owlClass,
									char				** name
								);

//
//		GetNameOfClassW                             (http://rdf.bg/gkdoc/CP64/GetNameOfClassW.html)
//				int64_t				owlClass							IN
//				wchar_t				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfClassW(
									int64_t				owlClass,
									wchar_t				** name
								);

//
//		GetNameOfClassEx                            (http://rdf.bg/gkdoc/CP64/GetNameOfClassEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				char				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
//	This call has the same behavior as GetNameOfClass, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
void		DECL STDC	GetNameOfClassEx(
									int64_t				model,
									int64_t				owlClass,
									char				** name
								);

//
//		GetNameOfClassWEx                           (http://rdf.bg/gkdoc/CP64/GetNameOfClassWEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				wchar_t				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
//	This call has the same behavior as GetNameOfClassW, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
void		DECL STDC	GetNameOfClassWEx(
									int64_t				model,
									int64_t				owlClass,
									wchar_t				** name
								);

//
//		SetClassPropertyCardinalityRestriction      (http://rdf.bg/gkdoc/CP64/SetClassPropertyCardinalityRestriction.html)
//				int64_t				owlClass							IN
//				int64_t				rdfProperty							IN
//				int64_t				minCard								IN
//				int64_t				maxCard								IN
//
//				void				returns
//
//	This function sets the minCard and maxCard of a certain property in the context of a class.
//	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
//	as within the cardinality restrictions as given by the property in context of any of its
//	(indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
void		DECL STDC	SetClassPropertyCardinalityRestriction(
									int64_t				owlClass,
									int64_t				rdfProperty,
									int64_t				minCard,
									int64_t				maxCard
								);

//
//		SetClassPropertyCardinalityRestrictionEx    (http://rdf.bg/gkdoc/CP64/SetClassPropertyCardinalityRestrictionEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				int64_t				rdfProperty							IN
//				int64_t				minCard								IN
//				int64_t				maxCard								IN
//
//				void				returns
//
//	This function sets the minCard and maxCard of a certain property in the context of a class.
//	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
//	as within the cardinality restrictions as given by the property in context of any of its
//	(indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
//	This call has the same behavior as SetClassPropertyCardinalityRestriction, however needs to be
//	used in case classes or properties are exchanged as a successive series of integers.
//
void		DECL STDC	SetClassPropertyCardinalityRestrictionEx(
									int64_t				model,
									int64_t				owlClass,
									int64_t				rdfProperty,
									int64_t				minCard,
									int64_t				maxCard
								);

//
//		GetClassPropertyCardinalityRestriction      (http://rdf.bg/gkdoc/CP64/GetClassPropertyCardinalityRestriction.html)
//				int64_t				owlClass							IN
//				int64_t				rdfProperty							IN
//				int64_t				* minCard							IN / OUT
//				int64_t				* maxCard							IN / OUT
//
//				void				returns
//
//	This function returns the minCard and maxCard of a certain
//	property in the context of a class. The cardinality of a property in 
//	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
//	as given by the property in context of any of its (indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
//	Note: this function does not return inherited restrictions. The example shows how to retrieve
//	this knowledge, as it is derived knowledge the call that used to be available is removed.
//
void		DECL STDC	GetClassPropertyCardinalityRestriction(
									int64_t				owlClass,
									int64_t				rdfProperty,
									int64_t				* minCard,
									int64_t				* maxCard
								);

//
//		GetClassPropertyCardinalityRestrictionEx    (http://rdf.bg/gkdoc/CP64/GetClassPropertyCardinalityRestrictionEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				int64_t				rdfProperty							IN
//				int64_t				* minCard							IN / OUT
//				int64_t				* maxCard							IN / OUT
//
//				void				returns
//
//	This function returns the minCard and maxCard of a certain
//	property in the context of a class. The cardinality of a property in 
//	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
//	as given by the property in context of any of its (indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
//	This call has the same behavior as GetClassPropertyCardinalityRestriction, however needs to be
//	used in case classes or properties are exchanged as a successive series of integers.
//
//	Note: this function does not return inherited restrictions. The example shows how to retrieve
//	this knowledge, as it is derived knowledge the call that used to be available is removed.
//
void		DECL STDC	GetClassPropertyCardinalityRestrictionEx(
									int64_t				model,
									int64_t				owlClass,
									int64_t				rdfProperty,
									int64_t				* minCard,
									int64_t				* maxCard
								);

//
//		GetGeometryClass                            (http://rdf.bg/gkdoc/CP64/GetGeometryClass.html)
//				int64_t				owlClass							IN
//
//				int64_t				returns								OUT
//
//	Returns non-zero if the owlClass is a geometry type. This call will return the input class
//	for all classes initially available. It will return as well non-for all classes created by the
//	user or loaded / imported through a model that (indirectly) inherit one of the
//	original classes available. in this case it returns the original available class
//	it inherits the behavior from.
//
int64_t		DECL STDC	GetGeometryClass(
									int64_t				owlClass
								);

//
//		GetGeometryClassEx                          (http://rdf.bg/gkdoc/CP64/GetGeometryClassEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//
//				int64_t				returns								OUT
//
//	Returns non-zero if the owlClass is a geometry type. This call will return the input class
//	for all classes initially available. It will return as well non-for all classes created by the
//	user or loaded / imported through a model that (indirectly) inherit one of the
//	original classes available. in this case it returns the original available class
//	it inherits the behavior from.
//
//	This call has the same behavior as GetGeometryClass, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
int64_t		DECL STDC	GetGeometryClassEx(
									int64_t				model,
									int64_t				owlClass
								);

//
//		IsClass                                     (http://rdf.bg/gkdoc/CP64/IsClass.html)
//				int64_t				owlClass							IN
//
//				bool				returns								OUT
//
//	Returns true if the argument owlClass is an actual active class in an active model. It returns false in all other cases,
//	i.e. this could mean the model is already closed, the class is inactive or removed or the session is closed.
//	It could also mean it represents a handle to another Thing, for example a property, instance or model.
//
bool		DECL STDC	IsClass(
									int64_t				owlClass
								);

//
//  Design Tree Properties API Calls
//

//
//		CreateProperty                              (http://rdf.bg/gkdoc/CP64/CreateProperty.html)
//				int64_t				model								IN
//				int64_t				rdfPropertyType						IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created property.
//	If the model input is zero or not a model handle 0 will be returned,
//
int64_t		DECL STDC	CreateProperty(
									int64_t				model,
									int64_t				rdfPropertyType,
									const char			* name
								);

//
//		CreatePropertyW                             (http://rdf.bg/gkdoc/CP64/CreatePropertyW.html)
//				int64_t				model								IN
//				int64_t				rdfPropertyType						IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created property.
//	If the model input is zero or not a model handle 0 will be returned,
//
int64_t		DECL STDC	CreatePropertyW(
									int64_t				model,
									int64_t				rdfPropertyType,
									const wchar_t		* name
								);

//
//		GetPropertyByName                           (http://rdf.bg/gkdoc/CP64/GetPropertyByName.html)
//				int64_t				model								IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
//	When the property does not exist yet and the name is unique
//	the property will be created on-the-fly and the handle will be returned.
//	When the name is not unique and given to a class or instance 0 will be returned.
//
int64_t		DECL STDC	GetPropertyByName(
									int64_t				model,
									const char			* name
								);

//
//		GetPropertyByNameW                          (http://rdf.bg/gkdoc/CP64/GetPropertyByNameW.html)
//				int64_t				model								IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
//	When the property does not exist yet and the name is unique
//	the property will be created on-the-fly and the handle will be returned.
//	When the name is not unique and given to a class or instance 0 will be returned.
//
int64_t		DECL STDC	GetPropertyByNameW(
									int64_t				model,
									const wchar_t		* name
								);

//
//		GetPropertiesByIterator                     (http://rdf.bg/gkdoc/CP64/GetPropertiesByIterator.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to a property.
//	If input property is zero, the handle will point to the first relevant property.
//	If all properties are past (or no relevant properties are found), the function will return 0.
//
int64_t		DECL STDC	GetPropertiesByIterator(
									int64_t				model,
									int64_t				rdfProperty
								);

//
//		SetPropertyRangeRestriction                 (http://rdf.bg/gkdoc/CP64/SetPropertyRangeRestriction.html)
//				int64_t				rdfProperty							IN
//				int64_t				owlClass							IN
//				int64_t				setting								IN
//
//				void				returns
//
//	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
//	be an objectp[roperty, i.e. relation.]
//	If rdfProperty is not an object property this call has no effect.
//
void		DECL STDC	SetPropertyRangeRestriction(
									int64_t				rdfProperty,
									int64_t				owlClass,
									int64_t				setting
								);

//
//		SetPropertyRangeRestrictionEx               (http://rdf.bg/gkdoc/CP64/SetPropertyRangeRestrictionEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//				int64_t				owlClass							IN
//				int64_t				setting								IN
//
//				void				returns
//
//	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
//	be an objectp[roperty, i.e. relation.]
//	If rdfProperty is not an object property this call has no effect.
//
void		DECL STDC	SetPropertyRangeRestrictionEx(
									int64_t				model,
									int64_t				rdfProperty,
									int64_t				owlClass,
									int64_t				setting
								);

//
//		GetRangeRestrictionsByIterator              (http://rdf.bg/gkdoc/CP64/GetRangeRestrictionsByIterator.html)
//				int64_t				rdfProperty							IN
//				int64_t				owlClass							IN
//
//				int64_t				returns								OUT
//
//	Returns the next class the property is restricted to.
//	If input class is zero, the handle will point to the first relevant class.
//	If all classes are past (or no relevant classes are found), the function will return 0.
//
int64_t		DECL STDC	GetRangeRestrictionsByIterator(
									int64_t				rdfProperty,
									int64_t				owlClass
								);

//
//		GetRangeRestrictionsByIteratorEx            (http://rdf.bg/gkdoc/CP64/GetRangeRestrictionsByIteratorEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//				int64_t				owlClass							IN
//
//				int64_t				returns								OUT
//
//	Returns the next class the property is restricted to.
//	If input class is zero, the handle will point to the first relevant class.
//	If all classes are past (or no relevant classes are found), the function will return 0.
//
int64_t		DECL STDC	GetRangeRestrictionsByIteratorEx(
									int64_t				model,
									int64_t				rdfProperty,
									int64_t				owlClass
								);

//
//		GetPropertyParentsByIterator                (http://rdf.bg/gkdoc/CP64/GetPropertyParentsByIterator.html)
//				int64_t				rdfProperty							IN
//				int64_t				parentRdfProperty					IN
//
//				int64_t				returns								OUT
//
//	Returns the next parent of the property.
//	If input parent is zero, the handle will point to the first relevant parent.
//	If all parent are past (or no relevant parent are found), the function will return 0.
//
int64_t		DECL STDC	GetPropertyParentsByIterator(
									int64_t				rdfProperty,
									int64_t				parentRdfProperty
								);

//
//		SetNameOfProperty                           (http://rdf.bg/gkdoc/CP64/SetNameOfProperty.html)
//				int64_t				rdfProperty							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the property, if no error it returns 0.
//	In case property does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfProperty(
									int64_t				rdfProperty,
									const char			* name
								);

//
//		SetNameOfPropertyW                          (http://rdf.bg/gkdoc/CP64/SetNameOfPropertyW.html)
//				int64_t				rdfProperty							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the property, if no error it returns 0.
//	In case property does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfPropertyW(
									int64_t				rdfProperty,
									const wchar_t		* name
								);

//
//		SetNameOfPropertyEx                         (http://rdf.bg/gkdoc/CP64/SetNameOfPropertyEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the property, if no error it returns 0.
//	In case property does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfPropertyEx(
									int64_t				model,
									int64_t				rdfProperty,
									const char			* name
								);

//
//		SetNameOfPropertyWEx                        (http://rdf.bg/gkdoc/CP64/SetNameOfPropertyWEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the property, if no error it returns 0.
//	In case property does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfPropertyWEx(
									int64_t				model,
									int64_t				rdfProperty,
									const wchar_t		* name
								);

//
//		GetNameOfProperty                           (http://rdf.bg/gkdoc/CP64/GetNameOfProperty.html)
//				int64_t				rdfProperty							IN
//				char				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfProperty(
									int64_t				rdfProperty,
									char				** name
								);

//
//		GetNameOfPropertyW                          (http://rdf.bg/gkdoc/CP64/GetNameOfPropertyW.html)
//				int64_t				rdfProperty							IN
//				wchar_t				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfPropertyW(
									int64_t				rdfProperty,
									wchar_t				** name
								);

//
//		GetNameOfPropertyEx                         (http://rdf.bg/gkdoc/CP64/GetNameOfPropertyEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//				char				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfPropertyEx(
									int64_t				model,
									int64_t				rdfProperty,
									char				** name
								);

//
//		GetNameOfPropertyWEx                        (http://rdf.bg/gkdoc/CP64/GetNameOfPropertyWEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//				wchar_t				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfPropertyWEx(
									int64_t				model,
									int64_t				rdfProperty,
									wchar_t				** name
								);

//
//		SetPropertyType                             (http://rdf.bg/gkdoc/CP64/SetPropertyType.html)
//				int64_t				rdfProperty							IN
//				int64_t				propertyType						IN
//
//				int64_t				returns								OUT
//
//	This function sets the type of the property. This is only allowed
//	if the type of the property was not set before.
//
//	The following values are possible for propertyType:
//			1	The property is an Object Property
//			2	The property is an Datatype Property of type Boolean
//			3	The property is an Datatype Property of type Char
//			4	The property is an Datatype Property of type Integer
//			5	The property is an Datatype Property of type Double
//	The return value of this call is GetPropertyType/Ex applied after applying
//	the type, normally this corresponds with the propertyType requested
//	to be set unless the property already has a different propertyType set before.
//
int64_t		DECL STDC	SetPropertyType(
									int64_t				rdfProperty,
									int64_t				propertyType
								);

//
//		SetPropertyTypeEx                           (http://rdf.bg/gkdoc/CP64/SetPropertyTypeEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//				int64_t				propertyType						IN
//
//				int64_t				returns								OUT
//
//
int64_t		DECL STDC	SetPropertyTypeEx(
									int64_t				model,
									int64_t				rdfProperty,
									int64_t				propertyType
								);

//
//		GetPropertyType                             (http://rdf.bg/gkdoc/CP64/GetPropertyType.html)
//				int64_t				rdfProperty							IN
//
//				int64_t				returns								OUT
//
//	This function returns the type of the property.
//	The following return values are possible:
//		0	The property is not defined yet
//		1	The property is an Object Type Property
//		2	The property is an Data Type Property of type Boolean
//		3	The property is an Data Type Property of type Char
//		4	The property is an Data Type Property of type Integer
//		5	The property is an Data Type Property of type Double
//
int64_t		DECL STDC	GetPropertyType(
									int64_t				rdfProperty
								);

//
//		GetPropertyTypeEx                           (http://rdf.bg/gkdoc/CP64/GetPropertyTypeEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//
//				int64_t				returns								OUT
//
//	This call has the same behavior as GetPropertyType, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t		DECL STDC	GetPropertyTypeEx(
									int64_t				model,
									int64_t				rdfProperty
								);

//
//		RemoveProperty                              (http://rdf.bg/gkdoc/CP64/RemoveProperty.html)
//				int64_t				rdfProperty							IN
//
//				int64_t				returns								OUT
//
//	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
//	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
//	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
//	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
//	the following bits represent the findings during removal:
//		bit 0:
//			0	Iunput as expected
//			1	Encountered an issue on input value, i.e. property was not recognized as property
//		bit 1:
//			0	No 'child' properties found
//			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
//		bit 2:
//			0	No instances found with value restrictions for this property
//			1	Vertex does contain 3D point info
//		bit 3:
//			0	No instances found with values for this property
//			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
//		bit 6:
//			0	Vertex does not contain first 2D texture info
//			1	Vertex does contain first 2D texture info
//		bit 7:
//			0	Vertex does not contain second 2D texture info
//			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
//
//		0	The property is not defined yet
//		1	The property is an Object Type Property
//		2	The property is an Data Type Property of type Boolean
//		3	The property is an Data Type Property of type Char
//		4	The property is an Data Type Property of type Integer
//		5	The property is an Data Type Property of type Double
//
int64_t		DECL STDC	RemoveProperty(
									int64_t				rdfProperty
								);

//
//		RemovePropertyEx                            (http://rdf.bg/gkdoc/CP64/RemovePropertyEx.html)
//				int64_t				model								IN
//				int64_t				rdfProperty							IN
//
//				int64_t				returns								OUT
//
//	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
//	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
//	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
//	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
//	the following bits represent the findings during removal:
//		bit 0:
//			0	Iunput as expected
//			1	Encountered an issue on input value, i.e. property was not recognized as property
//		bit 1:
//			0	No 'child' properties found
//			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
//		bit 2:
//			0	No instances found with value restrictions for this property
//			1	Vertex does contain 3D point info
//		bit 3:
//			0	No instances found with values for this property
//			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
//		bit 6:
//			0	Vertex does not contain first 2D texture info
//			1	Vertex does contain first 2D texture info
//		bit 7:
//			0	Vertex does not contain second 2D texture info
//			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
//
//		0	The property is not defined yet
//		1	The property is an Object Type Property
//		2	The property is an Data Type Property of type Boolean
//		3	The property is an Data Type Property of type Char
//		4	The property is an Data Type Property of type Integer
//		5	The property is an Data Type Property of type Double
//
//
//	This call has the same behavior as RemoveProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t		DECL STDC	RemovePropertyEx(
									int64_t				model,
									int64_t				rdfProperty
								);

//
//		IsProperty                                  (http://rdf.bg/gkdoc/CP64/IsProperty.html)
//				int64_t				rdfProperty							IN
//
//				bool				returns								OUT
//
//	Returns true if the argument rdfProperty is an actual active property in an active model. It returns false in all other cases,
//	i.e. this could mean the model is already closed, the property is inactive or removed or the session is closed.
//	It could also mean it represents a handle to another Thing, for example a class, instance or model.
//
bool		DECL STDC	IsProperty(
									int64_t				rdfProperty
								);

//
//  Design Tree Instances API Calls
//

//
//		CreateInstance                              (http://rdf.bg/gkdoc/CP64/CreateInstance.html)
//				int64_t				owlClass							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
int64_t		DECL STDC	CreateInstance(
									int64_t				owlClass,
									const char			* name
								);

//
//		CreateInstanceW                             (http://rdf.bg/gkdoc/CP64/CreateInstanceW.html)
//				int64_t				owlClass							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
int64_t		DECL STDC	CreateInstanceW(
									int64_t				owlClass,
									const wchar_t		* name
								);

//
//		CreateInstanceEx                            (http://rdf.bg/gkdoc/CP64/CreateInstanceEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
int64_t		DECL STDC	CreateInstanceEx(
									int64_t				model,
									int64_t				owlClass,
									const char			* name
								);

//
//		CreateInstanceWEx                           (http://rdf.bg/gkdoc/CP64/CreateInstanceWEx.html)
//				int64_t				model								IN
//				int64_t				owlClass							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
int64_t		DECL STDC	CreateInstanceWEx(
									int64_t				model,
									int64_t				owlClass,
									const wchar_t		* name
								);

//
//		GetInstancesByIterator                      (http://rdf.bg/gkdoc/CP64/GetInstancesByIterator.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to an instance.
//	If input instance is zero, the handle will point to the first relevant instance.
//	If all instances are past (or no relevant instances are found), the function will return 0.
//
int64_t		DECL STDC	GetInstancesByIterator(
									int64_t				model,
									int64_t				owlInstance
								);

//
//		GetInstanceClass                            (http://rdf.bg/gkdoc/CP64/GetInstanceClass.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	Returns the handle to the class of which an instances is instantiated.
//
//	Note: internally this function is not rich enough as support for multiple inheritance
//		  is making it impossible to answer this request with always one class handle.
//		  Even in the case of pure single inheritance an instance of a class is also an instance of 
//		  every parent classes in the inheritance tree. For now we expect single inheritance use
//		  and the returned class handle points to the 'lowest' class in the inheritance tree.
//
int64_t		DECL STDC	GetInstanceClass(
									int64_t				owlInstance
								);

//
//		GetInstanceClassEx                          (http://rdf.bg/gkdoc/CP64/GetInstanceClassEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//
int64_t		DECL STDC	GetInstanceClassEx(
									int64_t				model,
									int64_t				owlInstance
								);

//
//		GetInstancePropertyByIterator               (http://rdf.bg/gkdoc/CP64/GetInstancePropertyByIterator.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to the objectTypeProperty or dataTypeProperty connected to
//	the instance, this property can also contain a value, but for example also
//	the knowledge about cardinality restrictions in the context of this instance's class
//	and the exact cardinality in context of its instance.
//
int64_t		DECL STDC	GetInstancePropertyByIterator(
									int64_t				owlInstance,
									int64_t				rdfProperty
								);

//
//		GetInstancePropertyByIteratorEx             (http://rdf.bg/gkdoc/CP64/GetInstancePropertyByIteratorEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//
//				int64_t				returns								OUT
//
//
int64_t		DECL STDC	GetInstancePropertyByIteratorEx(
									int64_t				model,
									int64_t				owlInstance,
									int64_t				rdfProperty
								);

//
//		GetInstanceInverseReferencesByIterator      (http://rdf.bg/gkdoc/CP64/GetInstanceInverseReferencesByIterator.html)
//				int64_t				owlInstance							IN
//				int64_t				referencingOwlInstance				IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to the owlInstances refering this instance
//
int64_t		DECL STDC	GetInstanceInverseReferencesByIterator(
									int64_t				owlInstance,
									int64_t				referencingOwlInstance
								);

//
//		GetInstanceReferencesByIterator             (http://rdf.bg/gkdoc/CP64/GetInstanceReferencesByIterator.html)
//				int64_t				owlInstance							IN
//				int64_t				referencedOwlInstance				IN
//
//				int64_t				returns								OUT
//
//	Returns a handle to the owlInstance refered by this instance
//
int64_t		DECL STDC	GetInstanceReferencesByIterator(
									int64_t				owlInstance,
									int64_t				referencedOwlInstance
								);

//
//		SetNameOfInstance                           (http://rdf.bg/gkdoc/CP64/SetNameOfInstance.html)
//				int64_t				owlInstance							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the instance, if no error it returns 0.
//	In case instance does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfInstance(
									int64_t				owlInstance,
									const char			* name
								);

//
//		SetNameOfInstanceW                          (http://rdf.bg/gkdoc/CP64/SetNameOfInstanceW.html)
//				int64_t				owlInstance							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the instance, if no error it returns 0.
//	In case instance does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfInstanceW(
									int64_t				owlInstance,
									const wchar_t		* name
								);

//
//		SetNameOfInstanceEx                         (http://rdf.bg/gkdoc/CP64/SetNameOfInstanceEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				const char			* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the instance, if no error it returns 0.
//	In case instance does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfInstanceEx(
									int64_t				model,
									int64_t				owlInstance,
									const char			* name
								);

//
//		SetNameOfInstanceWEx                        (http://rdf.bg/gkdoc/CP64/SetNameOfInstanceWEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				const wchar_t		* name								IN
//
//				int64_t				returns								OUT
//
//	Sets/updates the name of the instance, if no error it returns 0.
//	In case instance does not exist it returns 1, when name cannot be updated 2.
//
int64_t		DECL STDC	SetNameOfInstanceWEx(
									int64_t				model,
									int64_t				owlInstance,
									const wchar_t		* name
								);

//
//		GetNameOfInstance                           (http://rdf.bg/gkdoc/CP64/GetNameOfInstance.html)
//				int64_t				owlInstance							IN
//				char				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfInstance(
									int64_t				owlInstance,
									char				** name
								);

//
//		GetNameOfInstanceW                          (http://rdf.bg/gkdoc/CP64/GetNameOfInstanceW.html)
//				int64_t				owlInstance							IN
//				wchar_t				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfInstanceW(
									int64_t				owlInstance,
									wchar_t				** name
								);

//
//		GetNameOfInstanceEx                         (http://rdf.bg/gkdoc/CP64/GetNameOfInstanceEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				char				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfInstanceEx(
									int64_t				model,
									int64_t				owlInstance,
									char				** name
								);

//
//		GetNameOfInstanceWEx                        (http://rdf.bg/gkdoc/CP64/GetNameOfInstanceWEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				wchar_t				** name								IN / OUT
//
//				void				returns
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
void		DECL STDC	GetNameOfInstanceWEx(
									int64_t				model,
									int64_t				owlInstance,
									wchar_t				** name
								);

//
//		SetDatatypeProperty                         (http://rdf.bg/gkdoc/CP64/SetDatatypeProperty.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				const void			* values							IN
//				int64_t				card								IN
//
//				int64_t				returns								OUT
//
//	This function sets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the values card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t		DECL STDC	SetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const void			* values,
									int64_t				card
								);

//
//		SetDatatypePropertyEx                       (http://rdf.bg/gkdoc/CP64/SetDatatypePropertyEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				const void			* values							IN
//				int64_t				card								IN
//
//				int64_t				returns								OUT
//
//	This function sets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the values card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as SetDatatypeProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t		DECL STDC	SetDatatypePropertyEx(
									int64_t				model,
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const void			* values,
									int64_t				card
								);

//
//		GetDatatypeProperty                         (http://rdf.bg/gkdoc/CP64/GetDatatypeProperty.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				void				** values							IN / OUT
//				int64_t				* card								IN / OUT
//
//				int64_t				returns								OUT
//
//	This function gets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the value card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
int64_t		DECL STDC	GetDatatypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									void				** values,
									int64_t				* card
								);

//
//		GetDatatypePropertyEx                       (http://rdf.bg/gkdoc/CP64/GetDatatypePropertyEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				void				** values							IN / OUT
//				int64_t				* card								IN / OUT
//
//				int64_t				returns								OUT
//
//	This function gets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the value card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as GetDatatypeProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t		DECL STDC	GetDatatypePropertyEx(
									int64_t				model,
									int64_t				owlInstance,
									int64_t				rdfProperty,
									void				** values,
									int64_t				* card
								);

//
//		SetObjectProperty                           (http://rdf.bg/gkdoc/CP64/SetObjectProperty.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				const int64_t		* values							IN
//				int64_t				card								IN
//
//				int64_t				returns								OUT
//
//	This function sets the value(s) of a certain objectTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the values card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t		DECL STDC	SetObjectProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const int64_t		* values,
									int64_t				card
								);

//
//		SetObjectPropertyEx                         (http://rdf.bg/gkdoc/CP64/SetObjectPropertyEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				const int64_t		* values							IN
//				int64_t				card								IN
//
//				int64_t				returns								OUT
//
//	This function sets the value(s) of a certain objectTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the values card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as SetObjectProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t		DECL STDC	SetObjectPropertyEx(
									int64_t				model,
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const int64_t		* values,
									int64_t				card
								);

//
//		GetObjectProperty                           (http://rdf.bg/gkdoc/CP64/GetObjectProperty.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				int64_t				** values							IN / OUT
//				int64_t				* card								IN / OUT
//
//				int64_t				returns								OUT
//
//	This function gets the value(s) of a certain objectProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the value card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
int64_t		DECL STDC	GetObjectProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									int64_t				** values,
									int64_t				* card
								);

//
//		GetObjectPropertyEx                         (http://rdf.bg/gkdoc/CP64/GetObjectPropertyEx.html)
//				int64_t				model								IN
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				int64_t				** values							IN / OUT
//				int64_t				* card								IN / OUT
//
//				int64_t				returns								OUT
//
//	This function gets the value(s) of a certain objectProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the values card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as GetObjectProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t		DECL STDC	GetObjectPropertyEx(
									int64_t				model,
									int64_t				owlInstance,
									int64_t				rdfProperty,
									int64_t				** values,
									int64_t				* card
								);

//
//		CreateInstanceInContextStructure            (http://rdf.bg/gkdoc/CP64/CreateInstanceInContextStructure.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	InstanceInContext structures give you more detailed information about
//	individual parts of the geometry of a certain instance viualized.
//	It is allowed to have more then 1 InstanceInContext structures per instance.
//	InstanceInContext structures are updated dynamically when the geometry
//	structure is updated.
//
int64_t		DECL STDC	CreateInstanceInContextStructure(
									int64_t				owlInstance
								);

//
//		DestroyInstanceInContextStructure           (http://rdf.bg/gkdoc/CP64/DestroyInstanceInContextStructure.html)
//				int64_t				owlInstanceInContext				IN
//
//				void				returns
//
//	InstanceInContext structures are updated dynamically and therfore even while the cost
//	in performance and memory is limited it is advised to destroy structures as soon
//	as they are obsolete.
//
void		DECL STDC	DestroyInstanceInContextStructure(
									int64_t				owlInstanceInContext
								);

//
//		InstanceInContextChild                      (http://rdf.bg/gkdoc/CP64/InstanceInContextChild.html)
//				int64_t				owlInstanceInContext				IN
//
//				int64_t				returns								OUT
//
//
int64_t		DECL STDC	InstanceInContextChild(
									int64_t				owlInstanceInContext
								);

//
//		InstanceInContextNext                       (http://rdf.bg/gkdoc/CP64/InstanceInContextNext.html)
//				int64_t				owlInstanceInContext				IN
//
//				int64_t				returns								OUT
//
//
int64_t		DECL STDC	InstanceInContextNext(
									int64_t				owlInstanceInContext
								);

//
//		InstanceInContextIsUpdated                  (http://rdf.bg/gkdoc/CP64/InstanceInContextIsUpdated.html)
//				int64_t				owlInstanceInContext				IN
//
//				int64_t				returns								OUT
//
//
int64_t		DECL STDC	InstanceInContextIsUpdated(
									int64_t				owlInstanceInContext
								);

//
//		RemoveInstance                              (http://rdf.bg/gkdoc/CP64/RemoveInstance.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	This function removes an instance from the internal structure.
//	In case copies are created by the host this function checks if all
//	copies are removed otherwise the instance will stay available.
//	Return value is 0 if everything went ok and positive in case of an error
//
int64_t		DECL STDC	RemoveInstance(
									int64_t				owlInstance
								);

//
//		RemoveInstanceRecursively                   (http://rdf.bg/gkdoc/CP64/RemoveInstanceRecursively.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	This function removes an instance recursively from the internal structure.
//	In case checkInverseRelations is non-zero only instances that are not referenced
//	by other existing instances.
//
//	Return value is total number of removed instances
//
int64_t		DECL STDC	RemoveInstanceRecursively(
									int64_t				owlInstance
								);

//
//		RemoveInstances                             (http://rdf.bg/gkdoc/CP64/RemoveInstances.html)
//				int64_t				model								IN
//
//				int64_t				returns								OUT
//
//	This function removes all available instances for the given model 
//	from the internal structure.
//	Return value is the number of removed instances.
//
int64_t		DECL STDC	RemoveInstances(
									int64_t				model
								);

//
//		IsInstance                                  (http://rdf.bg/gkdoc/CP64/IsInstance.html)
//				int64_t				owlInstance							IN
//
//				bool				returns								OUT
//
//	Returns true if the argument owlInstance is an actual active property in an active model. It returns false in all other cases,
//	i.e. this could mean the model is already closed, the instance is inactive or removed or the session is closed.
//	It could also mean it represents a handle to another Thing, for example a class, property or model.
//
bool		DECL STDC	IsInstance(
									int64_t				owlInstance
								);

//
//  Retrieve Geometry API Calls
//

//
//		CalculateInstance                           (http://rdf.bg/gkdoc/CP64/CalculateInstance.html)
//				int64_t				owlInstance							IN
//				int64_t				* vertexBufferSize					IN / OUT
//				int64_t				* indexBufferSize					IN / OUT
//				int64_t				* transformationBufferSize			IN / OUT
//
//				int64_t				returns								OUT
//
//	This function prepares the content to be ready so the buffers can be filled.
//	It returns the minimum size the buffers should be. This is only the case
//	when the pointer is given, all arguments are allowed to be nullptr.
//
//	Note: This function needs to be called directly before UpdateVertexBuffer(),
//		  UpdateIndexBuffer() and UpdateTransformationBuffer().
//
int64_t		DECL STDC	CalculateInstance(
									int64_t				owlInstance,
									int64_t				* vertexBufferSize,
									int64_t				* indexBufferSize,
									int64_t				* transformationBufferSize
								);

//
//		UpdateInstance                              (http://rdf.bg/gkdoc/CP64/UpdateInstance.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	This function prepares the content to be ready without filling the buffers
//	as done within CalculateInstance(). CalculateInstance calls this function as a start.
//	This function will also set the 'derived' values for the instance passed as argument.
//	For example the coordinates values of a MultiplicationMatrix will be set if the array is
//	defined.
//
int64_t		DECL STDC	UpdateInstance(
									int64_t				owlInstance
								);

//
//		InferenceInstance                           (http://rdf.bg/gkdoc/CP64/InferenceInstance.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	This function fills in values that are implicitely known but not given by the user. This function
//	can also be used to identify default values of properties if not given.
//
int64_t		DECL STDC	InferenceInstance(
									int64_t				owlInstance
								);

//
//		UpdateInstanceVertexBuffer                  (http://rdf.bg/gkdoc/CP64/UpdateInstanceVertexBuffer.html)
//				int64_t				owlInstance							IN
//				void				* vertexBuffer						IN / OUT
//
//				int64_t				returns								OUT
//
//	This function should be preceded by the function CalculateInstances(),
//	the only allowed other API functions in between are UpdateIndexBuffer()
//	and UpdateTransformationBuffer().
//	It is expected to be called with a buffer vertexBuffer of at least the size as 
//	given by CalculateInstances().
//	If not called for the first time it will expect to contain the same content as
//	from previous call, even is size is changed. This can be overruled by 
//	the function ClearedExternalBuffers().
//
int64_t		DECL STDC	UpdateInstanceVertexBuffer(
									int64_t				owlInstance,
									void				* vertexBuffer
								);

//
//		UpdateInstanceVertexBufferTrimmed           (http://rdf.bg/gkdoc/CP64/UpdateInstanceVertexBufferTrimmed.html)
//				int64_t				owlInstance							IN
//				void				* vertexBuffer						IN / OUT
//				int64_t				offset								IN
//				int64_t				size								IN
//
//				int64_t				returns								OUT
//
//	This function is an alternative for UpdateInstanceVertexBuffer(),
//	in case the vertex buffer should be divided over a set of arrays
//	this function allows to fill part of the vertex buffer given a
//	certain offset and size (both calculated in vertex element count).
//
int64_t		DECL STDC	UpdateInstanceVertexBufferTrimmed(
									int64_t				owlInstance,
									void				* vertexBuffer,
									int64_t				offset,
									int64_t				size
								);

//
//		UpdateInstanceIndexBuffer                   (http://rdf.bg/gkdoc/CP64/UpdateInstanceIndexBuffer.html)
//				int64_t				owlInstance							IN
//				void				* indexBuffer						IN / OUT
//
//				int64_t				returns								OUT
//
//	This function should be preceded by the function CalculateInstances(),
//	the only allowed other API functions in between are UpdateVertexBuffer()
//	and UpdateTransformationBuffer().
//	It is expected to be called with a buffer indexBuffer of at least the size as 
//	given by CalculateInstances().
//	If not called for the first time it will expect to contain the same content as
//	from previous call, even is size is changed. This can be overruled by 
//	the function ClearedExternalBuffers().
//
int64_t		DECL STDC	UpdateInstanceIndexBuffer(
									int64_t				owlInstance,
									void				* indexBuffer
								);

//
//		UpdateInstanceIndexBufferTrimmed            (http://rdf.bg/gkdoc/CP64/UpdateInstanceIndexBufferTrimmed.html)
//				int64_t				owlInstance							IN
//				void				* indexBuffer						IN / OUT
//				int64_t				offset								IN
//				int64_t				size								IN
//
//				int64_t				returns								OUT
//
//	This function is an alternative for UpdateInstanceIndexBuffer(),
//	in case the index buffer should be divided over a set of arrays
//	this function allows to fill part of the index buffer given a
//	certain offset and size.
//
int64_t		DECL STDC	UpdateInstanceIndexBufferTrimmed(
									int64_t				owlInstance,
									void				* indexBuffer,
									int64_t				offset,
									int64_t				size
								);

//
//		UpdateInstanceTransformationBuffer          (http://rdf.bg/gkdoc/CP64/UpdateInstanceTransformationBuffer.html)
//				int64_t				owlInstance							IN
//				void				* transformationBuffer				IN / OUT
//
//				int64_t				returns								OUT
//
//	This function should be preceded by the function CalculateInstances(),
//	the only allowed other API functions in between are UpdateVertexBuffer()
//	and UpdateIndexBuffer().
//	It is expected to be called with a buffer vertexBuffer of at least the size as 
//	given by CalculateInstances().
//	If not called for the first time it will expect to contain the same content as
//	from previous call, even is size is changed. This can be overruled by 
//	the function ClearedExternalBuffers().
//
int64_t		DECL STDC	UpdateInstanceTransformationBuffer(
									int64_t				owlInstance,
									void				* transformationBuffer
								);

//
//		ClearedInstanceExternalBuffers              (http://rdf.bg/gkdoc/CP64/ClearedInstanceExternalBuffers.html)
//				int64_t				owlInstance							IN
//
//				void				returns
//
//	This function tells the engine that all buffers have no memory of earlier filling 
//	for a certain instance.
//	This means that even when buffer content didn't changed it will be updated when
//	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
//	are called for this specific instance.
//
void		DECL STDC	ClearedInstanceExternalBuffers(
									int64_t				owlInstance
								);

//
//		ClearedExternalBuffers                      (http://rdf.bg/gkdoc/CP64/ClearedExternalBuffers.html)
//				int64_t				model								IN
//
//				void				returns
//
//	This function tells the engine that all buffers have no memory of earlier filling.
//	This means that even when buffer content didn't changed it will be updated when
//	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
//	are called.
//
void		DECL STDC	ClearedExternalBuffers(
									int64_t				model
								);

//
//		GetConceptualFaceCnt                        (http://rdf.bg/gkdoc/CP64/GetConceptualFaceCnt.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	This function returns the number of conceptual faces for a certain instance.
//
int64_t		DECL STDC	GetConceptualFaceCnt(
									int64_t				owlInstance
								);

//
//		GetConceptualFace                           (http://rdf.bg/gkdoc/CP64/GetConceptualFace.html)
//				int64_t				owlInstance							IN
//				int64_t				index								IN
//				int64_t				* startIndexTriangles				IN / OUT
//				int64_t				* noTriangles						IN / OUT
//
//				int64_t				returns								OUT
//
//	This function returns a handle to the conceptual face. Be aware that different
//	instances can return the same handles (however with possible different startIndices and noTriangles).
//	Argument index should be at least zero and smaller then return value of GetConceptualFaceCnt().
//	Argument startIndex shows the first index used.
//	Argument noTriangles returns the number of triangles, each triangle is existing of 3 unique indices.
//
int64_t		DECL STDC	GetConceptualFace(
									int64_t				owlInstance,
									int64_t				index,
									int64_t				* startIndexTriangles,
									int64_t				* noTriangles
								);

//
//		GetConceptualFaceEx                         (http://rdf.bg/gkdoc/CP64/GetConceptualFaceEx.html)
//				int64_t				owlInstance							IN
//				int64_t				index								IN
//				int64_t				* startIndexTriangles				IN / OUT
//				int64_t				* noIndicesTriangles				IN / OUT
//				int64_t				* startIndexLines					IN / OUT
//				int64_t				* noIndicesLines					IN / OUT
//				int64_t				* startIndexPoints					IN / OUT
//				int64_t				* noIndicesPoints					IN / OUT
//				int64_t				* startIndexFacePolygons			IN / OUT
//				int64_t				* noIndicesFacePolygons				IN / OUT
//				int64_t				* startIndexConceptualFacePolygons	IN / OUT
//				int64_t				* noIndicesConceptualFacePolygons	IN / OUT
//
//				int64_t				returns								OUT
//
//	This function returns a handle to the conceptual face. Be aware that different
//	instances can return the same handles (however with possible different startIndices and noTriangles).
//	Argument index should be at least zero and smaller then return value of GetConceptualFaceCnt().
//
int64_t		DECL STDC	GetConceptualFaceEx(
									int64_t				owlInstance,
									int64_t				index,
									int64_t				* startIndexTriangles,
									int64_t				* noIndicesTriangles,
									int64_t				* startIndexLines,
									int64_t				* noIndicesLines,
									int64_t				* startIndexPoints,
									int64_t				* noIndicesPoints,
									int64_t				* startIndexFacePolygons,
									int64_t				* noIndicesFacePolygons,
									int64_t				* startIndexConceptualFacePolygons,
									int64_t				* noIndicesConceptualFacePolygons
								);

//
//		GetConceptualFaceMaterial                   (http://rdf.bg/gkdoc/CP64/GetConceptualFaceMaterial.html)
//				int64_t				conceptualFace						IN
//
//				int64_t				returns								OUT
//
//	This function returns the material instance relevant for this
//	conceptual face.
//
int64_t		DECL STDC	GetConceptualFaceMaterial(
									int64_t				conceptualFace
								);

//
//		GetConceptualFaceOriginCnt                  (http://rdf.bg/gkdoc/CP64/GetConceptualFaceOriginCnt.html)
//				int64_t				conceptualFace						IN
//
//				int64_t				returns								OUT
//
//	This function returns the number of instances that are the source primitive/concept
//	for this conceptual face.
//
int64_t		DECL STDC	GetConceptualFaceOriginCnt(
									int64_t				conceptualFace
								);

//
//		GetConceptualFaceOrigin                     (http://rdf.bg/gkdoc/CP64/GetConceptualFaceOrigin.html)
//				int64_t				conceptualFace						IN
//				int64_t				index								IN
//
//				int64_t				returns								OUT
//
//	This function returns a handle to the instance that is the source primitive/concept
//	for this conceptual face.
//
int64_t		DECL STDC	GetConceptualFaceOrigin(
									int64_t				conceptualFace,
									int64_t				index
								);

//
//		GetConceptualFaceOriginEx                   (http://rdf.bg/gkdoc/CP64/GetConceptualFaceOriginEx.html)
//				int64_t				conceptualFace						IN
//				int64_t				index								IN
//				int64_t				* originatingOwlInstance			IN / OUT
//				int64_t				* originatingConceptualFace			IN / OUT
//
//				void				returns
//
//
void		DECL STDC	GetConceptualFaceOriginEx(
									int64_t				conceptualFace,
									int64_t				index,
									int64_t				* originatingOwlInstance,
									int64_t				* originatingConceptualFace
								);

//
//		GetFaceCnt                                  (http://rdf.bg/gkdoc/CP64/GetFaceCnt.html)
//				int64_t				owlInstance							IN
//
//				int64_t				returns								OUT
//
//	This function returns the number of faces for a certain instance.
//
int64_t		DECL STDC	GetFaceCnt(
									int64_t				owlInstance
								);

//
//		GetFace                                     (http://rdf.bg/gkdoc/CP64/GetFace.html)
//				int64_t				owlInstance							IN
//				int64_t				index								IN
//				int64_t				* startIndex						IN / OUT
//				int64_t				* noOpenings						IN / OUT
//
//				void				returns
//
//	This function gets the individual faces including the meta data, i.e. the number of openings within this specific face.
//	This call is for very dedicated use, it would be more common to iterate over the individual conceptual faces.
//
void		DECL STDC	GetFace(
									int64_t				owlInstance,
									int64_t				index,
									int64_t				* startIndex,
									int64_t				* noOpenings
								);

//
//		GetDependingPropertyCnt                     (http://rdf.bg/gkdoc/CP64/GetDependingPropertyCnt.html)
//				int64_t				baseOwlInstance						IN
//				int64_t				conceptualFace						IN
//
//				int64_t				returns								OUT
//
//	This function returns the number of properties that are of influence on the
//	location and form of the conceptualFace.
//
//	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
//
int64_t		DECL STDC	GetDependingPropertyCnt(
									int64_t				baseOwlInstance,
									int64_t				conceptualFace
								);

//
//		GetDependingProperty                        (http://rdf.bg/gkdoc/CP64/GetDependingProperty.html)
//				int64_t				baseOwlInstance						IN
//				int64_t				conceptualFace						IN
//				int64_t				index								IN
//				int64_t				* owlInstance						IN / OUT
//				int64_t				* datatypeProperty					IN / OUT
//
//				void				returns
//
//	This function returns a handle to the property that is the 'index'-th property
//	of influence on the form. It also returns the handle to instance this property
//	belongs to.
//
//	Note: the returned property is always a datatypeProperty
//	Note: if input is incorrect (for example index is in wrong domain) _property and
//		  instance will be both zero.
//	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
//
void		DECL STDC	GetDependingProperty(
									int64_t				baseOwlInstance,
									int64_t				conceptualFace,
									int64_t				index,
									int64_t				* owlInstance,
									int64_t				* datatypeProperty
								);

//
//		SetFormat                                   (http://rdf.bg/gkdoc/CP64/SetFormat.html)
//				int64_t				model								IN
//				int64_t				setting								IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	This function sets the type of export format, by setting a mask
//		bit 0 & 1:
//			00	Each vertex is unique (although mostly irrelevant UpdateIndexBuffer() and 
//				UpdateTransformationBuffer() are still returning information)
//			01	Each index is unique => vertices are not necessarily (although mostly
//				irrelevant UpdateTransformationBuffer() is still returning information)
//			10	Single level Transformations are used, most optimal when using DirectX till version 11
//				and OpenGL till version 2
//			11	Nested Transformations are used, most optimal but till 2011 no known support of
//				low level 3D interfaces like DirectX and OpenGL
//		bit 2:	(FORMAT_SIZE_VERTEX_DOUBLE)
//			0	Vertex items returned as float (4 byte/32 bit)
//			1	Vertex items returned as double (8 byte/64 bit)
//		bit 3:	(FORMAT_SIZE_INDEX_INT64)
//			0	Index items returned as int32_t (4 byte/32 bit)
//			1	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode)
//
//		bit 4:	(FORMAT_VERTEX_POINT)
//			0	Vertex does not contain 3D point info
//			1	Vertex does contain 3D point info
//		bit 5:	(FORMAT_VERTEX_NORMAL)
//			0	Vertex does not contain 3D normal vector info
//			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
//		bit 6:	(FORMAT_VERTEX_TEXTURE_UV)
//			0	Vertex does not contain first 2D texture info
//			1	Vertex does contain first 2D texture info
//		bit 7:	(FORMAT_VERTEX_TEXTURE2_UV)
//			0	Vertex does not contain second 2D texture info
//			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
//
//		bit 8:	(FORMAT_EXPORT_TRIANGLES)
//			0	No object form triangles are exported
//			1	Object form triangles are exported (effective if instance contains faces and/or solids)
//		bit 9:	(FORMAT_EXPORT_LINES)
//			0	No object polygon lines are exported
//			1	Object polygon lines are exported (effective if instance contains line representations)
//		bit 10:	(FORMAT_EXPORT_POINTS)
//			0	No object points are exported
//			1	Object points are exported (effective if instance contains point representations)
//
//		bit 11:	Reserved, by default 0
//
//		bit 12:	(FORMAT_EXPORT_FACE_POLYGONS)
//			0	No object face polygon lines are exported
//			1	Object face polygon lines (dense wireframe) are exported => if set, bit 8 will also be set
//		bit 13:	(FORMAT_EXPORT_CONCEPTUAL_FACE_POLYGONS)
//			0	No object conceptual face polygon lines are exported
//			1	Object conceptual face polygon lines (wireframe) are exported => if set, bit 12 will also be set
//		bit 14:	(FORMAT_EXPORT_POLYGONS_AS_TUPLES)
//			0	Polygon lines (wireframe) exported as list, i.e. typical 4 point polygon exported as  0 1 2 3 0 -1
//			1	Polygon lines (wireframe) exported as tuples, i.e. typical 4 point polygon exported as 0 1 1 2 2 3 3 0
//
//		bit 15:	(FORMAT_EXPORT_ADVANCED_NORMALS)
//			0	All normals of triangles are transformed orthogonal to the 2D face they belong to
//			1	Normals are exported to be in line with the original semantic form description (could be non orthogonal to the 2D face) 
//
//		bit 16:	(FORMAT_EXPORT_DIRECTX)
//			0	no specific behavior
//			1	Where possible DirectX compatibility is given to exported data (i.e. order of components in vertex buffer)
//					 => [bit 20, bit 21 both UNSET]
//					 => if set, bit 17 will be unset
//
//		bit 17:	(FORMAT_EXPORT_OPENGL)
//			0	no specific behavior
//			1	Where possible OpenGL compatibility is given to exported data (i.e. order of components in vertex buffer and inverted texture coordinates in Y direction)
//					 => [bit 20, bit 21 both SET]
//					 => if set, bit 16 will be unset
//
//		bit 18:	(FORMAT_EXPORT_DOUBLE_SIDED)
//			0	All faces are defined as calculated
//			1	Every face has exactly one opposite face (normally both index and vertex buffer are doubled in size)
//
//		bit 19:	Reserved, by default 0
//
//		bit 20-23:
//			0000	version 0 (used in case there is different behavior between versions in DirectX or OpenGL)
//			....	...
//			1111	version 15
//
//		bit 20:	(FORMAT_EXPORT_VERSION_0001)
//			0	Standard Triangle Rotation (LHS as expected by DirectX) 
//			1	Opposite Triangle Rotation (RHS as expected by OpenGL)
//		bit 21:	(FORMAT_EXPORT_VERSION_0010)
//			0	X, Y, Z (nX, nY, nZ) formatted as <X Y Z> considering internal concepts
//			1	X, Y, Z (nX, nY, nZ) formatted as <X -Z Y>, i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL)
//
//		bit 24:	(FORMAT_VERTEX_COLOR_AMBIENT)
//			0	Vertex does not contain Ambient color information
//			1	Vertex does contain Ambient color information
//		bit 25:	(FORMAT_VERTEX_COLOR_DIFFUSE)
//			0	Vertex does not contain Diffuse color information
//			1	Vertex does contain Diffuse color information
//		bit 26:	(FORMAT_VERTEX_COLOR_EMISSIVE)
//			0	Vertex does not contain Emissive color information
//			1	Vertex does contain Emissive color information
//		bit 27:	(FORMAT_VERTEX_COLOR_SPECULAR)
//			0	Vertex does not contain Specular color information
//			1	Vertex does contain Specular color information
//
//		bit 28:	(FORMAT_VERTEX_TEXTURE_TANGENT)
//			0	Vertex does not contain tangent vector for first texture
//			1	Vertex does contain tangent vector for first texture => if set, bit 6 will also be set
//		bit 29:	(FORMAT_VERTEX_TEXTURE_BINORMAL)
//			0	Vertex does not contain binormal vector for first texture
//			1	Vertex does contain binormal vector for first texture => if set, bit 6 will also be set
//		bit 30:	(FORMAT_VERTEX_TEXTURE2_TANGENT)		ONLY WORKS IN 64 BIT MODE
//			0	Vertex does not contain tangent vector for second texture
//			1	Vertex does contain tangent vector for second texture => if set, bit 6 will also be set
//		bit 31:	(FORMAT_VERTEX_TEXTURE2_BINORMAL)		ONLY WORKS IN 64 BIT MODE
//			0	Vertex does not contain binormal vector for second texture
//			1	Vertex does contain binormal vector for second texture => if set, bit 6 will also be set
//
//		bit 26-31:	Reserved, by default 0
//
//		bit 32-63:	Reserved, by default 0
//
//	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   1000 0001  0011 0000 = h0000 0000 - 0000 8130 = 33072
//
//
//	Depending on FORMAT_SIZE_VERTEX_DOUBLE each element in the vertex buffer is a double or float number.
//	Number of elements for each vertex depends on format setting. You can get the number by GetVertexElementsCounts. 
//	Each vertex block contains data items in an order according to the table below. The table also specifies when an item is present and number of elements 
//	it occupied. Use GetVertexDataOffset or GetVertexColor to get required item. 
//
//	#	Vertex data item	Included when format setting bit is on					Size (num of elements)
//	Point coordinates		X, Y, X				FORMAT_VERTEX_POINT	(bit 4)					3
//	Normal coordinates		Nx, Ny, Nz			FORMAT_VERTEX_NORMAL (bit 5)				3
//	Texture coordinates		T1u, T1v			FORMAT_VERTEX_TEXTURE_UV (bit 6)			2
//	2nd Texture coordinates	T2u, T2v			FORMAT_VERTEX_TEXTURE2_UV (bit 7)			2
//	Ambient color								FORMAT_VERTEX_COLOR_AMBIENT (bit 24)		1
//	Diffuse color								FORMAT_VERTEX_COLOR_DIFFUSE (bit 25)		1
//	Emissive color								FORMAT_VERTEX_COLOR _EMISSIVE (bit 26)		1
//	Specular color								FORMAT_VERTEX_COLOR _SPECULAR (bit 27)		1
//	Texture tangent			T1Tx, T1Ty, T1Tz	FORMAT_VERTEX_TEXTURE_TANGENT (bit 28)		3
//	Texture binormal		T1BNx,T1BNy,T1BNz	FORMAT_VERTEX_TEXTURE_BINORMAL (bit 29)		3
//	2nd texture tangent		T2Tx, T2Ty, T2Tz	FORMAT_VERTEX_TEXTURE2_TANGENT (bit 30)		3
//	2nd texture binormal	T2BNx,T2BNy,T2BNz	FORMAT_VERTEX_TEXTURE2_BINORMAL (bit 31)	3
//
int64_t		DECL STDC	SetFormat(
									int64_t				model,
									int64_t				setting,
									int64_t				mask
								);

//
//		GetFormat                                   (http://rdf.bg/gkdoc/CP64/GetFormat.html)
//				int64_t				model								IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	Returns the current format given a mask.
//
int64_t		DECL STDC	GetFormat(
									int64_t				model,
									int64_t				mask
								);

//
//		GetVertexDataOffset                         (http://rdf.bg/gkdoc/CP64/GetVertexDataOffset.html)
//				int64_t				requiredData						IN
//				int64_t				setting								IN
//
//				int32_t				returns								OUT
//
//	Returns offset of the required data in a vertex elements array with the specified format settings
//	requiredData is one of the control vertex data bits (FORMAT_VERTEX...) or 0 to get count of all elements in vertex buffer
//	Functions returns -1 if the required data are absent with the settings.
//
//	Ensure your settings are actual. They may be differ you pass to SetFormat (for example because of mask)
//	It's preferable to inquire resulting setting with GetFormat(model, GetFormat(0, 0))
//
//	Note: vertex buffer element is a double or a float number depending on FORMAT_SIZE_VERTEX_DOUBLE flag. 
//	If you need offset in bytes multiply by size of element.
//	Compare to SetFormat that returns size of vertex data in bytes.
//
int32_t		DECL STDC	GetVertexDataOffset(
									int64_t				requiredData,
									int64_t				setting
								);

//
//		SetBehavior                                 (http://rdf.bg/gkdoc/CP64/SetBehavior.html)
//				int64_t				model								IN
//				int64_t				setting								IN
//				int64_t				mask								IN
//
//				void				returns
//
//	This function sets the type of behavior, by setting a mask
//
//		bit 0-7:	Reserved, by default 0
//
//		bit 8:
//			0	Do not optimize
//			1	Vertex items returned as double (8 byte/64 bit)
//
//		bit 9-31:	Reserved, by default 0
//
//		bit 32-63:	Reserved, by default 0
//
//	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   0000 0001  0000 0000 = h0000 0000 - 0000 0100 = 256
//
void		DECL STDC	SetBehavior(
									int64_t				model,
									int64_t				setting,
									int64_t				mask
								);

//
//		GetBehavior                                 (http://rdf.bg/gkdoc/CP64/GetBehavior.html)
//				int64_t				model								IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	Returns the current behavior given a mask.
//
int64_t		DECL STDC	GetBehavior(
									int64_t				model,
									int64_t				mask
								);

//
//		SetVertexBufferTransformation               (http://rdf.bg/gkdoc/CP64/SetVertexBufferTransformation.html)
//				int64_t				model								IN
//				const double		* matrix							IN
//
//				void				returns
//
//	Sets the transformation for a Vertex Buffer.
//	The transformation will always be calculated in 64 bit, even if the vertex element size is 32 bit.
//	This function can be called just before updating the vertex buffer.
//
void		DECL STDC	SetVertexBufferTransformation(
									int64_t				model,
									const double		* matrix
								);

//
//		GetVertexBufferTransformation               (http://rdf.bg/gkdoc/CP64/GetVertexBufferTransformation.html)
//				int64_t				model								IN
//				double				* matrix							IN / OUT
//
//				void				returns
//
//	Gets the transformation for a Vertex Buffer.
//
void		DECL STDC	GetVertexBufferTransformation(
									int64_t				model,
									double				* matrix
								);

//
//		SetIndexBufferOffset                        (http://rdf.bg/gkdoc/CP64/SetIndexBufferOffset.html)
//				int64_t				model								IN
//				int64_t				offset								IN
//
//				void				returns
//
//	Sets the offset for an Index Buffer.
//	It is important call this function before updating the vertex buffer. 
//
void		DECL STDC	SetIndexBufferOffset(
									int64_t				model,
									int64_t				offset
								);

//
//		GetIndexBufferOffset                        (http://rdf.bg/gkdoc/CP64/GetIndexBufferOffset.html)
//				int64_t				model								IN
//
//				int64_t				returns								OUT
//
//	Gets the current offset for an Index Buffer.
//
int64_t		DECL STDC	GetIndexBufferOffset(
									int64_t				model
								);

//
//		SetVertexBufferOffset                       (http://rdf.bg/gkdoc/CP64/SetVertexBufferOffset.html)
//				int64_t				model								IN
//				double				x									IN
//				double				y									IN
//				double				z									IN
//
//				void				returns
//
//	Sets the offset for a Vertex Buffer.
//	The offset will always be calculated in 64 bit, even if the vertex element size is 32 bit.
//	This function can be called just before updating the vertex buffer.
//
void		DECL STDC	SetVertexBufferOffset(
									int64_t				model,
									double				x,
									double				y,
									double				z
								);

//
//		GetVertexBufferOffset                       (http://rdf.bg/gkdoc/CP64/GetVertexBufferOffset.html)
//				int64_t				model								IN
//				double				* x									IN / OUT
//				double				* y									IN / OUT
//				double				* z									IN / OUT
//
//				void				returns
//
//	Gets the offset for a Vertex Buffer.
//
void		DECL STDC	GetVertexBufferOffset(
									int64_t				model,
									double				* x,
									double				* y,
									double				* z
								);

//
//		SetDefaultColor                             (http://rdf.bg/gkdoc/CP64/SetDefaultColor.html)
//				int64_t				model								IN
//				int32_t				ambient								IN
//				int32_t				diffuse								IN
//				int32_t				emissive							IN
//				int32_t				specular							IN
//
//				void				returns
//
//	Set the default values for the colors defined as argument.
//
void		DECL STDC	SetDefaultColor(
									int64_t				model,
									int32_t				ambient,
									int32_t				diffuse,
									int32_t				emissive,
									int32_t				specular
								);

//
//		GetDefaultColor                             (http://rdf.bg/gkdoc/CP64/GetDefaultColor.html)
//				int64_t				model								IN
//				int32_t				* ambient							IN / OUT
//				int32_t				* diffuse							IN / OUT
//				int32_t				* emissive							IN / OUT
//				int32_t				* specular							IN / OUT
//
//				void				returns
//
//	Retrieve the default values for the colors defined as argument.
//
void		DECL STDC	GetDefaultColor(
									int64_t				model,
									int32_t				* ambient,
									int32_t				* diffuse,
									int32_t				* emissive,
									int32_t				* specular
								);

//
//		GetVertexColor                              (http://rdf.bg/gkdoc/CP64/GetVertexColor.html)
//				int64_t				model								IN
//				const void			* vertexBuffer						IN
//				int64_t				vertexIndex							IN
//				int64_t				requiredColor						IN
//				int64_t				setting								IN
//
//				int32_t				returns								OUT
//
//	Returns vertex color
//	requiredColor is one of the control vertex data bits applied to colors (FORMAT_VERTEX_COLOR...) 
//	If vertex format does provide required color, the model default color will be used
//
int32_t		DECL STDC	GetVertexColor(
									int64_t				model,
									const void			* vertexBuffer,
									int64_t				vertexIndex,
									int64_t				requiredColor,
									int64_t				setting
								);

//
//		CheckConsistency                            (http://rdf.bg/gkdoc/CP64/CheckConsistency.html)
//				int64_t				model								IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	This function returns information about the consistency of each instance.
//
//	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
//
//		bit 0:	Check Design Tree Consistency
//		bit 1:	Check Consistency for Triangle Output (through API)
//		bit 2:	Check Consistency for Line Output (through API)
//		bit 3:	Check Consistency for Point Output (through API)
//		bit 4:	Check Consistency for Generated Surfaces (through API)
//		bit 5:	Check Consistency for Generated Surfaces (internal)
//		bit 6:	Check Consistency for Generated Solids (through API)
//		bit 7:	Check Consistency for Generated Solids (internal)
//		bit 8:	Check Consistency for BoundingBox's
//		bit 9:	Check Consistency for Triangulation
//		bit 10: Check Consistency for Relations (through API)
//
//		bit 16:	Contains (Closed) Solid(s)
//		bit 18:	Contains (Closed) Infinite Solid(s)
//		bit 20:	Contains Closed Surface(s)
//		bit 21:	Contains Open Surface(s)
//		bit 22:	Contains Closed Infinite Surface(s)
//		bit 23:	Contains Open Infinite Surface(s)
//		bit 24:	Contains Closed Line(s)
//		bit 25:	Contains Open Line(s)
//		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
//		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
//		bit 28:	Contains (Closed) Point(s)
//
//	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
//	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected; 
//	any non-zero return value in Contains is an indication that this type of geometry is expected in one of the instances; 
//
int64_t		DECL STDC	CheckConsistency(
									int64_t				model,
									int64_t				mask
								);

//
//		CheckInstanceConsistency                    (http://rdf.bg/gkdoc/CP64/CheckInstanceConsistency.html)
//				int64_t				owlInstance							IN
//				int64_t				mask								IN
//
//				int64_t				returns								OUT
//
//	This function returns information about the consistency of the instance and indirectly referenced instances.
//
//	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
//
//		bit 0:	Check Design Tree Consistency
//		bit 1:	Check Consistency for Triangle Output (through API)
//		bit 2:	Check Consistency for Line Output (through API)
//		bit 3:	Check Consistency for Point Output (through API)
//		bit 4:	Check Consistency for Generated Surfaces (through API)
//		bit 5:	Check Consistency for Generated Surfaces (internal)
//		bit 6:	Check Consistency for Generated Solids (through API)
//		bit 7:	Check Consistency for Generated Solids (internal)
//		bit 8:	Check Consistency for BoundingBox's
//		bit 9:	Check Consistency for Triangulation
//		bit 10: Check Consistency for Relations (through API)
//
//		bit 16:	Contains (Closed) Solid(s)
//		bit 18:	Contains (Closed) Infinite Solid(s)
//		bit 20:	Contains Closed Surface(s)
//		bit 21:	Contains Open Surface(s)
//		bit 22:	Contains Closed Infinite Surface(s)
//		bit 23:	Contains Open Infinite Surface(s)
//		bit 24:	Contains Closed Line(s)
//		bit 25:	Contains Open Line(s)
//		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
//		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
//		bit 28:	Contains (Closed) Point(s)
//
//	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
//	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected regarding the given instance; 
//	any non-zero return value in Contains is an indication that this type of geometry is expected regarding the given instance; 
//
int64_t		DECL STDC	CheckInstanceConsistency(
									int64_t				owlInstance,
									int64_t				mask
								);

//
//		IsDuplicate                                 (http://rdf.bg/gkdoc/CP64/IsDuplicate.html)
//				int64_t				originalOwlInstance					IN
//				int64_t				duplicateOwlInstance				IN
//				double				* duplicateMatrix					IN / OUT
//				double				epsilon								IN
//				bool				checkMaterial						IN
//
//				bool				returns								OUT
//
//	Checks if two geometry representations are (almost) similar except for a transformation matrix.
//
bool		DECL STDC	IsDuplicate(
									int64_t				originalOwlInstance,
									int64_t				duplicateOwlInstance,
									double				* duplicateMatrix,
									double				epsilon,
									bool				checkMaterial
								);

//
//  Derived Geometry API Calls
//

//
//		GetPerimeter                                (http://rdf.bg/gkdoc/CP64/GetPerimeter.html)
//				int64_t				owlInstance							IN
//
//				double				returns								OUT
//
//	This function calculates the perimeter of an instance.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: due to internal structure using already calculated vertex buffer / index buffer does not
//		  give any performance benefits, in opposite to GetVolume and GetArea
//
double		DECL STDC	GetPerimeter(
									int64_t				owlInstance
								);

//
//		GetArea                                     (http://rdf.bg/gkdoc/CP64/GetArea.html)
//				int64_t				owlInstance							IN
//				const void			* vertexBuffer						IN
//				const void			* indexBuffer						IN
//
//				double				returns								OUT
//
//	This function calculates the area of an instance.
//	For perfomance reasons it is benefitial to call it with vertex and index buffer when
//	the arrays are calculated anyway or Volume and Area are needed.
//
//	There are two ways to call GetVolume:
//		vertexBuffer and indexBuffer are both zero: in this case the instance will be
//				recalculated when needed. It is expected the client does not
//				need the arrays itself or there is no performance issue.
//		vertexBuffer and indexBuffer are both given: the call is placed directly after
//				updateBuffer calls and no structural change to depending instances have 
//				been done in between. The transformationMatrix array is not needed,
//				even if it is being used due to not giving any performance gain to this
//				operation.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: in case precision is important and vertex buffer is 32 bit it is advised to
//		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
//
double		DECL STDC	GetArea(
									int64_t				owlInstance,
									const void			* vertexBuffer,
									const void			* indexBuffer
								);

//
//		GetVolume                                   (http://rdf.bg/gkdoc/CP64/GetVolume.html)
//				int64_t				owlInstance							IN
//				const void			* vertexBuffer						IN
//				const void			* indexBuffer						IN
//
//				double				returns								OUT
//
//	This function calculates the volume of an instance.
//	For perfomance reasons it is benefitial to call it with vertex and index buffer when
//	the arrays are calculated anyway or Volume and Area are needed.
//
//	There are two ways to call GetVolume:
//		vertexBuffer and indexBuffer are both zero: in this case the instance will be
//				recalculated when needed. It is expected the client does not
//				need the arrays itself or there is no performance issue.
//		vertexBuffer and indexBuffer are both given: the call is placed directly after
//				updateBuffer calls and no structural change to depending instances have 
//				been done in between. The transformationMatrix array is not needed,
//				even if it is being used due to not giving any performance gain to this
//				operation.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: in case precision is important and vertex buffer is 32 bit it is advised to
//		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
//
double		DECL STDC	GetVolume(
									int64_t				owlInstance,
									const void			* vertexBuffer,
									const void			* indexBuffer
								);

//
//		GetCenter                                   (http://rdf.bg/gkdoc/CP64/GetCenter.html)
//				int64_t				owlInstance							IN
//				const void			* vertexBuffer						IN
//				const void			* indexBuffer						IN
//				double				* center							IN / OUT
//
//				double				* returns							OUT
//
//	This function calculates the center of an instance.
//	For perfomance reasons it is benefitial to call it with vertex and index buffer when
//	the arrays are calculated anyway or Volume and Area are needed.
//
//	There are two ways to call GetCenter:
//		vertexBuffer and indexBuffer are both zero: in this case the instance will be
//				recalculated when needed. It is expected the client does not
//				need the arrays itself or there is no performance issue.
//		vertexBuffer and indexBuffer are both given: the call is placed directly after
//				updateBuffer calls and no structural change to depending instances have 
//				been done in between. The transformationMatrix array is not needed,
//				even if it is being used due to not giving any performance gain to this
//				operation.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: in case precision is important and vertex array is 32 bit it is advised to
//		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
//
double		DECL * STDC	GetCenter(
									int64_t				owlInstance,
									const void			* vertexBuffer,
									const void			* indexBuffer,
									double				* center
								);

//
//		GetCentroid                                 (http://rdf.bg/gkdoc/CP64/GetCentroid.html)
//				int64_t				owlInstance							IN
//				const void			* vertexBuffer						IN
//				const void			* indexBuffer						IN
//				double				* centroid							IN / OUT
//
//				double				returns								OUT
//
//
double		DECL STDC	GetCentroid(
									int64_t				owlInstance,
									const void			* vertexBuffer,
									const void			* indexBuffer,
									double				* centroid
								);

//
//		GetConceptualFacePerimeter                  (http://rdf.bg/gkdoc/CP64/GetConceptualFacePerimeter.html)
//				int64_t				conceptualFace						IN
//
//				double				returns								OUT
//
//	This function returns the perimeter of a given Conceptual Face.
//
double		DECL STDC	GetConceptualFacePerimeter(
									int64_t				conceptualFace
								);

//
//		GetConceptualFaceArea                       (http://rdf.bg/gkdoc/CP64/GetConceptualFaceArea.html)
//				int64_t				conceptualFace						IN
//				const void			* vertexBuffer						IN
//				const void			* indexBuffer						IN
//
//				double				returns								OUT
//
//	This function returns the area of a given Conceptual Face. The attributes vertex buffer
//	and index buffer are optional but will improve performance if defined.
//
double		DECL STDC	GetConceptualFaceArea(
									int64_t				conceptualFace,
									const void			* vertexBuffer,
									const void			* indexBuffer
								);

//
//		SetBoundingBoxReference                     (http://rdf.bg/gkdoc/CP64/SetBoundingBoxReference.html)
//				int64_t				owlInstance							IN
//				const double		* transformationMatrix				IN
//				const double		* startVector						IN
//				const double		* endVector							IN
//
//				void				returns
//
//	This function passes addresses from the hosting application. This enables
//	the engine to update these values without extra need for API calls. This is
//	especially of interest because the hosting application is not aware of what
//	instances are updated and 
//	The transformationMatrix has 12 double values: _11, _12, _13, _21, _22, _23, 
//	_31, _32, _33, _41, _42, _43.
//	The startVector is the leftundernear vector and the endVector is the 
//	rightupperfar vector, in all cases values are doubles (64 bit).
//
void		DECL STDC	SetBoundingBoxReference(
									int64_t				owlInstance,
									const double		* transformationMatrix,
									const double		* startVector,
									const double		* endVector
								);

//
//		GetBoundingBox                              (http://rdf.bg/gkdoc/CP64/GetBoundingBox.html)
//				int64_t				owlInstance							IN
//				double				* transformationMatrix				IN / OUT
//				double				* startVector						IN / OUT
//				double				* endVector							IN / OUT
//
//				bool				returns								OUT
//
//	When the transformationMatrix is given, it will fill an array of 12 double values.
//	When the transformationMatrix is left empty and both startVector and endVector are
//	given the boundingbox without transformation is calculated and returned.
//
bool		DECL STDC	GetBoundingBox(
									int64_t				owlInstance,
									double				* transformationMatrix,
									double				* startVector,
									double				* endVector
								);

//
//		GetRelativeTransformation                   (http://rdf.bg/gkdoc/CP64/GetRelativeTransformation.html)
//				int64_t				owlInstanceHead						IN
//				int64_t				owlInstanceTail						IN
//				double				* transformationMatrix				IN / OUT
//
//				void				returns
//
//	This function returns the relative transformation matrix between two instances, i.e. in practise
//	this means the matrices connected to the Transformation instances in the path in between.
//	The matrix is only given when a unique path through inverse relations can be found,
//	otherwise the identity matrix is returned.
//	owlInstanceHead is allowed to be not defined, i.e. zero.
//
void		DECL STDC	GetRelativeTransformation(
									int64_t				owlInstanceHead,
									int64_t				owlInstanceTail,
									double				* transformationMatrix
								);

//
//		GetDistance                                 (http://rdf.bg/gkdoc/CP64/GetDistance.html)
//				int64_t				firstOwlInstance					IN
//				int64_t				secondOwlInstance					IN
//				double				* pointFirstInstance				IN / OUT
//				double				* pointSecondInstance				IN / OUT
//
//				double				returns								OUT
//
//	This function returns the shortest distance between two instances.
//
double		DECL STDC	GetDistance(
									int64_t				firstOwlInstance,
									int64_t				secondOwlInstance,
									double				* pointFirstInstance,
									double				* pointSecondInstance
								);

//
//  Deprecated API Calls
//

//
//		GetTriangles                                (http://rdf.bg/gkdoc/CP64/GetTriangles___.html)
//				int64_t				owlInstance							IN
//				int64_t				* startIndex						IN / OUT
//				int64_t				* noTriangles						IN / OUT
//				int64_t				* startVertex						IN / OUT
//				int64_t				* firstNotUsedVertex				IN / OUT
//
//				void				returns
//
//	This call is deprecated as it became trivial and will be removed by end of 2020. The result from CalculateInstance exclusively exists of the relevant triangles when
//	SetFormat() is setting bit 8 and unsetting with bit 9, 10, 12 and 13 
//
void		DECL STDC	GetTriangles(
									int64_t				owlInstance,
									int64_t				* startIndex,
									int64_t				* noTriangles,
									int64_t				* startVertex,
									int64_t				* firstNotUsedVertex
								);

//
//		GetLines                                    (http://rdf.bg/gkdoc/CP64/GetLines___.html)
//				int64_t				owlInstance							IN
//				int64_t				* startIndex						IN / OUT
//				int64_t				* noLines							IN / OUT
//				int64_t				* startVertex						IN / OUT
//				int64_t				* firstNotUsedVertex				IN / OUT
//
//				void				returns
//
//	This call is deprecated as it became trivial and will be removed by end of 2020. The result from CalculateInstance exclusively exists of the relevant lines when
//	SetFormat() is setting bit 9 and unsetting with bit 8, 10, 12 and 13 
//
void		DECL STDC	GetLines(
									int64_t				owlInstance,
									int64_t				* startIndex,
									int64_t				* noLines,
									int64_t				* startVertex,
									int64_t				* firstNotUsedVertex
								);

//
//		GetPoints                                   (http://rdf.bg/gkdoc/CP64/GetPoints___.html)
//				int64_t				owlInstance							IN
//				int64_t				* startIndex						IN / OUT
//				int64_t				* noPoints							IN / OUT
//				int64_t				* startVertex						IN / OUT
//				int64_t				* firstNotUsedVertex				IN / OUT
//
//				void				returns
//
//	This call is deprecated as it became trivial and will be removed by end of 2020. The result from CalculateInstance exclusively exists of the relevant points when
//	SetFormat() is setting bit 10 and unsetting with bit 8, 9, 12 and 13 
//
void		DECL STDC	GetPoints(
									int64_t				owlInstance,
									int64_t				* startIndex,
									int64_t				* noPoints,
									int64_t				* startVertex,
									int64_t				* firstNotUsedVertex
								);

//
//		GetPropertyRestrictions                     (http://rdf.bg/gkdoc/CP64/GetPropertyRestrictions___.html)
//				int64_t				owlClass							IN
//				int64_t				rdfProperty							IN
//				int64_t				* minCard							IN / OUT
//				int64_t				* maxCard							IN / OUT
//
//				void				returns
//
//	This call is deprecated and will be removed by end of 2020. Please use the call GetClassPropertyCardinalityRestriction instead,
//
void		DECL STDC	GetPropertyRestrictions(
									int64_t				owlClass,
									int64_t				rdfProperty,
									int64_t				* minCard,
									int64_t				* maxCard
								);

//
//		GetPropertyRestrictionsConsolidated         (http://rdf.bg/gkdoc/CP64/GetPropertyRestrictionsConsolidated___.html)
//				int64_t				owlClass							IN
//				int64_t				rdfProperty							IN
//				int64_t				* minCard							IN / OUT
//				int64_t				* maxCard							IN / OUT
//
//				void				returns
//
//	This call is deprecated and will be removed by end of 2020. Please use the call GetClassPropertyCardinalityRestriction instead,
//	just rename the function name.
//
void		DECL STDC	GetPropertyRestrictionsConsolidated(
									int64_t				owlClass,
									int64_t				rdfProperty,
									int64_t				* minCard,
									int64_t				* maxCard
								);

//
//		IsGeometryType                              (http://rdf.bg/gkdoc/CP64/IsGeometryType___.html)
//				int64_t				owlClass							IN
//
//				bool				returns								OUT
//
//	This call is deprecated and will be removed by end of 2020. Please use the call GetGeometryClass instead, rename the function name
//	and interpret non-zero as true and zero as false.
//
bool		DECL STDC	IsGeometryType(
									int64_t				owlClass
								);

//
//		SetObjectTypeProperty                       (http://rdf.bg/gkdoc/CP64/SetObjectTypeProperty___.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				const int64_t		* values							IN
//				int64_t				card								IN
//
//				int64_t				returns								OUT
//
//	This call is deprecated and will be removed by end of 2020. Please use the call SetObjectProperty instead, just rename the function name.
//
int64_t		DECL STDC	SetObjectTypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const int64_t		* values,
									int64_t				card
								);

//
//		GetObjectTypeProperty                       (http://rdf.bg/gkdoc/CP64/GetObjectTypeProperty___.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				int64_t				** values							IN / OUT
//				int64_t				* card								IN / OUT
//
//				int64_t				returns								OUT
//
//	This call is deprecated and will be removed by end of 2020. Please use the call GetObjectProperty instead, just rename the function name.
//
int64_t		DECL STDC	GetObjectTypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									int64_t				** values,
									int64_t				* card
								);

//
//		SetDataTypeProperty                         (http://rdf.bg/gkdoc/CP64/SetDataTypeProperty___.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				const void			* values							IN
//				int64_t				card								IN
//
//				int64_t				returns								OUT
//
//	This call is deprecated and will be removed by end of 2020. Please use the call SetDatatypeProperty instead, just rename the function name.
//
int64_t		DECL STDC	SetDataTypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									const void			* values,
									int64_t				card
								);

//
//		GetDataTypeProperty                         (http://rdf.bg/gkdoc/CP64/GetDataTypeProperty___.html)
//				int64_t				owlInstance							IN
//				int64_t				rdfProperty							IN
//				void				** values							IN / OUT
//				int64_t				* card								IN / OUT
//
//				int64_t				returns								OUT
//
//	This call is deprecated and will be removed by end of 2020. Please use the call GetDatatypeProperty instead, just rename the function name.
//
int64_t		DECL STDC	GetDataTypeProperty(
									int64_t				owlInstance,
									int64_t				rdfProperty,
									void				** values,
									int64_t				* card
								);

//
//		InstanceCopyCreated                         (http://rdf.bg/gkdoc/CP64/InstanceCopyCreated___.html)
//				int64_t				owlInstance							IN
//
//				void				returns
//
//	This call is deprecated as the Copy concept is also deprecated and will be removed by end of 2020.
//
void		DECL STDC	InstanceCopyCreated(
									int64_t				owlInstance
								);

//
//		GetPropertyByNameAndType                    (http://rdf.bg/gkdoc/CP64/GetPropertyByNameAndType___.html)
//				int64_t				model								IN
//				const char			* name								IN
//				int64_t				rdfPropertyType						IN
//
//				int64_t				returns								OUT
//
//	This call is deprecated and will be removed by end of 2020.
//	Please use the call GetPropertyByName(Ex) / GetPropertyByNameW(Ex) + GetPropertyType(Ex) instead, just rename the function name.
//
int64_t		DECL STDC	GetPropertyByNameAndType(
									int64_t				model,
									const char			* name,
									int64_t				rdfPropertyType
								);

//
//		GetParentsByIterator                        (http://rdf.bg/gkdoc/CP64/GetParentsByIterator___.html)
//				int64_t				owlClassOrRdfProperty				IN
//				int64_t				parentOwlClassOrRdfProperty			IN
//
//				int64_t				returns								OUT
//
//	Returns the next parent of the class or property.
//	If input parent is zero, the handle will point to the first relevant parent.
//	If all parent are past (or no relevant parent are found), the function will return 0.
//
int64_t		DECL STDC	GetParentsByIterator(
									int64_t				owlClassOrRdfProperty,
									int64_t				parentOwlClassOrRdfProperty
								);


#ifdef __cplusplus
	}
#endif


#undef DECL
#undef STDC
#endif
