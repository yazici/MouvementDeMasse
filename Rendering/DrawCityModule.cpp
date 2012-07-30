#include "DrawCityModule.h"

#include <GL/glew.h>

#include "City/City.h"
#include "SkyComponent.h"
#include "GroundComponent.h"
#include "WaterComponent.h"
#include "CitizensComponent.h"
#include "JunctionsComponent.h"
#include "StreetsComponent.h"
#include "BuildingsComponent.h"
#include "BridgesComponent.h"

using namespace cellar;


DrawCityModule::DrawCityModule() :
    _city(0x0),
    _ground(0x0),
    _description(0x0),
    _minimalistShader(),
    _skyShader(),
    _groundShader(),
    _waterShader(),
    _infrastructShader(),    
    _components(),
    _skyComponent(0x0),
    _groundComponent(0x0),
    _waterComponent(0x0),
    _citizensComponent(0x0),
    _junctionsComponent(0x0),
    _streetsComponent(0x0),
    _buildingsComponent(0x0),
    _bridgesComponent(0x0)
{
    // Minimalist
    GLInOutProgramLocation bridgeLocations;
    bridgeLocations.setInput(0, "position_att");
    bridgeLocations.setInput(1, "normal_att");
    bridgeLocations.setInput(2, "texCoord_att");
    _minimalistShader.setInAndOutLocations(bridgeLocations);
    _minimalistShader.loadShadersFromFile("resources/shaders/minimalist.vert",
                                          "resources/shaders/minimalist.frag");


    // Sky
    GLInOutProgramLocation skyLocations;
    skyLocations.setInput(0, "psotion_att");
    skyLocations.setInput(1, "texCoord_att");
    _skyShader.setInAndOutLocations(skyLocations);
    _skyShader.loadShadersFromFile("resources/shaders/sky.vert",
                                   "resources/shaders/sky.frag");


    // Ground
    GLInOutProgramLocation groundLocations;
    groundLocations.setInput(0, "position_att");
    groundLocations.setInput(1, "normal_att");
    groundLocations.setInput(2, "texCoord_att");
    _groundShader.setInAndOutLocations(groundLocations);
    _groundShader.loadShadersFromFile("resources/shaders/ground.vert",
                                      "resources/shaders/ground.frag");

    // Water
    GLInOutProgramLocation waterLocations;
    waterLocations.setInput(0, "position_att");
    _waterShader.setInAndOutLocations(waterLocations);
    _waterShader.loadShadersFromFile("resources/shaders/water.vert",
                                     "resources/shaders/water.frag");

    // Infrastructure
    GLInOutProgramLocation infrastructureLocations;
    infrastructureLocations.setInput(0, "position_att");
    infrastructureLocations.setInput(1, "normal_att");
    infrastructureLocations.setInput(2, "texCoord_att");
    _infrastructShader.setInAndOutLocations(infrastructureLocations);
    _infrastructShader.loadShadersFromFile("resources/shaders/infrastruct.vert",
                                           "resources/shaders/infrastruct.frag");
}

DrawCityModule::~DrawCityModule()
{
    for(size_t i=0; i<_components.size(); ++i)
        delete _components[i];
}

void DrawCityModule::setup(City &city)
{
    _city = &city;
    _ground = &city.ground();
    _description = &city.description();

    setupShaders();
    update();

    for(size_t i=0; i<_components.size(); ++i)
        delete _components[i];
    _components.clear();

    _components.push_back(_skyComponent       = new SkyComponent(      *_city, _skyShader));
    _components.push_back(_groundComponent    = new GroundComponent(   *_city, _groundShader));
    _components.push_back(_waterComponent     = new WaterComponent(    *_city, _waterShader));
    _components.push_back(_citizensComponent  = new CitizensComponent( *_city, _minimalistShader));
    _components.push_back(_junctionsComponent = new JunctionsComponent(*_city, _infrastructShader));
    _components.push_back(_streetsComponent   = new StreetsComponent(  *_city, _infrastructShader));
    _components.push_back(_buildingsComponent = new BuildingsComponent(*_city, _infrastructShader));
    _components.push_back(_bridgesComponent   = new BridgesComponent(  *_city, _infrastructShader));

    for(size_t i=0; i<_components.size(); ++i)
        _components[i]->setup();
}

void DrawCityModule::setupShaders()
{
    _skyShader.pushThisProgram();
    _skyShader.setVec4f("SkyColor",     _description->curSkyColor);
    _skyShader.setVec4f("SkylineColor", _description->skylineColor);
    _skyShader.setVec4f("SunColor",     _description->sunColor);
    _skyShader.setFloat("SunRadius",    _description->sunRadius);
    _skyShader.setVec3f("SunPosition",  Vec3f(0.0f, 0.0f, -1.0f));
    _skyShader.setVec2f("TexShift",     Vec2f(0.0f, 0.0f));
    _skyShader.setInt("CloudsTexUnit",  0);
    _skyShader.popProgram();

    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _description->sunLight.direction);
    _groundShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _groundShader.setVec4f("sun.diffuse",   _description->sunLight.diffuse);
    _groundShader.setVec4f("sun.specular",  _description->sunLight.specular);
    _groundShader.setFloat("WaterHeight",   _ground->waterHeight());
    _groundShader.setVec4f("GrassColor",    _description->grassColor);
    _groundShader.setVec4f("MudColor",      _description->mudColor);
    _groundShader.setVec4f("WaterColor",    _description->waterColor);
    _groundShader.popProgram();

    _waterShader.pushThisProgram();
    _waterShader.setVec4f("sun.direction", _description->sunLight.direction);
    _waterShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _waterShader.setVec4f("sun.diffuse",   _description->sunLight.diffuse);
    _waterShader.setVec4f("sun.specular",  _description->sunLight.specular);
    _waterShader.setFloat("Shininess",     _description->waterShininess);
    _waterShader.setVec4f("WaterColor",    _description->waterColor);
    _waterShader.popProgram();

    _infrastructShader.pushThisProgram();
    _infrastructShader.setVec4f("sun.direction", _description->sunLight.direction);
    _infrastructShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _infrastructShader.setVec4f("sun.diffuse",   _description->sunLight.diffuse);
    _infrastructShader.setVec4f("sun.specular",  _description->sunLight.specular);
    _infrastructShader.setFloat("Shininess",     128.0f);
    _infrastructShader.setVec3f("Translation",   Vec3f(0.0f, 0.0f, 0.0f));
    _infrastructShader.setVec2f("RepeatFrom",    Vec2f(1.0f, 1.0f));
    _infrastructShader.setInt("TexUnit",         0);
    _infrastructShader.setInt("SpecUnit",        1);
    _infrastructShader.popProgram();
}

void DrawCityModule::draw()
{
    for(size_t i=0; i<_components.size(); ++i)
        _components[i]->draw();
}

void DrawCityModule::update()
{
    // Check if it was initialized
    if(_city == 0x0)
        return;

    // Sky color
    const double skyCoefCorrection = 0.1;
    Vec4f  nLightDir    = _description->sunLight.direction.normalized();
    double sunIntensity = maxVal(nLightDir * Vec4f(0, 0, -1, 0) + skyCoefCorrection, 0.0);
    double skyColorCoef = pow(sunIntensity, 0.75) / (skyCoefCorrection + 1.0);
    Vec4f  skyCol = _description->nightSkyColor * (1 - skyColorCoef) +
                    _description->daySkyColor   * skyColorCoef;
    _description->curSkyColor = skyCol;

    skyCol += _description->skylineColor;
    glClearColor(skyCol[0], skyCol[1], skyCol[2], skyCol[3]);


    // Sun ambient light
    const float AMBIENT_EFF_FACT = 0.50;
    const float BASE_INTENSITY = 0.03;
    const Vec4f BASE_LIGHT = Vec4f(BASE_INTENSITY, BASE_INTENSITY, BASE_INTENSITY, 0.0f);
    _description->sunLight.ambient = BASE_LIGHT + _description->curSkyColor * AMBIENT_EFF_FACT;


    // Sun position
    Vec4f sunDir = _city->sun().direction().normalized();
    _description->sunLight.direction = sunDir;
    _description->viewedSunDirection = _description->viewMat * sunDir;

    updateShaders();

    // Update Components
    for(size_t i=0; i<_components.size(); ++i)
        _components[i]->update();
}

void DrawCityModule::updateShaders()
{
    static Vec2f shift(0.0f, 0.0f);
    shift[1] += 0.0008;

    _skyShader.pushThisProgram();
    _skyShader.setVec4f("SkyColor", _description->curSkyColor);
    _skyShader.setVec4f("SunPosition", -_description->viewedSunDirection);
    _skyShader.setVec2f("TexShift", shift);
    _skyShader.popProgram();

    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _description->viewedSunDirection);
    _groundShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _groundShader.popProgram();

    _waterShader.pushThisProgram();
    _waterShader.setVec4f("sun.direction", _description->viewedSunDirection);
    _waterShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _waterShader.popProgram();

    _infrastructShader.pushThisProgram();
    _infrastructShader.setVec4f("sun.direction", _description->viewedSunDirection);
    _infrastructShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _infrastructShader.popProgram();
}

void DrawCityModule::updateProjectionMatrix(const Matrix4x4<float>& proj)
{
    _description->projMat = proj;

    updateShadersProjectionMatrix();
}

void DrawCityModule::updateShadersProjectionMatrix()
{    
    _minimalistShader.pushThisProgram();
    _minimalistShader.setMatrix4x4("ProjectionMatrix", _description->projMat);
    _minimalistShader.popProgram();

    _skyShader.pushThisProgram();
    _skyShader.setMatrix4x4("ProjectionMatrix", _description->projMat);
    _skyShader.popProgram();

    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ProjectionMatrix", _description->projMat);
    _groundShader.popProgram();

    _waterShader.pushThisProgram();
    _waterShader.setMatrix4x4("ProjectionMatrix", _description->projMat);
    _waterShader.popProgram();

    _infrastructShader.pushThisProgram();
    _infrastructShader.setMatrix4x4("ProjectionMatrix", _description->projMat);
    _infrastructShader.popProgram();
}

void DrawCityModule::updateModelViewMatrix(const Matrix4x4<float>& view)
{
    _description->viewMat = view;
    _description->normalMat = view.subMat3();

    updateShadersModelViewMatrix();
}

void DrawCityModule::updateShadersModelViewMatrix()
{
    _minimalistShader.pushThisProgram();
    _minimalistShader.setMatrix4x4("ViewMatrix", _description->viewMat);
    _minimalistShader.popProgram();

    _skyShader.pushThisProgram();
    _skyShader.setMatrix3x3("ViewMatrix",  _description->normalMat);
    _skyShader.popProgram();

    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ViewMatrix", _description->viewMat);
    _groundShader.setMatrix3x3("NormalMatrix",    _description->normalMat);
    _groundShader.popProgram();

    _waterShader.pushThisProgram();
    _waterShader.setMatrix4x4("ViewMatrix", _description->viewMat);
    _waterShader.setMatrix3x3("NormalMatrix",    _description->normalMat);
    _waterShader.popProgram();

    _infrastructShader.pushThisProgram();
    _infrastructShader.setMatrix4x4("ViewMatrix", _description->viewMat);
    _infrastructShader.setMatrix3x3("NormalMatrix",    _description->normalMat);
    _infrastructShader.popProgram();
}
