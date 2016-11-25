/// Laboratorium: Grafika Komputerowa
/// Prowadzący: Dr inż. Marek Woda
/// Zadanie 3
/// Zbigniew Czarnecki 209909

#include "common.h"

const float pi = 3.14159265359;

class Egg
{

private:

    float *** matrix;
    float *** colors;

    bool build = false;

public:

    int n;
    /// 0 - draw points
    /// 1 - draw lines
    /// 2 - drwa traingles
    int renderType;

    float posX, posY, posZ;

    /// Generuje "dwu"wymiarową tablicę punktów XYZ
    void prepareMatrix();

    float x(float u, float v);
    float y(float u, float v);
    float z(float u, float v);

    void transform2Egg();

    void randColors();
    void fourColors();
    void deleteSeam();

    void insertVertexWithColor(int i, int j);

    void drawPoints();
    void drawLines();
    void drawTriangles();

    void destruct();
    void draw();

};