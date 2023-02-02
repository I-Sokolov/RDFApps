using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Linq;

namespace RDF
{
	///	<summary>
	///	ENGINE FORMAT SPECIFICATION
	///	</summary>
	public class FORMAT
	{
		//	control precision / data size
		///	<summary></summary>
		public const Int64 SIZE_VERTEX_DOUBLE = (1<<2);      //	Vertex items returned as double (8 byte/64 bit) else - as float (4 byte/32 bit)
		///	<summary></summary>
		public const Int64 SIZE_INDEX_INT64 = (1<<3);        //	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode) - else as int32_t (4 byte/32 bit)

		//	control vertex data
		///	<summary>Vertex contains 3D point info</summary>
		public const Int64 VERTEX_POINT = (1<<4); 
		///	<summary>Vertex contains normal info</summary>
		public const Int64 VERTEX_NORMAL = (1<<5); 
		///	<summary>Vertex contains first 2D texture info</summary>
		public const Int64 VERTEX_TEXTURE_UV = (1<<6); 
		///	<summary>Vertex does contain tangent vector for first texture</summary>
		public const Int64 VERTEX_TEXTURE_TANGENT = (1<<28); 
		///	<summary>Vertex does contain binormal vector for first texture</summary>
		public const Int64 VERTEX_TEXTURE_BINORMAL = (1<<29); 
		///	<summary>Vertex contains second 2D texture info</summary>
		public const Int64 VERTEX_TEXTURE2_UV = (1<<7); 
		///	<summary>Vertex does contain tangent vector for second texture (only 64 bit platform)</summary>
		public const Int64 VERTEX_TEXTURE2_TANGENT = (1<<30); 
		///	<summary>Vertex does contain binormal vector for second texture (only 64 bit platform)</summary>
		public const Int64 VERTEX_TEXTURE2_BINORMAL = (1<<31); 
		///	<summary>Vertex does contain Ambient color information</summary>
		public const Int64 VERTEX_COLOR_AMBIENT = (1<<24); 
		///	<summary>Vertex does contain Diffuse color information</summary>
		public const Int64 VERTEX_COLOR_DIFFUSE = (1<<25); 
		///	<summary>Vertex does contain Emissive color information</summary>
		public const Int64 VERTEX_COLOR_EMISSIVE = (1<<26); 
		///	<summary>Vertex does contain Specular color information</summary>
		public const Int64 VERTEX_COLOR_SPECULAR = (1<<27); 
		//	control CalculateInstance behaviour
		///	<summary>Object form triangles are exported (effective if instance contains faces and/or solids)(triangulated surface representation)</summary>
		public const Int64 EXPORT_TRIANGLES = (1<<8);
		///	<summary>Object polygon lines are exported (effective if instance contains line representations)</summary>
		public const Int64 EXPORT_LINES = (1<<9);
		///	<summary>Object points are exported (effective if instance contains point representations)</summary>
		public const Int64 EXPORT_POINTS = (1<<10); 
		///	<summary>Object face polygon lines (dense wireframe) are exported (requires FORMAT_FLAG_CONTAINS_TRIANGLES)</summary>
		public const Int64 EXPORT_FACE_POLYGONS = (1<<12); 
		///	<summary>Object conceptual face polygon lines (wireframe) are exported </summary>
		public const Int64 EXPORT_CONCEPTUAL_FACE_POLYGONS = (1<<13); 
		///	<summary>Polygon lines (wireframe) exported as tuples (edges) - else as list (loop)</summary>
		public const Int64 EXPORT_POLYGONS_AS_TUPLES = (1<<14); 
		///	<summary>Normals are exported to be in line with the original semantic form description (orthogonal to conceprual face, could be non orthogonal to the planar face or triangle) - else all normals of triangles are transformed orthogonal to the palnar face or triangle they belong to</summary>
		public const Int64 EXPORT_ADVANCED_NORMALS = (1<<15); 
		///	<summary>Where possible DirectX compatibility is given to exported data. Unsets FORMAT_FLAG_OPENGL, FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010</summary>
		public const Int64 EXPORT_DIRECTX = (1<<16); 
		///	<summary>Where possible OpenGL compatibility is given to exported data. Unsets FORMAT_FLAG_DIRECTX. Sets FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010</summary>
		public const Int64 EXPORT_OPENGL = (1<<17); 
		///	<summary>Every face has exactly one opposite face (normally both index and vertex array are doubled in size)</summary>
		public const Int64 EXPORT_DOUBLE_SIDED = (1<<18); 
		///	<summary>Opposite Triangle Rotation (RHS as expected by OpenGL) - else  Standard Triangle Rotation (LHS as expected by DirectX)</summary>
		public const Int64 EXPORT_VERSION_0001 = (1<<20); 
		///	<summary>X, Y, Z (nX, nY, nZ) formatted as , i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL) - else X, Y, Z (nX, nY, nZ) formatted as considering internal concepts</summary>
		public const Int64 EXPORT_VERSION_0010 = (1<<21); 
		public const Int64 EXPORT_VERSION_0100 = (1<<22); 
		public const Int64 EXPORT_VERSION_1000 = (1<<23); 
	}

	///	<summary>
	///	Color pack/unpack conversion methods
	///	</summary>
	public class COLOR
	{
		///	<summary> get color from its components in range 0..255 </summary>
		public static UInt32 RGBW255(byte r, byte g, byte b, byte w) { return (UInt32)r << 24 | (UInt32)g << 16 | (UInt32)b << 8 | (UInt32)w; }

		///	<summary>get color from its components in range 0..1</summary>
		public static UInt32 RGBW(double r, double g, double b, double w) { return RGBW255((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(w * 255)); }

		///	<summary>get color red component in range 0..255</summary>
		public static byte GET_R255(UInt32 clr) { return (byte)((clr >> 24) & 0xFF); }

		///	<summary>get color green component in range 0..255</summary>
		public static byte GET_G255(UInt32 clr) { return (byte)((clr >> 16) & 0xFF); }

		///	<summary>get color blue component in range 0..255</summary>
		public static byte GET_B255(UInt32 clr) { return (byte)((clr >> 8) & 0xFF); }

		///	<summary>get color transparency in range 0..255</summary>
		public static byte GET_W255(UInt32 clr) { return (byte)(clr & 0xFF); }

		///	<summary>get color red component in range 0..1</summary>
		public static double GET_R(UInt32 clr) { return GET_R255(clr) / 255.0; }

		///	<summary>get color green component in range 0..1</summary>
		public static double GET_G(UInt32 clr) { return GET_G255(clr) / 255.0; }

		///	<summary>get color blue component in range 0..1</summary>
		public static double GET_B(UInt32 clr) { return GET_B255(clr) / 255.0; }

		///	<summary>get color trancparency in range 0..1</summary>
		public static double GET_W(UInt32 clr) { return GET_W255(clr) / 255.0; }

		///	<summary>get color from array of 4 components in range 0..255</summary>
		public static UInt32 RGBW255(byte[] r) { return RGBW255(r[0], r[1], r[2], r[3]); }

		///	<summary>get color from array of 4 components in range 0..1</summary>
		public static UInt32 RGBW(double[] r) { return RGBW(r[0], r[1], r[2], r[3]); }

		///	<summary>get color comonents in range 0..255 to arry of 4 elements</summary>
		public static byte[] GET_COMPONENTS255(UInt32 clr)
		{
			var	r = new byte[4]; 
			r[0] = GET_R255(clr); 
			r[1] = GET_G255(clr); 
			r[2] = GET_B255(clr); 
			r[3] = GET_W255(clr); 
			return	r; 
		}

		///	<summary>get color comonents in range 0..1 to arry of 4 elements</summary>
		public static double[] GET_COMPONENTS(UInt32 clr)
		{
			var	r = new double[4]; 
			r[0] = GET_R(clr); 
			r[1] = GET_G(clr); 
			r[2] = GET_B(clr); 
			r[3] = GET_W(clr); 
			return	r;
		}
	}//COLOR

    class engine
    {
        public const Int64 OBJECTPROPERTY_TYPE             = 1;
        public const Int64 DATATYPEPROPERTY_TYPE_BOOLEAN   = 2;
        public const Int64 DATATYPEPROPERTY_TYPE_CHAR      = 3;
        public const Int64 DATATYPEPROPERTY_TYPE_INTEGER   = 4;
        public const Int64 DATATYPEPROPERTY_TYPE_DOUBLE    = 5;
        public const Int64 DATATYPEPROPERTY_TYPE_BYTE      = 6;

        public const string enginedll = @"engine.dll";

        //
        //  Meta information API Calls
        //

		/// <summary>
		///		GetRevision                                 (http://rdf.bg/gkdoc/CS64/GetRevision.html)
		///
		///	Returns the revision number.
		///	The timeStamp is generated by the SVN system used during development.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRevision")]
		public static extern Int64 GetRevision(out IntPtr timeStamp);

		public static Int64 GetRevision()
		{
			IntPtr timeStamp = IntPtr.Zero;
			return RDF.engine.GetRevision(out timeStamp);
		}

		/// <summary>
		///		GetRevisionW                                (http://rdf.bg/gkdoc/CS64/GetRevisionW.html)
		///
		///	Returns the revision number.
		///	The timeStamp is generated by the SVN system used during development.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRevisionW")]
		public static extern Int64 GetRevisionW(out IntPtr timeStamp);

		public static Int64 GetRevisionW()
		{
			IntPtr timeStamp = IntPtr.Zero;
			return RDF.engine.GetRevisionW(out timeStamp);
		}

		/// <summary>
		///		GetProtection                               (http://rdf.bg/gkdoc/CS64/GetProtection.html)
		///
		///	This call is required to be called to enable the DLL to work if protection is active.
		///
		///	Returns the number of days (incl. this one) that this version is still active or 0 if no protection is embedded.
		///	In case no days are left and protection is active this call will return -1.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetProtection")]
		public static extern Int64 GetProtection();

		/// <summary>
		///		GetEnvironment                              (http://rdf.bg/gkdoc/CS64/GetEnvironment.html)
		///
		///	Returns the revision number similar to the call GetRevision.
		///	The environment variables will show known environment variables
		///	and if they are set, for example environment variables ABC known
		///	and unset and DEF as well as GHI known and set:
		///		environmentVariables = "ABC:F;DEF:T;GHI:T"
		///	Development variables are depending on the build environment
		///	As an example in windows systems where Visual Studio is used:
		///		developmentVariables = "...."
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetEnvironment")]
		public static extern Int64 GetEnvironment(out IntPtr environmentVariables, out IntPtr developmentVariables);

		/// <summary>
		///		GetEnvironmentW                             (http://rdf.bg/gkdoc/CS64/GetEnvironmentW.html)
		///
		///	Returns the revision number similar to the call GetRevision[W].
		///	The environment variables will show known environment variables
		///	and if they are set, for example environment variables ABC known
		///	and unset and DEF as well as GHI known and set:
		///		environmentVariables = "ABC:F;DEF:T;GHI:T"
		///	Development variables are depending on the build environment
		///	As an example in windows systems where Visual Studio is used:
		///		developmentVariables = "...."
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetEnvironmentW")]
		public static extern Int64 GetEnvironmentW(out IntPtr environmentVariables, out IntPtr developmentVariables);

		/// <summary>
		///		SetAssertionFile                            (http://rdf.bg/gkdoc/CS64/SetAssertionFile.html)
		///
		///	This function sets the file location where internal assertions should be written to.
		///	If the file name is not set (default) many internal control procedures are not executed
		///	and the code will be faster.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetAssertionFile")]
		public static extern void SetAssertionFile(string fileName);

		[DllImport(enginedll, EntryPoint = "SetAssertionFile")]
		public static extern void SetAssertionFile(byte[] fileName);

		/// <summary>
		///		SetAssertionFileW                           (http://rdf.bg/gkdoc/CS64/SetAssertionFileW.html)
		///
		///	This function sets the file location where internal assertions should be written to.
		///	If the file name is not set (default) many internal control procedures are not executed
		///	and the code will be faster.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetAssertionFileW")]
		public static extern void SetAssertionFileW(string fileName);

		[DllImport(enginedll, EntryPoint = "SetAssertionFileW")]
		public static extern void SetAssertionFileW(byte[] fileName);

		/// <summary>
		///		GetAssertionFile                            (http://rdf.bg/gkdoc/CS64/GetAssertionFile.html)
		///
		///	This function gets the file location as stored/set internally where internal assertions should be written to.
		///	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetAssertionFile")]
		public static extern void GetAssertionFile(out IntPtr fileName);

		public static string GetAssertionFile()
		{
			IntPtr fileName = IntPtr.Zero;
			RDF.engine.GetAssertionFile(out fileName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(fileName);
		}

		/// <summary>
		///		GetAssertionFileW                           (http://rdf.bg/gkdoc/CS64/GetAssertionFileW.html)
		///
		///	This function gets the file location as stored/set internally where internal assertions should be written to.
		///	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetAssertionFileW")]
		public static extern void GetAssertionFileW(out IntPtr fileName);

		public static string GetAssertionFileW()
		{
			IntPtr fileName = IntPtr.Zero;
			RDF.engine.GetAssertionFileW(out fileName);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(fileName);
		}

		/// <summary>
		///		SetCharacterSerialization                   (http://rdf.bg/gkdoc/CS64/SetCharacterSerialization.html)
		///
		///	This call defines how characters for names, strings will be serializaed and how
		///	they are expected to be serialized. An exception are the Open / Import / Save calls,
		///	these calls have a fixed way of serialization of path / file names.
		///
		///	If the encoding value is non-zero the following values are possible (if zero encoding is kept as defined)
		///		 32 [default]	encoding ignored
		///		 64				encoding Windows 1250
		///		 65				encoding Windows 1251
		///		 66				encoding Windows 1252
		///		 67				encoding Windows 1253
		///		 68				encoding Windows 1254
		///		 69				encoding Windows 1255
		///		 70				encoding Windows 1256
		///		 71				encoding Windows 1257
		///		 72				encoding Windows 1258
		///		128				encoding ISO8859 1
		///		129				encoding ISO8859 2
		///		130				encoding ISO8859 3
		///		131				encoding ISO8859 4
		///		132				encoding ISO8859 5
		///		133				encoding ISO8859 6
		///		134				encoding ISO8859 7
		///		135				encoding ISO8859 8
		///		136				encoding ISO8859 9
		///		137				encoding ISO8859 10
		///		138				encoding ISO8859 11
		///						encoding ISO8859 12 => does not exist
		///		140				encoding ISO8859 13
		///		141				encoding ISO8859 14
		///		142				encoding ISO8859 15
		///		143				encoding ISO8859 16
		///		160				encoding MACINTOSH CENTRAL EUROPEAN
		///		192				encoding SHIFT JIS X 213
		///
		///	The wcharBitSizeOverride value overrides the OS based size of wchar_t, the following values can be applied:
		///		0			wcharBitSizeOverride is ignored, override is not changed
		///		16			wchar_t interpreted as being 2 bytes wide (size of wchar_t in bits)
		///		32			wchar_t interpreted as being 4 bytes wide (size of wchar_t in bits)
		///		Any other value will reset the override and wchar_t will follow the OS based size of wchar_t
		///	Note: this setting is independent from the model, this call can also be called without a model defined.
		///
		///	The ascii value defines
		///		true [default]	8 bit serializatiom (size of char returned in bits)
		///		false			16/32 bit serialization (depending on the operating system, i.e. sizeof of wchar_t returned in number of bits)
		///	Note: this setting is model-dependent and requires a model present to have any effect.
		///
		///	The return value is the size of a single character in bits, i.e. 1 byte is 8 bits, the value for a wchar_t can be 16 or 32 depending on settings and operating system
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetCharacterSerialization")]
		public static extern Int64 SetCharacterSerialization(Int64 model, Int64 encoding, Int64 wcharBitSizeOverride, byte ascii);

		/// <summary>
		///		GetCharacterSerialization                   (http://rdf.bg/gkdoc/CS64/GetCharacterSerialization.html)
		///
		///	This call retrieves the values as set by 
		///
		///	The returns the size of a single character in bits, i.e. 1 byte is 8 bits, this can be 8, 16 or 32 depending on settings and operating system
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetCharacterSerialization")]
		public static extern Int64 GetCharacterSerialization(Int64 model, out Int64 encoding, out byte ascii);

		/// <summary>
		///		SetModellingStyle                           (http://rdf.bg/gkdoc/CS64/SetModellingStyle.html)
		///
		///	This call sets the modelling style.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetModellingStyle")]
		public static extern void SetModellingStyle(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetModellingStyle                           (http://rdf.bg/gkdoc/CS64/GetModellingStyle.html)
		///
		///	This call gets the modelling style.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetModellingStyle")]
		public static extern Int64 GetModellingStyle(Int64 model, Int64 mask);

		/// <summary>
		///		AbortModel                                  (http://rdf.bg/gkdoc/CS64/AbortModel.html)
		///
		///	This function abort running processes for a model. It can be used when a task takes more time than
		///	expected / available, or in case the requested results are not relevant anymore.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "AbortModel")]
		public static extern Int64 AbortModel(Int64 model, Int64 setting);

		/// <summary>
		///		GetSessionMetaInfo                          (http://rdf.bg/gkdoc/CS64/GetSessionMetaInfo.html)
		///
		///	This function is meant for debugging purposes and return statistics during processing.
		///	The return value represents the number of active models within the session (or zero if the model was not recognized).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetSessionMetaInfo")]
		public static extern Int64 GetSessionMetaInfo(out Int64 allocatedBlocks, out Int64 allocatedBytes, out Int64 nonUsedBlocks, out Int64 nonUsedBytes);

		/// <summary>
		///		GetModelMetaInfo                            (http://rdf.bg/gkdoc/CS64/GetModelMetaInfo.html)
		///
		///	This function is meant for debugging purposes and return statistics during processing.
		///	The return value represents the number of active models within the session (or zero if the model was not recognized).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, out Int64 activeProperties, out Int64 removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, out Int64 activeProperties, out Int64 removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, IntPtr activeProperties, IntPtr removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, IntPtr activeProperties, IntPtr removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, out Int64 activeProperties, out Int64 removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, out Int64 activeProperties, out Int64 removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, IntPtr activeProperties, IntPtr removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, IntPtr activeProperties, IntPtr removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		/// <summary>
		///		GetInstanceMetaInfo                         (http://rdf.bg/gkdoc/CS64/GetInstanceMetaInfo.html)
		///
		///	This function is meant for debugging purposes and return statistics during processing.
		///	The return value represents the number of active instances within the model (or zero if the instance was not recognized).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, out Int64 allocatedBlocks, out Int64 allocatedBytes);

		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, out Int64 allocatedBlocks, IntPtr allocatedBytes);

		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, IntPtr allocatedBlocks, out Int64 allocatedBytes);

		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, IntPtr allocatedBlocks, IntPtr allocatedBytes);

		/// <summary>
		///		GetSmoothness                               (http://rdf.bg/gkdoc/CS64/GetSmoothness.html)
		///
		///	This function returns the smoothness of a line or surface.
		///	In case the smoothness can be defined the degree will get assigned either
		///		0 - continuous curve / surface (i.e. degree 9)
		///		1 - the direction of the curve / surface is gradually changing (i.e. degree 1)
		///		2 - the change of direction of the curve / surface is gradually changing (i.e. degree 2)
		///	In return value of this function retuns the dimension of the found smoothness:
		///		0 - smoothness could not be defined
		///		1 - found the smoothness of a curve
		///		2 - found the smoothness of a surface
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetSmoothness")]
		public static extern Int64 GetSmoothness(Int64 owlInstance, out Int64 degree);

		/// <summary>
		///		AddState                                    (http://rdf.bg/gkdoc/CS64/AddState.html)
		///
		///	This call will integrate the current state information into the model.
		///
		///	Model should be non-zero.
		///
		///	If owlInstance is given the state is only applied on the owlInstance and all its children.
		///	If owlInstance is zero the state is applied on all owlInstances within a model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "AddState")]
		public static extern void AddState(Int64 model, Int64 owlInstance);

		/// <summary>
		///		GetModel                                    (http://rdf.bg/gkdoc/CS64/GetModel.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetModel")]
		public static extern Int64 GetModel(Int64 owlInstance);

		/// <summary>
		///		OrderedHandles                              (http://rdf.bg/gkdoc/CS64/OrderedHandles.html)
		///
		///	This call can be used in two ways. The optional arguments classCnt,
		///	propertyCnt and instanceCnt can be used to get the total amount of active classes,
		///	properies and instances available within the model.
		///
		///	The setting and mask can be used to order the handles given for classes,
		///	properties and instances.
		///		1 - if set this will number all classes with possible values [1 .. classCnt]
		///		2 - if set this will number all classes with possible values [1 .. propertyCnt]
		///		4 - if set this will number all classes with possible values [1 .. instanceCnt]
		///
		///	Note: when enabling ordered handles be aware that classes, properties and instances
		///		  can share the same handles, using the correct argument cannot be checked anymore
		///		  by the library itself. This could result in crashes in case of incorrect assignments
		///		  by the hosting application.
		///	Note: internally there is no performance gain / loss. This is purely meant for situations
		///		  where the hosting application can benefit performance wise from having an ordered list.
		///	Note: use in combination with other libraries is not adviced, i.e. when combined with the
		///		  IFC generation from the IFC Engine component for example
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, out Int64 propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, out Int64 propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, IntPtr propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, IntPtr propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, out Int64 propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, out Int64 propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, IntPtr propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, IntPtr propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		/// <summary>
		///		PeelArray                                   (http://rdf.bg/gkdoc/CS64/PeelArray.html)
		///
		///	This function introduces functionality that is missing or complicated in some programming languages.
		///	The attribute inValue is a reference to an array of references. The attribute outValue is a reference to the same array,
		///	however a number of elements earlier or further, i.e. number of elements being attribute elementSize. Be aware that as
		///	we are talking about references the offset is depending on 32 bit / 64 bit compilation.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "PeelArray")]
		public static extern void PeelArray(ref byte[] inValue, out byte outValue, Int64 elementSize);

		/// <summary>
		///		SetInternalCheck                            (http://rdf.bg/gkdoc/CS64/SetInternalCheck.html)
		///
		///	This function allows to enable or disable several active consistency checks. Enabling the checks can 
		///	introduce performance effects; it is helpfull for and meant for debugging on client side.
		///	If model is zero the consistency checks are set for all open and to be created models.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetInternalCheck")]
		public static extern void SetInternalCheck(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetInternalCheck                            (http://rdf.bg/gkdoc/CS64/GetInternalCheck.html)
		///
		///	This function returns all current enabled active consistency checks given the mask the function is 
		///	called for.
		///	When leaving mask and settinbg zero it will return all bits that can be set.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheck")]
		public static extern Int64 GetInternalCheck(Int64 model, Int64 mask);

		/// <summary>
		///		GetInternalCheckIssueCnt                    (http://rdf.bg/gkdoc/CS64/GetInternalCheckIssueCnt.html)
		///
		///	This function returns all issues found and not retrieved by the hosting application through 
		///	GetInternalCheckIssue() / GetInternalCheckIssueW().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheckIssueCnt")]
		public static extern Int64 GetInternalCheckIssueCnt(Int64 model);

		/// <summary>
		///		GetInternalCheckIssue                       (http://rdf.bg/gkdoc/CS64/GetInternalCheckIssue.html)
		///
		///	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
		///	The name and description represent the issue as ASCII string, if relevant the relating owlInstance
		///	will be returned through relatedOwlInstance.
		///	Namer, Description and relatedOwlInstance are optional.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheckIssue")]
		public static extern void GetInternalCheckIssue(Int64 model, out IntPtr name, out IntPtr description, out Int64 relatedOwlInstance);

		/// <summary>
		///		GetInternalCheckIssueW                      (http://rdf.bg/gkdoc/CS64/GetInternalCheckIssueW.html)
		///
		///	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
		///	The name and description represent the issue as Unicode string, if relevant the relating owlInstance
		///	will be returned through relatedOwlInstance.
		///	Namer, Description and relatedOwlInstance are optional.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheckIssueW")]
		public static extern void GetInternalCheckIssueW(Int64 model, out IntPtr name, out IntPtr description, out Int64 relatedOwlInstance);

		/// <summary>
		///		CloseSession                                (http://rdf.bg/gkdoc/CS64/CloseSession.html)
		///
		///	This function closes the session, after this call the geometry kernel cannot be used anymore.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CloseSession")]
		public static extern Int64 CloseSession();

		/// <summary>
		///		CleanMemory                                 (http://rdf.bg/gkdoc/CS64/CleanMemory.html)
		///
		///		This function ..
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CleanMemory")]
		public static extern void CleanMemory();

		/// <summary>
		///		ClearCache                                  (http://rdf.bg/gkdoc/CS64/ClearCache.html)
		///
		///		This function ..
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ClearCache")]
		public static extern void ClearCache(Int64 model);

        //
        //  File IO / Stream / Copy API Calls
        //

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate Int64 ReadCallBackFunction(IntPtr value);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void WriteCallBackFunction(IntPtr value, Int64 size);

		/// <summary>
		///		CreateModel                                 (http://rdf.bg/gkdoc/CS64/CreateModel.html)
		///
		///	This function creates and empty model.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateModel")]
		public static extern Int64 CreateModel();

		/// <summary>
		///		OpenModel                                   (http://rdf.bg/gkdoc/CS64/OpenModel.html)
		///
		///	This function opens the model on location file name.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModel")]
		public static extern Int64 OpenModel(string fileName);

		[DllImport(enginedll, EntryPoint = "OpenModel")]
		public static extern Int64 OpenModel(byte[] fileName);

		/// <summary>
		///		OpenModelW                                  (http://rdf.bg/gkdoc/CS64/OpenModelW.html)
		///
		///	This function opens the model on location file name.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModelW")]
		public static extern Int64 OpenModelW(string fileName);

		[DllImport(enginedll, EntryPoint = "OpenModelW")]
		public static extern Int64 OpenModelW(byte[] fileName);

		/// <summary>
		///		OpenModelS                                  (http://rdf.bg/gkdoc/CS64/OpenModelS.html)
		///
		///	This function opens the model via a stream.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModelS")]
		public static extern Int64 OpenModelS([MarshalAs(UnmanagedType.FunctionPtr)] ReadCallBackFunction callback);

		/// <summary>
		///		OpenModelA                                  (http://rdf.bg/gkdoc/CS64/OpenModelA.html)
		///
		///	This function opens the model via an array.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModelA")]
		public static extern Int64 OpenModelA(byte[] content, Int64 size);

		/// <summary>
		///		ImportModel                                 (http://rdf.bg/gkdoc/CS64/ImportModel.html)
		///
		///	This function imports a design tree on location file name.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModel")]
		public static extern Int64 ImportModel(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "ImportModel")]
		public static extern Int64 ImportModel(Int64 model, byte[] fileName);

		/// <summary>
		///		ImportModelW                                (http://rdf.bg/gkdoc/CS64/ImportModelW.html)
		///
		///	This function imports a design tree on location file name.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModelW")]
		public static extern Int64 ImportModelW(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "ImportModelW")]
		public static extern Int64 ImportModelW(Int64 model, byte[] fileName);

		/// <summary>
		///		ImportModelS                                (http://rdf.bg/gkdoc/CS64/ImportModelS.html)
		///
		///	This function imports a design tree via a stream.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModelS")]
		public static extern Int64 ImportModelS(Int64 model, [MarshalAs(UnmanagedType.FunctionPtr)] ReadCallBackFunction callback);

		/// <summary>
		///		ImportModelA                                (http://rdf.bg/gkdoc/CS64/ImportModelA.html)
		///
		///	This function imports a design tree via an array.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModelA")]
		public static extern Int64 ImportModelA(Int64 model, byte[] content, Int64 size);

		/// <summary>
		///		SaveInstanceTree                            (http://rdf.bg/gkdoc/CS64/SaveInstanceTree.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTree")]
		public static extern Int64 SaveInstanceTree(Int64 owlInstance, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceTree")]
		public static extern Int64 SaveInstanceTree(Int64 owlInstance, byte[] fileName);

		/// <summary>
		///		SaveInstanceTreeW                           (http://rdf.bg/gkdoc/CS64/SaveInstanceTreeW.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeW")]
		public static extern Int64 SaveInstanceTreeW(Int64 owlInstance, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeW")]
		public static extern Int64 SaveInstanceTreeW(Int64 owlInstance, byte[] fileName);

		/// <summary>
		///		SaveInstanceTreeS                           (http://rdf.bg/gkdoc/CS64/SaveInstanceTreeS.html)
		///
		///	This function saves the selected instance and its dependancies in a stream.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeS")]
		public static extern Int64 SaveInstanceTreeS(Int64 owlInstance, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, Int64 size);

		/// <summary>
		///		SaveInstanceTreeA                           (http://rdf.bg/gkdoc/CS64/SaveInstanceTreeA.html)
		///
		///	This function saves the selected instance and its dependancies in an array.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeA")]
		public static extern Int64 SaveInstanceTreeA(Int64 owlInstance, byte[] content, out Int64 size);

		/// <summary>
		///		SaveInstanceNetwork                         (http://rdf.bg/gkdoc/CS64/SaveInstanceNetwork.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetwork")]
		public static extern Int64 SaveInstanceNetwork(Int64 owlInstance, byte includeInverseRelations, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceNetwork")]
		public static extern Int64 SaveInstanceNetwork(Int64 owlInstance, byte includeInverseRelations, byte[] fileName);

		/// <summary>
		///		SaveInstanceNetworkW                        (http://rdf.bg/gkdoc/CS64/SaveInstanceNetworkW.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkW")]
		public static extern Int64 SaveInstanceNetworkW(Int64 owlInstance, byte includeInverseRelations, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkW")]
		public static extern Int64 SaveInstanceNetworkW(Int64 owlInstance, byte includeInverseRelations, byte[] fileName);

		/// <summary>
		///		SaveInstanceNetworkS                        (http://rdf.bg/gkdoc/CS64/SaveInstanceNetworkS.html)
		///
		///	This function saves the selected instance and its dependancies in a stream.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkS")]
		public static extern Int64 SaveInstanceNetworkS(Int64 owlInstance, byte includeInverseRelations, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, Int64 size);

		/// <summary>
		///		SaveInstanceNetworkA                        (http://rdf.bg/gkdoc/CS64/SaveInstanceNetworkA.html)
		///
		///	This function saves the selected instance and its dependancies in an array.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkA")]
		public static extern Int64 SaveInstanceNetworkA(Int64 owlInstance, byte includeInverseRelations, byte[] content, out Int64 size);

		/// <summary>
		///		SaveModel                                   (http://rdf.bg/gkdoc/CS64/SaveModel.html)
		///
		///	This function saves the current model on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModel")]
		public static extern Int64 SaveModel(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveModel")]
		public static extern Int64 SaveModel(Int64 model, byte[] fileName);

		/// <summary>
		///		SaveModelW                                  (http://rdf.bg/gkdoc/CS64/SaveModelW.html)
		///
		///	This function saves the current model on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModelW")]
		public static extern Int64 SaveModelW(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveModelW")]
		public static extern Int64 SaveModelW(Int64 model, byte[] fileName);

		/// <summary>
		///		SaveModelS                                  (http://rdf.bg/gkdoc/CS64/SaveModelS.html)
		///
		///	This function saves the current model in a stream.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModelS")]
		public static extern Int64 SaveModelS(Int64 model, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, Int64 size);

		/// <summary>
		///		SaveModelA                                  (http://rdf.bg/gkdoc/CS64/SaveModelA.html)
		///
		///	This function saves the current model in an array.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModelA")]
		public static extern Int64 SaveModelA(Int64 model, byte[] content, out Int64 size);

		/// <summary>
		///		SetOverrideFileIO                           (http://rdf.bg/gkdoc/CS64/SetOverrideFileIO.html)
		///
		///	This function overrides the type of file saved / exported independent of the extension given.
		///	By default the extension of the file name will define the type saved / exported:
		///		.rdf => generated RDF serialized content
		///		.ttl => generated TTL serialized content
		///		.bin => generated BIN/X serialized content
		///
		///	Available formats
		///		RDF
		///		TTL
		///		BIN/L - readible but large BIN format
		///		BIN/S - Optimized Binary, only running within given revision 
		///		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
		///
		///	Force file type (overrides extension), works only on save (open selects correct type automatically)
		///		bit0	bit1	bit2
		///		  0		  0		  0		[default] unset forced file type
		///		  0		  0		  1		RESERVED
		///		  0		  1		  0		TTL
		///		  0		  1		  1		RDF
		///		  1		  0		  0		BIN/X
		///		  1		  0		  1		BIN/S
		///		  1		  1		  0		RESERVED
		///		  1		  1		  1		BIN/L
		///
		///	Force exporting as Base64
		///		bit4
		///		  0		do not use Base64
		///		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetOverrideFileIO")]
		public static extern void SetOverrideFileIO(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetOverrideFileIO                           (http://rdf.bg/gkdoc/CS64/GetOverrideFileIO.html)
		///
		///	This function get the current overrides for type of file saved / exported independent of the extension given.
		///	By default the extension of the file name will define the type saved / exported:
		///		.rdf => generated RDF serialized content
		///		.ttl => generated TTL serialized content
		///		.bin => generated BIN/X serialized content
		///
		///	Available formats
		///		RDF
		///		TTL
		///		BIN/L - readible but large BIN format
		///		BIN/S - Optimized Binary, only running within given revision 
		///		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
		///
		///	Force file type (overrides extension), works only on save (open selects correct type automatically)
		///		bit0	bit1	bit2
		///		  0		  0		  0		[default] unset forced file type
		///		  0		  0		  1		RESERVED
		///		  0		  1		  0		TTL
		///		  0		  1		  1		RDF
		///		  1		  0		  0		BIN/X
		///		  1		  0		  1		BIN/S
		///		  1		  1		  0		RESERVED
		///		  1		  1		  1		BIN/L
		///
		///	Force exporting as Base64
		///		bit4
		///		  0		do not use Base64
		///		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetOverrideFileIO")]
		public static extern Int64 GetOverrideFileIO(Int64 model, Int64 mask);

		/// <summary>
		///		CopyInstanceTree                            (http://rdf.bg/gkdoc/CS64/CopyInstanceTree.html)
		///
		///	This function copies the instance tree towards a new model.
		///	In case model is empty a new model will be created (the handle to this new model can be retrieved through
		///	the call GetModel() based on the return value of this call).
		///	The model can be amy opem model, it can be zero (a new model will be created on-the-fly) and it can be
		///	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
		///	original instance tree.
		///
		///	The return value is the handle to the copied owlInstance in the model of choice.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CopyInstanceTree")]
		public static extern Int64 CopyInstanceTree(Int64 owlInstance, Int64 targetModel);

		/// <summary>
		///		CopyInstanceNetwork                         (http://rdf.bg/gkdoc/CS64/CopyInstanceNetwork.html)
		///
		///	This function copies the instance network towards a new model.
		///	An instance network is different from an instance tree in that it can contain 'loops', the performance
		///	from this call will be slower in case the tree / network is sparse.
		///	In case model is empty a new model will be created (the handle to this new model can be retrieved through
		///	the call GetModel() based on the return value of this call).
		///	The model can be amy opem model, it can be zero (a new model will be created on-the-fly) and it can be
		///	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
		///	original instance tree.
		///
		///	In case it is known we are talking about a tree (i.e. no internal loops) and inverse relations can be ignored
		///	the call CopyInstanceTree is a better choice concerning performance.
		///
		///	The return value is the handle to the copied owlInstance in the model of choice.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CopyInstanceNetwork")]
		public static extern Int64 CopyInstanceNetwork(Int64 owlInstance, byte includeInverseRelations, Int64 targetModel);

		/// <summary>
		///		CopyModel                                   (http://rdf.bg/gkdoc/CS64/CopyModel.html)
		///
		///	This function copies the complete structure of a model towards another or new model.
		///	In case the targetModel is empty a new model will be created.
		///	The owlInstance array (values) and it's cardinality (card) can be empty, in case they are
		///	non-empty the values are expected to contain owlInstance handles referencing in the source model
		///	after a successful copy the values will be adjusted into values referencing the copied owl instances
		///	in the new model. the list of values does not have to be complete or even unique and can even be empty.
		///
		///	sourceModel is not allowed to be empty, targetModel however can be empty or even equal to the source model.
		///
		///	The return value is the targetModel or in case this value was empty the newly created model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CopyModel")]
		public static extern Int64 CopyModel(Int64 sourceModel, Int64 targetModel, out Int64 values, Int64 card);

		/// <summary>
		///		CloseModel                                  (http://rdf.bg/gkdoc/CS64/CloseModel.html)
		///
		///	This function closes the model. After this call none of the instances and classes within the model
		///	can be used anymore, also garbage collection is not allowed anymore, in default compilation the
		///	model itself will be known in the kernel, however known to be disabled. Calls containing the model
		///	reference will be protected from crashing when called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CloseModel")]
		public static extern Int64 CloseModel(Int64 model);

        //
        //  Design Tree Classes API Calls
        //

		/// <summary>
		///		CreateClass                                 (http://rdf.bg/gkdoc/CS64/CreateClass.html)
		///
		///	Returns a handle to an on the fly created class.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateClass")]
		public static extern Int64 CreateClass(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "CreateClass")]
		public static extern Int64 CreateClass(Int64 model, byte[] name);

		/// <summary>
		///		CreateClassW                                (http://rdf.bg/gkdoc/CS64/CreateClassW.html)
		///
		///	Returns a handle to an on the fly created class.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateClassW")]
		public static extern Int64 CreateClassW(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "CreateClassW")]
		public static extern Int64 CreateClassW(Int64 model, byte[] name);

		/// <summary>
		///		GetClassByName                              (http://rdf.bg/gkdoc/CS64/GetClassByName.html)
		///
		///	Returns a handle to the class as stored inside.
		///	When the class does not exist yet and the name is unique
		///	the class will be created on the fly and the handle will be returned.
		///	When the name is not unique and given to an instance, objectTypeProperty
		///	or dataTypeProperty 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassByName")]
		public static extern Int64 GetClassByName(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetClassByName")]
		public static extern Int64 GetClassByName(Int64 model, byte[] name);

		/// <summary>
		///		GetClassByNameW                             (http://rdf.bg/gkdoc/CS64/GetClassByNameW.html)
		///
		///	Returns a handle to the class as stored inside.
		///	When the class does not exist yet and the name is unique
		///	the class will be created on the fly and the handle will be returned.
		///	When the name is not unique and given to an instance, objectTypeProperty
		///	or dataTypeProperty 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassByNameW")]
		public static extern Int64 GetClassByNameW(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetClassByNameW")]
		public static extern Int64 GetClassByNameW(Int64 model, byte[] name);

		/// <summary>
		///		GetClassesByIterator                        (http://rdf.bg/gkdoc/CS64/GetClassesByIterator.html)
		///
		///	Returns a handle to an class.
		///	If input class is zero, the handle will point to the first relevant class.
		///	If all classes are past (or no relevant classes are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassesByIterator")]
		public static extern Int64 GetClassesByIterator(Int64 model, Int64 owlClass);

		/// <summary>
		///		SetClassParent                              (http://rdf.bg/gkdoc/CS64/SetClassParent.html)
		///
		///	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
		///	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
		///	object properties (relations).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassParent")]
		public static extern void SetClassParent(Int64 owlClass, Int64 parentOwlClass, Int64 setting);

		/// <summary>
		///		SetClassParentEx                            (http://rdf.bg/gkdoc/CS64/SetClassParentEx.html)
		///
		///	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
		///	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
		///	object properties (relations).
		///
		///	This call has the same behavior as SetClassParent, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassParentEx")]
		public static extern void SetClassParentEx(Int64 model, Int64 owlClass, Int64 parentOwlClass, Int64 setting);

		/// <summary>
		///		GetClassParentsByIterator                   (http://rdf.bg/gkdoc/CS64/GetClassParentsByIterator.html)
		///
		///	Returns the next parent of the class.
		///	If input parent is zero, the handle will point to the first relevant parent.
		///	If all parent are past (or no relevant parent are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassParentsByIterator")]
		public static extern Int64 GetClassParentsByIterator(Int64 owlClass, Int64 parentOwlClass);

		/// <summary>
		///		SetNameOfClass                              (http://rdf.bg/gkdoc/CS64/SetNameOfClass.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClass")]
		public static extern Int64 SetNameOfClass(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClass")]
		public static extern Int64 SetNameOfClass(Int64 owlClass, byte[] name);

		/// <summary>
		///		SetNameOfClassW                             (http://rdf.bg/gkdoc/CS64/SetNameOfClassW.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClassW")]
		public static extern Int64 SetNameOfClassW(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClassW")]
		public static extern Int64 SetNameOfClassW(Int64 owlClass, byte[] name);

		/// <summary>
		///		SetNameOfClassEx                            (http://rdf.bg/gkdoc/CS64/SetNameOfClassEx.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		///
		///	This call has the same behavior as SetNameOfClass, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClassEx")]
		public static extern Int64 SetNameOfClassEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClassEx")]
		public static extern Int64 SetNameOfClassEx(Int64 model, Int64 owlClass, byte[] name);

		/// <summary>
		///		SetNameOfClassWEx                           (http://rdf.bg/gkdoc/CS64/SetNameOfClassWEx.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		///
		///	This call has the same behavior as SetNameOfClassW, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClassWEx")]
		public static extern Int64 SetNameOfClassWEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClassWEx")]
		public static extern Int64 SetNameOfClassWEx(Int64 model, Int64 owlClass, byte[] name);

		/// <summary>
		///		GetNameOfClass                              (http://rdf.bg/gkdoc/CS64/GetNameOfClass.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClass")]
		public static extern void GetNameOfClass(Int64 owlClass, out IntPtr name);

		public static string GetNameOfClass(Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfClass(owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfClassW                             (http://rdf.bg/gkdoc/CS64/GetNameOfClassW.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClassW")]
		public static extern void GetNameOfClassW(Int64 owlClass, out IntPtr name);

		public static string GetNameOfClassW(Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfClassW(owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		GetNameOfClassEx                            (http://rdf.bg/gkdoc/CS64/GetNameOfClassEx.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		///
		///	This call has the same behavior as GetNameOfClass, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClassEx")]
		public static extern void GetNameOfClassEx(Int64 model, Int64 owlClass, out IntPtr name);

		public static string GetNameOfClassEx(Int64 model, Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfClassEx(model, owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfClassWEx                           (http://rdf.bg/gkdoc/CS64/GetNameOfClassWEx.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		///
		///	This call has the same behavior as GetNameOfClassW, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClassWEx")]
		public static extern void GetNameOfClassWEx(Int64 model, Int64 owlClass, out IntPtr name);

		public static string GetNameOfClassWEx(Int64 model, Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfClassWEx(model, owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		SetClassPropertyCardinalityRestriction      (http://rdf.bg/gkdoc/CS64/SetClassPropertyCardinalityRestriction.html)
		///
		///	This function sets the minCard and maxCard of a certain property in the context of a class.
		///	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
		///	as within the cardinality restrictions as given by the property in context of any of its
		///	(indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassPropertyCardinalityRestriction")]
		public static extern void SetClassPropertyCardinalityRestriction(Int64 owlClass, Int64 rdfProperty, Int64 minCard, Int64 maxCard);

		/// <summary>
		///		SetClassPropertyCardinalityRestrictionEx    (http://rdf.bg/gkdoc/CS64/SetClassPropertyCardinalityRestrictionEx.html)
		///
		///	This function sets the minCard and maxCard of a certain property in the context of a class.
		///	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
		///	as within the cardinality restrictions as given by the property in context of any of its
		///	(indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		///
		///	This call has the same behavior as SetClassPropertyCardinalityRestriction, however needs to be
		///	used in case classes or properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassPropertyCardinalityRestrictionEx")]
		public static extern void SetClassPropertyCardinalityRestrictionEx(Int64 model, Int64 owlClass, Int64 rdfProperty, Int64 minCard, Int64 maxCard);

		/// <summary>
		///		GetClassPropertyCardinalityRestriction      (http://rdf.bg/gkdoc/CS64/GetClassPropertyCardinalityRestriction.html)
		///
		///	This function returns the minCard and maxCard of a certain
		///	property in the context of a class. The cardinality of a property in 
		///	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
		///	as given by the property in context of any of its (indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		///
		///	Note: this function does not return inherited restrictions. The example shows how to retrieve
		///	this knowledge, as it is derived knowledge the call that used to be available is removed.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassPropertyCardinalityRestriction")]
		public static extern void GetClassPropertyCardinalityRestriction(Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

		/// <summary>
		///		GetClassPropertyCardinalityRestrictionEx    (http://rdf.bg/gkdoc/CS64/GetClassPropertyCardinalityRestrictionEx.html)
		///
		///	This function returns the minCard and maxCard of a certain
		///	property in the context of a class. The cardinality of a property in 
		///	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
		///	as given by the property in context of any of its (indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		///
		///	This call has the same behavior as GetClassPropertyCardinalityRestriction, however needs to be
		///	used in case classes or properties are exchanged as a successive series of integers.
		///
		///	Note: this function does not return inherited restrictions. The example shows how to retrieve
		///	this knowledge, as it is derived knowledge the call that used to be available is removed.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassPropertyCardinalityRestrictionEx")]
		public static extern void GetClassPropertyCardinalityRestrictionEx(Int64 model, Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

		/// <summary>
		///		GetGeometryClass                            (http://rdf.bg/gkdoc/CS64/GetGeometryClass.html)
		///
		///	Returns non-zero if the owlClass is a geometry type. This call will return the input class
		///	for all classes initially available. It will return as well non-for all classes created by the
		///	user or loaded / imported through a model that (indirectly) inherit one of the
		///	original classes available. in this case it returns the original available class
		///	it inherits the behavior from.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetGeometryClass")]
		public static extern Int64 GetGeometryClass(Int64 owlClass);

		/// <summary>
		///		GetGeometryClassEx                          (http://rdf.bg/gkdoc/CS64/GetGeometryClassEx.html)
		///
		///	Returns non-zero if the owlClass is a geometry type. This call will return the input class
		///	for all classes initially available. It will return as well non-for all classes created by the
		///	user or loaded / imported through a model that (indirectly) inherit one of the
		///	original classes available. in this case it returns the original available class
		///	it inherits the behavior from.
		///
		///	This call has the same behavior as GetGeometryClass, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetGeometryClassEx")]
		public static extern Int64 GetGeometryClassEx(Int64 model, Int64 owlClass);

		/// <summary>
		///		IsClass                                     (http://rdf.bg/gkdoc/CS64/IsClass.html)
		///
		///	Returns true if the argument owlClass is an actual active class in an active model. It returns false in all other cases,
		///	i.e. this could mean the model is already closed, the class is inactive or removed or the session is closed.
		///	It could also mean it represents a handle to another Thing, for example a property, instance or model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsClass")]
		public static extern byte IsClass(Int64 owlClass);

        //
        //  Design Tree Properties API Calls
        //

		/// <summary>
		///		CreateProperty                              (http://rdf.bg/gkdoc/CS64/CreateProperty.html)
		///
		///	Returns a handle to an on the fly created property.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateProperty")]
		public static extern Int64 CreateProperty(Int64 model, Int64 rdfPropertyType, string name);

		[DllImport(enginedll, EntryPoint = "CreateProperty")]
		public static extern Int64 CreateProperty(Int64 model, Int64 rdfPropertyType, byte[] name);

		/// <summary>
		///		CreatePropertyW                             (http://rdf.bg/gkdoc/CS64/CreatePropertyW.html)
		///
		///	Returns a handle to an on the fly created property.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreatePropertyW")]
		public static extern Int64 CreatePropertyW(Int64 model, Int64 rdfPropertyType, string name);

		[DllImport(enginedll, EntryPoint = "CreatePropertyW")]
		public static extern Int64 CreatePropertyW(Int64 model, Int64 rdfPropertyType, byte[] name);

		/// <summary>
		///		GetPropertyByName                           (http://rdf.bg/gkdoc/CS64/GetPropertyByName.html)
		///
		///	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
		///	When the property does not exist yet and the name is unique
		///	the property will be created on-the-fly and the handle will be returned.
		///	When the name is not unique and given to a class or instance 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyByName")]
		public static extern Int64 GetPropertyByName(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetPropertyByName")]
		public static extern Int64 GetPropertyByName(Int64 model, byte[] name);

		/// <summary>
		///		GetPropertyByNameW                          (http://rdf.bg/gkdoc/CS64/GetPropertyByNameW.html)
		///
		///	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
		///	When the property does not exist yet and the name is unique
		///	the property will be created on-the-fly and the handle will be returned.
		///	When the name is not unique and given to a class or instance 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyByNameW")]
		public static extern Int64 GetPropertyByNameW(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetPropertyByNameW")]
		public static extern Int64 GetPropertyByNameW(Int64 model, byte[] name);

		/// <summary>
		///		GetPropertiesByIterator                     (http://rdf.bg/gkdoc/CS64/GetPropertiesByIterator.html)
		///
		///	Returns a handle to a property.
		///	If input property is zero, the handle will point to the first relevant property.
		///	If all properties are past (or no relevant properties are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertiesByIterator")]
		public static extern Int64 GetPropertiesByIterator(Int64 model, Int64 rdfProperty);

		/// <summary>
		///		SetPropertyRangeRestriction                 (http://rdf.bg/gkdoc/CS64/SetPropertyRangeRestriction.html)
		///
		///	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
		///	be an objectp[roperty, i.e. relation.]
		///	If rdfProperty is not an object property this call has no effect.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyRangeRestriction")]
		public static extern void SetPropertyRangeRestriction(Int64 rdfProperty, Int64 owlClass, Int64 setting);

		/// <summary>
		///		SetPropertyRangeRestrictionEx               (http://rdf.bg/gkdoc/CS64/SetPropertyRangeRestrictionEx.html)
		///
		///	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
		///	be an objectp[roperty, i.e. relation.]
		///	If rdfProperty is not an object property this call has no effect.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyRangeRestrictionEx")]
		public static extern void SetPropertyRangeRestrictionEx(Int64 model, Int64 rdfProperty, Int64 owlClass, Int64 setting);

		/// <summary>
		///		GetRangeRestrictionsByIterator              (http://rdf.bg/gkdoc/CS64/GetRangeRestrictionsByIterator.html)
		///
		///	Returns the next class the property is restricted to.
		///	If input class is zero, the handle will point to the first relevant class.
		///	If all classes are past (or no relevant classes are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRangeRestrictionsByIterator")]
		public static extern Int64 GetRangeRestrictionsByIterator(Int64 rdfProperty, Int64 owlClass);

		/// <summary>
		///		GetRangeRestrictionsByIteratorEx            (http://rdf.bg/gkdoc/CS64/GetRangeRestrictionsByIteratorEx.html)
		///
		///	Returns the next class the property is restricted to.
		///	If input class is zero, the handle will point to the first relevant class.
		///	If all classes are past (or no relevant classes are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRangeRestrictionsByIteratorEx")]
		public static extern Int64 GetRangeRestrictionsByIteratorEx(Int64 model, Int64 rdfProperty, Int64 owlClass);

		/// <summary>
		///		GetPropertyParentsByIterator                (http://rdf.bg/gkdoc/CS64/GetPropertyParentsByIterator.html)
		///
		///	Returns the next parent of the property.
		///	If input parent is zero, the handle will point to the first relevant parent.
		///	If all parent are past (or no relevant parent are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyParentsByIterator")]
		public static extern Int64 GetPropertyParentsByIterator(Int64 rdfProperty, Int64 parentRdfProperty);

		/// <summary>
		///		SetNameOfProperty                           (http://rdf.bg/gkdoc/CS64/SetNameOfProperty.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfProperty")]
		public static extern Int64 SetNameOfProperty(Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfProperty")]
		public static extern Int64 SetNameOfProperty(Int64 rdfProperty, byte[] name);

		/// <summary>
		///		SetNameOfPropertyW                          (http://rdf.bg/gkdoc/CS64/SetNameOfPropertyW.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyW")]
		public static extern Int64 SetNameOfPropertyW(Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyW")]
		public static extern Int64 SetNameOfPropertyW(Int64 rdfProperty, byte[] name);

		/// <summary>
		///		SetNameOfPropertyEx                         (http://rdf.bg/gkdoc/CS64/SetNameOfPropertyEx.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyEx")]
		public static extern Int64 SetNameOfPropertyEx(Int64 model, Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyEx")]
		public static extern Int64 SetNameOfPropertyEx(Int64 model, Int64 rdfProperty, byte[] name);

		/// <summary>
		///		SetNameOfPropertyWEx                        (http://rdf.bg/gkdoc/CS64/SetNameOfPropertyWEx.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyWEx")]
		public static extern Int64 SetNameOfPropertyWEx(Int64 model, Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyWEx")]
		public static extern Int64 SetNameOfPropertyWEx(Int64 model, Int64 rdfProperty, byte[] name);

		/// <summary>
		///		GetNameOfProperty                           (http://rdf.bg/gkdoc/CS64/GetNameOfProperty.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfProperty")]
		public static extern void GetNameOfProperty(Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfProperty(Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfProperty(rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfPropertyW                          (http://rdf.bg/gkdoc/CS64/GetNameOfPropertyW.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfPropertyW")]
		public static extern void GetNameOfPropertyW(Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfPropertyW(Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfPropertyW(rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		GetNameOfPropertyEx                         (http://rdf.bg/gkdoc/CS64/GetNameOfPropertyEx.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfPropertyEx")]
		public static extern void GetNameOfPropertyEx(Int64 model, Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfPropertyEx(Int64 model, Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfPropertyEx(model, rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfPropertyWEx                        (http://rdf.bg/gkdoc/CS64/GetNameOfPropertyWEx.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfPropertyWEx")]
		public static extern void GetNameOfPropertyWEx(Int64 model, Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfPropertyWEx(Int64 model, Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfPropertyWEx(model, rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		SetPropertyType                             (http://rdf.bg/gkdoc/CS64/SetPropertyType.html)
		///
		///	This function sets the type of the property. This is only allowed
		///	if the type of the property was not set before.
		///
		///	The following values are possible for propertyType:
		///			1	The property is an Object Property
		///			2	The property is an Datatype Property of type Boolean
		///			3	The property is an Datatype Property of type Char
		///			4	The property is an Datatype Property of type Integer
		///			5	The property is an Datatype Property of type Double
		///	The return value of this call is GetPropertyType/Ex applied after applying
		///	the type, normally this corresponds with the propertyType requested
		///	to be set unless the property already has a different propertyType set before.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyType")]
		public static extern Int64 SetPropertyType(Int64 rdfProperty, Int64 propertyType);

		/// <summary>
		///		SetPropertyTypeEx                           (http://rdf.bg/gkdoc/CS64/SetPropertyTypeEx.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyTypeEx")]
		public static extern Int64 SetPropertyTypeEx(Int64 model, Int64 rdfProperty, Int64 propertyType);

		/// <summary>
		///		GetPropertyType                             (http://rdf.bg/gkdoc/CS64/GetPropertyType.html)
		///
		///	This function returns the type of the property.
		///	The following return values are possible:
		///		0	The property is not defined yet
		///		1	The property is an Object Type Property
		///		2	The property is an Data Type Property of type Boolean
		///		3	The property is an Data Type Property of type Char
		///		4	The property is an Data Type Property of type Integer
		///		5	The property is an Data Type Property of type Double
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyType")]
		public static extern Int64 GetPropertyType(Int64 rdfProperty);

		/// <summary>
		///		GetPropertyTypeEx                           (http://rdf.bg/gkdoc/CS64/GetPropertyTypeEx.html)
		///
		///	This call has the same behavior as GetPropertyType, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyTypeEx")]
		public static extern Int64 GetPropertyTypeEx(Int64 model, Int64 rdfProperty);

		/// <summary>
		///		RemoveProperty                              (http://rdf.bg/gkdoc/CS64/RemoveProperty.html)
		///
		///	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
		///	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
		///	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
		///	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
		///	the following bits represent the findings during removal:
		///		bit 0:
		///			0	Iunput as expected
		///			1	Encountered an issue on input value, i.e. property was not recognized as property
		///		bit 1:
		///			0	No 'child' properties found
		///			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
		///		bit 2:
		///			0	No instances found with value restrictions for this property
		///			1	Vertex does contain 3D point info
		///		bit 3:
		///			0	No instances found with values for this property
		///			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
		///		bit 6:
		///			0	Vertex does not contain first 2D texture info
		///			1	Vertex does contain first 2D texture info
		///		bit 7:
		///			0	Vertex does not contain second 2D texture info
		///			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
		///
		///		0	The property is not defined yet
		///		1	The property is an Object Type Property
		///		2	The property is an Data Type Property of type Boolean
		///		3	The property is an Data Type Property of type Char
		///		4	The property is an Data Type Property of type Integer
		///		5	The property is an Data Type Property of type Double
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveProperty")]
		public static extern Int64 RemoveProperty(Int64 rdfProperty);

		/// <summary>
		///		RemovePropertyEx                            (http://rdf.bg/gkdoc/CS64/RemovePropertyEx.html)
		///
		///	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
		///	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
		///	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
		///	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
		///	the following bits represent the findings during removal:
		///		bit 0:
		///			0	Iunput as expected
		///			1	Encountered an issue on input value, i.e. property was not recognized as property
		///		bit 1:
		///			0	No 'child' properties found
		///			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
		///		bit 2:
		///			0	No instances found with value restrictions for this property
		///			1	Vertex does contain 3D point info
		///		bit 3:
		///			0	No instances found with values for this property
		///			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
		///		bit 6:
		///			0	Vertex does not contain first 2D texture info
		///			1	Vertex does contain first 2D texture info
		///		bit 7:
		///			0	Vertex does not contain second 2D texture info
		///			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
		///
		///		0	The property is not defined yet
		///		1	The property is an Object Type Property
		///		2	The property is an Data Type Property of type Boolean
		///		3	The property is an Data Type Property of type Char
		///		4	The property is an Data Type Property of type Integer
		///		5	The property is an Data Type Property of type Double
		///
		///
		///	This call has the same behavior as RemoveProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemovePropertyEx")]
		public static extern Int64 RemovePropertyEx(Int64 model, Int64 rdfProperty);

		/// <summary>
		///		IsProperty                                  (http://rdf.bg/gkdoc/CS64/IsProperty.html)
		///
		///	Returns true if the argument rdfProperty is an actual active property in an active model. It returns false in all other cases,
		///	i.e. this could mean the model is already closed, the property is inactive or removed or the session is closed.
		///	It could also mean it represents a handle to another Thing, for example a class, instance or model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsProperty")]
		public static extern byte IsProperty(Int64 rdfProperty);

        //
        //  Design Tree Instances API Calls
        //

		/// <summary>
		///		CreateInstance                              (http://rdf.bg/gkdoc/CS64/CreateInstance.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstance")]
		public static extern Int64 CreateInstance(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstance")]
		public static extern Int64 CreateInstance(Int64 owlClass, byte[] name);

		public static Int64 CreateInstance(Int64 owlClass)
		{
			string name = (string) null;
			return RDF.engine.CreateInstance(owlClass, name);
		}

		/// <summary>
		///		CreateInstanceW                             (http://rdf.bg/gkdoc/CS64/CreateInstanceW.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceW")]
		public static extern Int64 CreateInstanceW(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstanceW")]
		public static extern Int64 CreateInstanceW(Int64 owlClass, byte[] name);

		public static Int64 CreateInstanceW(Int64 owlClass)
		{
			string name = (string) null;
			return RDF.engine.CreateInstanceW(owlClass, name);
		}

		/// <summary>
		///		CreateInstanceEx                            (http://rdf.bg/gkdoc/CS64/CreateInstanceEx.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceEx")]
		public static extern Int64 CreateInstanceEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstanceEx")]
		public static extern Int64 CreateInstanceEx(Int64 model, Int64 owlClass, byte[] name);

		public static Int64 CreateInstanceEx(Int64 model, Int64 owlClass)
		{
			string name = (string) null;
			return RDF.engine.CreateInstanceEx(model, owlClass, name);
		}

		/// <summary>
		///		CreateInstanceWEx                           (http://rdf.bg/gkdoc/CS64/CreateInstanceWEx.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceWEx")]
		public static extern Int64 CreateInstanceWEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstanceWEx")]
		public static extern Int64 CreateInstanceWEx(Int64 model, Int64 owlClass, byte[] name);

		public static Int64 CreateInstanceWEx(Int64 model, Int64 owlClass)
		{
			string name = (string) null;
			return RDF.engine.CreateInstanceWEx(model, owlClass, name);
		}

		/// <summary>
		///		GetInstancesByIterator                      (http://rdf.bg/gkdoc/CS64/GetInstancesByIterator.html)
		///
		///	Returns a handle to an instance.
		///	If input instance is zero, the handle will point to the first relevant instance.
		///	If all instances are past (or no relevant instances are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstancesByIterator")]
		public static extern Int64 GetInstancesByIterator(Int64 model, Int64 owlInstance);

		/// <summary>
		///		GetInstanceClass                            (http://rdf.bg/gkdoc/CS64/GetInstanceClass.html)
		///
		///	Returns the handle to the class of which an instances is instantiated.
		///
		///	Note: internally this function is not rich enough as support for multiple inheritance
		///		  is making it impossible to answer this request with always one class handle.
		///		  Even in the case of pure single inheritance an instance of a class is also an instance of 
		///		  every parent classes in the inheritance tree. For now we expect single inheritance use
		///		  and the returned class handle points to the 'lowest' class in the inheritance tree.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceClass")]
		public static extern Int64 GetInstanceClass(Int64 owlInstance);

		/// <summary>
		///		GetInstanceClassEx                          (http://rdf.bg/gkdoc/CS64/GetInstanceClassEx.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceClassEx")]
		public static extern Int64 GetInstanceClassEx(Int64 model, Int64 owlInstance);

		/// <summary>
		///		GetInstancePropertyByIterator               (http://rdf.bg/gkdoc/CS64/GetInstancePropertyByIterator.html)
		///
		///	Returns a handle to the objectTypeProperty or dataTypeProperty connected to
		///	the instance, this property can also contain a value, but for example also
		///	the knowledge about cardinality restrictions in the context of this instance's class
		///	and the exact cardinality in context of its instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstancePropertyByIterator")]
		public static extern Int64 GetInstancePropertyByIterator(Int64 owlInstance, Int64 rdfProperty);

		/// <summary>
		///		GetInstancePropertyByIteratorEx             (http://rdf.bg/gkdoc/CS64/GetInstancePropertyByIteratorEx.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstancePropertyByIteratorEx")]
		public static extern Int64 GetInstancePropertyByIteratorEx(Int64 model, Int64 owlInstance, Int64 rdfProperty);

		/// <summary>
		///		GetInstanceInverseReferencesByIterator      (http://rdf.bg/gkdoc/CS64/GetInstanceInverseReferencesByIterator.html)
		///
		///	Returns a handle to the owlInstances refering this instance
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceInverseReferencesByIterator")]
		public static extern Int64 GetInstanceInverseReferencesByIterator(Int64 owlInstance, Int64 referencingOwlInstance);

		/// <summary>
		///		GetInstanceReferencesByIterator             (http://rdf.bg/gkdoc/CS64/GetInstanceReferencesByIterator.html)
		///
		///	Returns a handle to the owlInstance refered by this instance
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceReferencesByIterator")]
		public static extern Int64 GetInstanceReferencesByIterator(Int64 owlInstance, Int64 referencedOwlInstance);

		/// <summary>
		///		SetNameOfInstance                           (http://rdf.bg/gkdoc/CS64/SetNameOfInstance.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstance")]
		public static extern Int64 SetNameOfInstance(Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstance")]
		public static extern Int64 SetNameOfInstance(Int64 owlInstance, byte[] name);

		/// <summary>
		///		SetNameOfInstanceW                          (http://rdf.bg/gkdoc/CS64/SetNameOfInstanceW.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceW")]
		public static extern Int64 SetNameOfInstanceW(Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceW")]
		public static extern Int64 SetNameOfInstanceW(Int64 owlInstance, byte[] name);

		/// <summary>
		///		SetNameOfInstanceEx                         (http://rdf.bg/gkdoc/CS64/SetNameOfInstanceEx.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceEx")]
		public static extern Int64 SetNameOfInstanceEx(Int64 model, Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceEx")]
		public static extern Int64 SetNameOfInstanceEx(Int64 model, Int64 owlInstance, byte[] name);

		/// <summary>
		///		SetNameOfInstanceWEx                        (http://rdf.bg/gkdoc/CS64/SetNameOfInstanceWEx.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceWEx")]
		public static extern Int64 SetNameOfInstanceWEx(Int64 model, Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceWEx")]
		public static extern Int64 SetNameOfInstanceWEx(Int64 model, Int64 owlInstance, byte[] name);

		/// <summary>
		///		GetNameOfInstance                           (http://rdf.bg/gkdoc/CS64/GetNameOfInstance.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstance")]
		public static extern void GetNameOfInstance(Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstance(Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfInstance(owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfInstanceW                          (http://rdf.bg/gkdoc/CS64/GetNameOfInstanceW.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstanceW")]
		public static extern void GetNameOfInstanceW(Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstanceW(Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfInstanceW(owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		GetNameOfInstanceEx                         (http://rdf.bg/gkdoc/CS64/GetNameOfInstanceEx.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstanceEx")]
		public static extern void GetNameOfInstanceEx(Int64 model, Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstanceEx(Int64 model, Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfInstanceEx(model, owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfInstanceWEx                        (http://rdf.bg/gkdoc/CS64/GetNameOfInstanceWEx.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstanceWEx")]
		public static extern void GetNameOfInstanceWEx(Int64 model, Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstanceWEx(Int64 model, Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			RDF.engine.GetNameOfInstanceWEx(model, owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		SetDatatypeProperty                         (http://rdf.bg/gkdoc/CS64/SetDatatypeProperty.html)
		///
		///	This function sets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the values card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, ref byte values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, byte[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, Int64[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, ref double values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, double[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, ref string values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, string[] values, Int64 card);

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, bool value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);
			const Int64 card = 1;
			byte value_inByte = Convert.ToByte(value);
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, ref value_inByte, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, byte value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BYTE);
			const Int64 card = 1;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, Int64 value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_INTEGER);
			const Int64 card = 1;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, double value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);
			const Int64 card = 1;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, string value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_CHAR);
			const Int64 card = 1;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, bool[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);
			Int64 card = values.Length;
			byte[] values_inByte = values.Select((v) =>
				{
					return Convert.ToByte(v);
				}).ToArray();
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, values_inByte, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, byte[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BYTE);
			Int64 card = values.Length;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, values, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, Int64[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_INTEGER);
			Int64 card = values.Length;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, values, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, double[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);
			Int64 card = values.Length;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, values, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, string[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_CHAR);
			Int64 card = values.Length;
			return RDF.engine.SetDatatypeProperty(owlInstance, rdfProperty, values, card);
		}

		/// <summary>
		///		SetDatatypePropertyEx                       (http://rdf.bg/gkdoc/CS64/SetDatatypePropertyEx.html)
		///
		///	This function sets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the values card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as SetDatatypeProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, ref byte values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, byte[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, Int64[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, ref double values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, double[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, ref string values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, string[] values, Int64 card);

		/// <summary>
		///		GetDatatypeProperty                         (http://rdf.bg/gkdoc/CS64/GetDatatypeProperty.html)
		///
		///	This function gets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the value card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDatatypeProperty")]
		public static extern Int64 GetDatatypeProperty(Int64 owlInstance, Int64 rdfProperty, out IntPtr values, out Int64 card);

		public static bool[] GetDatatypeProperty_inBool(Int64 owlInstance, Int64 rdfProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			RDF.engine.GetDatatypeProperty(owlInstance, rdfProperty, out valuesPtr, out card);

			if (card > 0)
			{
				byte[] values_inByte = new byte[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values_inByte, 0, (int) card);
				bool[] values = values_inByte.Select((v) =>
				{
					return v != 0;
				}).ToArray();
				return values;
			}

			return null;
		}

		public static byte[] GetDatatypeProperty_inByte(Int64 owlInstance, Int64 rdfProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_BYTE);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			RDF.engine.GetDatatypeProperty(owlInstance, rdfProperty, out valuesPtr, out card);

			if (card > 0)
			{
				byte[] values = new byte[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		public static Int64[] GetDatatypeProperty_inInt64(Int64 owlInstance, Int64 rdfProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_INTEGER);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			RDF.engine.GetDatatypeProperty(owlInstance, rdfProperty, out valuesPtr, out card);

			if (card > 0)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		public static double[] GetDatatypeProperty_inDouble(Int64 owlInstance, Int64 rdfProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			RDF.engine.GetDatatypeProperty(owlInstance, rdfProperty, out valuesPtr, out card);

			if (card > 0)
			{
				double[] values = new double[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		public static string[] GetDatatypeProperty_inString(Int64 owlInstance, Int64 rdfProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == DATATYPEPROPERTY_TYPE_CHAR);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			RDF.engine.GetDatatypeProperty(owlInstance, rdfProperty, out valuesPtr, out card);

			if (card > 0)
			{
				IntPtr[] valuesRef = new IntPtr[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, valuesRef, 0, (int) card);

				string[] values = new string[card];
				for (int i = 0; i < (int) card; i++)
				{
					values[i] = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(valuesRef[i]);
				}

				return values;
			}

			return null;
		}

		/// <summary>
		///		GetDatatypePropertyEx                       (http://rdf.bg/gkdoc/CS64/GetDatatypePropertyEx.html)
		///
		///	This function gets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the value card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as GetDatatypeProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDatatypePropertyEx")]
		public static extern Int64 GetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, out IntPtr values, out Int64 card);

		/// <summary>
		///		SetObjectProperty                           (http://rdf.bg/gkdoc/CS64/SetObjectProperty.html)
		///
		///	This function sets the value(s) of a certain objectTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the values card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetObjectProperty")]
		public static extern Int64 SetObjectProperty(Int64 owlInstance, Int64 rdfProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetObjectProperty")]
		public static extern Int64 SetObjectProperty(Int64 owlInstance, Int64 rdfProperty, Int64[] values, Int64 card);

		public static Int64 SetObjectProperty(Int64 owlInstance, Int64 rdfProperty, Int64 value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == OBJECTPROPERTY_TYPE);
			const Int64	card = 1;
			return RDF.engine.SetObjectProperty(owlInstance, rdfProperty, ref value, card);
		}

		public static Int64 SetObjectProperty(Int64 owlInstance, Int64 rdfProperty, Int64[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == OBJECTPROPERTY_TYPE);
			Int64 card = values.Length;
			return RDF.engine.SetObjectProperty(owlInstance, rdfProperty, values, card);
		}

		/// <summary>
		///		SetObjectPropertyEx                         (http://rdf.bg/gkdoc/CS64/SetObjectPropertyEx.html)
		///
		///	This function sets the value(s) of a certain objectTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the values card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as SetObjectProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetObjectPropertyEx")]
		public static extern Int64 SetObjectPropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetObjectPropertyEx")]
		public static extern Int64 SetObjectPropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, Int64[] values, Int64 card);

		/// <summary>
		///		GetObjectProperty                           (http://rdf.bg/gkdoc/CS64/GetObjectProperty.html)
		///
		///	This function gets the value(s) of a certain objectProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the value card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetObjectProperty")]
		public static extern Int64 GetObjectProperty(Int64 owlInstance, Int64 rdfProperty, out IntPtr values, out Int64 card);

		public static Int64[] GetObjectProperty(Int64 owlInstance, Int64 rdfProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(rdfProperty) == OBJECTPROPERTY_TYPE);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			RDF.engine.GetObjectProperty(owlInstance, rdfProperty, out valuesPtr, out card);

			if (card > 0)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		/// <summary>
		///		GetObjectPropertyEx                         (http://rdf.bg/gkdoc/CS64/GetObjectPropertyEx.html)
		///
		///	This function gets the value(s) of a certain objectProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the values card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as GetObjectProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetObjectPropertyEx")]
		public static extern Int64 GetObjectPropertyEx(Int64 model, Int64 owlInstance, Int64 rdfProperty, out IntPtr values, out Int64 card);

		/// <summary>
		///		CreateInstanceInContextStructure            (http://rdf.bg/gkdoc/CS64/CreateInstanceInContextStructure.html)
		///
		///	InstanceInContext structures give you more detailed information about
		///	individual parts of the geometry of a certain instance viualized.
		///	It is allowed to have more then 1 InstanceInContext structures per instance.
		///	InstanceInContext structures are updated dynamically when the geometry
		///	structure is updated.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceInContextStructure")]
		public static extern Int64 CreateInstanceInContextStructure(Int64 owlInstance);

		/// <summary>
		///		DestroyInstanceInContextStructure           (http://rdf.bg/gkdoc/CS64/DestroyInstanceInContextStructure.html)
		///
		///	InstanceInContext structures are updated dynamically and therfore even while the cost
		///	in performance and memory is limited it is advised to destroy structures as soon
		///	as they are obsolete.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "DestroyInstanceInContextStructure")]
		public static extern void DestroyInstanceInContextStructure(Int64 owlInstanceInContext);

		/// <summary>
		///		InstanceInContextChild                      (http://rdf.bg/gkdoc/CS64/InstanceInContextChild.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InstanceInContextChild")]
		public static extern Int64 InstanceInContextChild(Int64 owlInstanceInContext);

		/// <summary>
		///		InstanceInContextNext                       (http://rdf.bg/gkdoc/CS64/InstanceInContextNext.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InstanceInContextNext")]
		public static extern Int64 InstanceInContextNext(Int64 owlInstanceInContext);

		/// <summary>
		///		InstanceInContextIsUpdated                  (http://rdf.bg/gkdoc/CS64/InstanceInContextIsUpdated.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InstanceInContextIsUpdated")]
		public static extern Int64 InstanceInContextIsUpdated(Int64 owlInstanceInContext);

		/// <summary>
		///		RemoveInstance                              (http://rdf.bg/gkdoc/CS64/RemoveInstance.html)
		///
		///	This function removes an instance from the internal structure.
		///	In case copies are created by the host this function checks if all
		///	copies are removed otherwise the instance will stay available.
		///	Return value is 0 if everything went ok and positive in case of an error
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveInstance")]
		public static extern Int64 RemoveInstance(Int64 owlInstance);

		/// <summary>
		///		RemoveInstanceRecursively                   (http://rdf.bg/gkdoc/CS64/RemoveInstanceRecursively.html)
		///
		///	This function removes an instance recursively from the internal structure.
		///	In case checkInverseRelations is non-zero only instances that are not referenced
		///	by other existing instances.
		///
		///	Return value is total number of removed instances
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveInstanceRecursively")]
		public static extern Int64 RemoveInstanceRecursively(Int64 owlInstance);

		/// <summary>
		///		RemoveInstances                             (http://rdf.bg/gkdoc/CS64/RemoveInstances.html)
		///
		///	This function removes all available instances for the given model 
		///	from the internal structure.
		///	Return value is the number of removed instances.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveInstances")]
		public static extern Int64 RemoveInstances(Int64 model);

		/// <summary>
		///		IsInstance                                  (http://rdf.bg/gkdoc/CS64/IsInstance.html)
		///
		///	Returns true if the argument owlInstance is an actual active property in an active model. It returns false in all other cases,
		///	i.e. this could mean the model is already closed, the instance is inactive or removed or the session is closed.
		///	It could also mean it represents a handle to another Thing, for example a class, property or model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsInstance")]
		public static extern byte IsInstance(Int64 owlInstance);

		/// <summary>
		///		IsKindOfClass                               (http://rdf.bg/gkdoc/CS64/IsKindOfClass.html)
		///
		///	...
		/// </summary>
		public static bool IsKindOfClass(Int64 myOwlClass, Int64 owlClass)
		{
			if (myOwlClass == owlClass)
				return	true;
			Int64	parentOwlClass = GetClassParentsByIterator(myOwlClass, 0);
			while (parentOwlClass != 0) {
				if (IsKindOfClass(parentOwlClass, owlClass))
					return	true;
				parentOwlClass = GetClassParentsByIterator(myOwlClass, parentOwlClass);
			}
			return	false;
		}

		/// <summary>
		///		IsInstanceOfClass                           (http://rdf.bg/gkdoc/CS64/IsInstanceOfClass.html)
		///
		///	...
		/// </summary>
		public static bool IsInstanceOfClass(Int64 owlInstance, string name)
		{
			return IsKindOfClass(GetInstanceClass(owlInstance), GetClassByName(GetModel(owlInstance), name));
		}

		public static bool IsInstanceOfClass(Int64 owlInstance, byte[] name)
		{
			return IsKindOfClass(GetInstanceClass(owlInstance), GetClassByName(GetModel(owlInstance), name));
		}

		/// <summary>
		///		IsInstanceOfClassExact                      (http://rdf.bg/gkdoc/CS64/IsInstanceOfClassExact.html)
		///
		///	...
		/// </summary>
		public static bool IsInstanceOfClassExact(Int64 owlInstance, string name)
		{
			return GetInstanceClass(owlInstance) == GetClassByName(GetModel(owlInstance), name);
		}

		public static bool IsInstanceOfClassExact(Int64 owlInstance, byte[] name)
		{
			return GetInstanceClass(owlInstance) == GetClassByName(GetModel(owlInstance), name);
		}

        //
        //  Retrieve Geometry API Calls
        //

		/// <summary>
		///		CalculateInstance                           (http://rdf.bg/gkdoc/CS64/CalculateInstance.html)
		///
		///	This function prepares the content to be ready so the buffers can be filled.
		///	It returns the minimum size the buffers should be. This is only the case
		///	when the pointer is given, all arguments are allowed to be nullptr.
		///
		///	Note: This function needs to be called directly before UpdateVertexBuffer(),
		///		  UpdateIndexBuffer() and UpdateTransformationBuffer().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, out Int64 indexBufferSize, out Int64 transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, out Int64 indexBufferSize, IntPtr transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, IntPtr indexBufferSize, IntPtr transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, IntPtr vertexBufferSize, IntPtr indexBufferSize, IntPtr transformationBufferSize);

		public static Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, out Int64 indexBufferSize)
		{
			return	RDF.engine.CalculateInstance(owlInstance, out vertexBufferSize, out indexBufferSize, IntPtr.Zero);
		}

		/// <summary>
		///		UpdateInstance                              (http://rdf.bg/gkdoc/CS64/UpdateInstance.html)
		///
		///	This function prepares the content to be ready without filling the buffers
		///	as done within CalculateInstance(). CalculateInstance calls this function as a start.
		///	This function will also set the 'derived' values for the instance passed as argument.
		///	For example the coordinates values of a MultiplicationMatrix will be set if the array is
		///	defined.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstance")]
		public static extern Int64 UpdateInstance(Int64 owlInstance);

		/// <summary>
		///		InferenceInstance                           (http://rdf.bg/gkdoc/CS64/InferenceInstance.html)
		///
		///	This function fills in values that are implicitely known but not given by the user. This function
		///	can also be used to identify default values of properties if not given.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InferenceInstance")]
		public static extern Int64 InferenceInstance(Int64 owlInstance);

		/// <summary>
		///		UpdateInstanceVertexBuffer                  (http://rdf.bg/gkdoc/CS64/UpdateInstanceVertexBuffer.html)
		///
		///	This function should be preceded by the function CalculateInstances(),
		///	the only allowed other API functions in between are UpdateIndexBuffer()
		///	and UpdateTransformationBuffer().
		///	It is expected to be called with a buffer vertexBuffer of at least the size as 
		///	given by CalculateInstances().
		///	If not called for the first time it will expect to contain the same content as
		///	from previous call, even is size is changed. This can be overruled by 
		///	the function ClearedExternalBuffers().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, out float vertexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, float[] vertexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, out double vertexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, double[] vertexBuffer);

		/// <summary>
		///		UpdateInstanceVertexBufferTrimmed           (http://rdf.bg/gkdoc/CS64/UpdateInstanceVertexBufferTrimmed.html)
		///
		///	This function is an alternative for UpdateInstanceVertexBuffer(),
		///	in case the vertex buffer should be divided over a set of arrays
		///	this function allows to fill part of the vertex buffer given a
		///	certain offset and size (both calculated in vertex element count).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, out float vertexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, float[] vertexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, out double vertexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, double[] vertexBuffer, Int64 offset, Int64 size);

		/// <summary>
		///		UpdateInstanceIndexBuffer                   (http://rdf.bg/gkdoc/CS64/UpdateInstanceIndexBuffer.html)
		///
		///	This function should be preceded by the function CalculateInstances(),
		///	the only allowed other API functions in between are UpdateVertexBuffer()
		///	and UpdateTransformationBuffer().
		///	It is expected to be called with a buffer indexBuffer of at least the size as 
		///	given by CalculateInstances().
		///	If not called for the first time it will expect to contain the same content as
		///	from previous call, even is size is changed. This can be overruled by 
		///	the function ClearedExternalBuffers().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, out Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, Int32[] indexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, out Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, Int64[] indexBuffer);

		/// <summary>
		///		UpdateInstanceIndexBufferTrimmed            (http://rdf.bg/gkdoc/CS64/UpdateInstanceIndexBufferTrimmed.html)
		///
		///	This function is an alternative for UpdateInstanceIndexBuffer(),
		///	in case the index buffer should be divided over a set of arrays
		///	this function allows to fill part of the index buffer given a
		///	certain offset and size.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, out Int32 indexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, Int32[] indexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, out Int64 indexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, Int64[] indexBuffer, Int64 offset, Int64 size);

		/// <summary>
		///		UpdateInstanceTransformationBuffer          (http://rdf.bg/gkdoc/CS64/UpdateInstanceTransformationBuffer.html)
		///
		///	This function should be preceded by the function CalculateInstances(),
		///	the only allowed other API functions in between are UpdateVertexBuffer()
		///	and UpdateIndexBuffer().
		///	It is expected to be called with a buffer vertexBuffer of at least the size as 
		///	given by CalculateInstances().
		///	If not called for the first time it will expect to contain the same content as
		///	from previous call, even is size is changed. This can be overruled by 
		///	the function ClearedExternalBuffers().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceTransformationBuffer")]
		public static extern Int64 UpdateInstanceTransformationBuffer(Int64 owlInstance, out double transformationBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceTransformationBuffer")]
		public static extern Int64 UpdateInstanceTransformationBuffer(Int64 owlInstance, double[] transformationBuffer);

		/// <summary>
		///		ClearedInstanceExternalBuffers              (http://rdf.bg/gkdoc/CS64/ClearedInstanceExternalBuffers.html)
		///
		///	This function tells the engine that all buffers have no memory of earlier filling 
		///	for a certain instance.
		///	This means that even when buffer content didn't changed it will be updated when
		///	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
		///	are called for this specific instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ClearedInstanceExternalBuffers")]
		public static extern void ClearedInstanceExternalBuffers(Int64 owlInstance);

		/// <summary>
		///		ClearedExternalBuffers                      (http://rdf.bg/gkdoc/CS64/ClearedExternalBuffers.html)
		///
		///	This function tells the engine that all buffers have no memory of earlier filling.
		///	This means that even when buffer content didn't changed it will be updated when
		///	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
		///	are called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ClearedExternalBuffers")]
		public static extern void ClearedExternalBuffers(Int64 model);

		/// <summary>
		///		GetConceptualFaceCnt                        (http://rdf.bg/gkdoc/CS64/GetConceptualFaceCnt.html)
		///
		///	This function returns the number of conceptual faces for a certain instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceCnt")]
		public static extern Int64 GetConceptualFaceCnt(Int64 owlInstance);

		/// <summary>
		///		GetConceptualFace                           (http://rdf.bg/gkdoc/CS64/GetConceptualFace.html)
		///
		///	This function returns a handle to the conceptual face. Be aware that different
		///	instances can return the same handles (however with possible different startIndices and noIndicesTriangles).
		///	Argument index should be at least zero and smaller then return value of GetConceptualFaceCnt().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		public static Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints)
		{
			return RDF.engine.GetConceptualFace(owlInstance, index, out startIndexTriangles, out noIndicesTriangles, out startIndexLines, out noIndicesLines, out startIndexPoints, out noIndicesPoints, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero);
		}

		public static Int64 GetConceptualFace(Int64 owlInstance, Int64 index)
		{
			return RDF.engine.GetConceptualFace(owlInstance, index, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		GetConceptualFaceMaterial                   (http://rdf.bg/gkdoc/CS64/GetConceptualFaceMaterial.html)
		///
		///	This function returns the material instance relevant for this
		///	conceptual face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceMaterial")]
		public static extern Int64 GetConceptualFaceMaterial(Int64 conceptualFace);

		/// <summary>
		///		GetConceptualFaceOriginCnt                  (http://rdf.bg/gkdoc/CS64/GetConceptualFaceOriginCnt.html)
		///
		///	This function returns the number of instances that are the source primitive/concept
		///	for this conceptual face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceOriginCnt")]
		public static extern Int64 GetConceptualFaceOriginCnt(Int64 conceptualFace);

		/// <summary>
		///		GetConceptualFaceOrigin                     (http://rdf.bg/gkdoc/CS64/GetConceptualFaceOrigin.html)
		///
		///	This function returns a handle to the instance that is the source primitive/concept
		///	for this conceptual face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceOrigin")]
		public static extern Int64 GetConceptualFaceOrigin(Int64 conceptualFace, Int64 index);

		/// <summary>
		///		GetConceptualFaceOriginEx                   (http://rdf.bg/gkdoc/CS64/GetConceptualFaceOriginEx.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceOriginEx")]
		public static extern void GetConceptualFaceOriginEx(Int64 conceptualFace, Int64 index, out Int64 originatingOwlInstance, out Int64 originatingConceptualFace);

		/// <summary>
		///		GetFaceCnt                                  (http://rdf.bg/gkdoc/CS64/GetFaceCnt.html)
		///
		///	This function returns the number of faces for a certain instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetFaceCnt")]
		public static extern Int64 GetFaceCnt(Int64 owlInstance);

		/// <summary>
		///		GetFace                                     (http://rdf.bg/gkdoc/CS64/GetFace.html)
		///
		///	This function gets the individual faces including the meta data, i.e. the number of openings within this specific face.
		///	This call is for very dedicated use, it would be more common to iterate over the individual conceptual faces.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetFace")]
		public static extern void GetFace(Int64 owlInstance, Int64 index, out Int64 startIndex, out Int64 noOpenings);

		/// <summary>
		///		GetDependingPropertyCnt                     (http://rdf.bg/gkdoc/CS64/GetDependingPropertyCnt.html)
		///
		///	This function returns the number of properties that are of influence on the
		///	location and form of the conceptualFace.
		///
		///	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDependingPropertyCnt")]
		public static extern Int64 GetDependingPropertyCnt(Int64 baseOwlInstance, Int64 conceptualFace);

		/// <summary>
		///		GetDependingProperty                        (http://rdf.bg/gkdoc/CS64/GetDependingProperty.html)
		///
		///	This function returns a handle to the property that is the 'index'-th property
		///	of influence on the form. It also returns the handle to instance this property
		///	belongs to.
		///
		///	Note: the returned property is always a datatypeProperty
		///	Note: if input is incorrect (for example index is in wrong domain) _property and
		///		  instance will be both zero.
		///	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDependingProperty")]
		public static extern void GetDependingProperty(Int64 baseOwlInstance, Int64 conceptualFace, Int64 index, out Int64 owlInstance, out Int64 datatypeProperty);

		/// <summary>
		///		SetFormat                                   (http://rdf.bg/gkdoc/CS64/SetFormat.html)
		///
		///	This function sets the type of export format, by setting a mask
		///		bit 0 & 1:
		///			00	Each vertex is unique (although mostly irrelevant UpdateIndexBuffer() and 
		///				UpdateTransformationBuffer() are still returning information)
		///			01	Each index is unique => vertices are not necessarily (although mostly
		///				irrelevant UpdateTransformationBuffer() is still returning information)
		///			10	Single level Transformations are used, most optimal when using DirectX till version 11
		///				and OpenGL till version 2
		///			11	Nested Transformations are used, most optimal but till 2011 no known support of
		///				low level 3D interfaces like DirectX and OpenGL
		///		bit 2:	(FORMAT_SIZE_VERTEX_DOUBLE)
		///			0	Vertex items returned as float (4 byte/32 bit)
		///			1	Vertex items returned as double (8 byte/64 bit)
		///		bit 3:	(FORMAT_SIZE_INDEX_INT64)
		///			0	Index items returned as int32_t (4 byte/32 bit)
		///			1	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode)
		///
		///		bit 4:	(FORMAT_VERTEX_POINT)
		///			0	Vertex does not contain 3D point info
		///			1	Vertex does contain 3D point info
		///		bit 5:	(FORMAT_VERTEX_NORMAL)
		///			0	Vertex does not contain 3D normal vector info
		///			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
		///		bit 6:	(FORMAT_VERTEX_TEXTURE_UV)
		///			0	Vertex does not contain first 2D texture info
		///			1	Vertex does contain first 2D texture info
		///		bit 7:	(FORMAT_VERTEX_TEXTURE2_UV)
		///			0	Vertex does not contain second 2D texture info
		///			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
		///
		///		bit 8:	(FORMAT_EXPORT_TRIANGLES)
		///			0	No object form triangles are exported
		///			1	Object form triangles are exported (effective if instance contains faces and/or solids)
		///		bit 9:	(FORMAT_EXPORT_LINES)
		///			0	No object polygon lines are exported
		///			1	Object polygon lines are exported (effective if instance contains line representations)
		///		bit 10:	(FORMAT_EXPORT_POINTS)
		///			0	No object points are exported
		///			1	Object points are exported (effective if instance contains point representations)
		///
		///		bit 11:	Reserved, by default 0
		///
		///		bit 12:	(FORMAT_EXPORT_FACE_POLYGONS)
		///			0	No object face polygon lines are exported
		///			1	Object face polygon lines (dense wireframe) are exported => if set, bit 8 will also be set
		///		bit 13:	(FORMAT_EXPORT_CONCEPTUAL_FACE_POLYGONS)
		///			0	No object conceptual face polygon lines are exported
		///			1	Object conceptual face polygon lines (wireframe) are exported => if set, bit 12 will also be set
		///		bit 14:	(FORMAT_EXPORT_POLYGONS_AS_TUPLES)
		///			0	Polygon lines (wireframe) exported as list, i.e. typical 4 point polygon exported as  0 1 2 3 0 -1
		///			1	Polygon lines (wireframe) exported as tuples, i.e. typical 4 point polygon exported as 0 1 1 2 2 3 3 0
		///
		///		bit 15:	(FORMAT_EXPORT_ADVANCED_NORMALS)
		///			0	All normals of triangles are transformed orthogonal to the 2D face they belong to
		///			1	Normals are exported to be in line with the original semantic form description (could be non orthogonal to the 2D face) 
		///
		///		bit 16:	(FORMAT_EXPORT_DIRECTX)
		///			0	no specific behavior
		///			1	Where possible DirectX compatibility is given to exported data (i.e. order of components in vertex buffer)
		///					 => [bit 20, bit 21 both UNSET]
		///					 => if set, bit 17 will be unset
		///
		///		bit 17:	(FORMAT_EXPORT_OPENGL)
		///			0	no specific behavior
		///			1	Where possible OpenGL compatibility is given to exported data (i.e. order of components in vertex buffer and inverted texture coordinates in Y direction)
		///					 => [bit 20, bit 21 both SET]
		///					 => if set, bit 16 will be unset
		///
		///		bit 18:	(FORMAT_EXPORT_DOUBLE_SIDED)
		///			0	All faces are defined as calculated
		///			1	Every face has exactly one opposite face (normally both index and vertex buffer are doubled in size)
		///
		///		bit 19:	Reserved, by default 0
		///
		///		bit 20-23:
		///			0000	version 0 (used in case there is different behavior between versions in DirectX or OpenGL)
		///			....	...
		///			1111	version 15
		///
		///		bit 20:	(FORMAT_EXPORT_VERSION_0001)
		///			0	Standard Triangle Rotation (LHS as expected by DirectX) 
		///			1	Opposite Triangle Rotation (RHS as expected by OpenGL)
		///		bit 21:	(FORMAT_EXPORT_VERSION_0010)
		///			0	X, Y, Z (nX, nY, nZ) formatted as <X Y Z> considering internal concepts
		///			1	X, Y, Z (nX, nY, nZ) formatted as <X -Z Y>, i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL)
		///
		///		bit 24:	(FORMAT_VERTEX_COLOR_AMBIENT)
		///			0	Vertex does not contain Ambient color information
		///			1	Vertex does contain Ambient color information
		///		bit 25:	(FORMAT_VERTEX_COLOR_DIFFUSE)
		///			0	Vertex does not contain Diffuse color information
		///			1	Vertex does contain Diffuse color information
		///		bit 26:	(FORMAT_VERTEX_COLOR_EMISSIVE)
		///			0	Vertex does not contain Emissive color information
		///			1	Vertex does contain Emissive color information
		///		bit 27:	(FORMAT_VERTEX_COLOR_SPECULAR)
		///			0	Vertex does not contain Specular color information
		///			1	Vertex does contain Specular color information
		///
		///		bit 28:	(FORMAT_VERTEX_TEXTURE_TANGENT)
		///			0	Vertex does not contain tangent vector for first texture
		///			1	Vertex does contain tangent vector for first texture => if set, bit 6 will also be set
		///		bit 29:	(FORMAT_VERTEX_TEXTURE_BINORMAL)
		///			0	Vertex does not contain binormal vector for first texture
		///			1	Vertex does contain binormal vector for first texture => if set, bit 6 will also be set
		///		bit 30:	(FORMAT_VERTEX_TEXTURE2_TANGENT)		ONLY WORKS IN 64 BIT MODE
		///			0	Vertex does not contain tangent vector for second texture
		///			1	Vertex does contain tangent vector for second texture => if set, bit 6 will also be set
		///		bit 31:	(FORMAT_VERTEX_TEXTURE2_BINORMAL)		ONLY WORKS IN 64 BIT MODE
		///			0	Vertex does not contain binormal vector for second texture
		///			1	Vertex does contain binormal vector for second texture => if set, bit 6 will also be set
		///
		///		bit 26-31:	Reserved, by default 0
		///
		///		bit 32-63:	Reserved, by default 0
		///
		///	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   1000 0001  0011 0000 = h0000 0000 - 0000 8130 = 33072
		///
		///
		///	Depending on FORMAT_SIZE_VERTEX_DOUBLE each element in the vertex buffer is a double or float number.
		///	Number of elements for each vertex depends on format setting. You can get the number by GetVertexElementsCounts. 
		///	Each vertex block contains data items in an order according to the table below. The table also specifies when an item is present and number of elements 
		///	it occupied. Use GetVertexDataOffset or GetVertexColor to get required item. 
		///
		///	#	Vertex data item	Included when format setting bit is on					Size (num of elements)
		///	Point coordinates		X, Y, X				FORMAT_VERTEX_POINT	(bit 4)					3
		///	Normal coordinates		Nx, Ny, Nz			FORMAT_VERTEX_NORMAL (bit 5)				3
		///	Texture coordinates		T1u, T1v			FORMAT_VERTEX_TEXTURE_UV (bit 6)			2
		///	2nd Texture coordinates	T2u, T2v			FORMAT_VERTEX_TEXTURE2_UV (bit 7)			2
		///	Ambient color								FORMAT_VERTEX_COLOR_AMBIENT (bit 24)		1
		///	Diffuse color								FORMAT_VERTEX_COLOR_DIFFUSE (bit 25)		1
		///	Emissive color								FORMAT_VERTEX_COLOR _EMISSIVE (bit 26)		1
		///	Specular color								FORMAT_VERTEX_COLOR _SPECULAR (bit 27)		1
		///	Texture tangent			T1Tx, T1Ty, T1Tz	FORMAT_VERTEX_TEXTURE_TANGENT (bit 28)		3
		///	Texture binormal		T1BNx,T1BNy,T1BNz	FORMAT_VERTEX_TEXTURE_BINORMAL (bit 29)		3
		///	2nd texture tangent		T2Tx, T2Ty, T2Tz	FORMAT_VERTEX_TEXTURE2_TANGENT (bit 30)		3
		///	2nd texture binormal	T2BNx,T2BNy,T2BNz	FORMAT_VERTEX_TEXTURE2_BINORMAL (bit 31)	3
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetFormat")]
		public static extern Int64 SetFormat(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetFormat                                   (http://rdf.bg/gkdoc/CS64/GetFormat.html)
		///
		///	Returns the current format given a mask.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetFormat")]
		public static extern Int64 GetFormat(Int64 model, Int64 mask);

		/// <summary>
		///		GetVertexDataOffset                         (http://rdf.bg/gkdoc/CS64/GetVertexDataOffset.html)
		///
		///	Returns offset of the required data in a vertex elements array with the specified format settings
		///	requiredData is one of the control vertex data bits (FORMAT_VERTEX...) or 0 to get count of all elements in vertex buffer
		///	Functions returns -1 if the required data are absent with the settings.
		///
		///	Ensure your settings are actual. They may be differ you pass to SetFormat (for example because of mask)
		///	It's preferable to inquire resulting setting with GetFormat(model, GetFormat(0, 0))
		///
		///	Note: vertex buffer element is a double or a float number depending on FORMAT_SIZE_VERTEX_DOUBLE flag. 
		///	If you need offset in bytes multiply by size of element.
		///	Compare to SetFormat that returns size of vertex data in bytes.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVertexDataOffset")]
		public static extern Int32 GetVertexDataOffset(Int64 requiredData, Int64 setting);

		/// <summary>
		///		SetBehavior                                 (http://rdf.bg/gkdoc/CS64/SetBehavior.html)
		///
		///	This function sets the type of behavior, by setting a mask
		///
		///		bit 0-7:	Reserved, by default 0
		///
		///		bit 8:
		///			0	Do not optimize
		///			1	Vertex items returned as double (8 byte/64 bit)
		///
		///		bit 9-31:	Reserved, by default 0
		///
		///		bit 32-63:	Reserved, by default 0
		///
		///	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   0000 0001  0000 0000 = h0000 0000 - 0000 0100 = 256
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetBehavior")]
		public static extern void SetBehavior(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetBehavior                                 (http://rdf.bg/gkdoc/CS64/GetBehavior.html)
		///
		///	Returns the current behavior given a mask.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetBehavior")]
		public static extern Int64 GetBehavior(Int64 model, Int64 mask);

		/// <summary>
		///		SetVertexBufferTransformation               (http://rdf.bg/gkdoc/CS64/SetVertexBufferTransformation.html)
		///
		///	Sets the transformation for a Vertex Buffer.
		///	The transformation will always be calculated in 64 bit, even if the vertex element size is 32 bit.
		///	This function can be called just before updating the vertex buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetVertexBufferTransformation")]
		public static extern void SetVertexBufferTransformation(Int64 model, ref double matrix);

		/// <summary>
		///		GetVertexBufferTransformation               (http://rdf.bg/gkdoc/CS64/GetVertexBufferTransformation.html)
		///
		///	Gets the transformation for a Vertex Buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVertexBufferTransformation")]
		public static extern void GetVertexBufferTransformation(Int64 model, out double matrix);

		/// <summary>
		///		SetIndexBufferOffset                        (http://rdf.bg/gkdoc/CS64/SetIndexBufferOffset.html)
		///
		///	Sets the offset for an Index Buffer.
		///	It is important call this function before updating the vertex buffer. 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetIndexBufferOffset")]
		public static extern void SetIndexBufferOffset(Int64 model, Int64 offset);

		/// <summary>
		///		GetIndexBufferOffset                        (http://rdf.bg/gkdoc/CS64/GetIndexBufferOffset.html)
		///
		///	Gets the current offset for an Index Buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetIndexBufferOffset")]
		public static extern Int64 GetIndexBufferOffset(Int64 model);

		/// <summary>
		///		SetVertexBufferOffset                       (http://rdf.bg/gkdoc/CS64/SetVertexBufferOffset.html)
		///
		///	Sets the offset for a Vertex Buffer.
		///	The offset will always be calculated in 64 bit, even if the vertex element size is 32 bit.
		///	This function can be called just before updating the vertex buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetVertexBufferOffset")]
		public static extern void SetVertexBufferOffset(Int64 model, double x, double y, double z);

		/// <summary>
		///		GetVertexBufferOffset                       (http://rdf.bg/gkdoc/CS64/GetVertexBufferOffset.html)
		///
		///	Gets the offset for a Vertex Buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVertexBufferOffset")]
		public static extern void GetVertexBufferOffset(Int64 model, out double x, out double y, out double z);

		/// <summary>
		///		SetDefaultColor                             (http://rdf.bg/gkdoc/CS64/SetDefaultColor.html)
		///
		///	Set the default values for the colors defined as argument.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetDefaultColor")]
		public static extern void SetDefaultColor(Int64 model, UInt32 ambient, UInt32 diffuse, UInt32 emissive, UInt32 specular);

		/// <summary>
		///		GetDefaultColor                             (http://rdf.bg/gkdoc/CS64/GetDefaultColor.html)
		///
		///	Retrieve the default values for the colors defined as argument.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDefaultColor")]
		public static extern void GetDefaultColor(Int64 model, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		/// <summary>
		///		CheckConsistency                            (http://rdf.bg/gkdoc/CS64/CheckConsistency.html)
		///
		///	This function returns information about the consistency of each instance.
		///
		///	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
		///
		///		bit 0:	Check Design Tree Consistency
		///		bit 1:	Check Consistency for Triangle Output (through API)
		///		bit 2:	Check Consistency for Line Output (through API)
		///		bit 3:	Check Consistency for Point Output (through API)
		///		bit 4:	Check Consistency for Generated Surfaces (through API)
		///		bit 5:	Check Consistency for Generated Surfaces (internal)
		///		bit 6:	Check Consistency for Generated Solids (through API)
		///		bit 7:	Check Consistency for Generated Solids (internal)
		///		bit 8:	Check Consistency for BoundingBox's
		///		bit 9:	Check Consistency for Triangulation
		///		bit 10: Check Consistency for Relations (through API)
		///
		///		bit 16:	Contains (Closed) Solid(s)
		///		bit 18:	Contains (Closed) Infinite Solid(s)
		///		bit 20:	Contains Closed Surface(s)
		///		bit 21:	Contains Open Surface(s)
		///		bit 22:	Contains Closed Infinite Surface(s)
		///		bit 23:	Contains Open Infinite Surface(s)
		///		bit 24:	Contains Closed Line(s)
		///		bit 25:	Contains Open Line(s)
		///		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
		///		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
		///		bit 28:	Contains (Closed) Point(s)
		///
		///	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
		///	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected; 
		///	any non-zero return value in Contains is an indication that this type of geometry is expected in one of the instances; 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CheckConsistency")]
		public static extern Int64 CheckConsistency(Int64 model, Int64 mask);

		/// <summary>
		///		CheckInstanceConsistency                    (http://rdf.bg/gkdoc/CS64/CheckInstanceConsistency.html)
		///
		///	This function returns information about the consistency of the instance and indirectly referenced instances.
		///
		///	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
		///
		///		bit 0:	Check Design Tree Consistency
		///		bit 1:	Check Consistency for Triangle Output (through API)
		///		bit 2:	Check Consistency for Line Output (through API)
		///		bit 3:	Check Consistency for Point Output (through API)
		///		bit 4:	Check Consistency for Generated Surfaces (through API)
		///		bit 5:	Check Consistency for Generated Surfaces (internal)
		///		bit 6:	Check Consistency for Generated Solids (through API)
		///		bit 7:	Check Consistency for Generated Solids (internal)
		///		bit 8:	Check Consistency for BoundingBox's
		///		bit 9:	Check Consistency for Triangulation
		///		bit 10: Check Consistency for Relations (through API)
		///
		///		bit 16:	Contains (Closed) Solid(s)
		///		bit 18:	Contains (Closed) Infinite Solid(s)
		///		bit 20:	Contains Closed Surface(s)
		///		bit 21:	Contains Open Surface(s)
		///		bit 22:	Contains Closed Infinite Surface(s)
		///		bit 23:	Contains Open Infinite Surface(s)
		///		bit 24:	Contains Closed Line(s)
		///		bit 25:	Contains Open Line(s)
		///		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
		///		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
		///		bit 28:	Contains (Closed) Point(s)
		///
		///	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
		///	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected regarding the given instance; 
		///	any non-zero return value in Contains is an indication that this type of geometry is expected regarding the given instance; 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CheckInstanceConsistency")]
		public static extern Int64 CheckInstanceConsistency(Int64 owlInstance, Int64 mask);

		/// <summary>
		///		IsDuplicate                                 (http://rdf.bg/gkdoc/CS64/IsDuplicate.html)
		///
		///	Checks if two geometry representations are (almost) similar except for a transformation matrix.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsDuplicate")]
		public static extern byte IsDuplicate(Int64 originalOwlInstance, Int64 duplicateOwlInstance, out double duplicateMatrix, double epsilon, byte checkMaterial);

        //
        //  Derived Geometry API Calls
        //

		/// <summary>
		///		GetPerimeter                                (http://rdf.bg/gkdoc/CS64/GetPerimeter.html)
		///
		///	This function calculates the perimeter of an instance.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: due to internal structure using already calculated vertex buffer / index buffer does not
		///		  give any performance benefits, in opposite to GetVolume and GetArea
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPerimeter")]
		public static extern double GetPerimeter(Int64 owlInstance);

		/// <summary>
		///		GetArea                                     (http://rdf.bg/gkdoc/CS64/GetArea.html)
		///
		///	This function calculates the area of an instance.
		///	For perfomance reasons it is benefitial to call it with vertex and index buffer when
		///	the arrays are calculated anyway or Volume and Area are needed.
		///
		///	There are two ways to call GetVolume:
		///		vertexBuffer and indexBuffer are both zero: in this case the instance will be
		///				recalculated when needed. It is expected the client does not
		///				need the arrays itself or there is no performance issue.
		///		vertexBuffer and indexBuffer are both given: the call is placed directly after
		///				updateBuffer calls and no structural change to depending instances have 
		///				been done in between. The transformationMatrix array is not needed,
		///				even if it is being used due to not giving any performance gain to this
		///				operation.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: in case precision is important and vertex buffer is 32 bit it is advised to
		///		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer);

		public static double GetArea(Int64 owlInstance)
		{
			return RDF.engine.GetArea(owlInstance, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		GetVolume                                   (http://rdf.bg/gkdoc/CS64/GetVolume.html)
		///
		///	This function calculates the volume of an instance.
		///	For perfomance reasons it is benefitial to call it with vertex and index buffer when
		///	the arrays are calculated anyway or Volume and Area are needed.
		///
		///	There are two ways to call GetVolume:
		///		vertexBuffer and indexBuffer are both zero: in this case the instance will be
		///				recalculated when needed. It is expected the client does not
		///				need the arrays itself or there is no performance issue.
		///		vertexBuffer and indexBuffer are both given: the call is placed directly after
		///				updateBuffer calls and no structural change to depending instances have 
		///				been done in between. The transformationMatrix array is not needed,
		///				even if it is being used due to not giving any performance gain to this
		///				operation.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: in case precision is important and vertex buffer is 32 bit it is advised to
		///		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer);

		public static double GetVolume(Int64 owlInstance)
		{
			return RDF.engine.GetVolume(owlInstance, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		GetCenter                                   (http://rdf.bg/gkdoc/CS64/GetCenter.html)
		///
		///	This function calculates the center of an instance.
		///	For perfomance reasons it is benefitial to call it with vertex and index buffer when
		///	the arrays are calculated anyway or Volume and Area are needed.
		///
		///	There are two ways to call GetCenter:
		///		vertexBuffer and indexBuffer are both zero: in this case the instance will be
		///				recalculated when needed. It is expected the client does not
		///				need the arrays itself or there is no performance issue.
		///		vertexBuffer and indexBuffer are both given: the call is placed directly after
		///				updateBuffer calls and no structural change to depending instances have 
		///				been done in between. The transformationMatrix array is not needed,
		///				even if it is being used due to not giving any performance gain to this
		///				operation.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: in case precision is important and vertex array is 32 bit it is advised to
		///		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer, out double center);

		public static void GetCenter(Int64 owlInstance, out double center)
		{
			RDF.engine.GetCenter(owlInstance, IntPtr.Zero, IntPtr.Zero, out center);
		}

		/// <summary>
		///		GetCentroid                                 (http://rdf.bg/gkdoc/CS64/GetCentroid.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer, out double centroid);

		public static double GetCentroid(Int64 owlInstance, out double centroid)
		{
			return RDF.engine.GetCentroid(owlInstance, IntPtr.Zero, IntPtr.Zero, out centroid);
		}

		/// <summary>
		///		GetConceptualFacePerimeter                  (http://rdf.bg/gkdoc/CS64/GetConceptualFacePerimeter.html)
		///
		///	This function returns the perimeter of a given Conceptual Face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFacePerimeter")]
		public static extern double GetConceptualFacePerimeter(Int64 conceptualFace);

		/// <summary>
		///		GetConceptualFaceArea                       (http://rdf.bg/gkdoc/CS64/GetConceptualFaceArea.html)
		///
		///	This function returns the area of a given Conceptual Face. The attributes vertex buffer
		///	and index buffer are optional but will improve performance if defined.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref float vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref float vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref double vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref double vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, IntPtr vertexBuffer, IntPtr indexBuffer);

		public static double GetConceptualFaceArea(Int64 conceptualFace)
		{
			return RDF.engine.GetConceptualFaceArea(conceptualFace, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		SetBoundingBoxReference                     (http://rdf.bg/gkdoc/CS64/SetBoundingBoxReference.html)
		///
		///	This function passes addresses from the hosting application. This enables
		///	the engine to update these values without extra need for API calls. This is
		///	especially of interest because the hosting application is not aware of what
		///	instances are updated and 
		///	The transformationMatrix has 12 double values: _11, _12, _13, _21, _22, _23, 
		///	_31, _32, _33, _41, _42, _43.
		///	The startVector is the leftundernear vector and the endVector is the 
		///	rightupperfar vector, in all cases values are doubles (64 bit).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetBoundingBoxReference")]
		public static extern void SetBoundingBoxReference(Int64 owlInstance, out double transformationMatrix, out double startVector, out double endVector);

		/// <summary>
		///		GetBoundingBox                              (http://rdf.bg/gkdoc/CS64/GetBoundingBox.html)
		///
		///	When the transformationMatrix is given, it will fill an array of 12 double values.
		///	When the transformationMatrix is left empty and both startVector and endVector are
		///	given the boundingbox without transformation is calculated and returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetBoundingBox")]
		public static extern byte GetBoundingBox(Int64 owlInstance, out double transformationMatrix, out double startVector, out double endVector);

		[DllImport(enginedll, EntryPoint = "GetBoundingBox")]
		public static extern byte GetBoundingBox(Int64 owlInstance, IntPtr transformationMatrix, out double startVector, out double endVector);

		public static byte GetBoundingBox(Int64 owlInstance, out double startVector, out double endVector)
		{
			return	RDF.engine.GetBoundingBox(owlInstance, IntPtr.Zero, out startVector, out endVector);
		}

		/// <summary>
		///		GetRelativeTransformation                   (http://rdf.bg/gkdoc/CS64/GetRelativeTransformation.html)
		///
		///	This function returns the relative transformation matrix between two instances, i.e. in practise
		///	this means the matrices connected to the Transformation instances in the path in between.
		///	The matrix is only given when a unique path through inverse relations can be found,
		///	otherwise the identity matrix is returned.
		///	owlInstanceHead is allowed to be not defined, i.e. zero.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRelativeTransformation")]
		public static extern void GetRelativeTransformation(Int64 owlInstanceHead, Int64 owlInstanceTail, out double transformationMatrix);

		/// <summary>
		///		GetDistance                                 (http://rdf.bg/gkdoc/CS64/GetDistance.html)
		///
		///	This function returns the shortest distance between two instances.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDistance")]
		public static extern double GetDistance(Int64 firstOwlInstance, Int64 secondOwlInstance, out double pointFirstInstance, out double pointSecondInstance);

		/// <summary>
		///		GetColorOfComponent                         (http://rdf.bg/gkdoc/CS64/GetColorOfComponent.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetColorOfComponent(Int64 owlInstanceColorComponent)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColorComponent, "ColorComponent"));
			Int64 model = GetModel(owlInstanceColorComponent);

			string[] rgbwNames = { "R", "G", "B", "W" };
			double[] rgbwValues = { 0.0, 0.0, 0.0, 0.0 };

			for (int i = 0; i < 4; i++) {
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				RDF.engine.GetDatatypeProperty(owlInstanceColorComponent, RDF.engine.GetPropertyByName(model, rgbwNames[i]), out valuesPtr, out card);
				if (card == 1) {
					double[] values = new double[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int)card);
					rgbwValues[i] = values[0];
				}
			}

			return RDF.COLOR.RGBW(rgbwValues);
		}

		/// <summary>
		///		SetColorOfComponent                         (http://rdf.bg/gkdoc/CS64/SetColorOfComponent.html)
		///
		///	...
		/// </summary>
		public static void SetColorOfComponent(Int64 owlInstanceColorComponent, UInt32 color)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColorComponent, "ColorComponent"));

			Int64 model = GetModel(owlInstanceColorComponent);

			string[] rgbwNames = { "R", "G", "B", "W" };
			double[] rgbwValues = RDF.COLOR.GET_COMPONENTS(color);

			for (int i = 0; i < 4; i++) {
				RDF.engine.SetDatatypeProperty(owlInstanceColorComponent, RDF.engine.GetPropertyByName(model, rgbwNames[i]), rgbwValues[i]);
			}
		}

		/// <summary>
		///		GetColor                                    (http://rdf.bg/gkdoc/CS64/GetColor.html)
		///
		///	...
		/// </summary>
		public static void GetColor(Int64 owlInstanceColor, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColor, "Color"));

			GetDefaultColor(GetModel(owlInstanceColor), out ambient, out diffuse, out emissive, out specular);

			string[] componentNames = { "ambient", "diffuse", "emissive", "specular" };
			UInt32[] componentColors = { ambient, diffuse, emissive, specular };

			for (int i = 0; i < 4; i++)
			{
				if (componentColors[i] != 0)
				{
					Int64 card = 0;
					IntPtr valuesPtr = IntPtr.Zero;
					GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), componentNames[i]), out valuesPtr, out card);

					if (card == 1)
					{
						Int64[] values = new Int64[card];
						System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
						Int64 owlInstanceColorComponent = values[0];
						componentColors[i] = GetColorOfComponent(owlInstanceColorComponent);
					}
				}
			}
		}

		/// <summary>
		///		SetColor                                    (http://rdf.bg/gkdoc/CS64/SetColor.html)
		///
		///	...
		/// </summary>
		public static void SetColor(Int64 owlInstanceColor, UInt32 ambient, UInt32 diffuse, UInt32 emissive, UInt32 specular)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColor, "Color"));

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "ambient"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], ambient);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), ambient);
				}
			}

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "diffuse"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], diffuse);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), diffuse);
				}
			}

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "emissive"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], emissive);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), emissive);
				}
			}

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "specular"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], specular);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), specular);
				}
			}
		}

		/// <summary>
		///		GetMaterialColor                            (http://rdf.bg/gkdoc/CS64/GetMaterialColor.html)
		///
		///	This function returns the color definition of any material instance. It will return default material
		///	in case the material does not have that specific color component defined.
		/// </summary>
		public static void GetMaterialColor(Int64 owlInstanceMaterial, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceMaterial, "Material"));

			GetDefaultColor(GetModel(owlInstanceMaterial), out ambient, out diffuse, out emissive, out specular);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetObjectProperty(owlInstanceMaterial, GetPropertyByName(GetModel(owlInstanceMaterial), "color"), out valuesPtr, out card);

			if (card == 1)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				if (values[0] != 0)
				{
					GetColor(values[0], out ambient, out diffuse, out emissive, out specular);
				}
			}
		}

		/// <summary>
		///		SetMaterialColor                            (http://rdf.bg/gkdoc/CS64/SetMaterialColor.html)
		///
		///	This function defines the color definition of any material instance.
		/// </summary>
		public static void SetMaterialColor(Int64 owlInstanceMaterial, UInt32 ambient, UInt32 diffuse, UInt32 emissive, UInt32 specular)
		{
			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetObjectProperty(owlInstanceMaterial, GetPropertyByName(GetModel(owlInstanceMaterial), "color"), out valuesPtr, out card);

			if (card == 1)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				SetColor(values[0], ambient, diffuse, emissive, specular);
			}
			else
			{
				SetColor(CreateInstance(GetClassByName(GetModel(owlInstanceMaterial), "color")), ambient, diffuse, emissive, specular);
			}
		}

		/// <summary>
		///		GetMaterialColorAmbient                     (http://rdf.bg/gkdoc/CS64/GetMaterialColorAmbient.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorAmbient(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return ambient;
		}

		/// <summary>
		///		GetMaterialColorDiffuse                     (http://rdf.bg/gkdoc/CS64/GetMaterialColorDiffuse.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorDiffuse(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return diffuse;
		}

		/// <summary>
		///		GetMaterialColorEmissive                    (http://rdf.bg/gkdoc/CS64/GetMaterialColorEmissive.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorEmissive(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return emissive;
		}

		/// <summary>
		///		GetMaterialColorSpecular                    (http://rdf.bg/gkdoc/CS64/GetMaterialColorSpecular.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorSpecular(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return specular;
		}

		/// <summary>
		///		GetPropertyRestrictionsConsolidated         (http://rdf.bg/gkdoc/CS64/GetPropertyRestrictionsConsolidated.html)
		///
		///	...
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyRestrictionsConsolidated")]
		public static extern void GetPropertyRestrictionsConsolidated(Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

        /// <summary>
        ///		GetClassPropertyAggregatedCardinalityRestriction         (http://rdf.bg/gkdoc/CS64/GetClassPropertyAggregatedCardinalityRestriction.html)
        ///
        ///	...
        /// </summary>
        [DllImport(enginedll, EntryPoint = "GetClassPropertyAggregatedCardinalityRestriction")]
        public static extern void GetClassPropertyAggregatedCardinalityRestriction(Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

        /// <summary>
        ///		GetVertexColor                              (http://rdf.bg/gkdoc/CS64/GetVertexColor.html)
        ///
        ///	Returns vertex color
        ///	requiredColor is one of the control vertex data bits applied to colors (FORMAT_VERTEX_COLOR...) 
        ///	If vertex format does provide required color, the model default color will be used
        /// </summary>
        [DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, IntPtr emissive, out UInt32 specular);

		/// <summary>
		///		GetVertexColorAmbient                       (http://rdf.bg/gkdoc/CS64/GetVertexColorAmbient.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorAmbient(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return ambient;
		}

		public static UInt32 GetVertexColorAmbient(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return ambient;
		}

		/// <summary>
		///		GetVertexColorDiffuse                       (http://rdf.bg/gkdoc/CS64/GetVertexColorDiffuse.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorDiffuse(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return diffuse;
		}

		public static UInt32 GetVertexColorDiffuse(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return diffuse;
		}

		/// <summary>
		///		GetVertexColorEmissive                      (http://rdf.bg/gkdoc/CS64/GetVertexColorEmissive.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorEmissive(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return emissive;
		}

		public static UInt32 GetVertexColorEmissive(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return emissive;
		}

		/// <summary>
		///		GetVertexColorSpecular                      (http://rdf.bg/gkdoc/CS64/GetVertexColorSpecular.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorSpecular(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return specular;
		}

		public static UInt32 GetVertexColorSpecular(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			RDF.engine.GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return specular;
		}
    }
}
