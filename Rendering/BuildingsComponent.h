#ifndef BUILDINGSCOMPONENT_H
#define BUILDINGSCOMPONENT_H

#include <vector>

#include "AbstractComponent.h"


class BuildingsComponent : public AbstractComponent
{
public:
    BuildingsComponent(City& city, cellar::GlProgram& shader);
    ~BuildingsComponent();

    void setup();
    void draw();

protected:
    void setupPositions();
    void setupBuidlindSides();
    void setupRoofTop();

    static const int _BUILDING_NB_BUFFS = 3;
    unsigned int _buildingBuffs[_BUILDING_NB_BUFFS];
    unsigned int _buildingWallsVao;
    const int    _buildingNbElems;
    std::vector< std::vector<unsigned int> > _buildingIndices;

    static const int _ROOF_NB_BUFFS = 3;
    unsigned int _roofBuffs[_ROOF_NB_BUFFS];
    unsigned int _roofVao;
    int          _roofNbElems;
    unsigned int _roofTex;
    unsigned int _roofSpec;
    std::vector< glm::vec3 > _roofPos;

    unsigned int _apartmentTex;
    unsigned int _apartmentSpec;
    std::vector< glm::vec3 > _apartmentPos;
    std::vector< int >           _apartmentNbStories;

    unsigned int _commerceTex;
    unsigned int _commerceSpec;
    std::vector< glm::vec3 > _commercePos;
    std::vector< int >           _commerceNbStories;
};

#endif // BUILDINGSCOMPONENT_H
