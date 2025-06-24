#pragma once

class FacetSurface
{
public:
    static bool CreateClass(OwlModel model);

public:
    static bool GetBoundingBox(OwlInstance inst, VECTOR3* startVector, VECTOR3* endVector, MATRIX* transformationMatrix, void*);
    static void CreateShell(OwlInstance inst, void*);
};

