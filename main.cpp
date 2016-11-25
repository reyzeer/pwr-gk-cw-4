/// Laboratorium: Grafika Komputerowa
/// Prowadzący: Dr inż. Marek Woda
/// Zadanie 3
/// Zbigniew Czarnecki 209909

/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi
//  układu współrzednych

/*************************************************************************************/

#include "common.h"

const float MIN_R = 7.0f;
const float MAX_R = 30.0f;
const float SPEED = 0.1f;

typedef float point3[3];
static GLfloat viewer[]= {0.0, 0.0, 10.0};

static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat phi = 0.0;
static GLfloat r = 10.0;
static GLfloat pix2angle_x;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_y;     // przelicznik pikseli na stopnie

/// stan klawiszy myszy
/// 0 - nie naciśnięto żadnego klawisza
/// 1 - naciśnięty zostać lewy klawisz
static GLint statusKey = 0;

/// poprzednia pozycja kursora myszy
static int x_pos_old=0;

/// poprzednia pozycja kursora myszy
static int y_pos_old=0;

/// różnica pomiędzy pozycją bieżącą i poprzednią kursora myszy
static int delta_x = 0;

/// różnica pomiędzy pozycją bieżącą i poprzednią kursora myszy
static int delta_y = 0;

/// Obiekt generujący jajko
Egg egg = Egg();

/*************************************************************************************/

/// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{

    if (btn==GLUT_LEFT_BUTTON  && state == GLUT_DOWN) {
        // przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        x_pos_old = x;
        y_pos_old = y;
        statusKey = 1;          // wcięnięty został lewy klawisz myszy
    } else if (btn==GLUT_RIGHT_BUTTON  && state == GLUT_DOWN) {
        y_pos_old = y;		// przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        statusKey = 2;			//wciśnięty został prawy klawisz myszy
    } else {
        statusKey = 0;          // nie został wcięnięty żaden klawisz
    }

}

/*************************************************************************************/

/// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
/// zmiennych globalnych
void Motion( GLsizei x, GLsizei y )
{

    delta_x=x-x_pos_old;     // obliczenie różnicy położenia kursora myszy
    delta_y=y-y_pos_old;     // obliczenie różnicy położenia kursora myszy

    x_pos_old=x;            // podstawienie bieżącego położenia jako poprzednie
    y_pos_old=y;            // podstawienie bieżącego położenia jako poprzednie

    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

void setTheta()
{
    //jeśli lewy klawisz myszy wciśnięty
    if (statusKey == 1) {
        theta += delta_x*pix2angle_x;    // modyfikacja kąta obrotu o kat proporcjonalny
        if (theta >= 360.0) {
            theta = 0.0;
        }
    }
}

void setPhi()
{
    // jeśli lewy klawisz myszy wcięnięty
    if (statusKey == 1) {
        phi += delta_y*pix2angle_y;    // do różnicy położeń kursora myszy
        if (phi >= 360.0) {
            phi = 0.0;
        }
    }
}

void setR()
{
    if (statusKey == 2) {
        r += 0.1 * delta_y;
        if (r <= MIN_R) { // ograniczenie zblizenia
            r = MIN_R;
        }
        if (r >= MAX_R) { // ograniczenie oddalenia
            r = MAX_R;
        }
    }
}

///Ustawia położenie obserwatora
void setViewer()
{

    setTheta();
    setPhi();
    setR();

    viewer[0] = r * cos(theta) * cos(phi);
    viewer[1] = r * sin(phi);
    viewer[2] = r * sin(theta) * cos(phi);

    // Zdefiniowanie położenia obserwatora
    gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, 0.0, 0.0, cos(phi), 0.0);

}

void prepareEgg()
{

    egg.n = 30;
    egg.posX = 0.0f;
    egg.posY = 5.0f;
    egg.posZ = 0.0f;
    egg.renderType = 2;

}

/// Funkcja określająca co ma być rysowane (zawsze wywoływana gdy trzeba przerysować scenę)
void RenderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszczącym

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej

    setViewer();

    RDraw::Axes();
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
            if (egg.n > 0) {
                egg.destruct();
                egg.n--;
            }
            break;
        case ']':
            egg.destruct();
            egg.n++;
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

    pix2angle_x = 360.0 * SPEED / (float)horizontal;  // przeliczenie pikseli na stopnie
    pix2angle_y = 360.0 * SPEED / (float)vertical;  // przeliczenie pikseli na stopnie

    glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bieżącej

    //GLdouble aspect = (horizontal > vertical) ? horizontal/vertical : vertical/horizontal;
    GLdouble aspect = (GLdouble) horizontal / (GLdouble) vertical;
    gluPerspective(70.0, aspect, 1.0, 30.0);
    // Ustawienie parametrów dla rzutu perspektywicznego

    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkości okna okna widoku (viewport) w zależności
    // relacji pomiędzy wysokością i szerokością okna

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej

}

/*************************************************************************************/

/// Główny punkt wejścia programu. Program działa w trybie konsoli
int main(int argc, char **argv)
{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(450, 450);

    glutCreateWindow("Jajko Cw 4 - Zbigniew Czarnecki 209909");

    glutDisplayFunc(RenderScene);
    // Określenie, że funkcja RenderScene będzie funkcją zwrotną
    // (callback function).  Bedzie ona wywoływana za każdym razem
    // gdy zajdzie potrzba przeryswania okna

    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
    // zazmiany rozmiaru okna

    glutKeyboardFunc(keys);

    glutMouseFunc(Mouse);
    // Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy

    glutMotionFunc(Motion);
    // Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy

    prepareEgg();

    MyInit();
    // Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
    // inicjalizacje konieczne  przed przystąpieniem do renderowania

    glEnable(GL_DEPTH_TEST);
    // Włączenie mechanizmu usuwania powierzchni niewidocznych

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

    return 0;

}
