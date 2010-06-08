
#ifndef _included_constructionyard_h
#define _included_constructionyard_h

#include "worldobject/building.h"


// ****************************************************************************
// Class ConstructionYard
// ****************************************************************************

#define YARD_NUMPRIMITIVES 9
#define YARD_NUMRUNGSPIKES 6

class ConstructionYard : public Building
{
protected:   
    Shape           *m_rung;
    Shape           *m_primitive;
    ShapeMarker     *m_primitives[YARD_NUMPRIMITIVES];
    ShapeMarker     *m_rungSpikes[YARD_NUMRUNGSPIKES];

    int     m_numPrimitives;
    int     m_numSurges;
    int     m_numTanksProduced;
    double   m_fractionPopulated;
    double   m_timer;
    
    double   m_alpha;

    bool    IsPopulationLocked();                       // Are there too many tanks already
    
public:
    ConstructionYard();

    bool Advance();
    
    void Render         ( double _predictionTime );
    void RenderAlphas   ( double _predictionTime );

    Matrix34 GetRungMatrix1();
    Matrix34 GetRungMatrix2();

    bool AddPrimitive();
    void AddPowerSurge();
};



// ****************************************************************************
// Class DisplayScreen
// ****************************************************************************

#define DISPLAYSCREEN_NUMRAYS 3

class DisplayScreen : public Building
{
protected:    
    Shape       *m_armour;
    ShapeMarker *m_rays[DISPLAYSCREEN_NUMRAYS];    

public:
    DisplayScreen();

    void RenderAlphas       ( double _predictionTime );	

};


#endif
