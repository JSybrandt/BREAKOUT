#ifndef Constants_H
#define Constants_H

//Defines to make life easier

#define Vector3 D3DXVECTOR3
#define Vector2 D3DXVECTOR2
#define Matrix D3DXMATRIX
#define Identity D3DXMatrixIdentity
#define Translate D3DXMatrixTranslation
#define RotateX D3DXMatrixRotationX
#define RotateY D3DXMatrixRotationY
#define RotateZ D3DXMatrixRotationZ
#define ToRadian D3DXToRadian
#define ToDegree D3DXToDegree
#define Normalize D3DXVec3Normalize
#define Length D3DXVec3Length
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define TEXTURE ID3D10ShaderResourceView

// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\CMwavebank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\CMsoundbank.xsb";
// XGS_FILE must be location of .xgs file.
const char XGS_FILE[]   = "audio\\Win\\audioProject.xgs";

const int CREAK_SIZE = 3;

//Many sound effects found using or relating to http://www.freesfx.co.uk/
//Audio cues
const char CLICK[] = "click";//Caelan
const char GROWL[] = "growl1";//Caelan
const char BG[] = "bg";//Caelan
const char CREAK[] = "creak";//Caelan
const char CREAK1[] = "creak1";//Caelan
const char CREAK2[] = "creak2";//Caelan
const char CREAK3[] = "creak3";//Caelan
const char HOWL[] = "howl";//Caelan
const char WALK1[] = "walk1";
const char WALK2[] = "walk2";
const char RUN1[] = "run1";
const char OPEN[] = "doorOpen";//Zared
const char CLOSE[] = "doorClose";//Zared
const char DOGGROWL[] = "dogGrowl";//Zared
const char JINGLE[] = "jingle";//Caelan
const char PICKUP[] = "pickup";//Caelan
const char UNLOCK[] = "unlock";//Caelan 
const char BITE[] = "bite";//Caelan 
const char BITESCREAM[] = "bitescream";//Caelan 
const char SHAKE[] = "shake"; //Caelan
const char ACTIVATION[] = "activation"; //Caelan
const char EMPTY[] = "empty"; //Caelan
const char LOW_GROWL[] = "lowGrowl";//Justin
const char BARKING[] = "dogBarking"; //Justin
const char BIRM[] = "birimingham";//Zared
const char MAGIC_MISSILE[] = "magicMissile";//Justin
const char GIGGLE1[] = "giggle1"; //Caelan
const char GIGGLE2[] = "giggle2"; //Caelan
const char CRYING[] = "crying"; //Caelan
#endif