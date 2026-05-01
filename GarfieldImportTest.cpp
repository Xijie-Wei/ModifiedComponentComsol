#include <TApplication.h>
#include <TApplication.h>
#include <TCanvas.h>

#include <iostream>
#include <omp.h>

#include <fstream>
#include <vector>
#include <string>

#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ParticleTypes.hh"
#include "Garfield/ComponentConstant.hh"
#include "ComponentComsol.hh"
#include "Garfield/AvalancheMicroscopic.hh"

using namespace Garfield;

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);
    
    MediumMagboltz gas("cf4", 100.);
    gas.SetPressure(1 * 760.);
    gas.SetTemperature(273.15);
    gas.LoadGasFile("1AtmCF4.gas");

    
    ComponentComsol fm;
    fm.Initialise("mesh.mphtxt", "dielectirc.dat", "potential.txt");
    std::cout << "Initialise magnetic field" << std::endl;
    fm.InitialiseMagnticField("mesh.mphtxt", "dielectirc.dat", "magneticfield.txt");
    fm.PrintRange();
    fm.SetGas(&gas);
    
    
    
    // Using ComponentConstant as a ref
    /*
    ComponentConstant fm;
    fm.SetMagneticField(0.,0.,0.);
    fm.SetElectricField(0.,0.,99.994);
    fm.SetArea(-5,-5,0,5,5,5);
    fm.SetMedium(&gas);
    */

    /*
    // test if can found magnetic field from fm (passed) 
    double bx,by,bz;
    int state;
    fm.MagneticField(0.,0.,1.,bx,by,bz,state);
    std::cout<< bx << by << bz<<std::endl;
    */

    std::cout<<"Magnetic field at component: " <<fm.HasMagneticField()<< std::endl;

    ViewField fieldView(&fm);
    // Set the normal vector of the viewing plane (xz plane).
    fieldView.SetPlane(0, -1, 0, 0, 0, 0);
    // Set the plot limits in the current viewing plane.
    fieldView.SetArea(-5, -.5 ,5,  5.5);
    TCanvas* cfPon = new TCanvas("cfP", "", 600, 600);
    cfPon->SetLeftMargin(0.16);
    fieldView.SetCanvas(cfPon);
    fieldView.PlotContour("p");
    cfPon->SaveAs("Potential.png");
    
    TCanvas* cfBMag = new TCanvas("cfBmag", "", 600, 600);
    cfBMag->SetLeftMargin(0.16);
    fieldView.SetCanvas(cfBMag);
    fieldView.PlotContour("bmag");
    cfBMag->SaveAs("BfieldMag.png");

    TCanvas* cfBZ = new TCanvas("cfBz", "", 600, 600);
    cfBZ->SetLeftMargin(0.16);
    fieldView.SetCanvas(cfBZ);
    fieldView.PlotContour("bz");
    cfBZ->SaveAs("Bfieldz.png");

    Sensor sensor(&fm);
    std::cout<< "Number of components: "<< sensor.GetNumberOfComponents()<< std::endl;
    sensor.EnableMagneticField(0, true);
    std::cout<<"Magnetic field at sensor: " <<sensor.HasMagneticField()<< std::endl;
    AvalancheMicroscopic aval(&sensor);
    aval.EnableMagneticField();

    /*
    // test if can found magnetic field from sensor (passed) 
    double bx,by,bz;
    int state;
    sensor.MagneticField(0.,0.,1.,bx,by,bz,state);
    std::cout<< bx << by << bz<<std::endl;
    */

    // Monte Carlo
    /*
    double Initx = 0., Inity = 1., Initz = 4.9, InitE = 0.; // [cm], [cm], [cm], [eV]
    int NumEvents = 500;

    std::vector<double> xFinas(NumEvents);
    std::vector<double> yFinas(NumEvents);
    //std::vector<double> zFinas(NumEvents);
    std::vector<double> tFinas(NumEvents);

    std::cout <<"Number of event simulated: " << NumEvents << std::endl;

    for (int idx = 0; idx < NumEvents; idx++){
        aval.AvalancheElectron( Initx, Inity, Initz, 0, InitE, 0, 0, 0);
        
        auto Electron = aval.GetElectrons();
        auto Endpoint = Electron[0].path.back();
        std::cout << idx + 1 << " / " << NumEvents << " Electron Endpoint x:" << Endpoint.x << " y: "<< Endpoint.y << " z: " << Endpoint.z << std::endl;  
        xFinas[idx] = Endpoint.x;
        yFinas[idx] = Endpoint.y;
        tFinas[idx] = Endpoint.t;

    }
    
    // outupt
    char filename[100];
    snprintf(filename, sizeof(filename), "./output.txt");
    std::ofstream outFile(filename);
    // the important part
    for (int idx = 0; idx < NumEvents; idx ++){ outFile << xFinas[idx] <<" , " << yFinas[idx]<<" , " << tFinas[idx] <<"\n";}
    outFile.close();
    */
}