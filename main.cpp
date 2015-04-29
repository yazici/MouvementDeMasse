#include <exception>
#include <iostream>
using namespace std;

#include <CellarWorkbench/Misc/Log.h>
using namespace cellar;

#include <Scaena/ScaenaApplication/Application.h>
#include <Scaena/ScaenaApplication/GlMainWindow.h>
#include <Scaena/Stage/QGLStage.h>
using namespace scaena;

#include "MdMPlay.h"


int main(int argc, char** argv) try
{
    getLog().setOuput(std::cout);

    getApplication().init(argc, argv);
    getApplication().setPlay(shared_ptr<AbstractPlay>(new MdMPlay()));

    QGLStage* stage = new QGLStage();
    getApplication().addCustomStage(stage);
    getApplication().chooseStage(stage->id());

    stage->setDrawSynch( false );
    stage->setDrawInterval( 0 );
    stage->setUpdateInterval( 0 );

    GlMainWindow window(stage);
    window.setGlWindowSpace(800, 600);
    window.centerOnScreen();
    window.show();
    

    return getApplication().execute();    
}
catch(exception& e)
{
    cerr << "Exception caught : " << e.what() << endl;
}
catch(...)
{
    cerr << "Exception passed threw.." << endl;
    throw;
}
