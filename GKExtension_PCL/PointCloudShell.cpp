
#include "pch.h"
#include "PointCloud.h"
#include "PointCloudShell.h"


/// <summary>
/// 
/// </summary>
bool PointCloudShell::GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix)
{
    auto instCloud = PointCloud::GetPointCloudInstance(inst);

    double* coords = NULL;
    if (auto Ncoords = PointCloud::GetPointsCoords(instCloud, &coords)) {

        startVector->x = startVector->y = startVector->z = DBL_MAX;
        endVector->x = endVector->y = endVector->z = -DBL_MAX;

        for (int_t i = 0; i < Ncoords/3; i++) {
            startVector->x = std::min(startVector->x, coords[3 * i + 0]);
            startVector->y = std::min(startVector->y, coords[3 * i + 1]);
            startVector->z = std::min(startVector->z, coords[3 * i + 2]);
            endVector->x = std::max(endVector->x, coords[3 * i + 0]);
            endVector->y = std::max(endVector->y, coords[3 * i + 1]);
            endVector->z = std::max(endVector->z, coords[3 * i + 2]);
        }

        MatrixIdentity(transformationMatrix);

        return true;
    }
    else {
        return false;
    }
}


/// <summary>
/// 
/// </summary>
static void GetMeshVerticies(pcl::PolygonMesh& mesh, SHELL* shell, IMemory* memory)
{
    // Convert the mesh's cloud to PointCloud<PointXYZ>
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromPCLPointCloud2(mesh.cloud, cloud);

    shell->noVertices = cloud.size();
    shell->nonTransformedVertices = (VECTOR3*)memory->Allocate(shell->noVertices * sizeof(VECTOR3));

    size_t i = 0;
    for (auto& pt : cloud) {
        shell->nonTransformedVertices[i].x = pt.x;
        shell->nonTransformedVertices[i].y = pt.y;
        shell->nonTransformedVertices[i].z = pt.z;
        i++;
    }
}



/// <summary>
/// 
/// </summary>
static STRUCT_EDGE** AddLoopEdge(int_t vertex, STRUCT_EDGE** ppNextEdge, IMemory* memory, bool lastPoint = false)
{
    assert(ppNextEdge && !*ppNextEdge);
    if (!ppNextEdge)
        return NULL;

    STRUCT_EDGE* edge = (STRUCT_EDGE*)memory->Allocate(sizeof(STRUCT_EDGE));
    if (lastPoint) {
        edge->localPointIndex = -(vertex + 1);
    }
    else {
        edge->localPointIndex = vertex;
    }
    edge->next = nullptr;

    *ppNextEdge = edge;

    return &(edge->next);
}

/// <summary>
/// 
/// </summary>
static void GetMeshFaces(pcl::PolygonMesh& mesh, STRUCT_FACE** ppFace, IMemory* memory)
{
    for (auto& polygon : mesh.polygons) {

        STRUCT_EDGE** loop = NULL;
        pcl::index_t first = -1;
        for (auto& vertex : polygon.vertices) {

            if (!loop) {
                auto face = (STRUCT_FACE*)memory->Allocate(sizeof(STRUCT_FACE));
                loop = &(face->edge);

                *ppFace = face;
                ppFace = &(face->next);

                first = vertex;
            }

            loop = AddLoopEdge(vertex, loop, memory);
        }

        if (loop && first >= 0) {
            AddLoopEdge(first, loop, memory, true);
        }

        //face->normal.x = 0.;
        //face->normal.y = 0.;
        //face->normal.z = -1.;
    }
}

/// <summary>
/// 
/// </summary>
void PointCloudShell::CreateShell(OwlInstance inst, SHELL* shell, IMemory* memory)
{
    OwlInstance instCloud = PointCloud::GetPointCloudInstance(inst);
    auto cloud = PointCloud::GetPointCloud(instCloud);
    if (!cloud) {
        return;
    }

    auto triangles = ReconstructMesh(inst, cloud);
    if (!triangles) {
        return;
    }

    //
    GetMeshVerticies(*triangles, shell, memory);

    //
    CONCEPTUAL_FACE* conceptualFace = memory->new__CONCEPTUAL_FACE();
    shell->conceptualFaces = conceptualFace;

    GetMeshFaces(*triangles, &conceptualFace->faces, memory);

    TEXTURE_GEOMETRY_PART* textureGeometryPart = memory->new__TEXTURE_GEOMETRY_PART(enum_texture_part::part_I);
    conceptualFace->textureGeometryPart = textureGeometryPart;

    //VECTOR3					referenceVector;
    //referenceVector.x = 1.;
    //referenceVector.y = 0.;
    //referenceVector.z = 0.;
    //myConceptualFace->conceptualFaceReferenceVector = memory->CreateReferenceVector(NULL, &referenceVector, &myConceptualFace->faces->normal, false);
}
