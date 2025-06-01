
#include "pch.h"
#include "PointCloud.h"
#include "PointCloudMesh.h"


/// <summary>
/// 
/// </summary>
bool PointCloudMesh::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void* /*pvAlgorithm*/)
{
    auto instCloud = PointCloud::GetPointCloudInstance(inst);
    return ::GetBoundingBox(instCloud, (double*)transformationMatrix, (double*)startVector, (double*)endVector);
}


/// <summary>
/// 
/// </summary>
static void SetupMeshVerticies(pcl::PolygonMesh& mesh, OwlInstance inst)
{
    SHELL* shell = rdfgeom_GetInstanceRepresentation(inst);
    assert(shell); if (!shell) return;

    // Convert the mesh's cloud to PointCloud<PointXYZ>
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromPCLPointCloud2(mesh.cloud, cloud);

    rdfgeom_AllocatePoints(inst, shell, cloud.size(), false, false);
    auto nonTransformedVertices = rdfgeom_GetPoints(shell);

    size_t i = 0;
    for (auto& pt : cloud) {
        nonTransformedVertices[i].x = pt.x;
        nonTransformedVertices[i].y = pt.y;
        nonTransformedVertices[i].z = pt.z;
        i++;
    }
}



/// <summary>
/// 
/// </summary>
static STRUCT_VERTEX** AddLoopEdge(int_t indPoint, STRUCT_VERTEX** ppNextEdge, OwlInstance inst, bool lastPoint = false)
{
    assert(ppNextEdge && !*ppNextEdge);
    if (!ppNextEdge)
        return NULL;

    rdfgeom_vertex_Create(inst, ppNextEdge, indPoint, lastPoint);

    return rdfgeom_vertex_GetNext(*ppNextEdge);
}

/// <summary>
/// 
/// </summary>
static void GetMeshFaces(pcl::PolygonMesh& mesh, STRUCT_FACE** ppFace, OwlInstance inst)
{
    for (auto& polygon : mesh.polygons) {

        STRUCT_VERTEX** loop = NULL;
        pcl::index_t first = -1;
        for (auto& vertex : polygon.vertices) {

            if (!loop) {

                rdfgeom_face_Create(inst, ppFace);

                loop = rdfgeom_face_GetBoundary(*ppFace);

                ppFace = rdfgeom_face_GetNext(*ppFace);

                first = vertex;
            }

            loop = AddLoopEdge(vertex, loop, inst);
        }

        if (loop && first >= 0) {
            AddLoopEdge(first, loop, inst, true);
        }

        //face->normal.x = 0.;
        //face->normal.y = 0.;
        //face->normal.z = -1.;
    }
}

/// <summary>
/// 
/// </summary>
void PointCloudMesh::CreateShell(OwlInstance inst, void *pvAlgorithm)
{
    if (!pvAlgorithm) {
        assert(false);
        return;
    }
    auto algorithm = (IAlgorithm*)pvAlgorithm;

    OwlInstance instCloud = PointCloud::GetPointCloudInstance(inst);
    auto cloud = PointCloud::GetPointsWithNormals(instCloud);
    if (!cloud || !cloud->size()) {
        return;
    }

    auto triangles = algorithm->ReconstructMesh(inst, cloud);
    if (!triangles) {
        return;
    }

    //
    SetupMeshVerticies(*triangles, inst);

    //
    SHELL* shell = rdfgeom_GetInstanceRepresentation(inst);
    assert(shell); if (!shell) return;

    CONCEPTUAL_FACE** ppFace = rdfgeom_GetConceptualFaces(shell);
    rdfgeom_cface_Create(inst, ppFace);

    auto conceptualFace = *ppFace;

    GetMeshFaces(*triangles, rdfgeom_cface_GetFaces(conceptualFace), inst);

    //VECTOR3					referenceVector;
    //referenceVector.x = 1.;
    //referenceVector.y = 0.;
    //referenceVector.z = 0.;
    //myConceptualFace->conceptualFaceReferenceVector = memory->CreateReferenceVector(NULL, &referenceVector, &myConceptualFace->faces->normal, false);
}
