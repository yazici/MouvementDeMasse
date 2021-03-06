#include "DrawCityModule.h"

#include <GL3/gl3w.h>

#include "City/City.h"
#include "SkyComponent.h"
#include "GroundComponent.h"
#include "WaterComponent.h"
#include "CitizensComponent.h"
#include "JunctionsComponent.h"
#include "StreetsComponent.h"
#include "BuildingsComponent.h"
#include "BridgesComponent.h"

using namespace std;
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
    GlInputsOutputs bridgeLocations;
    bridgeLocations.setInput(0, "position_att");
    bridgeLocations.setInput(1, "normal_att");
    bridgeLocations.setInput(2, "texCoord_att");
    _minimalistShader.setInAndOutLocations(bridgeLocations);
    _minimalistShader.addShader(GL_VERTEX_SHADER, "resources/shaders/minimalist.vert");
    _minimalistShader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/minimalist.frag");
    _minimalistShader.link();


    // Sky
    GlInputsOutputs skyLocations;
    skyLocations.setInput(0, "position_att");
    skyLocations.setInput(1, "texCoord_att");
    _skyShader.setInAndOutLocations(skyLocations);
    _skyShader.addShader(GL_VERTEX_SHADER, "resources/shaders/sky.vert");
    _skyShader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/sky.frag");
    _skyShader.link();


    // Ground
    GlInputsOutputs groundLocations;
    groundLocations.setInput(0, "position_att");
    groundLocations.setInput(1, "normal_att");
    groundLocations.setInput(2, "texCoord_att");
    _groundShader.setInAndOutLocations(groundLocations);
    _groundShader.addShader(GL_VERTEX_SHADER, "resources/shaders/ground.vert");
    _groundShader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/ground.frag");
    _groundShader.link();

    // Water
    GlInputsOutputs waterLocations;
    waterLocations.setInput(0, "position_att");
    _waterShader.setInAndOutLocations(waterLocations);
    _waterShader.addShader(GL_VERTEX_SHADER, "resources/shaders/water.vert");
    _waterShader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/water.frag");
    _waterShader.link();

    // Infrastructure
    GlInputsOutputs infrastructureLocations;
    infrastructureLocations.setInput(0, "position_att");
    infrastructureLocations.setInput(1, "normal_att");
    infrastructureLocations.setInput(2, "texCoord_att");
    _infrastructShader.setInAndOutLocations(infrastructureLocations);
    _infrastructShader.addShader(GL_VERTEX_SHADER, "resources/shaders/infrastruct.vert");
    _infrastructShader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/infrastruct.frag");
    _infrastructShader.link();
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
    _skyShader.pushProgram();
    _skyShader.setVec4f("NightSkyColor",_description->nightSkyColor);
    _skyShader.setVec4f("DaySkyColor",  _description->daySkyColor);
    _skyShader.setVec4f("SunSetColor",  _description->sunSetColor);
    _skyShader.setVec4f("SkylineColor", _description->skylineColor);
    _skyShader.setVec4f("SunColor",     _description->sunColor);
    _skyShader.setFloat("SunRadius",    _description->sunRadius);
    _skyShader.setVec3f("SunPosition",  glm::vec3(0.0f, 0.0f, -1.0f));
    _skyShader.setVec2f("TexShift",     glm::vec2(0.0f, 0.0f));
    _skyShader.setInt("CloudsTexUnit",  0);
    _skyShader.setInt("StarsTexUnit",   1);
    _skyShader.setMat4f("SkyRotMatrix",  _city->sun().rotationMatrix());
    _skyShader.popProgram();

    _groundShader.pushProgram();
    _groundShader.setVec4f("sun.direction", _description->sunLight.direction);
    _groundShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _groundShader.setVec4f("sun.diffuse",   _description->sunLight.diffuse);
    _groundShader.setVec4f("sun.specular",  _description->sunLight.specular);
    _groundShader.setFloat("WaterHeight",   _ground->waterHeight());
    _groundShader.setVec4f("GrassColor",    _description->grassColor);
    _groundShader.setVec4f("MudColor",      _description->mudColor);
    _groundShader.setVec4f("WaterColor",    _description->waterColor);
    _groundShader.popProgram();

    _waterShader.pushProgram();
    _waterShader.setVec4f("sun.direction", _description->sunLight.direction);
    _waterShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _waterShader.setVec4f("sun.diffuse",   _description->sunLight.diffuse);
    _waterShader.setVec4f("sun.specular",  _description->sunLight.specular);
    _waterShader.setFloat("Shininess",     _description->waterShininess);
    _waterShader.setVec4f("WaterColor",    _description->waterColor);
    _waterShader.popProgram();

    _infrastructShader.pushProgram();
    _infrastructShader.setVec4f("sun.direction", _description->sunLight.direction);
    _infrastructShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _infrastructShader.setVec4f("sun.diffuse",   _description->sunLight.diffuse);
    _infrastructShader.setVec4f("sun.specular",  _description->sunLight.specular);
    _infrastructShader.setFloat("Shininess",     128.0f);
    _infrastructShader.setVec3f("Translation",   glm::vec3(0.0f, 0.0f, 0.0f));
    _infrastructShader.setVec2f("RepeatFrom",    glm::vec2(1.0f, 1.0f));
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
    glm::vec4  nLightDir = glm::normalize(_description->sunLight.direction);
    float sunIntensity = glm::max(dot(nLightDir, glm::vec4(0, 0, -1, 0)) + skyCoefCorrection, 0.0);
    float skyColorCoef = pow(sunIntensity, 0.75) / (skyCoefCorrection + 1.0);
    glm::vec4  skyCol = _description->nightSkyColor * (1 - skyColorCoef) +
                    _description->daySkyColor   * skyColorCoef;
    skyCol += _description->skylineColor;
    glClearColor(skyCol[0], skyCol[1], skyCol[2], skyCol[3]);



    // Sun ambient light
    const float AMBIENT_EFF_FACT = 0.50f;
    const float BASE_INTENSITY = 0.03f;
    const glm::vec4 BASE_LIGHT = glm::vec4(BASE_INTENSITY, BASE_INTENSITY, BASE_INTENSITY, 0.0f);
    _description->sunLight.ambient = BASE_LIGHT + skyCol * AMBIENT_EFF_FACT;


    // Sun position
    glm::vec4 sunDir = glm::normalize(_city->sun().direction());
    _description->sunLight.direction = sunDir;
    _description->viewedSunDirection = _description->viewMat * sunDir;

    updateShaders();

    // Update Components
    for(size_t i=0; i<_components.size(); ++i)
        _components[i]->update();
}

void DrawCityModule::updateShaders()
{
    static glm::vec2 shift(0.0f, 0.0f);
    shift[1] += 0.0008f;

    _skyShader.pushProgram();
    _skyShader.setVec4f("VCSunPosition", -glm::normalize(_city->sun().direction()));
    _skyShader.setVec4f("ECSunPosition", -_description->viewedSunDirection);
    _skyShader.setVec2f("TexShift",      shift);
    _skyShader.setMat4f("SkyRotMatrix",  transpose(_city->sun().rotationMatrix()));
    _skyShader.popProgram();

    _groundShader.pushProgram();
    _groundShader.setVec4f("sun.direction", _description->viewedSunDirection);
    _groundShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _groundShader.popProgram();

    _waterShader.pushProgram();
    _waterShader.setVec4f("sun.direction", _description->viewedSunDirection);
    _waterShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _waterShader.popProgram();

    _infrastructShader.pushProgram();
    _infrastructShader.setVec4f("sun.direction", _description->viewedSunDirection);
    _infrastructShader.setVec4f("sun.ambient",   _description->sunLight.ambient);
    _infrastructShader.popProgram();
}

void DrawCityModule::updateProjectionMatrix(const glm::mat4& proj)
{
    _description->projMat = proj;

    updateShadersProjectionMatrix();
}

void DrawCityModule::updateShadersProjectionMatrix()
{    
    _minimalistShader.pushProgram();
    _minimalistShader.setMat4f("ProjectionMatrix", _description->projMat);
    _minimalistShader.popProgram();

    _skyShader.pushProgram();
    _skyShader.setMat4f("ProjectionMatrix", _description->projMat);
    _skyShader.popProgram();

    _groundShader.pushProgram();
    _groundShader.setMat4f("ProjectionMatrix", _description->projMat);
    _groundShader.popProgram();

    _waterShader.pushProgram();
    _waterShader.setMat4f("ProjectionMatrix", _description->projMat);
    _waterShader.popProgram();

    _infrastructShader.pushProgram();
    _infrastructShader.setMat4f("ProjectionMatrix", _description->projMat);
    _infrastructShader.popProgram();
}

void DrawCityModule::updateModelViewMatrix(const glm::mat4& view)
{
    _description->viewMat = view;
    _description->normalMat = glm::mat3(view);

    updateShadersModelViewMatrix();
}

void DrawCityModule::updateShadersModelViewMatrix()
{
    _minimalistShader.pushProgram();
    _minimalistShader.setMat4f("ViewMatrix", _description->viewMat);
    _minimalistShader.popProgram();

    _skyShader.pushProgram();
    _skyShader.setMat3f("ViewMatrix", _description->normalMat);
    _skyShader.popProgram();

    _groundShader.pushProgram();
    _groundShader.setMat4f("ViewMatrix",   _description->viewMat);
    _groundShader.setMat3f("NormalMatrix", _description->normalMat);
    _groundShader.popProgram();

    _waterShader.pushProgram();
    _waterShader.setMat4f("ViewMatrix",   _description->viewMat);
    _waterShader.setMat3f("NormalMatrix", _description->normalMat);
    _waterShader.popProgram();

    _infrastructShader.pushProgram();
    _infrastructShader.setMat4f("ViewMatrix",   _description->viewMat);
    _infrastructShader.setMat3f("NormalMatrix", _description->normalMat);
    _infrastructShader.popProgram();
}
