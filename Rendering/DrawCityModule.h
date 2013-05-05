#ifndef DRAWCITYMODULE_H
#define DRAWCITYMODULE_H

#include <vector>
#include <GL/GlProgram.h>

class City;
class Ground;
class Description;

class AbstractComponent;
class SkyComponent;
class GroundComponent;
class WaterComponent;
class CitizensComponent;
class JunctionsComponent;
class StreetsComponent;
class BuildingsComponent;
class BridgesComponent;


class DrawCityModule
{
public:
    DrawCityModule();
    ~DrawCityModule();

    void setup(City& city);
    void draw();
    void update();

    void updateProjectionMatrix(const cellar::Mat4f& proj);
    void updateModelViewMatrix(const cellar::Mat4f& view);

protected:
    void setupShaders();
    void updateShaders();
    void updateShadersProjectionMatrix();
    void updateShadersModelViewMatrix();

    City*   _city;
    Ground* _ground;
    Description* _description;

    // Shaders
    media::GlProgram _minimalistShader;
    media::GlProgram _skyShader;
    media::GlProgram _groundShader;
    media::GlProgram _waterShader;
    media::GlProgram _infrastructShader;

    std::vector<AbstractComponent*> _components;
    SkyComponent*       _skyComponent;
    GroundComponent*    _groundComponent;
    WaterComponent*     _waterComponent;
    CitizensComponent*  _citizensComponent;
    JunctionsComponent* _junctionsComponent;
    StreetsComponent*   _streetsComponent;
    BuildingsComponent* _buildingsComponent;
    BridgesComponent*   _bridgesComponent;
};



#endif // DRAWCITYMODULE_H
