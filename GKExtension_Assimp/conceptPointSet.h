#pragma once

class PointSet
{
public:
    static bool CreateClass(OwlModel model);

public:
    static bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*);
    static void CreateShell(OwlInstance inst, void*);

private:
    static void CopyCoordinates(double* dstValues , int_t dstDim, double* srcValues, int_t srcDim);
    template<typename TPoint> static void CopyPoints(TPoint* dstPoints, int_t numDstPt, int_t dstDim, double* srcCoords, int_t numSrcCoords, int_t srcDim);
};

