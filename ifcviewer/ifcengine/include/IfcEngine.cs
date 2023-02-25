using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Linq;
#if _WIN64
		using int_t = System.Int64;
#else
		using int_t = System.Int32;
#endif

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

	public enum enum_express_declaration : byte
	{
		__UNDEF = 0,
		__ENTITY,
		__ENUM,
		__SELECT,
		__DEFINED_TYPE
	};

	public enum enum_express_attr_type : byte
	{
		__NONE = 0, //attribute type is defined by reference domain entity
		__BINARY,
		__BINARY_32,
		__BOOLEAN,
		__ENUMERATION,
		__INTEGER,
		__LOGICAL,
		__NUMBER,
		__REAL,
		__SELECT,
		__STRING
	};

	public enum enum_express_aggr : byte
	{
		__NONE = 0,
		__ARRAY,
		__BAG,
		__LIST,
		__SET
	};

	public enum enum_validation_type : uint64_t
	{
		__NONE						= 0,
		__NO_OF_ARGUMENTS			= 1 << 0,	//number of arguments
		__ARGUMENT_EXPRESS_TYPE		= 1 << 1,	//argument value is correct entity, defined type or enumeration value
		__ARGUMENT_PRIM_TYPE		= 1 << 2,	//argument value has correct primitive type
		__REQUIRED_ARGUMENTS		= 1 << 3,	//non-optional arguments values are provided
		__ARRGEGATION_EXPECTED		= 1 << 4,	//aggregation is provided when expected
		__AGGREGATION_NOT_EXPECTED	= 1 << 5,   //aggregation is not used when not expected
		__AGGREGATION_SIZE			= 1 << 6,   //aggregation size
		__AGGREGATION_UNIQUE		= 1 << 7,	//elements in aggregations are unique when required
		__COMPLEX_INSTANCE			= 1 << 8,	//complex instances contains full parent chains
		__REFERENCE_EXISTS			= 1 << 9,	//referenced instance exists
		__ABSTRACT_ENTITY			= 1 << 10,  //abstract entity should not instantiate
		__WHERE_RULE				= 1 << 11,  //where-rule check
		__UNIQUE_RULE				= 1 << 12,	//unique-rule check
		__STAR_USAGE				= 1 << 13,  //* is used only for derived arguments
		__CALL_ARGUMENT				= 1 << 14,  //validateModel/validateInstance function argument should be model/instance
		__INTERNAL_ERROR			= 1 << 15   //unspecified error
	};

	public enum enum_validation_status : byte
	{
		__NONE = 0,
		__COMPLETE_ALL,		//all issues proceed
		__COMPLETE_NOT_ALL, //completed but some issues were excluded by option settings
		__TIME_EXCEED,		//validation was finished because of reach time limit
		__COUNT_EXCEED	    //validation was finished because of reach of issue's numbers limit
	};

	class ifcengine
	{
		public const int sdaiTYPE = 0; //C++ API generator specific

		public const int_t flagbit0 = 1;           // 2^^0    0000.0000..0000.0001
		public const int_t flagbit1 = 2;           // 2^^1    0000.0000..0000.0010
		public const int_t flagbit2 = 4;           // 2^^2    0000.0000..0000.0100
		public const int_t flagbit3 = 8;           // 2^^3    0000.0000..0000.1000
		public const int_t flagbit4 = 16;          // 2^^4    0000.0000..0001.0000
		public const int_t flagbit5 = 32;          // 2^^5    0000.0000..0010.0000
		public const int_t flagbit6 = 64;          // 2^^6    0000.0000..0100.0000
		public const int_t flagbit7 = 128;         // 2^^7    0000.0000..1000.0000
		public const int_t flagbit8 = 256;         // 2^^8    0000.0001..0000.0000
		public const int_t flagbit9 = 512;         // 2^^9    0000.0010..0000.0000
		public const int_t flagbit10 = 1024;       // 2^^10   0000.0100..0000.0000
		public const int_t flagbit11 = 2048;       // 2^^11   0000.1000..0000.0000
		public const int_t flagbit12 = 4096;       // 2^^12   0001.0000..0000.0000
		public const int_t flagbit13 = 8192;       // 2^^13   0010.0000..0000.0000
		public const int_t flagbit14 = 16384;      // 2^^14   0100.0000..0000.0000
		public const int_t flagbit15 = 32768;      // 2^^15   1000.0000..0000.0000

		public const int_t sdaiADB           = 1;
		public const int_t sdaiAGGR          = sdaiADB + 1;
		public const int_t sdaiBINARY        = sdaiAGGR + 1;
		public const int_t sdaiBOOLEAN       = sdaiBINARY + 1;
		public const int_t sdaiENUM          = sdaiBOOLEAN + 1;
		public const int_t sdaiINSTANCE      = sdaiENUM + 1;
		public const int_t sdaiINTEGER       = sdaiINSTANCE + 1;
		public const int_t sdaiLOGICAL       = sdaiINTEGER + 1;
		public const int_t sdaiREAL          = sdaiLOGICAL + 1;
		public const int_t sdaiSTRING        = sdaiREAL + 1;
		public const int_t sdaiUNICODE       = sdaiSTRING + 1;
		public const int_t sdaiEXPRESSSTRING = sdaiUNICODE + 1;
		public const int_t engiGLOBALID      = sdaiEXPRESSSTRING + 1;

		public const Int64 OBJECTPROPERTY_TYPE             = 1;
		public const Int64 DATATYPEPROPERTY_TYPE_BOOLEAN   = 2;
		public const Int64 DATATYPEPROPERTY_TYPE_CHAR      = 3;
		public const Int64 DATATYPEPROPERTY_TYPE_INTEGER   = 4;
		public const Int64 DATATYPEPROPERTY_TYPE_DOUBLE    = 5;
		public const Int64 DATATYPEPROPERTY_TYPE_BYTE      = 6;

		public const string IFCEngineDLL = @"IFCEngine.dll";

        //
        //  File IO API Calls
        //

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate Int64 ReadCallBackFunction(IntPtr value);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void WriteCallBackFunction(IntPtr value, Int64 size);

		/// <summary>
		///		sdaiCreateModelBN                                       (http://rdf.bg/ifcdoc/CS64/sdaiCreateModelBN.html)
		///
		///	This function creates and empty model (we expect with a schema file given).
		///	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		sdaiCreateModelBNUnicode                                (http://rdf.bg/ifcdoc/CS64/sdaiCreateModelBNUnicode.html)
		///
		///	This function creates and empty model (we expect with a schema file given).
		///	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		sdaiOpenModelBN                                         (http://rdf.bg/ifcdoc/CS64/sdaiOpenModelBN.html)
		///
		///	This function opens the model on location fileName.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		sdaiOpenModelBNUnicode                                  (http://rdf.bg/ifcdoc/CS64/sdaiOpenModelBNUnicode.html)
		///
		///	This function opens the model on location fileName.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		engiOpenModelByStream                                   (http://rdf.bg/ifcdoc/CS64/engiOpenModelByStream.html)
		///
		///	This function opens the model via a stream.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByStream")]
		public static extern int_t engiOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByStream")]
		public static extern int_t engiOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, byte[] schemaName);

		/// <summary>
		///		engiOpenModelByArray                                    (http://rdf.bg/ifcdoc/CS64/engiOpenModelByArray.html)
		///
		///	This function opens the model via an array.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByArray")]
		public static extern int_t engiOpenModelByArray(int_t repository, byte[] content, int_t size, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByArray")]
		public static extern int_t engiOpenModelByArray(int_t repository, byte[] content, int_t size, byte[] schemaName);

		/// <summary>
		///		sdaiSaveModelBN                                         (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelBN.html)
		///
		///	This function saves the model (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBN")]
		public static extern void sdaiSaveModelBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBN")]
		public static extern void sdaiSaveModelBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelBNUnicode                                  (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelBNUnicode.html)
		///
		///	This function saves the model (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBNUnicode")]
		public static extern void sdaiSaveModelBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBNUnicode")]
		public static extern void sdaiSaveModelBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		engiSaveModelByStream                                   (http://rdf.bg/ifcdoc/CS64/engiSaveModelByStream.html)
		///
		///	This function saves the model as an array.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveModelByStream")]
		public static extern void engiSaveModelByStream(int_t model, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, int_t size);

		/// <summary>
		///		engiSaveModelByArray                                    (http://rdf.bg/ifcdoc/CS64/engiSaveModelByArray.html)
		///
		///	This function saves the model as an array.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveModelByArray")]
		public static extern void engiSaveModelByArray(int_t model, byte[] content, out int_t size);

		/// <summary>
		///		sdaiSaveModelAsXmlBN                                    (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsXmlBN.html)
		///
		///	This function saves the model as XML according to IFC2x3's way of XML serialization (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBN")]
		public static extern void sdaiSaveModelAsXmlBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBN")]
		public static extern void sdaiSaveModelAsXmlBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsXmlBNUnicode                             (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsXmlBNUnicode.html)
		///
		///	This function saves the model as XML according to IFC2x3's way of XML serialization (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBNUnicode")]
		public static extern void sdaiSaveModelAsXmlBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBNUnicode")]
		public static extern void sdaiSaveModelAsXmlBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsSimpleXmlBN                              (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsSimpleXmlBN.html)
		///
		///	This function saves the model as XML according to IFC4's way of XML serialization (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBN")]
		public static extern void sdaiSaveModelAsSimpleXmlBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBN")]
		public static extern void sdaiSaveModelAsSimpleXmlBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsSimpleXmlBNUnicode                       (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsSimpleXmlBNUnicode.html)
		///
		///	This function saves the model as XML according to IFC4's way of XML serialization (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBNUnicode")]
		public static extern void sdaiSaveModelAsSimpleXmlBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBNUnicode")]
		public static extern void sdaiSaveModelAsSimpleXmlBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsJsonBN                                   (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsJsonBN.html)
		///
		///	This function saves the model as JSON according to IFC4's way of JSON serialization (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBN")]
		public static extern void sdaiSaveModelAsJsonBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBN")]
		public static extern void sdaiSaveModelAsJsonBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsJsonBNUnicode                            (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsJsonBNUnicode.html)
		///
		///	This function saves the model as JSON according to IFC4's way of JSON serialization (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBNUnicode")]
		public static extern void sdaiSaveModelAsJsonBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBNUnicode")]
		public static extern void sdaiSaveModelAsJsonBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		engiSaveSchemaBN                                        (http://rdf.bg/ifcdoc/CS64/engiSaveSchemaBN.html)
		///
		///	This function saves the schema.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBN")]
		public static extern byte engiSaveSchemaBN(int_t model, string filePath);

		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBN")]
		public static extern byte engiSaveSchemaBN(int_t model, byte[] filePath);

		/// <summary>
		///		engiSaveSchemaBNUnicode                                 (http://rdf.bg/ifcdoc/CS64/engiSaveSchemaBNUnicode.html)
		///
		///	This function saves the schema (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBNUnicode")]
		public static extern byte engiSaveSchemaBNUnicode(int_t model, string filePath);

		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBNUnicode")]
		public static extern byte engiSaveSchemaBNUnicode(int_t model, byte[] filePath);

		/// <summary>
		///		sdaiCloseModel                                          (http://rdf.bg/ifcdoc/CS64/sdaiCloseModel.html)
		///
		///	This function closes the model. After this call no instance handles will be available including all
		///	handles referencing the geometry of this specific file, in default compilation the model itself will
		///	be known in the kernel, however known to be disabled. Calls containing the model reference will be
		///	protected from crashing when called.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCloseModel")]
		public static extern void sdaiCloseModel(int_t model);

		/// <summary>
		///		setPrecisionDoubleExport                                (http://rdf.bg/ifcdoc/CS64/setPrecisionDoubleExport.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setPrecisionDoubleExport")]
		public static extern void setPrecisionDoubleExport(int_t model, int_t precisionCap, int_t precisionRound, byte clean);

        //
        //  Schema Reading API Calls
        //

		/// <summary>
		///		engiGetNextDeclarationIterator                          (http://rdf.bg/ifcdoc/CS64/engiGetNextDeclarationIterator.html)
		///
		///	This call returns next iterator of EXPRESS schema declarations.
		///	If the input iterator is NULL it returns first iterator.
		///	If the input iterator is last it returns NULL.
		///	Use engiGetDeclarationFromIterator to access EXPRESS declaration data. 
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetNextDeclarationIterator")]
		public static extern int_t engiGetNextDeclarationIterator(int_t model, int_t iterator);

		/// <summary>
		///		engiGetDeclarationFromIterator                          (http://rdf.bg/ifcdoc/CS64/engiGetDeclarationFromIterator.html)
		///
		///	This call returns handle to the EXPRESS schema declarations from iterator.
		///	It may be a handle to entity, or enumeration, select or type definition, use engiGetDeclarationType to recognize
		///	Use engiGetNextDeclarationIterator to get iterator.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetDeclarationFromIterator")]
		public static extern int_t engiGetDeclarationFromIterator(int_t model, int_t iterator);

		/// <summary>
		///		engiGetDeclarationType                                  (http://rdf.bg/ifcdoc/CS64/engiGetDeclarationType.html)
		///
		///	This call returns a type of the EXPRESS schema declarations from its handle.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetDeclarationType")]
		public static extern enum_express_declaration engiGetDeclarationType(int_t declaration);

		/// <summary>
		///		engiGetEnumerationElement                               (http://rdf.bg/ifcdoc/CS64/engiGetEnumerationElement.html)
		///
		///	This call returns a name of the enumaration element with the given index (zero based)
		///	It returns NULL if the index out of range
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEnumerationElement")]
		public static extern IntPtr engiGetEnumerationElement(int_t enumeration, int_t index);

		/// <summary>
		///		engiGetSelectElement                                    (http://rdf.bg/ifcdoc/CS64/engiGetSelectElement.html)
		///
		///	This call returns a declaration handle of the select element with the given index (zero based)
		///	It returns 0 if the index out of range
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetSelectElement")]
		public static extern int_t engiGetSelectElement(int_t select, int_t index);

		/// <summary>
		///		engiGetDefinedType                                      (http://rdf.bg/ifcdoc/CS64/engiGetDefinedType.html)
		///
		///	This call returns a simple type for defined type handle and can inquire referenced type, if any
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetDefinedType")]
		public static extern enum_express_attr_type engiGetDefinedType(int_t definedType, out int_t referencedDeclaration, out int_t aggregationDescriptor);

		/// <summary>
		///		sdaiGetEntity                                           (http://rdf.bg/ifcdoc/CS64/sdaiGetEntity.html)
		///
		///	This call retrieves a handle to an entity based on a given entity name.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntity")]
		public static extern int_t sdaiGetEntity(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntity")]
		public static extern int_t sdaiGetEntity(int_t model, byte[] entityName);

		/// <summary>
		///		engiGetEntityModel                                      (http://rdf.bg/ifcdoc/CS64/engiGetEntityModel.html)
		///
		///	This call retrieves a model based on a given entity handle.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityModel")]
		public static extern int_t engiGetEntityModel(int_t entity);

		/// <summary>
		///		engiGetEntityArgument                                   (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgument.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgument")]
		public static extern int_t engiGetEntityArgument(int_t entity, string argumentName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgument")]
		public static extern int_t engiGetEntityArgument(int_t entity, byte[] argumentName);

		/// <summary>
		///		engiGetEntityAttributeIndex                             (http://rdf.bg/ifcdoc/CS64/engiGetEntityAttributeIndex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndex")]
		public static extern int_t engiGetEntityAttributeIndex(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndex")]
		public static extern int_t engiGetEntityAttributeIndex(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetEntityAttributeIndexEx                           (http://rdf.bg/ifcdoc/CS64/engiGetEntityAttributeIndexEx.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndexEx")]
		public static extern int_t engiGetEntityAttributeIndexEx(int_t entity, string attributeName, byte countedWithParents, byte countedWithInverse);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndexEx")]
		public static extern int_t engiGetEntityAttributeIndexEx(int_t entity, byte[] attributeName, byte countedWithParents, byte countedWithInverse);

		/// <summary>
		///		engiGetEntityArgumentIndex                              (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgumentIndex.html)
		///
		///	DEPRECATED use engiGetEntityAttributeIndex
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentIndex")]
		public static extern int_t engiGetEntityArgumentIndex(int_t entity, string argumentName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentIndex")]
		public static extern int_t engiGetEntityArgumentIndex(int_t entity, byte[] argumentName);

		/// <summary>
		///		engiGetEntityArgumentName                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgumentName.html)
		///
		///	This call can be used to retrieve the name of the n-th argument of the given entity. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentName")]
		public static extern void engiGetEntityArgumentName(int_t entity, int_t index, int_t valueType, out IntPtr argumentName);

		public static string engiGetEntityArgumentName(int_t entity, int_t index)
		{
			IntPtr argumentName = IntPtr.Zero;
			engiGetEntityArgumentName(entity, index, sdaiSTRING, out argumentName); //marshaline expects ANSI
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(argumentName);
		}

		/// <summary>
		///		engiGetEntityArgumentType                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgumentType.html)
		///
		///	This call can be used to retrieve the type of the n-th argument of the given entity. In case of a select argument no relevant information is given by this call as it depends on the instance. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentType")]
		public static extern void engiGetEntityArgumentType(int_t entity, int_t index, out int_t argumentType);

		/// <summary>
		///		engiGetEntityCount                                      (http://rdf.bg/ifcdoc/CS64/engiGetEntityCount.html)
		///
		///	Returns the total number of entities within the loaded schema.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityCount")]
		public static extern int_t engiGetEntityCount(int_t model);

		/// <summary>
		///		engiGetEntityElement                                    (http://rdf.bg/ifcdoc/CS64/engiGetEntityElement.html)
		///
		///	This call returns a specific entity based on an index, the index needs to be 0 or higher but lower then the number of entities in the loaded schema.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityElement")]
		public static extern int_t engiGetEntityElement(int_t model, int_t index);

		/// <summary>
		///		sdaiGetEntityExtent                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetEntityExtent.html)
		///
		///	This call retrieves an aggregation that contains all instances of the entity given.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntityExtent")]
		public static extern int_t sdaiGetEntityExtent(int_t model, int_t entity);

		/// <summary>
		///		sdaiGetEntityExtentBN                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetEntityExtentBN.html)
		///
		///	This call retrieves an aggregation that contains all instances of the entity given.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntityExtentBN")]
		public static extern int_t sdaiGetEntityExtentBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntityExtentBN")]
		public static extern int_t sdaiGetEntityExtentBN(int_t model, byte[] entityName);

		/// <summary>
		///		engiGetEntityName                                       (http://rdf.bg/ifcdoc/CS64/engiGetEntityName.html)
		///
		///	This call can be used to get the name of the given entity.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityName")]
		public static extern void engiGetEntityName(int_t entity, int_t valueType, out IntPtr entityName);

		public static string engiGetEntityName(int_t entity)
		{
			IntPtr entityName = IntPtr.Zero;
			engiGetEntityName(entity, sdaiSTRING, out entityName); //marshaline expects ANSI
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(entityName);
		}

		/// <summary>
		///		engiGetEntityNoAttributes                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoAttributes.html)
		///
		///	This call returns the number of arguments, this includes the arguments of its (nested) parents and inverse argumnets.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoAttributes")]
		public static extern int_t engiGetEntityNoAttributes(int_t entity);

		/// <summary>
		///		engiGetEntityNoAttributesEx                             (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoAttributesEx.html)
		///
		///	This call returns the number of attributes, inclusion of parents and inverse depeds on includeParent and includeInverse values
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoAttributesEx")]
		public static extern int_t engiGetEntityNoAttributesEx(int_t entity, byte includeParent, byte includeInverse);

		/// <summary>
		///		engiGetEntityNoArguments                                (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoArguments.html)
		///
		///	DEPRECATED use engiGetEntityNoAttributes
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoArguments")]
		public static extern int_t engiGetEntityNoArguments(int_t entity);

		/// <summary>
		///		engiGetAttributeType                                    (http://rdf.bg/ifcdoc/CS64/engiGetAttributeType.html)
		///
		///	DEPRECATED use engiGetAttrType
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttributeType")]
		public static extern int_t engiGetAttributeType(int_t attribute);

		/// <summary>
		///		engiGetArgumentType                                     (http://rdf.bg/ifcdoc/CS64/engiGetArgumentType.html)
		///
		///	DEPR4ECATED use engiGetAttributeType
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetArgumentType")]
		public static extern int_t engiGetArgumentType(int_t argument);

		/// <summary>
		///		engiGetEntityParent                                     (http://rdf.bg/ifcdoc/CS64/engiGetEntityParent.html)
		///
		///	Returns the first direct parent entity, for example the parent of IfcObject is IfcObjectDefinition, of IfcObjectDefinition is IfcRoot and of IfcRoot is 0.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityParent")]
		public static extern int_t engiGetEntityParent(int_t entity);

		/// <summary>
		///		engiGetEntityNoParents                                  (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoParents.html)
		///
		///	Returns number of direct parents entity
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoParents")]
		public static extern int_t engiGetEntityNoParents(int_t entity);

		/// <summary>
		///		engiGetEntityParentEx                                   (http://rdf.bg/ifcdoc/CS64/engiGetEntityParentEx.html)
		///
		///	Returns the N-th direct parent of entity or NULL if index exceeds number of parents
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityParentEx")]
		public static extern int_t engiGetEntityParentEx(int_t entity, int_t index);

		/// <summary>
		///		engiGetAttrOptional                                     (http://rdf.bg/ifcdoc/CS64/engiGetAttrOptional.html)
		///
		///	This call can be used to check if an attribute is optional
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrOptional")]
		public static extern int_t engiGetAttrOptional(int_t attribute);

		/// <summary>
		///		engiGetAttrOptionalBN                                   (http://rdf.bg/ifcdoc/CS64/engiGetAttrOptionalBN.html)
		///
		///	This call can be used to check if an attribute is optional
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrOptionalBN")]
		public static extern int_t engiGetAttrOptionalBN(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrOptionalBN")]
		public static extern int_t engiGetAttrOptionalBN(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetEntityIsAbstract                                 (http://rdf.bg/ifcdoc/CS64/engiGetEntityIsAbstract.html)
		///
		///	This call can be used to check if an entity is abstract
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityIsAbstract")]
		public static extern int_t engiGetEntityIsAbstract(int_t entity);

		/// <summary>
		///		engiGetEntityIsAbstractBN                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityIsAbstractBN.html)
		///
		///	This call can be used to check if an entity is abstract
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityIsAbstractBN")]
		public static extern int_t engiGetEntityIsAbstractBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityIsAbstractBN")]
		public static extern int_t engiGetEntityIsAbstractBN(int_t model, byte[] entityName);

		/// <summary>
		///		engiGetAttrInverse                                      (http://rdf.bg/ifcdoc/CS64/engiGetAttrInverse.html)
		///
		///	This call can be used to check if an attribute is an inverse relation
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrInverse")]
		public static extern int_t engiGetAttrInverse(int_t attribute);

		/// <summary>
		///		engiGetAttrInverseBN                                    (http://rdf.bg/ifcdoc/CS64/engiGetAttrInverseBN.html)
		///
		///	This call can be used to check if an attribute is an inverse relation
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrInverseBN")]
		public static extern int_t engiGetAttrInverseBN(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrInverseBN")]
		public static extern int_t engiGetAttrInverseBN(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetAttrDomain                                       (http://rdf.bg/ifcdoc/CS64/engiGetAttrDomain.html)
		///
		///	This call can be used to get the domain of an attribute
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDomain")]
		public static extern void engiGetAttrDomain(int_t attribute, out IntPtr domainName);

		public static string engiGetAttrDomain(int_t attribute)
		{
			IntPtr domainName = IntPtr.Zero;
			engiGetAttrDomain(attribute, out domainName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(domainName);
		}

		/// <summary>
		///		engiGetAttrDomainBN                                     (http://rdf.bg/ifcdoc/CS64/engiGetAttrDomainBN.html)
		///
		///	This call can be used to get the domain of an attribute
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDomainBN")]
		public static extern void engiGetAttrDomainBN(int_t entity, string attributeName, out IntPtr domainName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDomainBN")]
		public static extern void engiGetAttrDomainBN(int_t entity, byte[] attributeName, out IntPtr domainName);

		public static string engiGetAttrDomainBN(int_t entity, byte[] attributeName)
		{
			IntPtr domainName = IntPtr.Zero;
			engiGetAttrDomainBN(entity, attributeName, out domainName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(domainName);
		}

		/// <summary>
		///		engiGetEnumerationValue                                 (http://rdf.bg/ifcdoc/CS64/engiGetEnumerationValue.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEnumerationValue")]
		public static extern void engiGetEnumerationValue(int_t attribute, int_t index, int_t valueType, out IntPtr enumerationValue);

		public static string engiGetEnumerationValue(int_t attribute, int_t index, int_t valueType)
		{
			IntPtr enumerationValue = IntPtr.Zero;
			engiGetEnumerationValue(attribute, index, sdaiSTRING, out enumerationValue); //marshaline expects ANSI
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(enumerationValue);
		}

		/// <summary>
		///		engiGetEntityAttributeByIndex                           (http://rdf.bg/ifcdoc/CS64/engiGetEntityAttributeByIndex.html)
		///
		///	Return attribute definition from attribute index
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeByIndex")]
		public static extern int_t engiGetEntityAttributeByIndex(int_t entity, int_t index, bool countedWithParents, bool countedWithInverse);

		/// <summary>
		///		engiGetAttributeTraits                                  (http://rdf.bg/ifcdoc/CS64/engiGetAttributeTraits.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttributeTraits")]
		public static extern void engiGetAttributeTraits(int_t attribute, out IntPtr name, out int_t definingEntity, out byte inverse, byte[] attrType, out int_t domainEntity, out int_t aggregationDescriptor, out byte optional, out byte unique);

		/// <summary>
		///		engiGetAggregation                                      (http://rdf.bg/ifcdoc/CS64/engiGetAggregation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggregation")]
		public static extern void engiGetAggregation(int_t aggregationDescriptor, out enum_express_aggr aggrType, out int_t cardinalityMin, out int_t cardinalityMax, out int_t nextAggregationLevelDescriptor);

        //
        //  Instance Header API Calls
        //

		/// <summary>
		///		SetSPFFHeader                                           (http://rdf.bg/ifcdoc/CS64/SetSPFFHeader.html)
		///
		///	This call is an aggregate of several SetSPFFHeaderItem calls. In several cases the header can be set easily with this call. In case an argument is zero, this argument will not be updated, i.e. it will not be filled with 0.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeader")]
		public static extern void SetSPFFHeader(int_t model, string description, string implementationLevel, string name, string timeStamp, string author, string organization, string preprocessorVersion, string originatingSystem, string authorization, string fileSchema);

		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeader")]
		public static extern void SetSPFFHeader(int_t model, byte[] description, byte[] implementationLevel, byte[] name, byte[] timeStamp, byte[] author, byte[] organization, byte[] preprocessorVersion, byte[] originatingSystem, byte[] authorization, byte[] fileSchema);

		/// <summary>
		///		SetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CS64/SetSPFFHeaderItem.html)
		///
		///	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeaderItem")]
		public static extern int_t SetSPFFHeaderItem(int_t model, int_t itemIndex, int_t itemSubIndex, int_t valueType, string value);

		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeaderItem")]
		public static extern int_t SetSPFFHeaderItem(int_t model, int_t itemIndex, int_t itemSubIndex, int_t valueType, byte[] value);

		/// <summary>
		///		GetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CS64/GetSPFFHeaderItem.html)
		///
		///	This call can be used to read a specific header item, the source code example is larger to show and explain how this call can be used.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "GetSPFFHeaderItem")]
		public static extern int_t GetSPFFHeaderItem(int_t model, int_t itemIndex, int_t itemSubIndex, int_t valueType, out IntPtr value);

		/// <summary>
		///		GetSPFFHeaderItemUnicode                                (http://rdf.bg/ifcdoc/CS64/GetSPFFHeaderItemUnicode.html)
		///
		///	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "GetSPFFHeaderItemUnicode")]
		public static extern int_t GetSPFFHeaderItemUnicode(int_t model, int_t itemIndex, int_t itemSubIndex, string buffer, int_t bufferLength);

		[DllImport(IFCEngineDLL, EntryPoint = "GetSPFFHeaderItemUnicode")]
		public static extern int_t GetSPFFHeaderItemUnicode(int_t model, int_t itemIndex, int_t itemSubIndex, byte[] buffer, int_t bufferLength);

        //
        //  Instance Reading API Calls
        //

		/// <summary>
		///		sdaiGetADBType                                          (http://rdf.bg/ifcdoc/CS64/sdaiGetADBType.html)
		///
		///	This call can be used to get the used type within this ADB type.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBType")]
		public static extern int_t sdaiGetADBType(int_t ADB);

		/// <summary>
		///		sdaiGetADBTypePath                                      (http://rdf.bg/ifcdoc/CS64/sdaiGetADBTypePath.html)
		///
		///	This call can be used to get the path of an ADB type.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBTypePath")]
		public static extern IntPtr sdaiGetADBTypePath(int_t ADB, int_t typeNameNumber);

		/// <summary>
		///		sdaiGetADBTypePathx                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetADBTypePathx.html)
		///
		///	This call is the same as sdaiGetADBTypePath, however can be used by porting to languages that have issues with returned char arrays.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBTypePathx")]
		public static extern void sdaiGetADBTypePathx(int_t ADB, int_t typeNameNumber, out IntPtr path);

		public static string sdaiGetADBTypePathx(int_t ADB, int_t typeNameNumber)
		{
			IntPtr path = IntPtr.Zero;
			sdaiGetADBTypePathx(ADB, typeNameNumber, out path);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(path);
		}

		/// <summary>
		///		sdaiGetADBValue                                         (http://rdf.bg/ifcdoc/CS64/sdaiGetADBValue.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out IntPtr value);

		/// <summary>
		///		sdaiCreateEmptyADB                                      (http://rdf.bg/ifcdoc/CS64/sdaiCreateEmptyADB.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateEmptyADB")]
		public static extern int_t sdaiCreateEmptyADB();

		/// <summary>
		///		sdaiDeleteADB                                           (http://rdf.bg/ifcdoc/CS64/sdaiDeleteADB.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteADB")]
		public static extern void sdaiDeleteADB(int_t ADB);

		/// <summary>
		///		engiGetAggrElement                                      (http://rdf.bg/ifcdoc/CS64/engiGetAggrElement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out IntPtr value);

		/// <summary>
		///		sdaiGetAggrByIndex                                      (http://rdf.bg/ifcdoc/CS64/sdaiGetAggrByIndex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out IntPtr value);

		/// <summary>
		///		engiGetAggrType                                         (http://rdf.bg/ifcdoc/CS64/engiGetAggrType.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrType")]
		public static extern void engiGetAggrType(int_t aggregate, out int_t aggregateType);

		/// <summary>
		///		engiGetAggrTypex                                        (http://rdf.bg/ifcdoc/CS64/engiGetAggrTypex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrTypex")]
		public static extern void engiGetAggrTypex(int_t aggregate, out int_t aggregateType);

		/// <summary>
		///		sdaiGetAttr                                             (http://rdf.bg/ifcdoc/CS64/sdaiGetAttr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out IntPtr value);

		/// <summary>
		///		sdaiGetAttrBN                                           (http://rdf.bg/ifcdoc/CS64/sdaiGetAttrBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out IntPtr value);

		/// <summary>
		///		sdaiGetAttrBNUnicode                                    (http://rdf.bg/ifcdoc/CS64/sdaiGetAttrBNUnicode.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBNUnicode")]
		public static extern int_t sdaiGetAttrBNUnicode(int_t instance, string attributeName, string buffer, int_t bufferLength);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBNUnicode")]
		public static extern int_t sdaiGetAttrBNUnicode(int_t instance, byte[] attributeName, byte[] buffer, int_t bufferLength);

		/// <summary>
		///		sdaiGetStringAttrBN                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetStringAttrBN.html)
		///
		///	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiSTRING.
		///	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
		///	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetStringAttrBN")]
		public static extern IntPtr sdaiGetStringAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetStringAttrBN")]
		public static extern IntPtr sdaiGetStringAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiGetInstanceAttrBN                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetInstanceAttrBN.html)
		///
		///	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiINSTANCE.
		///	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
		///	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetInstanceAttrBN")]
		public static extern int_t sdaiGetInstanceAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetInstanceAttrBN")]
		public static extern int_t sdaiGetInstanceAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiGetAggregationAttrBN                                (http://rdf.bg/ifcdoc/CS64/sdaiGetAggregationAttrBN.html)
		///
		///	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiAGGR.
		///	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
		///	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggregationAttrBN")]
		public static extern int_t sdaiGetAggregationAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggregationAttrBN")]
		public static extern int_t sdaiGetAggregationAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiGetAttrDefinition                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetAttrDefinition.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrDefinition")]
		public static extern int_t sdaiGetAttrDefinition(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrDefinition")]
		public static extern int_t sdaiGetAttrDefinition(int_t entity, byte[] attributeName);

		/// <summary>
		///		sdaiGetInstanceType                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetInstanceType.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetInstanceType")]
		public static extern int_t sdaiGetInstanceType(int_t instance);

		/// <summary>
		///		sdaiGetMemberCount                                      (http://rdf.bg/ifcdoc/CS64/sdaiGetMemberCount.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetMemberCount")]
		public static extern int_t sdaiGetMemberCount(int_t aggregate);

		/// <summary>
		///		sdaiIsKindOf                                            (http://rdf.bg/ifcdoc/CS64/sdaiIsKindOf.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsKindOf")]
		public static extern int_t sdaiIsKindOf(int_t instance, int_t entity);

		/// <summary>
		///		sdaiIsKindOfBN                                          (http://rdf.bg/ifcdoc/CS64/sdaiIsKindOfBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsKindOfBN")]
		public static extern int_t sdaiIsKindOfBN(int_t instance, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsKindOfBN")]
		public static extern int_t sdaiIsKindOfBN(int_t instance, byte[] entityName);

		/// <summary>
		///		engiGetAttrType                                         (http://rdf.bg/ifcdoc/CS64/engiGetAttrType.html)
		///
		///	Returns primitive SDAI data type for the attribute according to schema, e.g. sdaiINTEGER
		///
		///	In case of aggregation if will return base primitive type combined with engiTypeFlagAggr, e.g. sdaiINTEGER|engiTypeFlagAggr
		///
		///	For SELECT it will return sdaiINSTANCE if all options are instances or aggegation of instances, either sdaiADB
		///	In case of SELECT and sdaiINSTANCE, return value will be combined with engiTypeFlagAggrOption if some options are aggregation
		///	or engiTypeFlagAggr if all options are aggregations of instances
		///
		///	It works for direct and inverse attributes
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrType")]
		public static extern int_t engiGetAttrType(int_t attribute);

		/// <summary>
		///		engiGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CS64/engiGetAttrTypeBN.html)
		///
		///	Combines sdaiGetAttrDefinition and engiGetAttrType
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrTypeBN")]
		public static extern int_t engiGetAttrTypeBN(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrTypeBN")]
		public static extern int_t engiGetAttrTypeBN(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetInstanceAttrType                                 (http://rdf.bg/ifcdoc/CS64/engiGetInstanceAttrType.html)
		///
		///	Returns SDAI type for actual data stored in the instance for the attribute
		///	It may be primitive type, sdaiAGGR or sdaiADB
		///	Returns 0 for $ and * 
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceAttrType")]
		public static extern int_t engiGetInstanceAttrType(int_t instance, int_t attribute);

		/// <summary>
		///		engiGetInstanceAttrTypeBN                               (http://rdf.bg/ifcdoc/CS64/engiGetInstanceAttrTypeBN.html)
		///
		///	Combines sdaiGetAttrDefinition and engiGetInstanceAttrType
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceAttrTypeBN")]
		public static extern int_t engiGetInstanceAttrTypeBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceAttrTypeBN")]
		public static extern int_t engiGetInstanceAttrTypeBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiIsInstanceOf                                        (http://rdf.bg/ifcdoc/CS64/sdaiIsInstanceOf.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsInstanceOf")]
		public static extern int_t sdaiIsInstanceOf(int_t instance, int_t entity);

		/// <summary>
		///		sdaiIsInstanceOfBN                                      (http://rdf.bg/ifcdoc/CS64/sdaiIsInstanceOfBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsInstanceOfBN")]
		public static extern int_t sdaiIsInstanceOfBN(int_t instance, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsInstanceOfBN")]
		public static extern int_t sdaiIsInstanceOfBN(int_t instance, byte[] entityName);

		/// <summary>
		///		sdaiIsEqual                                             (http://rdf.bg/ifcdoc/CS64/sdaiIsEqual.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsEqual")]
		public static extern byte sdaiIsEqual(int_t instanceI, int_t instanceII);

		/// <summary>
		///		sdaiValidateAttribute                                   (http://rdf.bg/ifcdoc/CS64/sdaiValidateAttribute.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateAttribute")]
		public static extern int_t sdaiValidateAttribute(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiValidateAttributeBN                                 (http://rdf.bg/ifcdoc/CS64/sdaiValidateAttributeBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateAttributeBN")]
		public static extern int_t sdaiValidateAttributeBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateAttributeBN")]
		public static extern int_t sdaiValidateAttributeBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		engiGetInstanceClassInfo                                (http://rdf.bg/ifcdoc/CS64/engiGetInstanceClassInfo.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceClassInfo")]
		public static extern IntPtr engiGetInstanceClassInfo(int_t instance);

		/// <summary>
		///		engiGetInstanceClassInfoUC                              (http://rdf.bg/ifcdoc/CS64/engiGetInstanceClassInfoUC.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceClassInfoUC")]
		public static extern IntPtr engiGetInstanceClassInfoUC(int_t instance);

		/// <summary>
		///		engiGetInstanceMetaInfo                                 (http://rdf.bg/ifcdoc/CS64/engiGetInstanceMetaInfo.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceMetaInfo")]
		public static extern int_t engiGetInstanceMetaInfo(int_t instance, out int_t localId, out IntPtr entityName, out IntPtr entityNameUC);

		/// <summary>
		///		sdaiFindInstanceUsers                                   (http://rdf.bg/ifcdoc/CS64/sdaiFindInstanceUsers.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiFindInstanceUsers")]
		public static extern int_t sdaiFindInstanceUsers(int_t instance, int_t domain, int_t resultList);

		/// <summary>
		///		sdaiFindInstanceUsedInBN                                (http://rdf.bg/ifcdoc/CS64/sdaiFindInstanceUsedInBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiFindInstanceUsedInBN")]
		public static extern int_t sdaiFindInstanceUsedInBN(int_t instance, string roleName, int_t domain, int_t resultList);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiFindInstanceUsedInBN")]
		public static extern int_t sdaiFindInstanceUsedInBN(int_t instance, byte[] roleName, int_t domain, int_t resultList);

        //
        //  Instance Writing API Calls
        //

		/// <summary>
		///		sdaiPrepend                                             (http://rdf.bg/ifcdoc/CS64/sdaiPrepend.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t list, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t list, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t list, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t list, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t list, int_t valueType, byte[] value);

		/// <summary>
		///		sdaiAppend                                              (http://rdf.bg/ifcdoc/CS64/sdaiAppend.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t list, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t list, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t list, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t list, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t list, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t list, int_t valueType, string value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t list, int_t valueType, ref bool value);

		/// <summary>
		///		sdaiAdd                                                 (http://rdf.bg/ifcdoc/CS64/sdaiAdd.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t list, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t list, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t list, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t list, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t list, int_t valueType, byte[] value);

		/// <summary>
		///		sdaiInsertByIndex                                       (http://rdf.bg/ifcdoc/CS64/sdaiInsertByIndex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t list, int_t index, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t list, int_t index, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t list, int_t index, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t list, int_t index, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t list, int_t index, int_t valueType, byte[] value);

		/// <summary>
		///		sdaiCreateADB                                           (http://rdf.bg/ifcdoc/CS64/sdaiCreateADB.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, string value);

		/// <summary>
		///		sdaiCreateAggr                              (http://rdf.bg/ifcdoc/CS64/sdaiCreateAggr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateAggr")]
		public static extern int_t sdaiCreateAggr(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiCreateAggrBN                            (http://rdf.bg/ifcdoc/CS64/sdaiCreateAggrBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateAggrBN")]
		public static extern int_t sdaiCreateAggrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateAggrBN")]
		public static extern int_t sdaiCreateAggrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiCreateNPL                                           (http://rdf.bg/ifcdoc/CS64/sdaiCreateNPL.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateNPL")]
		public static extern int_t sdaiCreateNPL();

		/// <summary>
		///		sdaiDeleteNPL                                           (http://rdf.bg/ifcdoc/CS64/sdaiDeleteNPL.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteNPL")]
		public static extern void sdaiDeleteNPL(int_t list);

		/// <summary>
		///		sdaiCreateNestedAggr                                    (http://rdf.bg/ifcdoc/CS64/sdaiCreateNestedAggr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateNestedAggr")]
		public static extern int_t sdaiCreateNestedAggr(int_t aggregate);

		/// <summary>
		///		sdaiCreateInstance                                      (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstance")]
		public static extern int_t sdaiCreateInstance(int_t model, int_t entity);

		/// <summary>
		///		sdaiCreateInstanceBN                                    (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstanceBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBN")]
		public static extern int_t sdaiCreateInstanceBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBN")]
		public static extern int_t sdaiCreateInstanceBN(int_t model, byte[] entityName);

		/// <summary>
		///		sdaiDeleteInstance                                      (http://rdf.bg/ifcdoc/CS64/sdaiDeleteInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteInstance")]
		public static extern void sdaiDeleteInstance(int_t instance);

		/// <summary>
		///		sdaiPutADBTypePath                                      (http://rdf.bg/ifcdoc/CS64/sdaiPutADBTypePath.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutADBTypePath")]
		public static extern void sdaiPutADBTypePath(int_t ADB, int_t pathCount, string path);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutADBTypePath")]
		public static extern void sdaiPutADBTypePath(int_t ADB, int_t pathCount, byte[] path);

		/// <summary>
		///		sdaiPutAttr                                             (http://rdf.bg/ifcdoc/CS64/sdaiPutAttr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, byte[] value);

		/// <summary>
		///		sdaiPutAttrBN                                           (http://rdf.bg/ifcdoc/CS64/sdaiPutAttrBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, string value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, string value);

		/// <summary>
		///		sdaiUnsetAttr                                           (http://rdf.bg/ifcdoc/CS64/sdaiUnsetAttr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiUnsetAttr")]
		public static extern void sdaiUnsetAttr(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiUnsetAttrBN                                         (http://rdf.bg/ifcdoc/CS64/sdaiUnsetAttrBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiUnsetAttrBN")]
		public static extern void sdaiUnsetAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiUnsetAttrBN")]
		public static extern void sdaiUnsetAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		engiSetComment                                          (http://rdf.bg/ifcdoc/CS64/engiSetComment.html)
		///
		///	This call can be used to add a comment to an instance when exporting the content. The comment is available in the exported/saved IFC file.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSetComment")]
		public static extern void engiSetComment(int_t instance, string comment);

		[DllImport(IFCEngineDLL, EntryPoint = "engiSetComment")]
		public static extern void engiSetComment(int_t instance, byte[] comment);

		/// <summary>
		///		engiGetInstanceLocalId                                  (http://rdf.bg/ifcdoc/CS64/engiGetInstanceLocalId.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceLocalId")]
		public static extern int_t engiGetInstanceLocalId(int_t instance);

		/// <summary>
		///		sdaiTestAttr                                            (http://rdf.bg/ifcdoc/CS64/sdaiTestAttr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiTestAttr")]
		public static extern int_t sdaiTestAttr(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiTestAttrBN                                          (http://rdf.bg/ifcdoc/CS64/sdaiTestAttrBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiTestAttrBN")]
		public static extern int_t sdaiTestAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiTestAttrBN")]
		public static extern int_t sdaiTestAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiCreateInstanceEI                                    (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstanceEI.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceEI")]
		public static extern int_t sdaiCreateInstanceEI(int_t model, int_t entity, int_t express_id);

		/// <summary>
		///		sdaiCreateInstanceBNEI                                  (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstanceBNEI.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBNEI")]
		public static extern int_t sdaiCreateInstanceBNEI(int_t model, string entityName, int_t express_id);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBNEI")]
		public static extern int_t sdaiCreateInstanceBNEI(int_t model, byte[] entityName, int_t express_id);

		/// <summary>
		///		setSegmentation                                         (http://rdf.bg/ifcdoc/CS64/setSegmentation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setSegmentation")]
		public static extern void setSegmentation(int_t model, int_t segmentationParts, double segmentationLength);

		/// <summary>
		///		getSegmentation                                         (http://rdf.bg/ifcdoc/CS64/getSegmentation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getSegmentation")]
		public static extern void getSegmentation(int_t model, out int_t segmentationParts, out double segmentationLength);

		/// <summary>
		///		setEpsilon                                              (http://rdf.bg/ifcdoc/CS64/setEpsilon.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setEpsilon")]
		public static extern void setEpsilon(int_t model, int_t mask, double absoluteEpsilon, double relativeEpsilon);

		/// <summary>
		///		getEpsilon                                              (http://rdf.bg/ifcdoc/CS64/getEpsilon.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getEpsilon")]
		public static extern int_t getEpsilon(int_t model, int_t mask, out double absoluteEpsilon, out double relativeEpsilon);

        //
        //  Controling API Calls
        //

		/// <summary>
		///		circleSegments                                          (http://rdf.bg/ifcdoc/CS64/circleSegments.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "circleSegments")]
		public static extern void circleSegments(int_t circles, int_t smallCircles);

		/// <summary>
		///		setMaximumSegmentationLength                            (http://rdf.bg/ifcdoc/CS64/setMaximumSegmentationLength.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setMaximumSegmentationLength")]
		public static extern void setMaximumSegmentationLength(int_t model, double length);

		/// <summary>
		///		getUnitConversionFactor                                 (http://rdf.bg/ifcdoc/CS64/getUnitConversionFactor.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getUnitConversionFactor")]
		public static extern double getUnitConversionFactor(int_t model, string unitType, out IntPtr unitPrefix, out IntPtr unitName, out IntPtr SIUnitName);

		[DllImport(IFCEngineDLL, EntryPoint = "getUnitConversionFactor")]
		public static extern double getUnitConversionFactor(int_t model, byte[] unitType, out IntPtr unitPrefix, out IntPtr unitName, out IntPtr SIUnitName);

		/// <summary>
		///		setBRepProperties                                       (http://rdf.bg/ifcdoc/CS64/setBRepProperties.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setBRepProperties")]
		public static extern void setBRepProperties(int_t model, Int64 consistencyCheck, double fraction, double epsilon, int_t maxVerticesSize);

		/// <summary>
		///		cleanMemory                                             (http://rdf.bg/ifcdoc/CS64/cleanMemory.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "cleanMemory")]
		public static extern void cleanMemory(int_t model, int_t mode);

		/// <summary>
		///		internalGetP21Line                                      (http://rdf.bg/ifcdoc/CS64/internalGetP21Line.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetP21Line")]
		public static extern int_t internalGetP21Line(int_t instance);

		/// <summary>
		///		internalForceInstanceFromP21Line                        (http://rdf.bg/ifcdoc/CS64/internalForceInstanceFromP21Line.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalForceInstanceFromP21Line")]
		public static extern int_t internalForceInstanceFromP21Line(int_t model, int_t P21Line);

		/// <summary>
		///		internalGetInstanceFromP21Line                          (http://rdf.bg/ifcdoc/CS64/internalGetInstanceFromP21Line.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetInstanceFromP21Line")]
		public static extern int_t internalGetInstanceFromP21Line(int_t model, int_t P21Line);

		/// <summary>
		///		internalGetXMLID                                        (http://rdf.bg/ifcdoc/CS64/internalGetXMLID.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetXMLID")]
		public static extern void internalGetXMLID(int_t instance, out IntPtr XMLID);

		public static string internalGetXMLID(int_t instance)
		{
			IntPtr XMLID = IntPtr.Zero;
			internalGetXMLID(instance, out XMLID);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(XMLID);
		}

		/// <summary>
		///		setStringUnicode                                        (http://rdf.bg/ifcdoc/CS64/setStringUnicode.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setStringUnicode")]
		public static extern int_t setStringUnicode(int_t unicode);

		/// <summary>
		///		setFilter                                               (http://rdf.bg/ifcdoc/CS64/setFilter.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setFilter")]
		public static extern void setFilter(int_t model, int_t setting, int_t mask);

		/// <summary>
		///		getFilter                                               (http://rdf.bg/ifcdoc/CS64/getFilter.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getFilter")]
		public static extern int_t getFilter(int_t model, int_t mask);

        //
        //  Uncategorized API Calls
        //

		/// <summary>
		///		xxxxGetEntityAndSubTypesExtent                          (http://rdf.bg/ifcdoc/CS64/xxxxGetEntityAndSubTypesExtent.html)
		///
		///	model input parameter is irrelevant, but is required for backwards compatibility
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetEntityAndSubTypesExtent")]
		public static extern int_t xxxxGetEntityAndSubTypesExtent(int_t model, int_t entity);

		/// <summary>
		///		xxxxGetEntityAndSubTypesExtentBN                        (http://rdf.bg/ifcdoc/CS64/xxxxGetEntityAndSubTypesExtentBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetEntityAndSubTypesExtentBN")]
		public static extern int_t xxxxGetEntityAndSubTypesExtentBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetEntityAndSubTypesExtentBN")]
		public static extern int_t xxxxGetEntityAndSubTypesExtentBN(int_t model, byte[] entityName);

		/// <summary>
		///		xxxxGetAllInstances                                     (http://rdf.bg/ifcdoc/CS64/xxxxGetAllInstances.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAllInstances")]
		public static extern int_t xxxxGetAllInstances(int_t model);

		/// <summary>
		///		xxxxGetInstancesUsing                                   (http://rdf.bg/ifcdoc/CS64/xxxxGetInstancesUsing.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetInstancesUsing")]
		public static extern int_t xxxxGetInstancesUsing(int_t instance);

		/// <summary>
		///		xxxxDeleteFromAggregation                               (http://rdf.bg/ifcdoc/CS64/xxxxDeleteFromAggregation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxDeleteFromAggregation")]
		public static extern int_t xxxxDeleteFromAggregation(int_t instance, int_t aggregate, int_t elementIndex);

		/// <summary>
		///		xxxxGetAttrDefinitionByValue                            (http://rdf.bg/ifcdoc/CS64/xxxxGetAttrDefinitionByValue.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrDefinitionByValue")]
		public static extern int_t xxxxGetAttrDefinitionByValue(int_t instance, ref int_t value);

		/// <summary>
		///		xxxxGetAttrNameByIndex                                  (http://rdf.bg/ifcdoc/CS64/xxxxGetAttrNameByIndex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrNameByIndex")]
		public static extern void xxxxGetAttrNameByIndex(int_t instance, int_t index, out IntPtr name);

		public static string xxxxGetAttrNameByIndex(int_t instance, int_t index)
		{
			IntPtr name = IntPtr.Zero;
			xxxxGetAttrNameByIndex(instance, index, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		iterateOverInstances                                    (http://rdf.bg/ifcdoc/CS64/iterateOverInstances.html)
		///
		///	This function interates over all available instances loaded in memory, it is the fastest way to find all instances.
		///	Argument entity and entityName are both optional and if non-zero are filled with respectively the entity handle and entity name as char array.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "iterateOverInstances")]
		public static extern int_t iterateOverInstances(int_t model, int_t instance, out int_t entity, out IntPtr entityName);

		/// <summary>
		///		iterateOverProperties                                   (http://rdf.bg/ifcdoc/CS64/iterateOverProperties.html)
		///
		///	This function iterated over all available attributes of a specific given entity.
		///	This call is typically used in combination with iterateOverInstances(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "iterateOverProperties")]
		public static extern int_t iterateOverProperties(int_t entity, int_t index);

		/// <summary>
		///		sdaiGetAggrByIterator                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetAggrByIterator.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out IntPtr value);

		/// <summary>
		///		sdaiPutAggrByIterator                                   (http://rdf.bg/ifcdoc/CS64/sdaiPutAggrByIterator.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, ref int_t value);

		/// <summary>
		///		internalSetLink                                         (http://rdf.bg/ifcdoc/CS64/internalSetLink.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalSetLink")]
		public static extern void internalSetLink(int_t instance, string attributeName, int_t linked_id);

		[DllImport(IFCEngineDLL, EntryPoint = "internalSetLink")]
		public static extern void internalSetLink(int_t instance, byte[] attributeName, int_t linked_id);

		/// <summary>
		///		internalAddAggrLink                                     (http://rdf.bg/ifcdoc/CS64/internalAddAggrLink.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalAddAggrLink")]
		public static extern void internalAddAggrLink(int_t list, int_t linked_id);

		/// <summary>
		///		engiGetNotReferedAggr                                   (http://rdf.bg/ifcdoc/CS64/engiGetNotReferedAggr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetNotReferedAggr")]
		public static extern void engiGetNotReferedAggr(int_t model, out int_t value);

		/// <summary>
		///		engiGetAttributeAggr                                    (http://rdf.bg/ifcdoc/CS64/engiGetAttributeAggr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttributeAggr")]
		public static extern void engiGetAttributeAggr(int_t instance, out int_t value);

		/// <summary>
		///		engiGetAggrUnknownElement                               (http://rdf.bg/ifcdoc/CS64/engiGetAggrUnknownElement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out IntPtr value);

		/// <summary>
		///		sdaiErrorQuery                                          (http://rdf.bg/ifcdoc/CS64/sdaiErrorQuery.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiErrorQuery")]
		public static extern int_t sdaiErrorQuery();

        //
        //  Geometry Kernel related API Calls
        //

		/// <summary>
		///		owlGetModel                                             (http://rdf.bg/ifcdoc/CS64/owlGetModel.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlGetModel")]
		public static extern void owlGetModel(int_t model, out Int64 owlModel);

		/// <summary>
		///		owlGetInstance                                          (http://rdf.bg/ifcdoc/CS64/owlGetInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlGetInstance")]
		public static extern void owlGetInstance(int_t model, int_t instance, out Int64 owlInstance);

		/// <summary>
		///		owlMaterialInstance                                     (http://rdf.bg/ifcdoc/CS64/owlMaterialInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlMaterialInstance")]
		public static extern void owlMaterialInstance(int_t instanceBase, int_t instanceContext, out Int64 owlInstance);

		/// <summary>
		///		owlBuildInstance                                        (http://rdf.bg/ifcdoc/CS64/owlBuildInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlBuildInstance")]
		public static extern void owlBuildInstance(int_t model, int_t instance, out Int64 owlInstance);

		/// <summary>
		///		owlBuildInstanceInContext                               (http://rdf.bg/ifcdoc/CS64/owlBuildInstanceInContext.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlBuildInstanceInContext")]
		public static extern void owlBuildInstanceInContext(int_t instanceBase, int_t instanceContext, out Int64 owlInstance);

		/// <summary>
		///		engiInstanceUsesSegmentation                            (http://rdf.bg/ifcdoc/CS64/engiInstanceUsesSegmentation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiInstanceUsesSegmentation")]
		public static extern byte engiInstanceUsesSegmentation(int_t instance);

		/// <summary>
		///		owlBuildInstances                                       (http://rdf.bg/ifcdoc/CS64/owlBuildInstances.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlBuildInstances")]
		public static extern void owlBuildInstances(int_t model, int_t instance, out Int64 owlInstanceComplete, out Int64 owlInstanceSolids, out Int64 owlInstanceVoids);

		/// <summary>
		///		owlGetMappedItem                                        (http://rdf.bg/ifcdoc/CS64/owlGetMappedItem.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlGetMappedItem")]
		public static extern void owlGetMappedItem(int_t model, int_t instance, out Int64 owlInstance, out double transformationMatrix);

		/// <summary>
		///		getInstanceDerivedPropertiesInModelling                 (http://rdf.bg/ifcdoc/CS64/getInstanceDerivedPropertiesInModelling.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceDerivedPropertiesInModelling")]
		public static extern int_t getInstanceDerivedPropertiesInModelling(int_t model, int_t instance, out double height, out double width, out double thickness);

		/// <summary>
		///		getInstanceDerivedBoundingBox                           (http://rdf.bg/ifcdoc/CS64/getInstanceDerivedBoundingBox.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceDerivedBoundingBox")]
		public static extern int_t getInstanceDerivedBoundingBox(int_t model, int_t instance, out double Ox, out double Oy, out double Oz, out double Vx, out double Vy, out double Vz);

		/// <summary>
		///		getInstanceTransformationMatrix                         (http://rdf.bg/ifcdoc/CS64/getInstanceTransformationMatrix.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceTransformationMatrix")]
		public static extern int_t getInstanceTransformationMatrix(int_t model, int_t instance, out double _11, out double _12, out double _13, out double _14, out double _21, out double _22, out double _23, out double _24, out double _31, out double _32, out double _33, out double _34, out double _41, out double _42, out double _43, out double _44);

		/// <summary>
		///		getInstanceDerivedTransformationMatrix                  (http://rdf.bg/ifcdoc/CS64/getInstanceDerivedTransformationMatrix.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceDerivedTransformationMatrix")]
		public static extern int_t getInstanceDerivedTransformationMatrix(int_t model, int_t instance, out double _11, out double _12, out double _13, out double _14, out double _21, out double _22, out double _23, out double _24, out double _31, out double _32, out double _33, out double _34, out double _41, out double _42, out double _43, out double _44);

		/// <summary>
		///		internalGetBoundingBox                                  (http://rdf.bg/ifcdoc/CS64/internalGetBoundingBox.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetBoundingBox")]
		public static extern int_t internalGetBoundingBox(int_t model, int_t instance);

		/// <summary>
		///		internalGetCenter                                       (http://rdf.bg/ifcdoc/CS64/internalGetCenter.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetCenter")]
		public static extern int_t internalGetCenter(int_t model, int_t instance);

		/// <summary>
		///		getGlobalPlacement                                      (http://rdf.bg/ifcdoc/CS64/getGlobalPlacement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getGlobalPlacement")]
		public static extern int_t getGlobalPlacement(int_t model, out double origin);

		/// <summary>
		///		setGlobalPlacement                                      (http://rdf.bg/ifcdoc/CS64/setGlobalPlacement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setGlobalPlacement")]
		public static extern int_t setGlobalPlacement(int_t model, ref double origin, byte includeRotation);

		/// <summary>
		///		getTimeStamp                                            (http://rdf.bg/ifcdoc/CS64/getTimeStamp.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getTimeStamp")]
		public static extern int_t getTimeStamp(int_t model);

		/// <summary>
		///		setInstanceReference                                    (http://rdf.bg/ifcdoc/CS64/setInstanceReference.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setInstanceReference")]
		public static extern int_t setInstanceReference(int_t instance, int_t value);

		/// <summary>
		///		getInstanceReference                                    (http://rdf.bg/ifcdoc/CS64/getInstanceReference.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceReference")]
		public static extern int_t getInstanceReference(int_t instance);

		/// <summary>
		///		inferenceInstance                                       (http://rdf.bg/ifcdoc/CS64/inferenceInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "inferenceInstance")]
		public static extern int_t inferenceInstance(int_t instance);

		/// <summary>
		///		sdaiValidateSchemaInstance                              (http://rdf.bg/ifcdoc/CS64/sdaiValidateSchemaInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateSchemaInstance")]
		public static extern int_t sdaiValidateSchemaInstance(int_t instance);

        //
        //  Deprecated API Calls (GENERIC)
        //

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate Int64 ReadCallBackFunction(IntPtr value);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void WriteCallBackFunction(IntPtr value, Int64 size);

		/// <summary>
		///		engiAttrIsInverse                                       (http://rdf.bg/ifcdoc/CS64/engiAttrIsInverse.html)
		///
		///	This call is deprecated, please use call engiAttrIsInverse.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiAttrIsInverse")]
		public static extern int_t engiAttrIsInverse(int_t attribute);

		/// <summary>
		///		xxxxOpenModelByStream                                   (http://rdf.bg/ifcdoc/CS64/xxxxOpenModelByStream.html)
		///
		///	This call is deprecated, please use call engiOpenModelByStream.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxOpenModelByStream")]
		public static extern int_t xxxxOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "xxxxOpenModelByStream")]
		public static extern int_t xxxxOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, byte[] schemaName);

		/// <summary>
		///		sdaiCreateIterator                                      (http://rdf.bg/ifcdoc/CS64/sdaiCreateIterator.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateIterator")]
		public static extern int_t sdaiCreateIterator(ref int_t aggregate);

		/// <summary>
		///		sdaiDeleteIterator                                      (http://rdf.bg/ifcdoc/CS64/sdaiDeleteIterator.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteIterator")]
		public static extern void sdaiDeleteIterator(int_t iterator);

		/// <summary>
		///		sdaiBeginning                                           (http://rdf.bg/ifcdoc/CS64/sdaiBeginning.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiBeginning")]
		public static extern void sdaiBeginning(int_t iterator);

		/// <summary>
		///		sdaiNext                                                (http://rdf.bg/ifcdoc/CS64/sdaiNext.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiNext")]
		public static extern int_t sdaiNext(int_t iterator);

		/// <summary>
		///		sdaiPrevious                                            (http://rdf.bg/ifcdoc/CS64/sdaiPrevious.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrevious")]
		public static extern int_t sdaiPrevious(int_t iterator);

		/// <summary>
		///		sdaiEnd                                                 (http://rdf.bg/ifcdoc/CS64/sdaiEnd.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiEnd")]
		public static extern void sdaiEnd(int_t iterator);

		/// <summary>
		///		sdaiplusGetAggregationType                              (http://rdf.bg/ifcdoc/CS64/sdaiplusGetAggregationType.html)
		///
		///	This call is deprecated, please use call ....
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiplusGetAggregationType")]
		public static extern int_t sdaiplusGetAggregationType(int_t instance, int_t aggregate);

		/// <summary>
		///		xxxxGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CS64/xxxxGetAttrTypeBN.html)
		///
		///	This call is deprecated, please use calls engiGetAttrTypeBN(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrTypeBN")]
		public static extern int_t xxxxGetAttrTypeBN(int_t instance, string attributeName, out IntPtr attributeType);

		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrTypeBN")]
		public static extern int_t xxxxGetAttrTypeBN(int_t instance, byte[] attributeName, out IntPtr attributeType);

        //
        //  Validation
        //

		/// <summary>
		///		validateSetOptions                                      (http://rdf.bg/ifcdoc/CS64/validateSetOptions.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateSetOptions")]
		public static extern void validateSetOptions(int_t timeLimitSeconds, int_t issueCntLimit, byte showEachIssueOnce, UInt64 issueTypes, UInt64 mask);

		/// <summary>
		///		validateGetOptions                                      (http://rdf.bg/ifcdoc/CS64/validateGetOptions.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetOptions")]
		public static extern UInt64 validateGetOptions(out int_t timeLimitSeconds, out int_t issueCntLimit, out byte showEachIssueOnce, UInt64 mask);

		/// <summary>
		///		validateModel                                           (http://rdf.bg/ifcdoc/CS64/validateModel.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateModel")]
		public static extern int_t validateModel(int_t model);

		/// <summary>
		///		validateInstance                                        (http://rdf.bg/ifcdoc/CS64/validateInstance.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateInstance")]
		public static extern int_t validateInstance(int_t instance);

		/// <summary>
		///		validateFreeResults                                     (http://rdf.bg/ifcdoc/CS64/validateFreeResults.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateFreeResults")]
		public static extern void validateFreeResults(int_t results);

		/// <summary>
		///		validateGetFirstIssue                                   (http://rdf.bg/ifcdoc/CS64/validateGetFirstIssue.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetFirstIssue")]
		public static extern int_t validateGetFirstIssue(int_t results);

		/// <summary>
		///		validateGetNextIssue                                    (http://rdf.bg/ifcdoc/CS64/validateGetNextIssue.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetNextIssue")]
		public static extern int_t validateGetNextIssue(int_t issue);

		/// <summary>
		///		validateGetStatus                                       (http://rdf.bg/ifcdoc/CS64/validateGetStatus.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetStatus")]
		public static extern enum_validation_status validateGetStatus(int_t results);

		/// <summary>
		///		validateGetIssueType                                    (http://rdf.bg/ifcdoc/CS64/validateGetIssueType.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetIssueType")]
		public static extern UInt64 validateGetIssueType(int_t issue);

		/// <summary>
		///		validateGetInstance                                     (http://rdf.bg/ifcdoc/CS64/validateGetInstance.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetInstance")]
		public static extern int_t validateGetInstance(int_t issue);

		/// <summary>
		///		validateGetInstanceRelated                              (http://rdf.bg/ifcdoc/CS64/validateGetInstanceRelated.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetInstanceRelated")]
		public static extern int_t validateGetInstanceRelated(int_t issue);

		/// <summary>
		///		validateGetEntity                                       (http://rdf.bg/ifcdoc/CS64/validateGetEntity.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetEntity")]
		public static extern int_t validateGetEntity(int_t issue);

		/// <summary>
		///		validateGetAttr                                         (http://rdf.bg/ifcdoc/CS64/validateGetAttr.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetAttr")]
		public static extern int_t validateGetAttr(int_t issue);

		/// <summary>
		///		validateGetAggrLevel                                    (http://rdf.bg/ifcdoc/CS64/validateGetAggrLevel.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetAggrLevel")]
		public static extern int_t validateGetAggrLevel(int_t issue);

		/// <summary>
		///		validateGetAggrIndArray                                 (http://rdf.bg/ifcdoc/CS64/validateGetAggrIndArray.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetAggrIndArray")]
		public static extern int_t validateGetAggrIndArray(int_t issue);

		/// <summary>
		///		validateGetIssueLevel                                   (http://rdf.bg/ifcdoc/CS64/validateGetIssueLevel.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetIssueLevel")]
		public static extern int_t validateGetIssueLevel(int_t issue);

		/// <summary>
		///		validateGetDescription                                  (http://rdf.bg/ifcdoc/CS64/validateGetDescription.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetDescription")]
		public static extern IntPtr validateGetDescription(int_t issue);

        //
        //  Deprecated API Calls (GEOMETRY)
        //

		/// <summary>
		///		initializeModellingInstance                             (http://rdf.bg/ifcdoc/CS64/initializeModellingInstance.html)
		///
		///	This call is deprecated, please use call CalculateInstance().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "initializeModellingInstance")]
		public static extern int_t initializeModellingInstance(int_t model, out int_t noVertices, out int_t noIndices, double scale, int_t instance);

		/// <summary>
		///		finalizeModelling                                       (http://rdf.bg/ifcdoc/CS64/finalizeModelling.html)
		///
		///	This call is deprecated, please use call UpdateInstanceVertexBuffer() and UpdateInstanceIndexBuffer().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "finalizeModelling")]
		public static extern int_t finalizeModelling(int_t model, float[] vertices, out int_t indices, int_t FVF);

		/// <summary>
		///		getInstanceInModelling                                  (http://rdf.bg/ifcdoc/CS64/getInstanceInModelling.html)
		///
		///	This call is deprecated, there is no direct / easy replacement although the functionality is present. If you still use this call please contact RDF to find a solution together.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceInModelling")]
		public static extern int_t getInstanceInModelling(int_t model, int_t instance, int_t mode, out int_t startVertex, out int_t startIndex, out int_t primitiveCount);

		/// <summary>
		///		setVertexOffset                                         (http://rdf.bg/ifcdoc/CS64/setVertexOffset.html)
		///
		///	This call is deprecated, please use call SetVertexBufferOffset().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setVertexOffset")]
		public static extern void setVertexOffset(int_t model, double x, double y, double z);

		/// <summary>
		///		setFormat                                               (http://rdf.bg/ifcdoc/CS64/setFormat.html)
		///
		///	This call is deprecated, please use call SetFormat().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setFormat")]
		public static extern void setFormat(int_t model, int_t setting, int_t mask);

		/// <summary>
		///		getConceptualFaceCnt                                    (http://rdf.bg/ifcdoc/CS64/getConceptualFaceCnt.html)
		///
		///	This call is deprecated, please use call GetConceptualFaceCnt().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getConceptualFaceCnt")]
		public static extern int_t getConceptualFaceCnt(int_t instance);

		/// <summary>
		///		getConceptualFaceEx                                     (http://rdf.bg/ifcdoc/CS64/getConceptualFaceEx.html)
		///
		///	This call is deprecated, please use call GetConceptualFaceEx().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getConceptualFaceEx")]
		public static extern int_t getConceptualFaceEx(int_t instance, int_t index, out int_t startIndexTriangles, out int_t noIndicesTriangles, out int_t startIndexLines, out int_t noIndicesLines, out int_t startIndexPoints, out int_t noIndicesPoints, out int_t startIndexFacePolygons, out int_t noIndicesFacePolygons, out int_t startIndexConceptualFacePolygons, out int_t noIndicesConceptualFacePolygons);

		/// <summary>
		///		createGeometryConversion                                (http://rdf.bg/ifcdoc/CS64/createGeometryConversion.html)
		///
		///	This call is deprecated, please use call ... .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "createGeometryConversion")]
		public static extern void createGeometryConversion(int_t instance, out Int64 owlInstance);

		/// <summary>
		///		convertInstance                                         (http://rdf.bg/ifcdoc/CS64/convertInstance.html)
		///
		///	This call is deprecated, please use call ... .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "convertInstance")]
		public static extern void convertInstance(int_t instance);

		/// <summary>
		///		initializeModellingInstanceEx                           (http://rdf.bg/ifcdoc/CS64/initializeModellingInstanceEx.html)
		///
		///	This call is deprecated, please use call CalculateInstance().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "initializeModellingInstanceEx")]
		public static extern int_t initializeModellingInstanceEx(int_t model, out int_t noVertices, out int_t noIndices, double scale, int_t instance, int_t instanceList);

		/// <summary>
		///		exportModellingAsOWL                                    (http://rdf.bg/ifcdoc/CS64/exportModellingAsOWL.html)
		///
		///	This call is deprecated, please contact us if you use this call.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "exportModellingAsOWL")]
		public static extern void exportModellingAsOWL(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "exportModellingAsOWL")]
		public static extern void exportModellingAsOWL(int_t model, byte[] fileName);
    }
}
