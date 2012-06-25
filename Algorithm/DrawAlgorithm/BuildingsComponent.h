#ifndef BUILDINGSCOMPONENT_H
#define BUILDINGSCOMPONENT_H

#include <vector>
#include <MathsAndPhysics/Vector.h>
struct DrawCityCommonData;


class BuildingsComponent
{
public:
    BuildingsComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    float landHeight(int i, int j);
    void setupPositions();
    void setupBuidlindSides();
    void setupRoofTop();

    DrawCityCommonData& _common;
    unsigned int _buildingWallsVao;
    int          _buildingNbElems;

    unsigned int _roofVao;
    int          _roofNbElems;
    unsigned int _roofTex;
    unsigned int _roofSpec;
    std::vector< cellar::Vec3f > _roofPos;

    unsigned int _apartmentTex;
    unsigned int _apartmentSpec;
    std::vector< cellar::Vec3f > _apartmentsPos;
    std::vector< float > _apartmentTexScaleCoeff;

    unsigned int _commerceTex;
    unsigned int _commerceSpec;
    std::vector< cellar::Vec3f > _commercePos;
    std::vector< float > _commerceTexScaleCoeff;
};

#endif // BUILDINGSCOMPONENT_H
