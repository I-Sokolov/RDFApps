// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS

#include <pcl/io/ply_io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/surface/poisson.h>
#include <pcl/surface/marching_cubes_hoppe.h>
//#include <pcl/surface/ball_pivoting.h>
//#include <pcl/surface/rbf.h> 
#include <pcl/surface/mls.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>


// add headers that you want to pre-compile here
#include "framework.h"

#include <stdio.h>

#include <string>
#include <algorithm>

#include "engine.h"
#include "rdfgeom.h"

#ifdef _DEBUG
#define LOG_ERROR printf
#else
#define LOG_ERROR printf
#endif

#define REQUIRED(condition, message) if (!(condition)){ LOG_ERROR(message); return false;}

#endif //PCH_H
