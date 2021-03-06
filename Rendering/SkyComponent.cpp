#include "SkyComponent.h"

#include <vector>

#include <GLM/gtc/constants.hpp>

#include <GL3/gl3w.h>

#include <CellarWorkbench/GL/GlToolkit.h>


using namespace std;
using namespace cellar;


SkyComponent::SkyComponent(City &city, GlProgram &shader) :
    AbstractComponent(city, shader),
    _skyBuffs(),
    _skyVao(0),
    _skyNbElems(0),
    _cloudsTex(0),
    _nightSkyTex(0)
{
    glGenVertexArrays(1, &_skyVao);
    glGenBuffers(_SKY_NB_BUFFS, _skyBuffs);

    _cloudsTex = GlToolkit::genTextureId(_city.sky().cloudsImage());
}

SkyComponent::~SkyComponent()
{
    glDeleteVertexArrays(1, &_skyVao);
    glDeleteBuffers(_SKY_NB_BUFFS, _skyBuffs);
    GlToolkit::deleteTextureId(_cloudsTex);
    GlToolkit::deleteTextureId(_nightSkyTex);
}

void SkyComponent::setup()
{
    setupSky();
}

void SkyComponent::setupSky()
{
    vector<glm::vec3> positions;

    const float r = 2.0f;

    positions.push_back(glm::vec3(0.0f,           0.0f,          r));
    positions.push_back(glm::vec3(r*cos(6*glm::pi<float>()/3),  r*sin(6*glm::pi<float>()/3), -r/2.0f));
    positions.push_back(glm::vec3(r*cos(4*glm::pi<float>()/3),  r*sin(4*glm::pi<float>()/3), -r/2.0f));
    positions.push_back(glm::vec3(r*cos(2*glm::pi<float>()/3),  r*sin(2*glm::pi<float>()/3), -r/2.0f));
    positions.push_back(glm::vec3(r*cos(0*glm::pi<float>()/3),  r*sin(0*glm::pi<float>()/3), -r/2.0f));
    _skyNbElems = static_cast<int>(positions.size());


    //VAO setup
    glBindVertexArray(_skyVao);

    int position_loc = _shader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, _skyBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    _shader.setTexture(GL_TEXTURE0, GL_TEXTURE_2D, _cloudsTex);
    _shader.setTexture(GL_TEXTURE1, GL_TEXTURE_2D, "resources/textures/nightSkySphere.png");
}

void SkyComponent::draw()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);

    _shader.pushProgram();
    glBindVertexArray(_skyVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _skyNbElems);
    _shader.popProgram();

    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
}
