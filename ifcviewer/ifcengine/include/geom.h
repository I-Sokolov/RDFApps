//
// Helper classes (C++ wrappers)
//
#ifndef __RDF_LTD__GEOM_H
#define __RDF_LTD__GEOM_H


#ifndef ASSERT
#define ASSERT assert
#endif


#include	"engine.h"


namespace GEOM
{
    class AdvancedFace;
    class AdvancedFace2D;
    class AdvancedFace3D;
    class AdvancedFaceMapped;
    class AlignedSegment;
    class AlignedSegments;
    class Alignment;
    class AmbientLight;
    class Appearance;
    class Arc3D;
    class BezierCurve;
    class BezierSurface;
    class BiQuadraticParabola;
    class Blend;
    class BlossCurve;
    class BooleanOperation;
    class BooleanOperation2D;
    class BoundaryRepresentation;
    class Box;
    class BSplineCurve;
    class BSplineSurface;
    class Circle;
    class CircleByPoints;
    class ClippedPyramid;
    class Clipping;
    class Clothoid;
    class ClothoidCurve;
    class Collection;
    class Color;
    class ColorComponent;
    class Cone;
    class ConicalCurve;
    class ConicalSurface;
    class Copy;
    class CosineCurve;
    class Cube;
    class CubicParabola;
    class Cuboid;
    class Curve;
    class CurvesFromSurface;
    class Cylinder;
    class CylindricalSurface;
    class DegenerateToroidalSurface;
    class Deviation;
    class DirectionalLight;
    class DirectLight;
    class Dummy;
    class Ellipse;
    class EllipticCone;
    class Environment;
    class ExtrudedPolygon;
    class ExtrudedPolygonTapered;
    class ExtrusionAreaSolid;
    class ExtrusionAreaSolidSet;
    class Face;
    class Face2D;
    class Face2DSet;
    class FiniteSurface;
    class FrustumCone;
    class GeometricItem;
    class HornTorus;
    class Hyperbola;
    class InfiniteSurface;
    class InverseMatrix;
    class InvertedCurve;
    class InvertedSurface;
    class Light;
    class Line3D;
    class Line3Dn;
    class Line3DSet;
    class LineByFace;
    class Material;
    class Mathematics;
    class Matrix;
    class MatrixMultiplication;
    class Mesh;
    class NURBSCurve;
    class NURBSSurface;
    class Parabola;
    class Plane;
    class PlaneSurface;
    class Point;
    class Point3D;
    class Point3DSet;
    class Point3DSetByGeometricItem;
    class PointLight;
    class PointLoop;
    class Polygon2D;
    class Polygon3D;
    class PolyLine3D;
    class Prism;
    class Profile;
    class Projection;
    class Pyramid;
    class RationalBezierCurve;
    class RationalBezierSurface;
    class RectangleCurve;
    class Repetition;
    class RingTorus;
    class Shadow;
    class SineCurve;
    class SkewedCone;
    class SkewedFrustumCone;
    class Solid;
    class SolidBySurface;
    class SolidLine;
    class Sphere;
    class SphericalSurface;
    class SpindleTorus;
    class Spiral;
    class SplineCurve;
    class SplineSurface;
    class SpotLight;
    class Surface;
    class SurfaceBySweptCurve;
    class SurfaceOfLinearExtrusion;
    class SurfaceOfRevolution;
    class SweptAreaSolid;
    class SweptAreaSolidSet;
    class SweptAreaSolidTapered;
    class SweptBlend;
    class Texture;
    class ToroidalSurface;
    class Torus;
    class Transformation;
    class TransitionalCurve;
    class TriangleCurve;
    class TriangleReduction;
    class TriangleSet;
    class Vector;
    class Vector3;
    class View;
    class World;


    /// <summary>
    /// Provides utility methods to interact with a genetic instnace of OWL class
    /// You also can use object of this class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Instance
    {
    protected:
        /// <summary>
        /// underlyed instance handle
        /// </summary>
        int64_t m_instance;

    public:
        /// <summary>
        /// Create an instance of specified class
        /// </summary>
        static int64_t Create(int64_t model, const char* className, const char* instanseName)
        {
            int64_t clsid = GetClassByName(model, className);
            ASSERT(clsid != 0);

            int64_t instance = CreateInstance(clsid, instanseName);
            ASSERT(instance != 0);

            return instance;
        }

        /// <summary>
        /// Create an instance of specified class
        /// </summary>
        static int64_t Create(int64_t model, const char* className) { return Create(model, className, NULL); }

        /// <summary>
        /// Constructs object that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Instance(int64_t instance, const char* cls)
        {
            m_instance = instance;
#ifdef _DEBUG
            if (m_instance != 0 && cls != NULL) {
                int64_t clsid1 = GetInstanceClass(m_instance);
                int64_t model = GetModel(m_instance);
                int64_t clsid2 = GetClassByName(model, cls);
                ASSERT(clsid1 == clsid2);
            }
#endif
        }


        /// <summary>
        /// Conversion to instance handle, so the object of the class can be used anywhere where a handle required
        /// </summary>
        operator int64_t() { return m_instance; }

        /// <summary>
        /// Get property id from property name
        /// </summary>
        int64_t GetPropertyId(const char* name)
        {
            int64_t model = GetModel(m_instance);
            ASSERT(model != 0);

            int64_t propId = GetPropertyByName(model, name);
            ASSERT(propId != 0);

            return propId;
        }

        /// <summary>
        /// 
        /// </summary>
        template<typename TElem> void SetDatatypeProperty(const char* name, TElem* values, int64_t count)
        {
            int64_t propId = GetPropertyId(name);
            int64_t res = ::SetDatatypeProperty(m_instance, propId, values, count);
            ASSERT(res == 0);
        }


        /// <summary>
        /// 
        /// </summary>
        template<typename TElem> TElem* GetDatatypeProperty(const char* name, int64_t* pCount)
        {
            int64_t propId = GetPropertyId(name);

            TElem* values = NULL;
            int64_t count = 0;
            int64_t res = ::GetDatatypeProperty(m_instance, propId, (void**)&values, &count);
            ASSERT(res == 0);

            if (pCount) {
                *pCount = count;
            }

            if (count > 0) {
                return values;
            }
            else {
                return NULL;
            }
        }


        /// <summary>
        /// 
        /// </summary>
        template<class TInstance> void SetObjectProperty(const char* name, const TInstance* instances, int64_t count)
        {
            int64_t propId = GetPropertyId(name);
            int64_t res = ::SetObjectProperty(m_instance, propId, (int64_t*)instances, count);
            ASSERT(res == 0);
        }

        /// <summary>
        /// 
        /// </summary>
        template<class TInstance> TInstance* GetObjectProperty(const char* name, int64_t* pCount)
        {
            int64_t propId = GetPropertyId(name);

            int64_t* values = NULL;
            int64_t count = 0;
            int64_t res = ::GetObjectProperty(m_instance, propId, &values, &count);
            ASSERT(res == 0);

            if (pCount) {
                *pCount = count;
            }

            if (count > 0) {
                return (TInstance*)values;
            }
            else {
                return NULL;
            }
        }
    };


    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class GeometricItem
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class GeometricItem : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class GeometricItem and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static GeometricItem Create(int64_t model, const char* name=NULL) { return GeometricItem(Instance::Create(model, "GeometricItem", name), "GeometricItem");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        GeometricItem(int64_t instance = NULL, const char* checkClassName = NULL)
            : Instance(instance, (checkClassName != NULL) ? checkClassName : "GeometricItem")
        {}

       //
       // Properties with known cardinality restrictions to GeometricItem
       //

        ///<summary>Sets relationship from this instance to an instance of Material</summary>
        void set_material(const Material& instance) { SetObjectProperty<Material>("material", &instance, 1); }
        ///<summary>Get related instance</summary>
        Material* get_material() { return GetObjectProperty<Material>("material", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class AdvancedFace
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFace : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFace and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace Create(int64_t model, const char* name=NULL) { return AdvancedFace(Instance::Create(model, "AdvancedFace", name), "AdvancedFace");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        AdvancedFace(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "AdvancedFace")
        {}

       //
       // Properties with known cardinality restrictions to AdvancedFace
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_bounds(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("bounds", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_bounds(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("bounds", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_bounds(int64_t* pCount) { return GetObjectProperty<Curve>("bounds", pCount); }
        int64_t* get_bounds_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("bounds", pCount); }
        ///<summary>Sets relationship from this instance to an instance of Surface</summary>
        void set_surface(const Surface& instance) { SetObjectProperty<Surface>("surface", &instance, 1); }
        ///<summary>Get related instance</summary>
        Surface* get_surface() { return GetObjectProperty<Surface>("surface", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Face
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Face : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Face and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face Create(int64_t model, const char* name=NULL) { return Face(Instance::Create(model, "Face", name), "Face");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Face(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Face")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class AdvancedFace2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFace2D : public AdvancedFace
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFace2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace2D Create(int64_t model, const char* name=NULL) { return AdvancedFace2D(Instance::Create(model, "AdvancedFace2D", name), "AdvancedFace2D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        AdvancedFace2D(int64_t instance = NULL, const char* checkClassName = NULL)
            : AdvancedFace(instance, (checkClassName != NULL) ? checkClassName : "AdvancedFace2D")
        {}

       //
       // Properties with known cardinality restrictions to AdvancedFace2D
       //

        ///<summary>Sets value of clipped</summary>
        void set_clipped(bool value) { SetDatatypeProperty ("clipped", &value, 1); }
        ///<summary>Gets value of clipped, returns NULL is the property was not set</summary>
        bool* get_clipped() { return GetDatatypeProperty<bool>("clipped", NULL); }
        ///<summary>Sets value of scalingX</summary>
        void set_scalingX(double value) { SetDatatypeProperty ("scalingX", &value, 1); }
        ///<summary>Gets value of scalingX, returns NULL is the property was not set</summary>
        double* get_scalingX() { return GetDatatypeProperty<double>("scalingX", NULL); }
        ///<summary>Sets value of scalingY</summary>
        void set_scalingY(double value) { SetDatatypeProperty ("scalingY", &value, 1); }
        ///<summary>Gets value of scalingY, returns NULL is the property was not set</summary>
        double* get_scalingY() { return GetDatatypeProperty<double>("scalingY", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class AdvancedFace3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFace3D : public AdvancedFace
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFace3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace3D Create(int64_t model, const char* name=NULL) { return AdvancedFace3D(Instance::Create(model, "AdvancedFace3D", name), "AdvancedFace3D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        AdvancedFace3D(int64_t instance = NULL, const char* checkClassName = NULL)
            : AdvancedFace(instance, (checkClassName != NULL) ? checkClassName : "AdvancedFace3D")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Curve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Curve : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Curve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Curve Create(int64_t model, const char* name=NULL) { return Curve(Instance::Create(model, "Curve", name), "Curve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Curve(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Curve")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class AdvancedFaceMapped
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFaceMapped : public AdvancedFace
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFaceMapped and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFaceMapped Create(int64_t model, const char* name=NULL) { return AdvancedFaceMapped(Instance::Create(model, "AdvancedFaceMapped", name), "AdvancedFaceMapped");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        AdvancedFaceMapped(int64_t instance = NULL, const char* checkClassName = NULL)
            : AdvancedFace(instance, (checkClassName != NULL) ? checkClassName : "AdvancedFaceMapped")
        {}

       //
       // Properties with known cardinality restrictions to AdvancedFaceMapped
       //

        ///<summary>Sets value of scalingX</summary>
        void set_scalingX(double value) { SetDatatypeProperty ("scalingX", &value, 1); }
        ///<summary>Gets value of scalingX, returns NULL is the property was not set</summary>
        double* get_scalingX() { return GetDatatypeProperty<double>("scalingX", NULL); }
        ///<summary>Sets value of scalingY</summary>
        void set_scalingY(double value) { SetDatatypeProperty ("scalingY", &value, 1); }
        ///<summary>Gets value of scalingY, returns NULL is the property was not set</summary>
        double* get_scalingY() { return GetDatatypeProperty<double>("scalingY", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class AlignedSegment
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AlignedSegment : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AlignedSegment and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AlignedSegment Create(int64_t model, const char* name=NULL) { return AlignedSegment(Instance::Create(model, "AlignedSegment", name), "AlignedSegment");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        AlignedSegment(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "AlignedSegment")
        {}

       //
       // Properties with known cardinality restrictions to AlignedSegment
       //

        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of offsetX</summary>
        void set_offsetX(double value) { SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets value of offsetX, returns NULL is the property was not set</summary>
        double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        void set_offsetY(double value) { SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets value of offsetY, returns NULL is the property was not set</summary>
        double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_segment(const Curve& instance) { SetObjectProperty<Curve>("segment", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_segment() { return GetObjectProperty<Curve>("segment", NULL); }
        ///<summary>Sets values of tangentDirectionStart. OWL cardinality 3..3</summary>
        void set_tangentDirectionStart(double* values, int64_t count) { SetDatatypeProperty ("tangentDirectionStart", values, count); }
        ///<summary>Gets values of tangentDirectionStart. OWL cardinality 3..3</summary>
        double* get_tangentDirectionStart(int64_t* pCount) { return GetDatatypeProperty<double>("tangentDirectionStart", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class AlignedSegments
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AlignedSegments : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AlignedSegments and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AlignedSegments Create(int64_t model, const char* name=NULL) { return AlignedSegments(Instance::Create(model, "AlignedSegments", name), "AlignedSegments");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        AlignedSegments(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "AlignedSegments")
        {}

       //
       // Properties with known cardinality restrictions to AlignedSegments
       //

        ///<summary>Sets relationships from this instance to an array of AlignedSegment. OWL cardinality 0..-1</summary>
        void set_segments(const AlignedSegment* instances, int64_t count) { SetObjectProperty<AlignedSegment>("segments", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_segments(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("segments", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        AlignedSegment* get_segments(int64_t* pCount) { return GetObjectProperty<AlignedSegment>("segments", pCount); }
        int64_t* get_segments_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("segments", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Alignment
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Alignment : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Alignment and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Alignment Create(int64_t model, const char* name=NULL) { return Alignment(Instance::Create(model, "Alignment", name), "Alignment");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Alignment(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Alignment")
        {}

       //
       // Properties with known cardinality restrictions to Alignment
       //

        ///<summary>Sets relationship from this instance to an instance of AlignedSegments</summary>
        void set_horizontal(const AlignedSegments& instance) { SetObjectProperty<AlignedSegments>("horizontal", &instance, 1); }
        ///<summary>Get related instance</summary>
        AlignedSegments* get_horizontal() { return GetObjectProperty<AlignedSegments>("horizontal", NULL); }
        ///<summary>Sets value of offsetX</summary>
        void set_offsetX(double value) { SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets value of offsetX, returns NULL is the property was not set</summary>
        double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of type</summary>
        void set_type(long value) { SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets value of type, returns NULL is the property was not set</summary>
        long* get_type() { return GetDatatypeProperty<long>("type", NULL); }
        ///<summary>Sets relationship from this instance to an instance of AlignedSegments</summary>
        void set_vertical(const AlignedSegments& instance) { SetObjectProperty<AlignedSegments>("vertical", &instance, 1); }
        ///<summary>Get related instance</summary>
        AlignedSegments* get_vertical() { return GetObjectProperty<AlignedSegments>("vertical", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Environment
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Environment : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Environment and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Environment Create(int64_t model, const char* name=NULL) { return Environment(Instance::Create(model, "Environment", name), "Environment");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Environment(int64_t instance = NULL, const char* checkClassName = NULL)
            : Instance(instance, (checkClassName != NULL) ? checkClassName : "Environment")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Light
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Light : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Light and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Light Create(int64_t model, const char* name=NULL) { return Light(Instance::Create(model, "Light", name), "Light");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Light(int64_t instance = NULL, const char* checkClassName = NULL)
            : Environment(instance, (checkClassName != NULL) ? checkClassName : "Light")
        {}

       //
       // Properties with known cardinality restrictions to Light
       //

        ///<summary>Sets relationship from this instance to an instance of Color</summary>
        void set_color(const Color& instance) { SetObjectProperty<Color>("color", &instance, 1); }
        ///<summary>Get related instance</summary>
        Color* get_color() { return GetObjectProperty<Color>("color", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class AmbientLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AmbientLight : public Light
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AmbientLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AmbientLight Create(int64_t model, const char* name=NULL) { return AmbientLight(Instance::Create(model, "AmbientLight", name), "AmbientLight");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        AmbientLight(int64_t instance = NULL, const char* checkClassName = NULL)
            : Light(instance, (checkClassName != NULL) ? checkClassName : "AmbientLight")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Appearance
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Appearance : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Appearance and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Appearance Create(int64_t model, const char* name=NULL) { return Appearance(Instance::Create(model, "Appearance", name), "Appearance");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Appearance(int64_t instance = NULL, const char* checkClassName = NULL)
            : Instance(instance, (checkClassName != NULL) ? checkClassName : "Appearance")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Arc3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Arc3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Arc3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Arc3D Create(int64_t model, const char* name=NULL) { return Arc3D(Instance::Create(model, "Arc3D", name), "Arc3D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Arc3D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Arc3D")
        {}

       //
       // Properties with known cardinality restrictions to Arc3D
       //

        ///<summary>Sets value of hasNormals</summary>
        void set_hasNormals(bool value) { SetDatatypeProperty ("hasNormals", &value, 1); }
        ///<summary>Gets value of hasNormals, returns NULL is the property was not set</summary>
        bool* get_hasNormals() { return GetDatatypeProperty<bool>("hasNormals", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
        ///<summary>Sets value of size</summary>
        void set_size(double value) { SetDatatypeProperty ("size", &value, 1); }
        ///<summary>Gets value of size, returns NULL is the property was not set</summary>
        double* get_size() { return GetDatatypeProperty<double>("size", NULL); }
        ///<summary>Sets value of start</summary>
        void set_start(double value) { SetDatatypeProperty ("start", &value, 1); }
        ///<summary>Gets value of start, returns NULL is the property was not set</summary>
        double* get_start() { return GetDatatypeProperty<double>("start", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SplineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SplineCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SplineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SplineCurve Create(int64_t model, const char* name=NULL) { return SplineCurve(Instance::Create(model, "SplineCurve", name), "SplineCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SplineCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "SplineCurve")
        {}

       //
       // Properties with known cardinality restrictions to SplineCurve
       //

        ///<summary>Sets value of closed</summary>
        void set_closed(bool value) { SetDatatypeProperty ("closed", &value, 1); }
        ///<summary>Gets value of closed, returns NULL is the property was not set</summary>
        bool* get_closed() { return GetDatatypeProperty<bool>("closed", NULL); }
        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 2..-1</summary>
        void set_controlPoints(const Point3D* instances, int64_t count) { SetObjectProperty<Point3D>("controlPoints", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 2..-1</summary>
        void set_controlPoints(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("controlPoints", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 2..-1</summary>
        Point3D* get_controlPoints(int64_t* pCount) { return GetObjectProperty<Point3D>("controlPoints", pCount); }
        int64_t* get_controlPoints_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("controlPoints", pCount); }
        ///<summary>Sets value of count</summary>
        void set_count(long value) { SetDatatypeProperty ("count", &value, 1); }
        ///<summary>Gets value of count, returns NULL is the property was not set</summary>
        long* get_count() { return GetDatatypeProperty<long>("count", NULL); }
        ///<summary>Sets value of degree</summary>
        void set_degree(long value) { SetDatatypeProperty ("degree", &value, 1); }
        ///<summary>Gets value of degree, returns NULL is the property was not set</summary>
        long* get_degree() { return GetDatatypeProperty<long>("degree", NULL); }
        ///<summary>Sets value of segmentationLength</summary>
        void set_segmentationLength(double value) { SetDatatypeProperty ("segmentationLength", &value, 1); }
        ///<summary>Gets value of segmentationLength, returns NULL is the property was not set</summary>
        double* get_segmentationLength() { return GetDatatypeProperty<double>("segmentationLength", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BezierCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BezierCurve : public SplineCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BezierCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BezierCurve Create(int64_t model, const char* name=NULL) { return BezierCurve(Instance::Create(model, "BezierCurve", name), "BezierCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BezierCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : SplineCurve(instance, (checkClassName != NULL) ? checkClassName : "BezierCurve")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Surface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Surface : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Surface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Surface Create(int64_t model, const char* name=NULL) { return Surface(Instance::Create(model, "Surface", name), "Surface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Surface(int64_t instance = NULL, const char* checkClassName = NULL)
            : Face(instance, (checkClassName != NULL) ? checkClassName : "Surface")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class FiniteSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class FiniteSurface : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class FiniteSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static FiniteSurface Create(int64_t model, const char* name=NULL) { return FiniteSurface(Instance::Create(model, "FiniteSurface", name), "FiniteSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        FiniteSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : Surface(instance, (checkClassName != NULL) ? checkClassName : "FiniteSurface")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SplineSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SplineSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SplineSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SplineSurface Create(int64_t model, const char* name=NULL) { return SplineSurface(Instance::Create(model, "SplineSurface", name), "SplineSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SplineSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : FiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "SplineSurface")
        {}

       //
       // Properties with known cardinality restrictions to SplineSurface
       //

        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 4..-1</summary>
        void set_controlPoints(const Point3D* instances, int64_t count) { SetObjectProperty<Point3D>("controlPoints", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 4..-1</summary>
        void set_controlPoints(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("controlPoints", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 4..-1</summary>
        Point3D* get_controlPoints(int64_t* pCount) { return GetObjectProperty<Point3D>("controlPoints", pCount); }
        int64_t* get_controlPoints_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("controlPoints", pCount); }
        ///<summary>Sets value of segmentationLength</summary>
        void set_segmentationLength(double value) { SetDatatypeProperty ("segmentationLength", &value, 1); }
        ///<summary>Gets value of segmentationLength, returns NULL is the property was not set</summary>
        double* get_segmentationLength() { return GetDatatypeProperty<double>("segmentationLength", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
        ///<summary>Sets value of uClosed</summary>
        void set_uClosed(bool value) { SetDatatypeProperty ("uClosed", &value, 1); }
        ///<summary>Gets value of uClosed, returns NULL is the property was not set</summary>
        bool* get_uClosed() { return GetDatatypeProperty<bool>("uClosed", NULL); }
        ///<summary>Sets value of uCount</summary>
        void set_uCount(long value) { SetDatatypeProperty ("uCount", &value, 1); }
        ///<summary>Gets value of uCount, returns NULL is the property was not set</summary>
        long* get_uCount() { return GetDatatypeProperty<long>("uCount", NULL); }
        ///<summary>Sets value of uDegree</summary>
        void set_uDegree(long value) { SetDatatypeProperty ("uDegree", &value, 1); }
        ///<summary>Gets value of uDegree, returns NULL is the property was not set</summary>
        long* get_uDegree() { return GetDatatypeProperty<long>("uDegree", NULL); }
        ///<summary>Sets value of uSegmentationParts</summary>
        void set_uSegmentationParts(long value) { SetDatatypeProperty ("uSegmentationParts", &value, 1); }
        ///<summary>Gets value of uSegmentationParts, returns NULL is the property was not set</summary>
        long* get_uSegmentationParts() { return GetDatatypeProperty<long>("uSegmentationParts", NULL); }
        ///<summary>Sets value of vClosed</summary>
        void set_vClosed(bool value) { SetDatatypeProperty ("vClosed", &value, 1); }
        ///<summary>Gets value of vClosed, returns NULL is the property was not set</summary>
        bool* get_vClosed() { return GetDatatypeProperty<bool>("vClosed", NULL); }
        ///<summary>Sets value of vCount</summary>
        void set_vCount(long value) { SetDatatypeProperty ("vCount", &value, 1); }
        ///<summary>Gets value of vCount, returns NULL is the property was not set</summary>
        long* get_vCount() { return GetDatatypeProperty<long>("vCount", NULL); }
        ///<summary>Sets value of vDegree</summary>
        void set_vDegree(long value) { SetDatatypeProperty ("vDegree", &value, 1); }
        ///<summary>Gets value of vDegree, returns NULL is the property was not set</summary>
        long* get_vDegree() { return GetDatatypeProperty<long>("vDegree", NULL); }
        ///<summary>Sets value of vSegmentationParts</summary>
        void set_vSegmentationParts(long value) { SetDatatypeProperty ("vSegmentationParts", &value, 1); }
        ///<summary>Gets value of vSegmentationParts, returns NULL is the property was not set</summary>
        long* get_vSegmentationParts() { return GetDatatypeProperty<long>("vSegmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BezierSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BezierSurface : public SplineSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BezierSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BezierSurface Create(int64_t model, const char* name=NULL) { return BezierSurface(Instance::Create(model, "BezierSurface", name), "BezierSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BezierSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : SplineSurface(instance, (checkClassName != NULL) ? checkClassName : "BezierSurface")
        {}

       //
       // Properties with known cardinality restrictions to BezierSurface
       //

        ///<summary>Sets value of setting</summary>
        void set_setting(long value) { SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets value of setting, returns NULL is the property was not set</summary>
        long* get_setting() { return GetDatatypeProperty<long>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class TransitionalCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TransitionalCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TransitionalCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TransitionalCurve Create(int64_t model, const char* name=NULL) { return TransitionalCurve(Instance::Create(model, "TransitionalCurve", name), "TransitionalCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        TransitionalCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "TransitionalCurve")
        {}

       //
       // Properties with known cardinality restrictions to TransitionalCurve
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        void set_direction(const Vector& instance) { SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_path(const Curve& instance) { SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets value of radiusI</summary>
        void set_radiusI(double value) { SetDatatypeProperty ("radiusI", &value, 1); }
        ///<summary>Gets value of radiusI, returns NULL is the property was not set</summary>
        double* get_radiusI() { return GetDatatypeProperty<double>("radiusI", NULL); }
        ///<summary>Sets value of radiusII</summary>
        void set_radiusII(double value) { SetDatatypeProperty ("radiusII", &value, 1); }
        ///<summary>Gets value of radiusII, returns NULL is the property was not set</summary>
        double* get_radiusII() { return GetDatatypeProperty<double>("radiusII", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BiQuadraticParabola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BiQuadraticParabola : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BiQuadraticParabola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BiQuadraticParabola Create(int64_t model, const char* name=NULL) { return BiQuadraticParabola(Instance::Create(model, "BiQuadraticParabola", name), "BiQuadraticParabola");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BiQuadraticParabola(int64_t instance = NULL, const char* checkClassName = NULL)
            : TransitionalCurve(instance, (checkClassName != NULL) ? checkClassName : "BiQuadraticParabola")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Solid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Solid : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Solid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Solid Create(int64_t model, const char* name=NULL) { return Solid(Instance::Create(model, "Solid", name), "Solid");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Solid(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Solid")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Blend
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Blend : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Blend and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Blend Create(int64_t model, const char* name=NULL) { return Blend(Instance::Create(model, "Blend", name), "Blend");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Blend(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Blend")
        {}

       //
       // Properties with known cardinality restrictions to Blend
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_bottomPolygon(const Curve& instance) { SetObjectProperty<Curve>("bottomPolygon", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_bottomPolygon() { return GetObjectProperty<Curve>("bottomPolygon", NULL); }
        ///<summary>Sets value of forceSolid</summary>
        void set_forceSolid(bool value) { SetDatatypeProperty ("forceSolid", &value, 1); }
        ///<summary>Gets value of forceSolid, returns NULL is the property was not set</summary>
        bool* get_forceSolid() { return GetDatatypeProperty<bool>("forceSolid", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets value of hasBottom</summary>
        void set_hasBottom(bool value) { SetDatatypeProperty ("hasBottom", &value, 1); }
        ///<summary>Gets value of hasBottom, returns NULL is the property was not set</summary>
        bool* get_hasBottom() { return GetDatatypeProperty<bool>("hasBottom", NULL); }
        ///<summary>Sets value of hasTop</summary>
        void set_hasTop(bool value) { SetDatatypeProperty ("hasTop", &value, 1); }
        ///<summary>Gets value of hasTop, returns NULL is the property was not set</summary>
        bool* get_hasTop() { return GetDatatypeProperty<bool>("hasTop", NULL); }
        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_topPolygon(const Curve& instance) { SetObjectProperty<Curve>("topPolygon", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_topPolygon() { return GetObjectProperty<Curve>("topPolygon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BlossCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BlossCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BlossCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BlossCurve Create(int64_t model, const char* name=NULL) { return BlossCurve(Instance::Create(model, "BlossCurve", name), "BlossCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BlossCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : TransitionalCurve(instance, (checkClassName != NULL) ? checkClassName : "BlossCurve")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BooleanOperation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BooleanOperation : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BooleanOperation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BooleanOperation Create(int64_t model, const char* name=NULL) { return BooleanOperation(Instance::Create(model, "BooleanOperation", name), "BooleanOperation");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BooleanOperation(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "BooleanOperation")
        {}

       //
       // Properties with known cardinality restrictions to BooleanOperation
       //

        ///<summary>Sets relationship from this instance to an instance of Deviation</summary>
        void set_deviation(const Deviation& instance) { SetObjectProperty<Deviation>("deviation", &instance, 1); }
        ///<summary>Get related instance</summary>
        Deviation* get_deviation() { return GetObjectProperty<Deviation>("deviation", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_firstObject(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("firstObject", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_firstObject() { return GetObjectProperty<GeometricItem>("firstObject", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_secondObject(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("secondObject", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_secondObject() { return GetObjectProperty<GeometricItem>("secondObject", NULL); }
        ///<summary>Sets value of setting</summary>
        void set_setting(long value) { SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets value of setting, returns NULL is the property was not set</summary>
        long* get_setting() { return GetDatatypeProperty<long>("setting", NULL); }
        ///<summary>Sets value of type</summary>
        void set_type(long value) { SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets value of type, returns NULL is the property was not set</summary>
        long* get_type() { return GetDatatypeProperty<long>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BooleanOperation2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BooleanOperation2D : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BooleanOperation2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BooleanOperation2D Create(int64_t model, const char* name=NULL) { return BooleanOperation2D(Instance::Create(model, "BooleanOperation2D", name), "BooleanOperation2D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BooleanOperation2D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Face(instance, (checkClassName != NULL) ? checkClassName : "BooleanOperation2D")
        {}

       //
       // Properties with known cardinality restrictions to BooleanOperation2D
       //

        ///<summary>Sets relationship from this instance to an instance of Deviation</summary>
        void set_deviation(const Deviation& instance) { SetObjectProperty<Deviation>("deviation", &instance, 1); }
        ///<summary>Get related instance</summary>
        Deviation* get_deviation() { return GetObjectProperty<Deviation>("deviation", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_firstObject(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("firstObject", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_firstObject() { return GetObjectProperty<GeometricItem>("firstObject", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_secondObject(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("secondObject", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_secondObject() { return GetObjectProperty<GeometricItem>("secondObject", NULL); }
        ///<summary>Sets value of type</summary>
        void set_type(long value) { SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets value of type, returns NULL is the property was not set</summary>
        long* get_type() { return GetDatatypeProperty<long>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BoundaryRepresentation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BoundaryRepresentation : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BoundaryRepresentation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BoundaryRepresentation Create(int64_t model, const char* name=NULL) { return BoundaryRepresentation(Instance::Create(model, "BoundaryRepresentation", name), "BoundaryRepresentation");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BoundaryRepresentation(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "BoundaryRepresentation")
        {}

       //
       // Properties with known cardinality restrictions to BoundaryRepresentation
       //

        ///<summary>Sets value of consistencyCheck</summary>
        void set_consistencyCheck(long value) { SetDatatypeProperty ("consistencyCheck", &value, 1); }
        ///<summary>Gets value of consistencyCheck, returns NULL is the property was not set</summary>
        long* get_consistencyCheck() { return GetDatatypeProperty<long>("consistencyCheck", NULL); }
        ///<summary>Sets value of epsilon</summary>
        void set_epsilon(double value) { SetDatatypeProperty ("epsilon", &value, 1); }
        ///<summary>Gets value of epsilon, returns NULL is the property was not set</summary>
        double* get_epsilon() { return GetDatatypeProperty<double>("epsilon", NULL); }
        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 0..-1</summary>
        void set_faces(const Face* instances, int64_t count) { SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_faces(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
        ///<summary>Sets values of flags. OWL cardinality 0..-1</summary>
        void set_flags(long* values, int64_t count) { SetDatatypeProperty ("flags", values, count); }
        ///<summary>Gets values of flags. OWL cardinality 0..-1</summary>
        long* get_flags(int64_t* pCount) { return GetDatatypeProperty<long>("flags", pCount); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets values of indices. OWL cardinality 0..-1</summary>
        void set_indices(long* values, int64_t count) { SetDatatypeProperty ("indices", values, count); }
        ///<summary>Gets values of indices. OWL cardinality 0..-1</summary>
        long* get_indices(int64_t* pCount) { return GetDatatypeProperty<long>("indices", pCount); }
        ///<summary>Sets values of normalCoordinates. OWL cardinality 0..-1</summary>
        void set_normalCoordinates(double* values, int64_t count) { SetDatatypeProperty ("normalCoordinates", values, count); }
        ///<summary>Gets values of normalCoordinates. OWL cardinality 0..-1</summary>
        double* get_normalCoordinates(int64_t* pCount) { return GetDatatypeProperty<double>("normalCoordinates", pCount); }
        ///<summary>Sets values of normalIndices. OWL cardinality 0..-1</summary>
        void set_normalIndices(long* values, int64_t count) { SetDatatypeProperty ("normalIndices", values, count); }
        ///<summary>Gets values of normalIndices. OWL cardinality 0..-1</summary>
        long* get_normalIndices(int64_t* pCount) { return GetDatatypeProperty<long>("normalIndices", pCount); }
        ///<summary>Sets value of relativeEpsilon</summary>
        void set_relativeEpsilon(double value) { SetDatatypeProperty ("relativeEpsilon", &value, 1); }
        ///<summary>Gets value of relativeEpsilon, returns NULL is the property was not set</summary>
        double* get_relativeEpsilon() { return GetDatatypeProperty<double>("relativeEpsilon", NULL); }
        ///<summary>Sets values of textureCoordinates. OWL cardinality 0..-1</summary>
        void set_textureCoordinates(double* values, int64_t count) { SetDatatypeProperty ("textureCoordinates", values, count); }
        ///<summary>Gets values of textureCoordinates. OWL cardinality 0..-1</summary>
        double* get_textureCoordinates(int64_t* pCount) { return GetDatatypeProperty<double>("textureCoordinates", pCount); }
        ///<summary>Sets values of textureIndices. OWL cardinality 0..-1</summary>
        void set_textureIndices(long* values, int64_t count) { SetDatatypeProperty ("textureIndices", values, count); }
        ///<summary>Gets values of textureIndices. OWL cardinality 0..-1</summary>
        long* get_textureIndices(int64_t* pCount) { return GetDatatypeProperty<long>("textureIndices", pCount); }
        ///<summary>Sets values of vertices. OWL cardinality 0..-1</summary>
        void set_vertices(double* values, int64_t count) { SetDatatypeProperty ("vertices", values, count); }
        ///<summary>Gets values of vertices. OWL cardinality 0..-1</summary>
        double* get_vertices(int64_t* pCount) { return GetDatatypeProperty<double>("vertices", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Box
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Box : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Box and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Box Create(int64_t model, const char* name=NULL) { return Box(Instance::Create(model, "Box", name), "Box");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Box(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Box")
        {}

       //
       // Properties with known cardinality restrictions to Box
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of width</summary>
        void set_width(double value) { SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets value of width, returns NULL is the property was not set</summary>
        double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BSplineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BSplineCurve : public SplineCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BSplineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BSplineCurve Create(int64_t model, const char* name=NULL) { return BSplineCurve(Instance::Create(model, "BSplineCurve", name), "BSplineCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BSplineCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : SplineCurve(instance, (checkClassName != NULL) ? checkClassName : "BSplineCurve")
        {}

       //
       // Properties with known cardinality restrictions to BSplineCurve
       //

        ///<summary>Sets values of knotMultiplicities. OWL cardinality 0..-1</summary>
        void set_knotMultiplicities(long* values, int64_t count) { SetDatatypeProperty ("knotMultiplicities", values, count); }
        ///<summary>Gets values of knotMultiplicities. OWL cardinality 0..-1</summary>
        long* get_knotMultiplicities(int64_t* pCount) { return GetDatatypeProperty<long>("knotMultiplicities", pCount); }
        ///<summary>Sets values of knots. OWL cardinality 2..-1</summary>
        void set_knots(double* values, int64_t count) { SetDatatypeProperty ("knots", values, count); }
        ///<summary>Gets values of knots. OWL cardinality 2..-1</summary>
        double* get_knots(int64_t* pCount) { return GetDatatypeProperty<double>("knots", pCount); }
        ///<summary>Sets value of setting</summary>
        void set_setting(long value) { SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets value of setting, returns NULL is the property was not set</summary>
        long* get_setting() { return GetDatatypeProperty<long>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class BSplineSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BSplineSurface : public SplineSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BSplineSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BSplineSurface Create(int64_t model, const char* name=NULL) { return BSplineSurface(Instance::Create(model, "BSplineSurface", name), "BSplineSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        BSplineSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : SplineSurface(instance, (checkClassName != NULL) ? checkClassName : "BSplineSurface")
        {}

       //
       // Properties with known cardinality restrictions to BSplineSurface
       //

        ///<summary>Sets value of setting</summary>
        void set_setting(long value) { SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets value of setting, returns NULL is the property was not set</summary>
        long* get_setting() { return GetDatatypeProperty<long>("setting", NULL); }
        ///<summary>Sets values of uKnotMultiplicities. OWL cardinality 0..-1</summary>
        void set_uKnotMultiplicities(long* values, int64_t count) { SetDatatypeProperty ("uKnotMultiplicities", values, count); }
        ///<summary>Gets values of uKnotMultiplicities. OWL cardinality 0..-1</summary>
        long* get_uKnotMultiplicities(int64_t* pCount) { return GetDatatypeProperty<long>("uKnotMultiplicities", pCount); }
        ///<summary>Sets values of uKnots. OWL cardinality 2..-1</summary>
        void set_uKnots(double* values, int64_t count) { SetDatatypeProperty ("uKnots", values, count); }
        ///<summary>Gets values of uKnots. OWL cardinality 2..-1</summary>
        double* get_uKnots(int64_t* pCount) { return GetDatatypeProperty<double>("uKnots", pCount); }
        ///<summary>Sets values of vKnotMultiplicities. OWL cardinality 0..-1</summary>
        void set_vKnotMultiplicities(long* values, int64_t count) { SetDatatypeProperty ("vKnotMultiplicities", values, count); }
        ///<summary>Gets values of vKnotMultiplicities. OWL cardinality 0..-1</summary>
        long* get_vKnotMultiplicities(int64_t* pCount) { return GetDatatypeProperty<long>("vKnotMultiplicities", pCount); }
        ///<summary>Sets values of vKnots. OWL cardinality 2..-1</summary>
        void set_vKnots(double* values, int64_t count) { SetDatatypeProperty ("vKnots", values, count); }
        ///<summary>Gets values of vKnots. OWL cardinality 2..-1</summary>
        double* get_vKnots(int64_t* pCount) { return GetDatatypeProperty<double>("vKnots", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ConicalCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ConicalCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ConicalCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ConicalCurve Create(int64_t model, const char* name=NULL) { return ConicalCurve(Instance::Create(model, "ConicalCurve", name), "ConicalCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ConicalCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "ConicalCurve")
        {}

       //
       // Properties with known cardinality restrictions to ConicalCurve
       //

        ///<summary>Sets value of a</summary>
        void set_a(double value) { SetDatatypeProperty ("a", &value, 1); }
        ///<summary>Gets value of a, returns NULL is the property was not set</summary>
        double* get_a() { return GetDatatypeProperty<double>("a", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
        ///<summary>Sets value of size</summary>
        void set_size(double value) { SetDatatypeProperty ("size", &value, 1); }
        ///<summary>Gets value of size, returns NULL is the property was not set</summary>
        double* get_size() { return GetDatatypeProperty<double>("size", NULL); }
        ///<summary>Sets value of start</summary>
        void set_start(double value) { SetDatatypeProperty ("start", &value, 1); }
        ///<summary>Gets value of start, returns NULL is the property was not set</summary>
        double* get_start() { return GetDatatypeProperty<double>("start", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Circle
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Circle : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Circle and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Circle Create(int64_t model, const char* name=NULL) { return Circle(Instance::Create(model, "Circle", name), "Circle");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Circle(int64_t instance = NULL, const char* checkClassName = NULL)
            : ConicalCurve(instance, (checkClassName != NULL) ? checkClassName : "Circle")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class CircleByPoints
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CircleByPoints : public Circle
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CircleByPoints and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CircleByPoints Create(int64_t model, const char* name=NULL) { return CircleByPoints(Instance::Create(model, "CircleByPoints", name), "CircleByPoints");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        CircleByPoints(int64_t instance = NULL, const char* checkClassName = NULL)
            : Circle(instance, (checkClassName != NULL) ? checkClassName : "CircleByPoints")
        {}

       //
       // Properties with known cardinality restrictions to CircleByPoints
       //

        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 3..3</summary>
        void set_pointReferences(const Point3D* instances, int64_t count) { SetObjectProperty<Point3D>("pointReferences", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 3..3</summary>
        void set_pointReferences(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("pointReferences", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 3..3</summary>
        Point3D* get_pointReferences(int64_t* pCount) { return GetObjectProperty<Point3D>("pointReferences", pCount); }
        int64_t* get_pointReferences_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("pointReferences", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ClippedPyramid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ClippedPyramid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ClippedPyramid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ClippedPyramid Create(int64_t model, const char* name=NULL) { return ClippedPyramid(Instance::Create(model, "ClippedPyramid", name), "ClippedPyramid");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ClippedPyramid(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "ClippedPyramid")
        {}

       //
       // Properties with known cardinality restrictions to ClippedPyramid
       //

        ///<summary>Sets values of coordinates. OWL cardinality 12..12</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 12..12</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..12</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 0..12</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Clipping
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Clipping : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Clipping and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Clipping Create(int64_t model, const char* name=NULL) { return Clipping(Instance::Create(model, "Clipping", name), "Clipping");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Clipping(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Clipping")
        {}

       //
       // Properties with known cardinality restrictions to Clipping
       //

        ///<summary>Sets relationship from this instance to an instance of Deviation</summary>
        void set_deviation(const Deviation& instance) { SetObjectProperty<Deviation>("deviation", &instance, 1); }
        ///<summary>Get related instance</summary>
        Deviation* get_deviation() { return GetObjectProperty<Deviation>("deviation", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_object(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Plane</summary>
        void set_plane(const Plane& instance) { SetObjectProperty<Plane>("plane", &instance, 1); }
        ///<summary>Get related instance</summary>
        Plane* get_plane() { return GetObjectProperty<Plane>("plane", NULL); }
        ///<summary>Sets value of type</summary>
        void set_type(long value) { SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets value of type, returns NULL is the property was not set</summary>
        long* get_type() { return GetDatatypeProperty<long>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Clothoid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Clothoid : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Clothoid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Clothoid Create(int64_t model, const char* name=NULL) { return Clothoid(Instance::Create(model, "Clothoid", name), "Clothoid");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Clothoid(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Clothoid")
        {}

       //
       // Properties with known cardinality restrictions to Clothoid
       //

        ///<summary>Sets value of A</summary>
        void set_A(double value) { SetDatatypeProperty ("A", &value, 1); }
        ///<summary>Gets value of A, returns NULL is the property was not set</summary>
        double* get_A() { return GetDatatypeProperty<double>("A", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        void set_direction(const Vector& instance) { SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of orientation</summary>
        void set_orientation(long value) { SetDatatypeProperty ("orientation", &value, 1); }
        ///<summary>Gets value of orientation, returns NULL is the property was not set</summary>
        long* get_orientation() { return GetDatatypeProperty<long>("orientation", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ClothoidCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ClothoidCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ClothoidCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ClothoidCurve Create(int64_t model, const char* name=NULL) { return ClothoidCurve(Instance::Create(model, "ClothoidCurve", name), "ClothoidCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ClothoidCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : TransitionalCurve(instance, (checkClassName != NULL) ? checkClassName : "ClothoidCurve")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Collection
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Collection : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Collection and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Collection Create(int64_t model, const char* name=NULL) { return Collection(Instance::Create(model, "Collection", name), "Collection");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Collection(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Collection")
        {}

       //
       // Properties with known cardinality restrictions to Collection
       //

        ///<summary>Sets value of consistencyCheck</summary>
        void set_consistencyCheck(long value) { SetDatatypeProperty ("consistencyCheck", &value, 1); }
        ///<summary>Gets value of consistencyCheck, returns NULL is the property was not set</summary>
        long* get_consistencyCheck() { return GetDatatypeProperty<long>("consistencyCheck", NULL); }
        ///<summary>Sets value of epsilon</summary>
        void set_epsilon(double value) { SetDatatypeProperty ("epsilon", &value, 1); }
        ///<summary>Gets value of epsilon, returns NULL is the property was not set</summary>
        double* get_epsilon() { return GetDatatypeProperty<double>("epsilon", NULL); }
        ///<summary>Sets value of forceSolid</summary>
        void set_forceSolid(bool value) { SetDatatypeProperty ("forceSolid", &value, 1); }
        ///<summary>Gets value of forceSolid, returns NULL is the property was not set</summary>
        bool* get_forceSolid() { return GetDatatypeProperty<bool>("forceSolid", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationships from this instance to an array of GeometricItem. OWL cardinality 0..-1</summary>
        void set_objects(const GeometricItem* instances, int64_t count) { SetObjectProperty<GeometricItem>("objects", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_objects(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("objects", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        GeometricItem* get_objects(int64_t* pCount) { return GetObjectProperty<GeometricItem>("objects", pCount); }
        int64_t* get_objects_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("objects", pCount); }
        ///<summary>Sets value of relativeEpsilon</summary>
        void set_relativeEpsilon(double value) { SetDatatypeProperty ("relativeEpsilon", &value, 1); }
        ///<summary>Gets value of relativeEpsilon, returns NULL is the property was not set</summary>
        double* get_relativeEpsilon() { return GetDatatypeProperty<double>("relativeEpsilon", NULL); }
        ///<summary>Sets value of representsSolid</summary>
        void set_representsSolid(bool value) { SetDatatypeProperty ("representsSolid", &value, 1); }
        ///<summary>Gets value of representsSolid, returns NULL is the property was not set</summary>
        bool* get_representsSolid() { return GetDatatypeProperty<bool>("representsSolid", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Color
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Color : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Color and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Color Create(int64_t model, const char* name=NULL) { return Color(Instance::Create(model, "Color", name), "Color");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Color(int64_t instance = NULL, const char* checkClassName = NULL)
            : Appearance(instance, (checkClassName != NULL) ? checkClassName : "Color")
        {}

       //
       // Properties with known cardinality restrictions to Color
       //

        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        void set_ambient(const ColorComponent& instance) { SetObjectProperty<ColorComponent>("ambient", &instance, 1); }
        ///<summary>Get related instance</summary>
        ColorComponent* get_ambient() { return GetObjectProperty<ColorComponent>("ambient", NULL); }
        ///<summary>Sets value of ambientReflectance</summary>
        void set_ambientReflectance(double value) { SetDatatypeProperty ("ambientReflectance", &value, 1); }
        ///<summary>Gets value of ambientReflectance, returns NULL is the property was not set</summary>
        double* get_ambientReflectance() { return GetDatatypeProperty<double>("ambientReflectance", NULL); }
        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        void set_diffuse(const ColorComponent& instance) { SetObjectProperty<ColorComponent>("diffuse", &instance, 1); }
        ///<summary>Get related instance</summary>
        ColorComponent* get_diffuse() { return GetObjectProperty<ColorComponent>("diffuse", NULL); }
        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        void set_emissive(const ColorComponent& instance) { SetObjectProperty<ColorComponent>("emissive", &instance, 1); }
        ///<summary>Get related instance</summary>
        ColorComponent* get_emissive() { return GetObjectProperty<ColorComponent>("emissive", NULL); }
        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        void set_specular(const ColorComponent& instance) { SetObjectProperty<ColorComponent>("specular", &instance, 1); }
        ///<summary>Get related instance</summary>
        ColorComponent* get_specular() { return GetObjectProperty<ColorComponent>("specular", NULL); }
        ///<summary>Sets value of transparency</summary>
        void set_transparency(double value) { SetDatatypeProperty ("transparency", &value, 1); }
        ///<summary>Gets value of transparency, returns NULL is the property was not set</summary>
        double* get_transparency() { return GetDatatypeProperty<double>("transparency", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ColorComponent
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ColorComponent : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ColorComponent and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ColorComponent Create(int64_t model, const char* name=NULL) { return ColorComponent(Instance::Create(model, "ColorComponent", name), "ColorComponent");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ColorComponent(int64_t instance = NULL, const char* checkClassName = NULL)
            : Appearance(instance, (checkClassName != NULL) ? checkClassName : "ColorComponent")
        {}

       //
       // Properties with known cardinality restrictions to ColorComponent
       //

        ///<summary>Sets value of B</summary>
        void set_B(double value) { SetDatatypeProperty ("B", &value, 1); }
        ///<summary>Gets value of B, returns NULL is the property was not set</summary>
        double* get_B() { return GetDatatypeProperty<double>("B", NULL); }
        ///<summary>Sets value of G</summary>
        void set_G(double value) { SetDatatypeProperty ("G", &value, 1); }
        ///<summary>Gets value of G, returns NULL is the property was not set</summary>
        double* get_G() { return GetDatatypeProperty<double>("G", NULL); }
        ///<summary>Sets value of R</summary>
        void set_R(double value) { SetDatatypeProperty ("R", &value, 1); }
        ///<summary>Gets value of R, returns NULL is the property was not set</summary>
        double* get_R() { return GetDatatypeProperty<double>("R", NULL); }
        ///<summary>Sets value of W</summary>
        void set_W(double value) { SetDatatypeProperty ("W", &value, 1); }
        ///<summary>Gets value of W, returns NULL is the property was not set</summary>
        double* get_W() { return GetDatatypeProperty<double>("W", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Cone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cone Create(int64_t model, const char* name=NULL) { return Cone(Instance::Create(model, "Cone", name), "Cone");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Cone(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Cone")
        {}

       //
       // Properties with known cardinality restrictions to Cone
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class InfiniteSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InfiniteSurface : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InfiniteSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InfiniteSurface Create(int64_t model, const char* name=NULL) { return InfiniteSurface(Instance::Create(model, "InfiniteSurface", name), "InfiniteSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        InfiniteSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : Surface(instance, (checkClassName != NULL) ? checkClassName : "InfiniteSurface")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ConicalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ConicalSurface : public InfiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ConicalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ConicalSurface Create(int64_t model, const char* name=NULL) { return ConicalSurface(Instance::Create(model, "ConicalSurface", name), "ConicalSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ConicalSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : InfiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "ConicalSurface")
        {}

       //
       // Properties with known cardinality restrictions to ConicalSurface
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
        ///<summary>Sets value of semiVerticalAngle</summary>
        void set_semiVerticalAngle(double value) { SetDatatypeProperty ("semiVerticalAngle", &value, 1); }
        ///<summary>Gets value of semiVerticalAngle, returns NULL is the property was not set</summary>
        double* get_semiVerticalAngle() { return GetDatatypeProperty<double>("semiVerticalAngle", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Copy
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Copy : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Copy and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Copy Create(int64_t model, const char* name=NULL) { return Copy(Instance::Create(model, "Copy", name), "Copy");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Copy(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Copy")
        {}

       //
       // Properties with known cardinality restrictions to Copy
       //

        ///<summary>Sets value of instanceReference</summary>
        void set_instanceReference(long value) { SetDatatypeProperty ("instanceReference", &value, 1); }
        ///<summary>Gets value of instanceReference, returns NULL is the property was not set</summary>
        long* get_instanceReference() { return GetDatatypeProperty<long>("instanceReference", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class CosineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CosineCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CosineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CosineCurve Create(int64_t model, const char* name=NULL) { return CosineCurve(Instance::Create(model, "CosineCurve", name), "CosineCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        CosineCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : TransitionalCurve(instance, (checkClassName != NULL) ? checkClassName : "CosineCurve")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Cube
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cube : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cube and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cube Create(int64_t model, const char* name=NULL) { return Cube(Instance::Create(model, "Cube", name), "Cube");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Cube(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Cube")
        {}

       //
       // Properties with known cardinality restrictions to Cube
       //

        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class CubicParabola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CubicParabola : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CubicParabola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CubicParabola Create(int64_t model, const char* name=NULL) { return CubicParabola(Instance::Create(model, "CubicParabola", name), "CubicParabola");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        CubicParabola(int64_t instance = NULL, const char* checkClassName = NULL)
            : TransitionalCurve(instance, (checkClassName != NULL) ? checkClassName : "CubicParabola")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Cuboid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cuboid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cuboid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cuboid Create(int64_t model, const char* name=NULL) { return Cuboid(Instance::Create(model, "Cuboid", name), "Cuboid");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Cuboid(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Cuboid")
        {}

       //
       // Properties with known cardinality restrictions to Cuboid
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of width</summary>
        void set_width(double value) { SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets value of width, returns NULL is the property was not set</summary>
        double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class CurvesFromSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CurvesFromSurface : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CurvesFromSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CurvesFromSurface Create(int64_t model, const char* name=NULL) { return CurvesFromSurface(Instance::Create(model, "CurvesFromSurface", name), "CurvesFromSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        CurvesFromSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "CurvesFromSurface")
        {}

       //
       // Properties with known cardinality restrictions to CurvesFromSurface
       //

        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 1..-1</summary>
        void set_faces(const Face* instances, int64_t count) { SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        void set_faces(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1</summary>
        Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Cylinder
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cylinder : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cylinder and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cylinder Create(int64_t model, const char* name=NULL) { return Cylinder(Instance::Create(model, "Cylinder", name), "Cylinder");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Cylinder(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Cylinder")
        {}

       //
       // Properties with known cardinality restrictions to Cylinder
       //

        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class CylindricalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CylindricalSurface : public InfiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CylindricalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CylindricalSurface Create(int64_t model, const char* name=NULL) { return CylindricalSurface(Instance::Create(model, "CylindricalSurface", name), "CylindricalSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        CylindricalSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : InfiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "CylindricalSurface")
        {}

       //
       // Properties with known cardinality restrictions to CylindricalSurface
       //

        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class DegenerateToroidalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class DegenerateToroidalSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class DegenerateToroidalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DegenerateToroidalSurface Create(int64_t model, const char* name=NULL) { return DegenerateToroidalSurface(Instance::Create(model, "DegenerateToroidalSurface", name), "DegenerateToroidalSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        DegenerateToroidalSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : FiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "DegenerateToroidalSurface")
        {}

       //
       // Properties with known cardinality restrictions to DegenerateToroidalSurface
       //

        ///<summary>Sets value of majorRadius</summary>
        void set_majorRadius(double value) { SetDatatypeProperty ("majorRadius", &value, 1); }
        ///<summary>Gets value of majorRadius, returns NULL is the property was not set</summary>
        double* get_majorRadius() { return GetDatatypeProperty<double>("majorRadius", NULL); }
        ///<summary>Sets value of minorRadius</summary>
        void set_minorRadius(double value) { SetDatatypeProperty ("minorRadius", &value, 1); }
        ///<summary>Gets value of minorRadius, returns NULL is the property was not set</summary>
        double* get_minorRadius() { return GetDatatypeProperty<double>("minorRadius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Deviation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Deviation : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Deviation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Deviation Create(int64_t model, const char* name=NULL) { return Deviation(Instance::Create(model, "Deviation", name), "Deviation");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Deviation(int64_t instance = NULL, const char* checkClassName = NULL)
            : Instance(instance, (checkClassName != NULL) ? checkClassName : "Deviation")
        {}

       //
       // Properties with known cardinality restrictions to Deviation
       //

        ///<summary>Sets value of absoluteEpsilon</summary>
        void set_absoluteEpsilon(double value) { SetDatatypeProperty ("absoluteEpsilon", &value, 1); }
        ///<summary>Gets value of absoluteEpsilon, returns NULL is the property was not set</summary>
        double* get_absoluteEpsilon() { return GetDatatypeProperty<double>("absoluteEpsilon", NULL); }
        ///<summary>Sets value of relativeEpsilon</summary>
        void set_relativeEpsilon(double value) { SetDatatypeProperty ("relativeEpsilon", &value, 1); }
        ///<summary>Gets value of relativeEpsilon, returns NULL is the property was not set</summary>
        double* get_relativeEpsilon() { return GetDatatypeProperty<double>("relativeEpsilon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class DirectLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class DirectLight : public Light
    {
    public:
        /// <summary>
        /// Create new instace of OWL class DirectLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DirectLight Create(int64_t model, const char* name=NULL) { return DirectLight(Instance::Create(model, "DirectLight", name), "DirectLight");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        DirectLight(int64_t instance = NULL, const char* checkClassName = NULL)
            : Light(instance, (checkClassName != NULL) ? checkClassName : "DirectLight")
        {}

       //
       // Properties with known cardinality restrictions to DirectLight
       //

        ///<summary>Sets value of attenuation</summary>
        void set_attenuation(double value) { SetDatatypeProperty ("attenuation", &value, 1); }
        ///<summary>Gets value of attenuation, returns NULL is the property was not set</summary>
        double* get_attenuation() { return GetDatatypeProperty<double>("attenuation", NULL); }
        ///<summary>Sets value of range</summary>
        void set_range(double value) { SetDatatypeProperty ("range", &value, 1); }
        ///<summary>Gets value of range, returns NULL is the property was not set</summary>
        double* get_range() { return GetDatatypeProperty<double>("range", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class DirectionalLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class DirectionalLight : public DirectLight
    {
    public:
        /// <summary>
        /// Create new instace of OWL class DirectionalLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DirectionalLight Create(int64_t model, const char* name=NULL) { return DirectionalLight(Instance::Create(model, "DirectionalLight", name), "DirectionalLight");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        DirectionalLight(int64_t instance = NULL, const char* checkClassName = NULL)
            : DirectLight(instance, (checkClassName != NULL) ? checkClassName : "DirectionalLight")
        {}

       //
       // Properties with known cardinality restrictions to DirectionalLight
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        void set_direction(const Vector& instance) { SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Dummy
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Dummy : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Dummy and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Dummy Create(int64_t model, const char* name=NULL) { return Dummy(Instance::Create(model, "Dummy", name), "Dummy");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Dummy(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Dummy")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Ellipse
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Ellipse : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Ellipse and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Ellipse Create(int64_t model, const char* name=NULL) { return Ellipse(Instance::Create(model, "Ellipse", name), "Ellipse");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Ellipse(int64_t instance = NULL, const char* checkClassName = NULL)
            : ConicalCurve(instance, (checkClassName != NULL) ? checkClassName : "Ellipse")
        {}

       //
       // Properties with known cardinality restrictions to Ellipse
       //

        ///<summary>Sets value of b</summary>
        void set_b(double value) { SetDatatypeProperty ("b", &value, 1); }
        ///<summary>Gets value of b, returns NULL is the property was not set</summary>
        double* get_b() { return GetDatatypeProperty<double>("b", NULL); }
        ///<summary>Sets value of radiusI</summary>
        void set_radiusI(double value) { SetDatatypeProperty ("radiusI", &value, 1); }
        ///<summary>Gets value of radiusI, returns NULL is the property was not set</summary>
        double* get_radiusI() { return GetDatatypeProperty<double>("radiusI", NULL); }
        ///<summary>Sets value of radiusII</summary>
        void set_radiusII(double value) { SetDatatypeProperty ("radiusII", &value, 1); }
        ///<summary>Gets value of radiusII, returns NULL is the property was not set</summary>
        double* get_radiusII() { return GetDatatypeProperty<double>("radiusII", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class EllipticCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class EllipticCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class EllipticCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static EllipticCone Create(int64_t model, const char* name=NULL) { return EllipticCone(Instance::Create(model, "EllipticCone", name), "EllipticCone");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        EllipticCone(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "EllipticCone")
        {}

       //
       // Properties with known cardinality restrictions to EllipticCone
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radiusI</summary>
        void set_radiusI(double value) { SetDatatypeProperty ("radiusI", &value, 1); }
        ///<summary>Gets value of radiusI, returns NULL is the property was not set</summary>
        double* get_radiusI() { return GetDatatypeProperty<double>("radiusI", NULL); }
        ///<summary>Sets value of radiusII</summary>
        void set_radiusII(double value) { SetDatatypeProperty ("radiusII", &value, 1); }
        ///<summary>Gets value of radiusII, returns NULL is the property was not set</summary>
        double* get_radiusII() { return GetDatatypeProperty<double>("radiusII", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ExtrudedPolygon
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrudedPolygon : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrudedPolygon and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrudedPolygon Create(int64_t model, const char* name=NULL) { return ExtrudedPolygon(Instance::Create(model, "ExtrudedPolygon", name), "ExtrudedPolygon");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ExtrudedPolygon(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "ExtrudedPolygon")
        {}

       //
       // Properties with known cardinality restrictions to ExtrudedPolygon
       //

        ///<summary>Sets value of extrusionLength</summary>
        void set_extrusionLength(double value) { SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets value of extrusionLength, returns NULL is the property was not set</summary>
        double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets values of openingPoints. OWL cardinality 0..-1</summary>
        void set_openingPoints(double* values, int64_t count) { SetDatatypeProperty ("openingPoints", values, count); }
        ///<summary>Gets values of openingPoints. OWL cardinality 0..-1</summary>
        double* get_openingPoints(int64_t* pCount) { return GetDatatypeProperty<double>("openingPoints", pCount); }
        ///<summary>Sets values of openingSizes. OWL cardinality 0..-1</summary>
        void set_openingSizes(long* values, int64_t count) { SetDatatypeProperty ("openingSizes", values, count); }
        ///<summary>Gets values of openingSizes. OWL cardinality 0..-1</summary>
        long* get_openingSizes(int64_t* pCount) { return GetDatatypeProperty<long>("openingSizes", pCount); }
        ///<summary>Sets values of points. OWL cardinality 6..-1</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 6..-1</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of polygonDirection. OWL cardinality 0..3</summary>
        void set_polygonDirection(double* values, int64_t count) { SetDatatypeProperty ("polygonDirection", values, count); }
        ///<summary>Gets values of polygonDirection. OWL cardinality 0..3</summary>
        double* get_polygonDirection(int64_t* pCount) { return GetDatatypeProperty<double>("polygonDirection", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ExtrudedPolygonTapered
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrudedPolygonTapered : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrudedPolygonTapered and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrudedPolygonTapered Create(int64_t model, const char* name=NULL) { return ExtrudedPolygonTapered(Instance::Create(model, "ExtrudedPolygonTapered", name), "ExtrudedPolygonTapered");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ExtrudedPolygonTapered(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "ExtrudedPolygonTapered")
        {}

       //
       // Properties with known cardinality restrictions to ExtrudedPolygonTapered
       //

        ///<summary>Sets value of extrusionLength</summary>
        void set_extrusionLength(double value) { SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets value of extrusionLength, returns NULL is the property was not set</summary>
        double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets values of openingPoints. OWL cardinality 0..-1</summary>
        void set_openingPoints(double* values, int64_t count) { SetDatatypeProperty ("openingPoints", values, count); }
        ///<summary>Gets values of openingPoints. OWL cardinality 0..-1</summary>
        double* get_openingPoints(int64_t* pCount) { return GetDatatypeProperty<double>("openingPoints", pCount); }
        ///<summary>Sets values of openingPointsEnd. OWL cardinality 0..-1</summary>
        void set_openingPointsEnd(double* values, int64_t count) { SetDatatypeProperty ("openingPointsEnd", values, count); }
        ///<summary>Gets values of openingPointsEnd. OWL cardinality 0..-1</summary>
        double* get_openingPointsEnd(int64_t* pCount) { return GetDatatypeProperty<double>("openingPointsEnd", pCount); }
        ///<summary>Sets values of openingSizes. OWL cardinality 0..-1</summary>
        void set_openingSizes(long* values, int64_t count) { SetDatatypeProperty ("openingSizes", values, count); }
        ///<summary>Gets values of openingSizes. OWL cardinality 0..-1</summary>
        long* get_openingSizes(int64_t* pCount) { return GetDatatypeProperty<long>("openingSizes", pCount); }
        ///<summary>Sets values of openingSizesEnd. OWL cardinality 0..-1</summary>
        void set_openingSizesEnd(long* values, int64_t count) { SetDatatypeProperty ("openingSizesEnd", values, count); }
        ///<summary>Gets values of openingSizesEnd. OWL cardinality 0..-1</summary>
        long* get_openingSizesEnd(int64_t* pCount) { return GetDatatypeProperty<long>("openingSizesEnd", pCount); }
        ///<summary>Sets values of points. OWL cardinality 6..-1</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 6..-1</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of pointsEnd. OWL cardinality 6..-1</summary>
        void set_pointsEnd(double* values, int64_t count) { SetDatatypeProperty ("pointsEnd", values, count); }
        ///<summary>Gets values of pointsEnd. OWL cardinality 6..-1</summary>
        double* get_pointsEnd(int64_t* pCount) { return GetDatatypeProperty<double>("pointsEnd", pCount); }
        ///<summary>Sets values of polygonDirection. OWL cardinality 0..3</summary>
        void set_polygonDirection(double* values, int64_t count) { SetDatatypeProperty ("polygonDirection", values, count); }
        ///<summary>Gets values of polygonDirection. OWL cardinality 0..3</summary>
        double* get_polygonDirection(int64_t* pCount) { return GetDatatypeProperty<double>("polygonDirection", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ExtrusionAreaSolid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrusionAreaSolid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrusionAreaSolid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrusionAreaSolid Create(int64_t model, const char* name=NULL) { return ExtrusionAreaSolid(Instance::Create(model, "ExtrusionAreaSolid", name), "ExtrusionAreaSolid");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ExtrusionAreaSolid(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "ExtrusionAreaSolid")
        {}

       //
       // Properties with known cardinality restrictions to ExtrusionAreaSolid
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_extrusionArea(const Curve& instance) { SetObjectProperty<Curve>("extrusionArea", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_extrusionArea() { return GetObjectProperty<Curve>("extrusionArea", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_extrusionAreaOpenings(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("extrusionAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_extrusionAreaOpenings(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("extrusionAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_extrusionAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("extrusionAreaOpenings", pCount); }
        int64_t* get_extrusionAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("extrusionAreaOpenings", pCount); }
        ///<summary>Sets values of extrusionDirection. OWL cardinality 0..3</summary>
        void set_extrusionDirection(double* values, int64_t count) { SetDatatypeProperty ("extrusionDirection", values, count); }
        ///<summary>Gets values of extrusionDirection. OWL cardinality 0..3</summary>
        double* get_extrusionDirection(int64_t* pCount) { return GetDatatypeProperty<double>("extrusionDirection", pCount); }
        ///<summary>Sets value of extrusionLength</summary>
        void set_extrusionLength(double value) { SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets value of extrusionLength, returns NULL is the property was not set</summary>
        double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ExtrusionAreaSolidSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrusionAreaSolidSet : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrusionAreaSolidSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrusionAreaSolidSet Create(int64_t model, const char* name=NULL) { return ExtrusionAreaSolidSet(Instance::Create(model, "ExtrusionAreaSolidSet", name), "ExtrusionAreaSolidSet");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ExtrusionAreaSolidSet(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "ExtrusionAreaSolidSet")
        {}

       //
       // Properties with known cardinality restrictions to ExtrusionAreaSolidSet
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        void set_extrusionAreaSet(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("extrusionAreaSet", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        void set_extrusionAreaSet(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("extrusionAreaSet", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1</summary>
        Curve* get_extrusionAreaSet(int64_t* pCount) { return GetObjectProperty<Curve>("extrusionAreaSet", pCount); }
        int64_t* get_extrusionAreaSet_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("extrusionAreaSet", pCount); }
        ///<summary>Sets values of extrusionDirection. OWL cardinality 0..3</summary>
        void set_extrusionDirection(double* values, int64_t count) { SetDatatypeProperty ("extrusionDirection", values, count); }
        ///<summary>Gets values of extrusionDirection. OWL cardinality 0..3</summary>
        double* get_extrusionDirection(int64_t* pCount) { return GetDatatypeProperty<double>("extrusionDirection", pCount); }
        ///<summary>Sets value of extrusionLength</summary>
        void set_extrusionLength(double value) { SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets value of extrusionLength, returns NULL is the property was not set</summary>
        double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Face2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Face2D : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Face2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face2D Create(int64_t model, const char* name=NULL) { return Face2D(Instance::Create(model, "Face2D", name), "Face2D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Face2D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Face(instance, (checkClassName != NULL) ? checkClassName : "Face2D")
        {}

       //
       // Properties with known cardinality restrictions to Face2D
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_innerPolygons(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("innerPolygons", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_innerPolygons(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("innerPolygons", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_innerPolygons(int64_t* pCount) { return GetObjectProperty<Curve>("innerPolygons", pCount); }
        int64_t* get_innerPolygons_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("innerPolygons", pCount); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_outerPolygon(const Curve& instance) { SetObjectProperty<Curve>("outerPolygon", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_outerPolygon() { return GetObjectProperty<Curve>("outerPolygon", NULL); }
        ///<summary>Sets value of setting</summary>
        void set_setting(long value) { SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets value of setting, returns NULL is the property was not set</summary>
        long* get_setting() { return GetDatatypeProperty<long>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Face2DSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Face2DSet : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Face2DSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face2DSet Create(int64_t model, const char* name=NULL) { return Face2DSet(Instance::Create(model, "Face2DSet", name), "Face2DSet");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Face2DSet(int64_t instance = NULL, const char* checkClassName = NULL)
            : Face(instance, (checkClassName != NULL) ? checkClassName : "Face2DSet")
        {}

       //
       // Properties with known cardinality restrictions to Face2DSet
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_polygons(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("polygons", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_polygons(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("polygons", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_polygons(int64_t* pCount) { return GetObjectProperty<Curve>("polygons", pCount); }
        int64_t* get_polygons_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("polygons", pCount); }
        ///<summary>Sets value of setting</summary>
        void set_setting(long value) { SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets value of setting, returns NULL is the property was not set</summary>
        long* get_setting() { return GetDatatypeProperty<long>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class FrustumCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class FrustumCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class FrustumCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static FrustumCone Create(int64_t model, const char* name=NULL) { return FrustumCone(Instance::Create(model, "FrustumCone", name), "FrustumCone");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        FrustumCone(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "FrustumCone")
        {}

       //
       // Properties with known cardinality restrictions to FrustumCone
       //

        ///<summary>Sets value of factor</summary>
        void set_factor(double value) { SetDatatypeProperty ("factor", &value, 1); }
        ///<summary>Gets value of factor, returns NULL is the property was not set</summary>
        double* get_factor() { return GetDatatypeProperty<double>("factor", NULL); }
        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Torus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Torus : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Torus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Torus Create(int64_t model, const char* name=NULL) { return Torus(Instance::Create(model, "Torus", name), "Torus");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Torus(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Torus")
        {}

       //
       // Properties with known cardinality restrictions to Torus
       //

        ///<summary>Sets value of majorRadius</summary>
        void set_majorRadius(double value) { SetDatatypeProperty ("majorRadius", &value, 1); }
        ///<summary>Gets value of majorRadius, returns NULL is the property was not set</summary>
        double* get_majorRadius() { return GetDatatypeProperty<double>("majorRadius", NULL); }
        ///<summary>Sets value of minorRadius</summary>
        void set_minorRadius(double value) { SetDatatypeProperty ("minorRadius", &value, 1); }
        ///<summary>Gets value of minorRadius, returns NULL is the property was not set</summary>
        double* get_minorRadius() { return GetDatatypeProperty<double>("minorRadius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class HornTorus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class HornTorus : public Torus
    {
    public:
        /// <summary>
        /// Create new instace of OWL class HornTorus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static HornTorus Create(int64_t model, const char* name=NULL) { return HornTorus(Instance::Create(model, "HornTorus", name), "HornTorus");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        HornTorus(int64_t instance = NULL, const char* checkClassName = NULL)
            : Torus(instance, (checkClassName != NULL) ? checkClassName : "HornTorus")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Hyperbola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Hyperbola : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Hyperbola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Hyperbola Create(int64_t model, const char* name=NULL) { return Hyperbola(Instance::Create(model, "Hyperbola", name), "Hyperbola");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Hyperbola(int64_t instance = NULL, const char* checkClassName = NULL)
            : ConicalCurve(instance, (checkClassName != NULL) ? checkClassName : "Hyperbola")
        {}

       //
       // Properties with known cardinality restrictions to Hyperbola
       //

        ///<summary>Sets value of b</summary>
        void set_b(double value) { SetDatatypeProperty ("b", &value, 1); }
        ///<summary>Gets value of b, returns NULL is the property was not set</summary>
        double* get_b() { return GetDatatypeProperty<double>("b", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Mathematics
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Mathematics : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Mathematics and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Mathematics Create(int64_t model, const char* name=NULL) { return Mathematics(Instance::Create(model, "Mathematics", name), "Mathematics");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Mathematics(int64_t instance = NULL, const char* checkClassName = NULL)
            : Instance(instance, (checkClassName != NULL) ? checkClassName : "Mathematics")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Matrix
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Matrix : public Mathematics
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Matrix and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Matrix Create(int64_t model, const char* name=NULL) { return Matrix(Instance::Create(model, "Matrix", name), "Matrix");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Matrix(int64_t instance = NULL, const char* checkClassName = NULL)
            : Mathematics(instance, (checkClassName != NULL) ? checkClassName : "Matrix")
        {}

       //
       // Properties with known cardinality restrictions to Matrix
       //

        ///<summary>Sets value of _11</summary>
        void set__11(double value) { SetDatatypeProperty ("_11", &value, 1); }
        ///<summary>Gets value of _11, returns NULL is the property was not set</summary>
        double* get__11() { return GetDatatypeProperty<double>("_11", NULL); }
        ///<summary>Sets value of _12</summary>
        void set__12(double value) { SetDatatypeProperty ("_12", &value, 1); }
        ///<summary>Gets value of _12, returns NULL is the property was not set</summary>
        double* get__12() { return GetDatatypeProperty<double>("_12", NULL); }
        ///<summary>Sets value of _13</summary>
        void set__13(double value) { SetDatatypeProperty ("_13", &value, 1); }
        ///<summary>Gets value of _13, returns NULL is the property was not set</summary>
        double* get__13() { return GetDatatypeProperty<double>("_13", NULL); }
        ///<summary>Sets value of _21</summary>
        void set__21(double value) { SetDatatypeProperty ("_21", &value, 1); }
        ///<summary>Gets value of _21, returns NULL is the property was not set</summary>
        double* get__21() { return GetDatatypeProperty<double>("_21", NULL); }
        ///<summary>Sets value of _22</summary>
        void set__22(double value) { SetDatatypeProperty ("_22", &value, 1); }
        ///<summary>Gets value of _22, returns NULL is the property was not set</summary>
        double* get__22() { return GetDatatypeProperty<double>("_22", NULL); }
        ///<summary>Sets value of _23</summary>
        void set__23(double value) { SetDatatypeProperty ("_23", &value, 1); }
        ///<summary>Gets value of _23, returns NULL is the property was not set</summary>
        double* get__23() { return GetDatatypeProperty<double>("_23", NULL); }
        ///<summary>Sets value of _31</summary>
        void set__31(double value) { SetDatatypeProperty ("_31", &value, 1); }
        ///<summary>Gets value of _31, returns NULL is the property was not set</summary>
        double* get__31() { return GetDatatypeProperty<double>("_31", NULL); }
        ///<summary>Sets value of _32</summary>
        void set__32(double value) { SetDatatypeProperty ("_32", &value, 1); }
        ///<summary>Gets value of _32, returns NULL is the property was not set</summary>
        double* get__32() { return GetDatatypeProperty<double>("_32", NULL); }
        ///<summary>Sets value of _33</summary>
        void set__33(double value) { SetDatatypeProperty ("_33", &value, 1); }
        ///<summary>Gets value of _33, returns NULL is the property was not set</summary>
        double* get__33() { return GetDatatypeProperty<double>("_33", NULL); }
        ///<summary>Sets value of _41</summary>
        void set__41(double value) { SetDatatypeProperty ("_41", &value, 1); }
        ///<summary>Gets value of _41, returns NULL is the property was not set</summary>
        double* get__41() { return GetDatatypeProperty<double>("_41", NULL); }
        ///<summary>Sets value of _42</summary>
        void set__42(double value) { SetDatatypeProperty ("_42", &value, 1); }
        ///<summary>Gets value of _42, returns NULL is the property was not set</summary>
        double* get__42() { return GetDatatypeProperty<double>("_42", NULL); }
        ///<summary>Sets value of _43</summary>
        void set__43(double value) { SetDatatypeProperty ("_43", &value, 1); }
        ///<summary>Gets value of _43, returns NULL is the property was not set</summary>
        double* get__43() { return GetDatatypeProperty<double>("_43", NULL); }
        ///<summary>Sets values of coordinates. OWL cardinality 0..12</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..12</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class InverseMatrix
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InverseMatrix : public Matrix
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InverseMatrix and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InverseMatrix Create(int64_t model, const char* name=NULL) { return InverseMatrix(Instance::Create(model, "InverseMatrix", name), "InverseMatrix");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        InverseMatrix(int64_t instance = NULL, const char* checkClassName = NULL)
            : Matrix(instance, (checkClassName != NULL) ? checkClassName : "InverseMatrix")
        {}

       //
       // Properties with known cardinality restrictions to InverseMatrix
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_matrix(const Matrix& instance) { SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class InvertedCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InvertedCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InvertedCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InvertedCurve Create(int64_t model, const char* name=NULL) { return InvertedCurve(Instance::Create(model, "InvertedCurve", name), "InvertedCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        InvertedCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "InvertedCurve")
        {}

       //
       // Properties with known cardinality restrictions to InvertedCurve
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_curve(const Curve& instance) { SetObjectProperty<Curve>("curve", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_curve() { return GetObjectProperty<Curve>("curve", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class InvertedSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InvertedSurface : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InvertedSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InvertedSurface Create(int64_t model, const char* name=NULL) { return InvertedSurface(Instance::Create(model, "InvertedSurface", name), "InvertedSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        InvertedSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : Surface(instance, (checkClassName != NULL) ? checkClassName : "InvertedSurface")
        {}

       //
       // Properties with known cardinality restrictions to InvertedSurface
       //

        ///<summary>Sets relationship from this instance to an instance of Surface</summary>
        void set_surface(const Surface& instance) { SetObjectProperty<Surface>("surface", &instance, 1); }
        ///<summary>Get related instance</summary>
        Surface* get_surface() { return GetObjectProperty<Surface>("surface", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Line3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Line3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Line3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3D Create(int64_t model, const char* name=NULL) { return Line3D(Instance::Create(model, "Line3D", name), "Line3D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Line3D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Line3D")
        {}

       //
       // Properties with known cardinality restrictions to Line3D
       //

        ///<summary>Sets value of asOpenGL</summary>
        void set_asOpenGL(bool value) { SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets value of asOpenGL, returns NULL is the property was not set</summary>
        bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of points. OWL cardinality 6..6</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 6..6</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Line3Dn
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Line3Dn : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Line3Dn and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3Dn Create(int64_t model, const char* name=NULL) { return Line3Dn(Instance::Create(model, "Line3Dn", name), "Line3Dn");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Line3Dn(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Line3Dn")
        {}

       //
       // Properties with known cardinality restrictions to Line3Dn
       //

        ///<summary>Sets value of asOpenGL</summary>
        void set_asOpenGL(bool value) { SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets value of asOpenGL, returns NULL is the property was not set</summary>
        bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of endDirection. OWL cardinality 0..3</summary>
        void set_endDirection(double* values, int64_t count) { SetDatatypeProperty ("endDirection", values, count); }
        ///<summary>Gets values of endDirection. OWL cardinality 0..3</summary>
        double* get_endDirection(int64_t* pCount) { return GetDatatypeProperty<double>("endDirection", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..6</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 0..6</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of startDirection. OWL cardinality 0..3</summary>
        void set_startDirection(double* values, int64_t count) { SetDatatypeProperty ("startDirection", values, count); }
        ///<summary>Gets values of startDirection. OWL cardinality 0..3</summary>
        double* get_startDirection(int64_t* pCount) { return GetDatatypeProperty<double>("startDirection", pCount); }
        ///<summary>Sets value of x0</summary>
        void set_x0(double value) { SetDatatypeProperty ("x0", &value, 1); }
        ///<summary>Gets value of x0, returns NULL is the property was not set</summary>
        double* get_x0() { return GetDatatypeProperty<double>("x0", NULL); }
        ///<summary>Sets value of x1</summary>
        void set_x1(double value) { SetDatatypeProperty ("x1", &value, 1); }
        ///<summary>Gets value of x1, returns NULL is the property was not set</summary>
        double* get_x1() { return GetDatatypeProperty<double>("x1", NULL); }
        ///<summary>Sets value of y0</summary>
        void set_y0(double value) { SetDatatypeProperty ("y0", &value, 1); }
        ///<summary>Gets value of y0, returns NULL is the property was not set</summary>
        double* get_y0() { return GetDatatypeProperty<double>("y0", NULL); }
        ///<summary>Sets value of y1</summary>
        void set_y1(double value) { SetDatatypeProperty ("y1", &value, 1); }
        ///<summary>Gets value of y1, returns NULL is the property was not set</summary>
        double* get_y1() { return GetDatatypeProperty<double>("y1", NULL); }
        ///<summary>Sets value of z0</summary>
        void set_z0(double value) { SetDatatypeProperty ("z0", &value, 1); }
        ///<summary>Gets value of z0, returns NULL is the property was not set</summary>
        double* get_z0() { return GetDatatypeProperty<double>("z0", NULL); }
        ///<summary>Sets value of z1</summary>
        void set_z1(double value) { SetDatatypeProperty ("z1", &value, 1); }
        ///<summary>Gets value of z1, returns NULL is the property was not set</summary>
        double* get_z1() { return GetDatatypeProperty<double>("z1", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Line3DSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Line3DSet : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Line3DSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3DSet Create(int64_t model, const char* name=NULL) { return Line3DSet(Instance::Create(model, "Line3DSet", name), "Line3DSet");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Line3DSet(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Line3DSet")
        {}

       //
       // Properties with known cardinality restrictions to Line3DSet
       //

        ///<summary>Sets value of asOpenGL</summary>
        void set_asOpenGL(bool value) { SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets value of asOpenGL, returns NULL is the property was not set</summary>
        bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of points. OWL cardinality 0..-1</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 0..-1</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class LineByFace
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class LineByFace : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class LineByFace and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static LineByFace Create(int64_t model, const char* name=NULL) { return LineByFace(Instance::Create(model, "LineByFace", name), "LineByFace");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        LineByFace(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "LineByFace")
        {}

       //
       // Properties with known cardinality restrictions to LineByFace
       //

        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 1..-1</summary>
        void set_faces(const Face* instances, int64_t count) { SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        void set_faces(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1</summary>
        Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Material
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Material : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Material and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Material Create(int64_t model, const char* name=NULL) { return Material(Instance::Create(model, "Material", name), "Material");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Material(int64_t instance = NULL, const char* checkClassName = NULL)
            : Appearance(instance, (checkClassName != NULL) ? checkClassName : "Material")
        {}

       //
       // Properties with known cardinality restrictions to Material
       //

        ///<summary>Sets relationship from this instance to an instance of Color</summary>
        void set_color(const Color& instance) { SetObjectProperty<Color>("color", &instance, 1); }
        ///<summary>Get related instance</summary>
        Color* get_color() { return GetObjectProperty<Color>("color", NULL); }
        ///<summary>Sets relationships from this instance to an array of Texture. OWL cardinality 0..2</summary>
        void set_textures(const Texture* instances, int64_t count) { SetObjectProperty<Texture>("textures", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..2</summary>
        void set_textures(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("textures", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..2</summary>
        Texture* get_textures(int64_t* pCount) { return GetObjectProperty<Texture>("textures", pCount); }
        int64_t* get_textures_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("textures", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class MatrixMultiplication
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class MatrixMultiplication : public Matrix
    {
    public:
        /// <summary>
        /// Create new instace of OWL class MatrixMultiplication and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static MatrixMultiplication Create(int64_t model, const char* name=NULL) { return MatrixMultiplication(Instance::Create(model, "MatrixMultiplication", name), "MatrixMultiplication");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        MatrixMultiplication(int64_t instance = NULL, const char* checkClassName = NULL)
            : Matrix(instance, (checkClassName != NULL) ? checkClassName : "MatrixMultiplication")
        {}

       //
       // Properties with known cardinality restrictions to MatrixMultiplication
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_firstMatrix(const Matrix& instance) { SetObjectProperty<Matrix>("firstMatrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_firstMatrix() { return GetObjectProperty<Matrix>("firstMatrix", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_secondMatrix(const Matrix& instance) { SetObjectProperty<Matrix>("secondMatrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_secondMatrix() { return GetObjectProperty<Matrix>("secondMatrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Mesh
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Mesh : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Mesh and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Mesh Create(int64_t model, const char* name=NULL) { return Mesh(Instance::Create(model, "Mesh", name), "Mesh");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Mesh(int64_t instance = NULL, const char* checkClassName = NULL)
            : Face(instance, (checkClassName != NULL) ? checkClassName : "Mesh")
        {}

       //
       // Properties with known cardinality restrictions to Mesh
       //

        ///<summary>Sets relationship from this instance to an instance of Point3DSet</summary>
        void set_pointSet(const Point3DSet& instance) { SetObjectProperty<Point3DSet>("pointSet", &instance, 1); }
        ///<summary>Get related instance</summary>
        Point3DSet* get_pointSet() { return GetObjectProperty<Point3DSet>("pointSet", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class NURBSCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class NURBSCurve : public BSplineCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class NURBSCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static NURBSCurve Create(int64_t model, const char* name=NULL) { return NURBSCurve(Instance::Create(model, "NURBSCurve", name), "NURBSCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        NURBSCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : BSplineCurve(instance, (checkClassName != NULL) ? checkClassName : "NURBSCurve")
        {}

       //
       // Properties with known cardinality restrictions to NURBSCurve
       //

        ///<summary>Sets values of weights. OWL cardinality 2..-1</summary>
        void set_weights(double* values, int64_t count) { SetDatatypeProperty ("weights", values, count); }
        ///<summary>Gets values of weights. OWL cardinality 2..-1</summary>
        double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class NURBSSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class NURBSSurface : public BSplineSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class NURBSSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static NURBSSurface Create(int64_t model, const char* name=NULL) { return NURBSSurface(Instance::Create(model, "NURBSSurface", name), "NURBSSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        NURBSSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : BSplineSurface(instance, (checkClassName != NULL) ? checkClassName : "NURBSSurface")
        {}

       //
       // Properties with known cardinality restrictions to NURBSSurface
       //

        ///<summary>Sets values of weights. OWL cardinality 4..-1</summary>
        void set_weights(double* values, int64_t count) { SetDatatypeProperty ("weights", values, count); }
        ///<summary>Gets values of weights. OWL cardinality 4..-1</summary>
        double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Parabola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Parabola : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Parabola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Parabola Create(int64_t model, const char* name=NULL) { return Parabola(Instance::Create(model, "Parabola", name), "Parabola");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Parabola(int64_t instance = NULL, const char* checkClassName = NULL)
            : ConicalCurve(instance, (checkClassName != NULL) ? checkClassName : "Parabola")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Plane
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Plane : public Mathematics
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Plane and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Plane Create(int64_t model, const char* name=NULL) { return Plane(Instance::Create(model, "Plane", name), "Plane");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Plane(int64_t instance = NULL, const char* checkClassName = NULL)
            : Mathematics(instance, (checkClassName != NULL) ? checkClassName : "Plane")
        {}

       //
       // Properties with known cardinality restrictions to Plane
       //

        ///<summary>Sets value of A</summary>
        void set_A(double value) { SetDatatypeProperty ("A", &value, 1); }
        ///<summary>Gets value of A, returns NULL is the property was not set</summary>
        double* get_A() { return GetDatatypeProperty<double>("A", NULL); }
        ///<summary>Sets value of B</summary>
        void set_B(double value) { SetDatatypeProperty ("B", &value, 1); }
        ///<summary>Gets value of B, returns NULL is the property was not set</summary>
        double* get_B() { return GetDatatypeProperty<double>("B", NULL); }
        ///<summary>Sets value of C</summary>
        void set_C(double value) { SetDatatypeProperty ("C", &value, 1); }
        ///<summary>Gets value of C, returns NULL is the property was not set</summary>
        double* get_C() { return GetDatatypeProperty<double>("C", NULL); }
        ///<summary>Sets value of D</summary>
        void set_D(double value) { SetDatatypeProperty ("D", &value, 1); }
        ///<summary>Gets value of D, returns NULL is the property was not set</summary>
        double* get_D() { return GetDatatypeProperty<double>("D", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class PlaneSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PlaneSurface : public InfiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PlaneSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PlaneSurface Create(int64_t model, const char* name=NULL) { return PlaneSurface(Instance::Create(model, "PlaneSurface", name), "PlaneSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        PlaneSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : InfiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "PlaneSurface")
        {}

       //
       // Properties with known cardinality restrictions to PlaneSurface
       //

        ///<summary>Sets relationship from this instance to an instance of Plane</summary>
        void set_plane(const Plane& instance) { SetObjectProperty<Plane>("plane", &instance, 1); }
        ///<summary>Get related instance</summary>
        Plane* get_plane() { return GetObjectProperty<Plane>("plane", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Point
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point Create(int64_t model, const char* name=NULL) { return Point(Instance::Create(model, "Point", name), "Point");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Point(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Point")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Point3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point3D : public Point
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3D Create(int64_t model, const char* name=NULL) { return Point3D(Instance::Create(model, "Point3D", name), "Point3D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Point3D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Point(instance, (checkClassName != NULL) ? checkClassName : "Point3D")
        {}

       //
       // Properties with known cardinality restrictions to Point3D
       //

        ///<summary>Sets value of asOpenGL</summary>
        void set_asOpenGL(bool value) { SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets value of asOpenGL, returns NULL is the property was not set</summary>
        bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of coordinates. OWL cardinality 0..3</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..3</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..3</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 0..3</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets value of x</summary>
        void set_x(double value) { SetDatatypeProperty ("x", &value, 1); }
        ///<summary>Gets value of x, returns NULL is the property was not set</summary>
        double* get_x() { return GetDatatypeProperty<double>("x", NULL); }
        ///<summary>Sets value of y</summary>
        void set_y(double value) { SetDatatypeProperty ("y", &value, 1); }
        ///<summary>Gets value of y, returns NULL is the property was not set</summary>
        double* get_y() { return GetDatatypeProperty<double>("y", NULL); }
        ///<summary>Sets value of z</summary>
        void set_z(double value) { SetDatatypeProperty ("z", &value, 1); }
        ///<summary>Gets value of z, returns NULL is the property was not set</summary>
        double* get_z() { return GetDatatypeProperty<double>("z", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Point3DSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point3DSet : public Point
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point3DSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3DSet Create(int64_t model, const char* name=NULL) { return Point3DSet(Instance::Create(model, "Point3DSet", name), "Point3DSet");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Point3DSet(int64_t instance = NULL, const char* checkClassName = NULL)
            : Point(instance, (checkClassName != NULL) ? checkClassName : "Point3DSet")
        {}

       //
       // Properties with known cardinality restrictions to Point3DSet
       //

        ///<summary>Sets value of asOpenGL</summary>
        void set_asOpenGL(bool value) { SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets value of asOpenGL, returns NULL is the property was not set</summary>
        bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of coordinates. OWL cardinality 0..-1</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..-1</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..-1</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 0..-1</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Point3DSetByGeometricItem
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point3DSetByGeometricItem : public Point3DSet
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point3DSetByGeometricItem and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3DSetByGeometricItem Create(int64_t model, const char* name=NULL) { return Point3DSetByGeometricItem(Instance::Create(model, "Point3DSetByGeometricItem", name), "Point3DSetByGeometricItem");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Point3DSetByGeometricItem(int64_t instance = NULL, const char* checkClassName = NULL)
            : Point3DSet(instance, (checkClassName != NULL) ? checkClassName : "Point3DSetByGeometricItem")
        {}

       //
       // Properties with known cardinality restrictions to Point3DSetByGeometricItem
       //

        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_object(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class PointLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PointLight : public DirectLight
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PointLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PointLight Create(int64_t model, const char* name=NULL) { return PointLight(Instance::Create(model, "PointLight", name), "PointLight");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        PointLight(int64_t instance = NULL, const char* checkClassName = NULL)
            : DirectLight(instance, (checkClassName != NULL) ? checkClassName : "PointLight")
        {}

       //
       // Properties with known cardinality restrictions to PointLight
       //

        ///<summary>Sets relationship from this instance to an instance of Point3D</summary>
        void set_position(const Point3D& instance) { SetObjectProperty<Point3D>("position", &instance, 1); }
        ///<summary>Get related instance</summary>
        Point3D* get_position() { return GetObjectProperty<Point3D>("position", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class PointLoop
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PointLoop : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PointLoop and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PointLoop Create(int64_t model, const char* name=NULL) { return PointLoop(Instance::Create(model, "PointLoop", name), "PointLoop");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        PointLoop(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "PointLoop")
        {}

       //
       // Properties with known cardinality restrictions to PointLoop
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..3</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..3</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of x</summary>
        void set_x(double value) { SetDatatypeProperty ("x", &value, 1); }
        ///<summary>Gets value of x, returns NULL is the property was not set</summary>
        double* get_x() { return GetDatatypeProperty<double>("x", NULL); }
        ///<summary>Sets value of y</summary>
        void set_y(double value) { SetDatatypeProperty ("y", &value, 1); }
        ///<summary>Gets value of y, returns NULL is the property was not set</summary>
        double* get_y() { return GetDatatypeProperty<double>("y", NULL); }
        ///<summary>Sets value of z</summary>
        void set_z(double value) { SetDatatypeProperty ("z", &value, 1); }
        ///<summary>Gets value of z, returns NULL is the property was not set</summary>
        double* get_z() { return GetDatatypeProperty<double>("z", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Polygon2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Polygon2D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Polygon2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon2D Create(int64_t model, const char* name=NULL) { return Polygon2D(Instance::Create(model, "Polygon2D", name), "Polygon2D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Polygon2D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Polygon2D")
        {}

       //
       // Properties with known cardinality restrictions to Polygon2D
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        void set_lineParts(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("lineParts", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        void set_lineParts(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("lineParts", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1</summary>
        Curve* get_lineParts(int64_t* pCount) { return GetObjectProperty<Curve>("lineParts", pCount); }
        int64_t* get_lineParts_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("lineParts", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Polygon3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Polygon3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Polygon3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon3D Create(int64_t model, const char* name=NULL) { return Polygon3D(Instance::Create(model, "Polygon3D", name), "Polygon3D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Polygon3D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Polygon3D")
        {}

       //
       // Properties with known cardinality restrictions to Polygon3D
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        void set_lineParts(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("lineParts", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        void set_lineParts(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("lineParts", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1</summary>
        Curve* get_lineParts(int64_t* pCount) { return GetObjectProperty<Curve>("lineParts", pCount); }
        int64_t* get_lineParts_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("lineParts", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class PolyLine3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PolyLine3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PolyLine3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PolyLine3D Create(int64_t model, const char* name=NULL) { return PolyLine3D(Instance::Create(model, "PolyLine3D", name), "PolyLine3D");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        PolyLine3D(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "PolyLine3D")
        {}

       //
       // Properties with known cardinality restrictions to PolyLine3D
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..-1</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..-1</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 0..-1</summary>
        void set_pointReferences(const Point3D* instances, int64_t count) { SetObjectProperty<Point3D>("pointReferences", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_pointReferences(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("pointReferences", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Point3D* get_pointReferences(int64_t* pCount) { return GetObjectProperty<Point3D>("pointReferences", pCount); }
        int64_t* get_pointReferences_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("pointReferences", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..-1</summary>
        void set_points(double* values, int64_t count) { SetDatatypeProperty ("points", values, count); }
        ///<summary>Gets values of points. OWL cardinality 0..-1</summary>
        double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of tangent. OWL cardinality 0..-1</summary>
        void set_tangent(double* values, int64_t count) { SetDatatypeProperty ("tangent", values, count); }
        ///<summary>Gets values of tangent. OWL cardinality 0..-1</summary>
        double* get_tangent(int64_t* pCount) { return GetDatatypeProperty<double>("tangent", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Prism
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Prism : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Prism and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Prism Create(int64_t model, const char* name=NULL) { return Prism(Instance::Create(model, "Prism", name), "Prism");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Prism(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Prism")
        {}

       //
       // Properties with known cardinality restrictions to Prism
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Profile
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Profile : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Profile and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Profile Create(int64_t model, const char* name=NULL) { return Profile(Instance::Create(model, "Profile", name), "Profile");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Profile(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Profile")
        {}

       //
       // Properties with known cardinality restrictions to Profile
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_innerPolygons(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("innerPolygons", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_innerPolygons(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("innerPolygons", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_innerPolygons(int64_t* pCount) { return GetObjectProperty<Curve>("innerPolygons", pCount); }
        int64_t* get_innerPolygons_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("innerPolygons", pCount); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_outerPolygon(const Curve& instance) { SetObjectProperty<Curve>("outerPolygon", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_outerPolygon() { return GetObjectProperty<Curve>("outerPolygon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Projection
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Projection : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Projection and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Projection Create(int64_t model, const char* name=NULL) { return Projection(Instance::Create(model, "Projection", name), "Projection");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Projection(int64_t instance = NULL, const char* checkClassName = NULL)
            : Environment(instance, (checkClassName != NULL) ? checkClassName : "Projection")
        {}

       //
       // Properties with known cardinality restrictions to Projection
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_matrix(const Matrix& instance) { SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Pyramid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Pyramid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Pyramid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Pyramid Create(int64_t model, const char* name=NULL) { return Pyramid(Instance::Create(model, "Pyramid", name), "Pyramid");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Pyramid(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Pyramid")
        {}

       //
       // Properties with known cardinality restrictions to Pyramid
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        void set_length(double value) { SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets value of length, returns NULL is the property was not set</summary>
        double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of width</summary>
        void set_width(double value) { SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets value of width, returns NULL is the property was not set</summary>
        double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class RationalBezierCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RationalBezierCurve : public BezierCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RationalBezierCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RationalBezierCurve Create(int64_t model, const char* name=NULL) { return RationalBezierCurve(Instance::Create(model, "RationalBezierCurve", name), "RationalBezierCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        RationalBezierCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : BezierCurve(instance, (checkClassName != NULL) ? checkClassName : "RationalBezierCurve")
        {}

       //
       // Properties with known cardinality restrictions to RationalBezierCurve
       //

        ///<summary>Sets values of weights. OWL cardinality 2..-1</summary>
        void set_weights(double* values, int64_t count) { SetDatatypeProperty ("weights", values, count); }
        ///<summary>Gets values of weights. OWL cardinality 2..-1</summary>
        double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class RationalBezierSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RationalBezierSurface : public BezierSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RationalBezierSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RationalBezierSurface Create(int64_t model, const char* name=NULL) { return RationalBezierSurface(Instance::Create(model, "RationalBezierSurface", name), "RationalBezierSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        RationalBezierSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : BezierSurface(instance, (checkClassName != NULL) ? checkClassName : "RationalBezierSurface")
        {}

       //
       // Properties with known cardinality restrictions to RationalBezierSurface
       //

        ///<summary>Sets values of weights. OWL cardinality 4..-1</summary>
        void set_weights(double* values, int64_t count) { SetDatatypeProperty ("weights", values, count); }
        ///<summary>Gets values of weights. OWL cardinality 4..-1</summary>
        double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class RectangleCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RectangleCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RectangleCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RectangleCurve Create(int64_t model, const char* name=NULL) { return RectangleCurve(Instance::Create(model, "RectangleCurve", name), "RectangleCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        RectangleCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "RectangleCurve")
        {}

       //
       // Properties with known cardinality restrictions to RectangleCurve
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetX</summary>
        void set_offsetX(double value) { SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets value of offsetX, returns NULL is the property was not set</summary>
        double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        void set_offsetY(double value) { SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets value of offsetY, returns NULL is the property was not set</summary>
        double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of offsetZ</summary>
        void set_offsetZ(double value) { SetDatatypeProperty ("offsetZ", &value, 1); }
        ///<summary>Gets value of offsetZ, returns NULL is the property was not set</summary>
        double* get_offsetZ() { return GetDatatypeProperty<double>("offsetZ", NULL); }
        ///<summary>Sets value of width</summary>
        void set_width(double value) { SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets value of width, returns NULL is the property was not set</summary>
        double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Repetition
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Repetition : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Repetition and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Repetition Create(int64_t model, const char* name=NULL) { return Repetition(Instance::Create(model, "Repetition", name), "Repetition");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Repetition(int64_t instance = NULL, const char* checkClassName = NULL)
            : GeometricItem(instance, (checkClassName != NULL) ? checkClassName : "Repetition")
        {}

       //
       // Properties with known cardinality restrictions to Repetition
       //

        ///<summary>Sets value of count</summary>
        void set_count(long value) { SetDatatypeProperty ("count", &value, 1); }
        ///<summary>Gets value of count, returns NULL is the property was not set</summary>
        long* get_count() { return GetDatatypeProperty<long>("count", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_matrix(const Matrix& instance) { SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_object(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class RingTorus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RingTorus : public Torus
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RingTorus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RingTorus Create(int64_t model, const char* name=NULL) { return RingTorus(Instance::Create(model, "RingTorus", name), "RingTorus");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        RingTorus(int64_t instance = NULL, const char* checkClassName = NULL)
            : Torus(instance, (checkClassName != NULL) ? checkClassName : "RingTorus")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Shadow
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Shadow : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Shadow and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Shadow Create(int64_t model, const char* name=NULL) { return Shadow(Instance::Create(model, "Shadow", name), "Shadow");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Shadow(int64_t instance = NULL, const char* checkClassName = NULL)
            : Face(instance, (checkClassName != NULL) ? checkClassName : "Shadow")
        {}

       //
       // Properties with known cardinality restrictions to Shadow
       //

        ///<summary>Sets relationship from this instance to an instance of Vector3</summary>
        void set_lightDirection(const Vector3& instance) { SetObjectProperty<Vector3>("lightDirection", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector3* get_lightDirection() { return GetObjectProperty<Vector3>("lightDirection", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Point3D</summary>
        void set_lightPoint(const Point3D& instance) { SetObjectProperty<Point3D>("lightPoint", &instance, 1); }
        ///<summary>Get related instance</summary>
        Point3D* get_lightPoint() { return GetObjectProperty<Point3D>("lightPoint", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_object(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Plane</summary>
        void set_plane(const Plane& instance) { SetObjectProperty<Plane>("plane", &instance, 1); }
        ///<summary>Get related instance</summary>
        Plane* get_plane() { return GetObjectProperty<Plane>("plane", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Vector3</summary>
        void set_planeRefDirection(const Vector3& instance) { SetObjectProperty<Vector3>("planeRefDirection", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector3* get_planeRefDirection() { return GetObjectProperty<Vector3>("planeRefDirection", NULL); }
        ///<summary>Sets value of type</summary>
        void set_type(long value) { SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets value of type, returns NULL is the property was not set</summary>
        long* get_type() { return GetDatatypeProperty<long>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SineCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SineCurve Create(int64_t model, const char* name=NULL) { return SineCurve(Instance::Create(model, "SineCurve", name), "SineCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SineCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : TransitionalCurve(instance, (checkClassName != NULL) ? checkClassName : "SineCurve")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SkewedCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SkewedCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SkewedCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SkewedCone Create(int64_t model, const char* name=NULL) { return SkewedCone(Instance::Create(model, "SkewedCone", name), "SkewedCone");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SkewedCone(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SkewedCone")
        {}

       //
       // Properties with known cardinality restrictions to SkewedCone
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetX</summary>
        void set_offsetX(double value) { SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets value of offsetX, returns NULL is the property was not set</summary>
        double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        void set_offsetY(double value) { SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets value of offsetY, returns NULL is the property was not set</summary>
        double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SkewedFrustumCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SkewedFrustumCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SkewedFrustumCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SkewedFrustumCone Create(int64_t model, const char* name=NULL) { return SkewedFrustumCone(Instance::Create(model, "SkewedFrustumCone", name), "SkewedFrustumCone");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SkewedFrustumCone(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SkewedFrustumCone")
        {}

       //
       // Properties with known cardinality restrictions to SkewedFrustumCone
       //

        ///<summary>Sets value of factor</summary>
        void set_factor(double value) { SetDatatypeProperty ("factor", &value, 1); }
        ///<summary>Gets value of factor, returns NULL is the property was not set</summary>
        double* get_factor() { return GetDatatypeProperty<double>("factor", NULL); }
        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetX</summary>
        void set_offsetX(double value) { SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets value of offsetX, returns NULL is the property was not set</summary>
        double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        void set_offsetY(double value) { SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets value of offsetY, returns NULL is the property was not set</summary>
        double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SolidBySurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SolidBySurface : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SolidBySurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SolidBySurface Create(int64_t model, const char* name=NULL) { return SolidBySurface(Instance::Create(model, "SolidBySurface", name), "SolidBySurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SolidBySurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SolidBySurface")
        {}

       //
       // Properties with known cardinality restrictions to SolidBySurface
       //

        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 1..-1</summary>
        void set_faces(const Face* instances, int64_t count) { SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        void set_faces(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1</summary>
        Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SolidLine
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SolidLine : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SolidLine and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SolidLine Create(int64_t model, const char* name=NULL) { return SolidLine(Instance::Create(model, "SolidLine", name), "SolidLine");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SolidLine(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SolidLine")
        {}

       //
       // Properties with known cardinality restrictions to SolidLine
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..6</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..6</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
        ///<summary>Sets value of thickness</summary>
        void set_thickness(double value) { SetDatatypeProperty ("thickness", &value, 1); }
        ///<summary>Gets value of thickness, returns NULL is the property was not set</summary>
        double* get_thickness() { return GetDatatypeProperty<double>("thickness", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Sphere
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Sphere : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Sphere and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Sphere Create(int64_t model, const char* name=NULL) { return Sphere(Instance::Create(model, "Sphere", name), "Sphere");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Sphere(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "Sphere")
        {}

       //
       // Properties with known cardinality restrictions to Sphere
       //

        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SphericalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SphericalSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SphericalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SphericalSurface Create(int64_t model, const char* name=NULL) { return SphericalSurface(Instance::Create(model, "SphericalSurface", name), "SphericalSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SphericalSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : FiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "SphericalSurface")
        {}

       //
       // Properties with known cardinality restrictions to SphericalSurface
       //

        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SpindleTorus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SpindleTorus : public Torus
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SpindleTorus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SpindleTorus Create(int64_t model, const char* name=NULL) { return SpindleTorus(Instance::Create(model, "SpindleTorus", name), "SpindleTorus");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SpindleTorus(int64_t instance = NULL, const char* checkClassName = NULL)
            : Torus(instance, (checkClassName != NULL) ? checkClassName : "SpindleTorus")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Spiral
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Spiral : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Spiral and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Spiral Create(int64_t model, const char* name=NULL) { return Spiral(Instance::Create(model, "Spiral", name), "Spiral");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Spiral(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Spiral")
        {}

       //
       // Properties with known cardinality restrictions to Spiral
       //

        ///<summary>Sets value of height</summary>
        void set_height(double value) { SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets value of height, returns NULL is the property was not set</summary>
        double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetZ</summary>
        void set_offsetZ(double value) { SetDatatypeProperty ("offsetZ", &value, 1); }
        ///<summary>Gets value of offsetZ, returns NULL is the property was not set</summary>
        double* get_offsetZ() { return GetDatatypeProperty<double>("offsetZ", NULL); }
        ///<summary>Sets value of radius</summary>
        void set_radius(double value) { SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets value of radius, returns NULL is the property was not set</summary>
        double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
        ///<summary>Sets value of size</summary>
        void set_size(double value) { SetDatatypeProperty ("size", &value, 1); }
        ///<summary>Gets value of size, returns NULL is the property was not set</summary>
        double* get_size() { return GetDatatypeProperty<double>("size", NULL); }
        ///<summary>Sets value of start</summary>
        void set_start(double value) { SetDatatypeProperty ("start", &value, 1); }
        ///<summary>Gets value of start, returns NULL is the property was not set</summary>
        double* get_start() { return GetDatatypeProperty<double>("start", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SpotLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SpotLight : public DirectLight
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SpotLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SpotLight Create(int64_t model, const char* name=NULL) { return SpotLight(Instance::Create(model, "SpotLight", name), "SpotLight");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SpotLight(int64_t instance = NULL, const char* checkClassName = NULL)
            : DirectLight(instance, (checkClassName != NULL) ? checkClassName : "SpotLight")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SurfaceBySweptCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SurfaceBySweptCurve : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SurfaceBySweptCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceBySweptCurve Create(int64_t model, const char* name=NULL) { return SurfaceBySweptCurve(Instance::Create(model, "SurfaceBySweptCurve", name), "SurfaceBySweptCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SurfaceBySweptCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : FiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "SurfaceBySweptCurve")
        {}

       //
       // Properties with known cardinality restrictions to SurfaceBySweptCurve
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_path(const Curve& instance) { SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_sweptArea(const Curve& instance) { SetObjectProperty<Curve>("sweptArea", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_sweptArea() { return GetObjectProperty<Curve>("sweptArea", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_sweptAreaOpenings(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("sweptAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_sweptAreaOpenings(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("sweptAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_sweptAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaOpenings", pCount); }
        int64_t* get_sweptAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaOpenings", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SurfaceOfLinearExtrusion
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SurfaceOfLinearExtrusion : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SurfaceOfLinearExtrusion and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceOfLinearExtrusion Create(int64_t model, const char* name=NULL) { return SurfaceOfLinearExtrusion(Instance::Create(model, "SurfaceOfLinearExtrusion", name), "SurfaceOfLinearExtrusion");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SurfaceOfLinearExtrusion(int64_t instance = NULL, const char* checkClassName = NULL)
            : FiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "SurfaceOfLinearExtrusion")
        {}

       //
       // Properties with known cardinality restrictions to SurfaceOfLinearExtrusion
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        void set_extrusion(const Vector& instance) { SetObjectProperty<Vector>("extrusion", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector* get_extrusion() { return GetObjectProperty<Vector>("extrusion", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_polygon(const Curve& instance) { SetObjectProperty<Curve>("polygon", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_polygon() { return GetObjectProperty<Curve>("polygon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SurfaceOfRevolution
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SurfaceOfRevolution : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SurfaceOfRevolution and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceOfRevolution Create(int64_t model, const char* name=NULL) { return SurfaceOfRevolution(Instance::Create(model, "SurfaceOfRevolution", name), "SurfaceOfRevolution");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SurfaceOfRevolution(int64_t instance = NULL, const char* checkClassName = NULL)
            : Surface(instance, (checkClassName != NULL) ? checkClassName : "SurfaceOfRevolution")
        {}

       //
       // Properties with known cardinality restrictions to SurfaceOfRevolution
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_path(const Curve& instance) { SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SweptAreaSolid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptAreaSolid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptAreaSolid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolid Create(int64_t model, const char* name=NULL) { return SweptAreaSolid(Instance::Create(model, "SweptAreaSolid", name), "SweptAreaSolid");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SweptAreaSolid(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SweptAreaSolid")
        {}

       //
       // Properties with known cardinality restrictions to SweptAreaSolid
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        void set_direction(const Vector& instance) { SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_path(const Curve& instance) { SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_sweptArea(const Curve& instance) { SetObjectProperty<Curve>("sweptArea", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_sweptArea() { return GetObjectProperty<Curve>("sweptArea", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_sweptAreaOpenings(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("sweptAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_sweptAreaOpenings(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("sweptAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_sweptAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaOpenings", pCount); }
        int64_t* get_sweptAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaOpenings", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SweptAreaSolidSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptAreaSolidSet : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptAreaSolidSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolidSet Create(int64_t model, const char* name=NULL) { return SweptAreaSolidSet(Instance::Create(model, "SweptAreaSolidSet", name), "SweptAreaSolidSet");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SweptAreaSolidSet(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SweptAreaSolidSet")
        {}

       //
       // Properties with known cardinality restrictions to SweptAreaSolidSet
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        void set_direction(const Vector& instance) { SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_path(const Curve& instance) { SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        void set_sweptAreaSet(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("sweptAreaSet", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        void set_sweptAreaSet(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("sweptAreaSet", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1</summary>
        Curve* get_sweptAreaSet(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaSet", pCount); }
        int64_t* get_sweptAreaSet_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaSet", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SweptAreaSolidTapered
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptAreaSolidTapered : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptAreaSolidTapered and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolidTapered Create(int64_t model, const char* name=NULL) { return SweptAreaSolidTapered(Instance::Create(model, "SweptAreaSolidTapered", name), "SweptAreaSolidTapered");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SweptAreaSolidTapered(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SweptAreaSolidTapered")
        {}

       //
       // Properties with known cardinality restrictions to SweptAreaSolidTapered
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        void set_direction(const Vector& instance) { SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance</summary>
        Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_path(const Curve& instance) { SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_sweptArea(const Curve& instance) { SetObjectProperty<Curve>("sweptArea", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_sweptArea() { return GetObjectProperty<Curve>("sweptArea", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_sweptAreaEnd(const Curve& instance) { SetObjectProperty<Curve>("sweptAreaEnd", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_sweptAreaEnd() { return GetObjectProperty<Curve>("sweptAreaEnd", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_sweptAreaEndOpenings(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("sweptAreaEndOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_sweptAreaEndOpenings(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("sweptAreaEndOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_sweptAreaEndOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaEndOpenings", pCount); }
        int64_t* get_sweptAreaEndOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaEndOpenings", pCount); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        void set_sweptAreaOpenings(const Curve* instances, int64_t count) { SetObjectProperty<Curve>("sweptAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        void set_sweptAreaOpenings(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("sweptAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1</summary>
        Curve* get_sweptAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaOpenings", pCount); }
        int64_t* get_sweptAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaOpenings", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class SweptBlend
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptBlend : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptBlend and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptBlend Create(int64_t model, const char* name=NULL) { return SweptBlend(Instance::Create(model, "SweptBlend", name), "SweptBlend");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        SweptBlend(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "SweptBlend")
        {}

       //
       // Properties with known cardinality restrictions to SweptBlend
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_bottomPolygon(const Curve& instance) { SetObjectProperty<Curve>("bottomPolygon", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_bottomPolygon() { return GetObjectProperty<Curve>("bottomPolygon", NULL); }
        ///<summary>Sets values of connectionMap. OWL cardinality 0..-1</summary>
        void set_connectionMap(long* values, int64_t count) { SetDatatypeProperty ("connectionMap", values, count); }
        ///<summary>Gets values of connectionMap. OWL cardinality 0..-1</summary>
        long* get_connectionMap(int64_t* pCount) { return GetDatatypeProperty<long>("connectionMap", pCount); }
        ///<summary>Sets values of forcedStaticDirection. OWL cardinality 0..3</summary>
        void set_forcedStaticDirection(double* values, int64_t count) { SetDatatypeProperty ("forcedStaticDirection", values, count); }
        ///<summary>Gets values of forcedStaticDirection. OWL cardinality 0..3</summary>
        double* get_forcedStaticDirection(int64_t* pCount) { return GetDatatypeProperty<double>("forcedStaticDirection", pCount); }
        ///<summary>Sets value of forceSolid</summary>
        void set_forceSolid(bool value) { SetDatatypeProperty ("forceSolid", &value, 1); }
        ///<summary>Gets value of forceSolid, returns NULL is the property was not set</summary>
        bool* get_forceSolid() { return GetDatatypeProperty<bool>("forceSolid", NULL); }
        ///<summary>Sets value of fraction</summary>
        void set_fraction(double value) { SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets value of fraction, returns NULL is the property was not set</summary>
        double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets value of hasBottom</summary>
        void set_hasBottom(bool value) { SetDatatypeProperty ("hasBottom", &value, 1); }
        ///<summary>Gets value of hasBottom, returns NULL is the property was not set</summary>
        bool* get_hasBottom() { return GetDatatypeProperty<bool>("hasBottom", NULL); }
        ///<summary>Sets value of hasTop</summary>
        void set_hasTop(bool value) { SetDatatypeProperty ("hasTop", &value, 1); }
        ///<summary>Gets value of hasTop, returns NULL is the property was not set</summary>
        bool* get_hasTop() { return GetDatatypeProperty<bool>("hasTop", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_path(const Curve& instance) { SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        void set_topPolygon(const Curve& instance) { SetObjectProperty<Curve>("topPolygon", &instance, 1); }
        ///<summary>Get related instance</summary>
        Curve* get_topPolygon() { return GetObjectProperty<Curve>("topPolygon", NULL); }
        ///<summary>Sets value of usesAbsolutePlacement</summary>
        void set_usesAbsolutePlacement(bool value) { SetDatatypeProperty ("usesAbsolutePlacement", &value, 1); }
        ///<summary>Gets value of usesAbsolutePlacement, returns NULL is the property was not set</summary>
        bool* get_usesAbsolutePlacement() { return GetDatatypeProperty<bool>("usesAbsolutePlacement", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Texture
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Texture : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Texture and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Texture Create(int64_t model, const char* name=NULL) { return Texture(Instance::Create(model, "Texture", name), "Texture");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Texture(int64_t instance = NULL, const char* checkClassName = NULL)
            : Appearance(instance, (checkClassName != NULL) ? checkClassName : "Texture")
        {}

       //
       // Properties with known cardinality restrictions to Texture
       //

        ///<summary>Sets value of name</summary>
        void set_name(const char* value) { SetDatatypeProperty ("name", &value, 1); }
        ///<summary>Gets value of name, returns NULL is the property was not set</summary>
        const char** get_name() { return GetDatatypeProperty<const char*>("name", NULL); }
        ///<summary>Sets value of offsetX</summary>
        void set_offsetX(double value) { SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets value of offsetX, returns NULL is the property was not set</summary>
        double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        void set_offsetY(double value) { SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets value of offsetY, returns NULL is the property was not set</summary>
        double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets values of origin. OWL cardinality 0..3</summary>
        void set_origin(double* values, int64_t count) { SetDatatypeProperty ("origin", values, count); }
        ///<summary>Gets values of origin. OWL cardinality 0..3</summary>
        double* get_origin(int64_t* pCount) { return GetDatatypeProperty<double>("origin", pCount); }
        ///<summary>Sets value of rotation</summary>
        void set_rotation(double value) { SetDatatypeProperty ("rotation", &value, 1); }
        ///<summary>Gets value of rotation, returns NULL is the property was not set</summary>
        double* get_rotation() { return GetDatatypeProperty<double>("rotation", NULL); }
        ///<summary>Sets value of scalingX</summary>
        void set_scalingX(double value) { SetDatatypeProperty ("scalingX", &value, 1); }
        ///<summary>Gets value of scalingX, returns NULL is the property was not set</summary>
        double* get_scalingX() { return GetDatatypeProperty<double>("scalingX", NULL); }
        ///<summary>Sets value of scalingY</summary>
        void set_scalingY(double value) { SetDatatypeProperty ("scalingY", &value, 1); }
        ///<summary>Gets value of scalingY, returns NULL is the property was not set</summary>
        double* get_scalingY() { return GetDatatypeProperty<double>("scalingY", NULL); }
        ///<summary>Sets value of type</summary>
        void set_type(long value) { SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets value of type, returns NULL is the property was not set</summary>
        long* get_type() { return GetDatatypeProperty<long>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class ToroidalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ToroidalSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ToroidalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ToroidalSurface Create(int64_t model, const char* name=NULL) { return ToroidalSurface(Instance::Create(model, "ToroidalSurface", name), "ToroidalSurface");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        ToroidalSurface(int64_t instance = NULL, const char* checkClassName = NULL)
            : FiniteSurface(instance, (checkClassName != NULL) ? checkClassName : "ToroidalSurface")
        {}

       //
       // Properties with known cardinality restrictions to ToroidalSurface
       //

        ///<summary>Sets value of majorRadius</summary>
        void set_majorRadius(double value) { SetDatatypeProperty ("majorRadius", &value, 1); }
        ///<summary>Gets value of majorRadius, returns NULL is the property was not set</summary>
        double* get_majorRadius() { return GetDatatypeProperty<double>("majorRadius", NULL); }
        ///<summary>Sets value of minorRadius</summary>
        void set_minorRadius(double value) { SetDatatypeProperty ("minorRadius", &value, 1); }
        ///<summary>Gets value of minorRadius, returns NULL is the property was not set</summary>
        double* get_minorRadius() { return GetDatatypeProperty<double>("minorRadius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        void set_segmentationParts(long value) { SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets value of segmentationParts, returns NULL is the property was not set</summary>
        long* get_segmentationParts() { return GetDatatypeProperty<long>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Transformation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Transformation : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Transformation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Transformation Create(int64_t model, const char* name=NULL) { return Transformation(Instance::Create(model, "Transformation", name), "Transformation");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Transformation(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "Transformation")
        {}

       //
       // Properties with known cardinality restrictions to Transformation
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_matrix(const Matrix& instance) { SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_object(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class TriangleCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TriangleCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TriangleCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleCurve Create(int64_t model, const char* name=NULL) { return TriangleCurve(Instance::Create(model, "TriangleCurve", name), "TriangleCurve");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        TriangleCurve(int64_t instance = NULL, const char* checkClassName = NULL)
            : Curve(instance, (checkClassName != NULL) ? checkClassName : "TriangleCurve")
        {}

       //
       // Properties with known cardinality restrictions to TriangleCurve
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..9</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..9</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of offsetX</summary>
        void set_offsetX(double value) { SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets value of offsetX, returns NULL is the property was not set</summary>
        double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        void set_offsetY(double value) { SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets value of offsetY, returns NULL is the property was not set</summary>
        double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of offsetZ</summary>
        void set_offsetZ(double value) { SetDatatypeProperty ("offsetZ", &value, 1); }
        ///<summary>Gets value of offsetZ, returns NULL is the property was not set</summary>
        double* get_offsetZ() { return GetDatatypeProperty<double>("offsetZ", NULL); }
        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 0..3</summary>
        void set_pointReferences(const Point3D* instances, int64_t count) { SetObjectProperty<Point3D>("pointReferences", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..3</summary>
        void set_pointReferences(const int64_t* instances, int64_t count) { SetObjectProperty<int64_t>("pointReferences", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..3</summary>
        Point3D* get_pointReferences(int64_t* pCount) { return GetObjectProperty<Point3D>("pointReferences", pCount); }
        int64_t* get_pointReferences_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("pointReferences", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class TriangleReduction
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TriangleReduction : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TriangleReduction and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleReduction Create(int64_t model, const char* name=NULL) { return TriangleReduction(Instance::Create(model, "TriangleReduction", name), "TriangleReduction");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        TriangleReduction(int64_t instance = NULL, const char* checkClassName = NULL)
            : Solid(instance, (checkClassName != NULL) ? checkClassName : "TriangleReduction")
        {}

       //
       // Properties with known cardinality restrictions to TriangleReduction
       //

        ///<summary>Sets value of innerFraction</summary>
        void set_innerFraction(double value) { SetDatatypeProperty ("innerFraction", &value, 1); }
        ///<summary>Gets value of innerFraction, returns NULL is the property was not set</summary>
        double* get_innerFraction() { return GetDatatypeProperty<double>("innerFraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        void set_object(const GeometricItem& instance) { SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance</summary>
        GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
        ///<summary>Sets value of outerFraction</summary>
        void set_outerFraction(double value) { SetDatatypeProperty ("outerFraction", &value, 1); }
        ///<summary>Gets value of outerFraction, returns NULL is the property was not set</summary>
        double* get_outerFraction() { return GetDatatypeProperty<double>("outerFraction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class TriangleSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TriangleSet : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TriangleSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleSet Create(int64_t model, const char* name=NULL) { return TriangleSet(Instance::Create(model, "TriangleSet", name), "TriangleSet");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        TriangleSet(int64_t instance = NULL, const char* checkClassName = NULL)
            : Surface(instance, (checkClassName != NULL) ? checkClassName : "TriangleSet")
        {}

       //
       // Properties with known cardinality restrictions to TriangleSet
       //

        ///<summary>Sets values of indices. OWL cardinality 0..-1</summary>
        void set_indices(long* values, int64_t count) { SetDatatypeProperty ("indices", values, count); }
        ///<summary>Gets values of indices. OWL cardinality 0..-1</summary>
        long* get_indices(int64_t* pCount) { return GetDatatypeProperty<long>("indices", pCount); }
        ///<summary>Sets values of vertices. OWL cardinality 3..-1</summary>
        void set_vertices(double* values, int64_t count) { SetDatatypeProperty ("vertices", values, count); }
        ///<summary>Gets values of vertices. OWL cardinality 3..-1</summary>
        double* get_vertices(int64_t* pCount) { return GetDatatypeProperty<double>("vertices", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Vector
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Vector : public Mathematics
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Vector and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Vector Create(int64_t model, const char* name=NULL) { return Vector(Instance::Create(model, "Vector", name), "Vector");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Vector(int64_t instance = NULL, const char* checkClassName = NULL)
            : Mathematics(instance, (checkClassName != NULL) ? checkClassName : "Vector")
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class Vector3
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Vector3 : public Vector
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Vector3 and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Vector3 Create(int64_t model, const char* name=NULL) { return Vector3(Instance::Create(model, "Vector3", name), "Vector3");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Vector3(int64_t instance = NULL, const char* checkClassName = NULL)
            : Vector(instance, (checkClassName != NULL) ? checkClassName : "Vector3")
        {}

       //
       // Properties with known cardinality restrictions to Vector3
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..3</summary>
        void set_coordinates(double* values, int64_t count) { SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Gets values of coordinates. OWL cardinality 0..3</summary>
        double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of x</summary>
        void set_x(double value) { SetDatatypeProperty ("x", &value, 1); }
        ///<summary>Gets value of x, returns NULL is the property was not set</summary>
        double* get_x() { return GetDatatypeProperty<double>("x", NULL); }
        ///<summary>Sets value of y</summary>
        void set_y(double value) { SetDatatypeProperty ("y", &value, 1); }
        ///<summary>Gets value of y, returns NULL is the property was not set</summary>
        double* get_y() { return GetDatatypeProperty<double>("y", NULL); }
        ///<summary>Sets value of z</summary>
        void set_z(double value) { SetDatatypeProperty ("z", &value, 1); }
        ///<summary>Gets value of z, returns NULL is the property was not set</summary>
        double* get_z() { return GetDatatypeProperty<double>("z", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class View
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class View : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class View and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static View Create(int64_t model, const char* name=NULL) { return View(Instance::Create(model, "View", name), "View");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        View(int64_t instance = NULL, const char* checkClassName = NULL)
            : Environment(instance, (checkClassName != NULL) ? checkClassName : "View")
        {}

       //
       // Properties with known cardinality restrictions to View
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_matrix(const Matrix& instance) { SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instnace of OWL class World
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class World : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class World and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static World Create(int64_t model, const char* name=NULL) { return World(Instance::Create(model, "World", name), "World");}
        
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        World(int64_t instance = NULL, const char* checkClassName = NULL)
            : Environment(instance, (checkClassName != NULL) ? checkClassName : "World")
        {}

       //
       // Properties with known cardinality restrictions to World
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        void set_matrix(const Matrix& instance) { SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance</summary>
        Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };
}


#endif
