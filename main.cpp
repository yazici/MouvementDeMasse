#include <exception>
#include <iostream>
using namespace std;

#include <DataStructure/PGrid3D.h>

#include <ScaenaApplication/Application.h>
#include <Stages/QGLStage.h>
using namespace scaena;

#include "MdMPlay.h"
#include "Ui/MainWindow.h"

class Test
{
public:
    Test() {cout << "C";}
    ~Test() {cout << "D";}
};

int main(int argc, char** argv) try
{
    getApplication().init(argc, argv, new MdMPlay());

    QGLStage* stage = new QGLStage();
    getApplication().addCustomStage(stage);
    getApplication().chooseStage(stage->id());

    stage->setDrawSynch( false );
    stage->setDrawInterval( 16 );
    stage->setUpdateInterval( 16 );

    MainWindow window(stage);
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
