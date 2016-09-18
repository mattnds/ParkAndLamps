//----------------------------------------------------------------------------

const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

extern vec4 points_cube[NumVerticesCube];
extern vec4 colors[NumVerticesCube];

void colorcube();


//----------------------------------------------------------------------------

const int segments = 64;
const int NumVerticesCylinder = segments*6 + segments*3*2;

extern vec4 points_cylinder[NumVerticesCylinder];

void colortube();