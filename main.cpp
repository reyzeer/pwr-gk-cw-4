/// Laboratorium: Grafika Komputerowa
/// Prowadzący: Dr inż. Marek Woda
/// Zadanie 1
/// Zbigniew Czarnecki 209909

/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi
//  układu współrzednych

/*************************************************************************************/

#include "common.h"

/// trzy kąty obrotu
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
/// Obiekt generujący jajko
Egg egg = Egg();
/// Prędkość obrotu
float speed = 0.5;

/*************************************************************************************/

void spinEgg()
{

    theta[0] -= speed;
    if (theta[0] > 360.0) theta[0] -= 360.0;

    theta[1] -= speed;
    if (theta[1] > 360.0) theta[1] -= 360.0;
    theta[2] -= speed;
    if (theta[2] > 360.0) theta[2] -= 360.0;

    glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

/// Funkcja określająca co ma być rysowane (zawsze wywoływana gdy trzeba przerysować scenę)
void RenderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszczącym

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej

    RDraw::Axes();

    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);

    glRotated(60.0, 1.0, 1.0, 1.0);
    egg.draw();

    //glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty

    glFlush();
    // Przekazanie poleceń rysujących do wykonania

    glutSwapBuffers();

}

/*************************************************************************************/

void keys(unsigned char key, int x, int y)
{

    switch (key) {

        case 'p':
            //Points
            egg.renderType = 0;
            break;
        case 'w':
            //Lines
            egg.renderType = 1;
            break;
        case 's':
            //Triangles
            egg.renderType = 2;
            break;

        case '[':
            egg.destruct();
            if (egg.n > 0) {
                egg.n--;
            }
            break;
        case ']':
            egg.destruct();
            egg.n++;
            break;
        case '-':
            speed *= 0.9090909;
            break;
        case '+':
            speed *= 1.1;
            break;

    }

    RenderScene(); // przerysowanie obrazu sceny
}

/*************************************************************************************/

/// Funkcja ustalająca stan renderowania
void MyInit(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Kolor czyszcący (wypełnienia okna) ustawiono na czarny

}

/*************************************************************************************/

/// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
/// w przypadku zmiany rozmiarów okna.
/// Parametry vertical i horizontal (wysokość i szerokość okna) są
/// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

    GLfloat AspectRatio;
    // Deklaracja zmiennej AspectRatio  określającej proporcję
    // wymiarów okna

    if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
        vertical = 1;

    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkościokna okna widoku (viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)

    glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bieżącej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie współczynnika  proporcji okna
    // Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
    // przestrzeni ograniczającej pozwalającej zachować właściwe
    // proporcje rysowanego obiektu.
    // Do okreslenia przestrzeni ograniczjącej służy funkcja
    // glOrtho(...)

    if (horizontal <= vertical)
        glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
    else
        glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu

    glLoadIdentity();
    // Czyszcenie macierzy bieżącej

}

/*************************************************************************************/

/// Główny punkt wejścia programu. Program działa w trybie konsoli
int main(int argc, char **argv)
{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(450, 450);

    glutCreateWindow("Jajko - Zbigniew Czarnecki 209909");

    glutDisplayFunc(RenderScene);
    // Określenie, że funkcja RenderScene będzie funkcją zwrotną
    // (callback function).  Bedzie ona wywoływana za każdym razem
    // gdy zajdzie potrzba przeryswania okna


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
    // zazmiany rozmiaru okna

    glutKeyboardFunc(keys);
    glutIdleFunc(spinEgg);

    egg.n = 30;
    egg.posX = 0.0f;
    egg.posY = 5.0f;
    egg.posZ = 0.0f;
    egg.renderType = 2;

    MyInit();
    // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
    // inicjalizacje konieczne  przed przystąpieniem do renderowania

    glEnable(GL_DEPTH_TEST);
    // Włączenie mechanizmu usuwania powierzchni niewidocznych

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

    return 0;

}
