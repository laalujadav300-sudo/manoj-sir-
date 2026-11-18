#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glut.h>

using namespace std;

int edges = 0;
vector<int> pntX, pntY;
double sx = 1.0, sy = 1.0;
double angleRad = 0.0;

int rounded(double value)
{
    return static_cast<int>(std::lround(value));
}

void drawOriginal()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; ++i)
        glVertex2i(pntX[i], pntY[i]);
    glEnd();
}

void drawScaledRotated()
{
    glColor3f(0.75f, 0.0f, 0.75f);
    glBegin(GL_POLYGON);

    for (int i = 0; i < edges; ++i)
    {
        // Order: scale first, then rotate the scaled coordinates.
        double x1 = pntX[i] * sx;
        double y1 = pntY[i] * sy;

        double newX = x1 * cos(angleRad) - y1 * sin(angleRad);
        double newY = x1 * sin(angleRad) + y1 * cos(angleRad);

        glVertex2i(rounded(newX), rounded(newY));
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawOriginal();
    drawScaledRotated();
    glFlush();
}

void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-640.0, 640.0, -480.0, 480.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    int angleDegrees = 0;

    cout << "--- SCALING FOLLOWED BY ROTATION ---\n";
    cout << "Enter number of polygon vertices (minimum 3): ";
    cin >> edges;

    if (!cin || edges < 3 || edges > 100)
    {
        cerr << "Invalid number of vertices. Enter a value from 3 to 100.\n";
        return 1;
    }

    pntX.resize(edges);
    pntY.resize(edges);

    for (int i = 0; i < edges; ++i)
    {
        cout << "Enter vertex " << i + 1 << " (X Y): ";
        cin >> pntX[i] >> pntY[i];
        if (!cin)
        {
            cerr << "Invalid vertex coordinates.\n";
            return 1;
        }
    }

    cout << "Enter scaling factors (sx sy): ";
    cin >> sx >> sy;
    cout << "Enter rotation angle in degrees: ";
    cin >> angleDegrees;

    if (!cin)
    {
        cerr << "Invalid transformation values.\n";
        return 1;
    }

    const double pi = acos(-1.0);
    angleRad = angleDegrees * pi / 180.0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scaling Followed by Rotation");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 6 - BASIC 2D TRANSFORMATIONS
PROGRAM: SCALING FOLLOWED BY ROTATION
===============================================================================

1. AIM
To implement scaling followed by rotation on a 2D polygon and display the
original and transformed polygon using C++, OpenGL and GLUT.

2. OBJECTIVES
1. To understand scaling and rotation transformations.
2. To accept a polygon and transformation values from the user.
3. To resize the polygon using X and Y scaling factors.
4. To rotate the scaled polygon by a specified angle.
5. To convert the angle from degrees to radians.
6. To observe the effect of applying transformations in sequence.

3. REQUIREMENTS
Software: C++ compiler, OpenGL, GLU and GLUT/FreeGLUT development libraries.
Hardware: Computer with a graphics display.

4. THEORY
A 2D transformation changes an object's coordinates in the XY plane.
A polygon is made up of ordered vertices. In this program every vertex
is scaled first and the resulting point is rotated second.

Scaling:
Scaling changes the dimensions of an object:
    x1 = x * sx
    y1 = y * sy
sx controls horizontal scaling and sy controls vertical scaling.
Equal factors produce uniform scaling. Different factors produce
non-uniform scaling.

Rotation:
Rotation changes the orientation of an object around the origin:
    x' = x1*cos(theta) - y1*sin(theta)
    y' = x1*sin(theta) + y1*cos(theta)
The angle must be supplied in radians to C++ sin() and cos() functions.
    radians = degrees*pi/180

Combined transformation in this program:
    x' = (x*sx)*cos(theta) - (y*sy)*sin(theta)
    y' = (x*sx)*sin(theta) + (y*sy)*cos(theta)
The program scales first and rotates the scaled coordinates second.
Changing the order generally changes the final coordinates.

Matrix representation:
Scaling matrix:
    S = [ sx  0  0 ]
        [  0 sy  0 ]
        [  0  0  1 ]

Rotation matrix:
    R = [ cos(theta) -sin(theta) 0 ]
        [ sin(theta)  cos(theta) 0 ]
        [     0           0      1 ]

Using column-vector notation, scaling followed by rotation is R*S.

5. ALGORITHM
1. Start.
2. Read the number of polygon vertices.
3. Validate the vertex count.
4. Read the X and Y coordinates of all vertices.
5. Read scaling factors sx and sy.
6. Read the rotation angle in degrees.
7. Convert the angle into radians.
8. Initialize GLUT and create the graphics window.
9. Set the white background and orthographic projection.
10. Draw the original polygon in red.
11. For every vertex, calculate x1=x*sx and y1=y*sy.
12. Rotate the scaled point using sine and cosine.
13. Round the final coordinates.
14. Draw the transformed polygon in magenta.
15. Flush drawing commands and stop when the window closes.

6. WORKING OF THE PROGRAM
- pntX and pntY store the original polygon coordinates.
- sx and sy store the scaling factors.
- angleRad stores the rotation angle in radians.
- rounded() converts floating-point values into integer coordinates.
- drawOriginal() draws the original polygon in red.
- drawScaledRotated() performs scaling first and rotation second.
- display() draws both polygons.
- initialize() sets the background and viewing region.
- main() reads user input and starts GLUT.

7. IMPORTANT FUNCTIONS
std::vector: dynamic storage for polygon coordinates.
std::lround(): rounds floating-point values to the nearest integer.
sin()/cos(): calculate trigonometric values for rotation.
acos(-1.0): provides pi for angle conversion.
gluOrtho2D(): defines the 2D coordinate region.
glBegin(GL_POLYGON)/glEnd(): specify a filled polygon.
glVertex2i(): supplies each 2D vertex.
glFlush(): executes pending drawing commands.

8. EXECUTION COMMAND
Install the OpenGL development package if needed:
    sudo apt-get install freeglut3-dev
Compile:
    g++ scaRot6.cpp -o scaRot6 -lGL -lGLU -lglut
Run:
    ./scaRot6

9. SAMPLE INPUT AND OUTPUT
Sample input:
    Number of vertices: 3
    Vertex 1: 100 100
    Vertex 2: 200 100
    Vertex 3: 150 200
    Scaling factors: 2 1
    Rotation angle: 90 degrees

After scaling:
    (100,100) -> (200,100)
    (200,100) -> (400,100)
    (150,200) -> (300,200)

After 90-degree counter-clockwise rotation:
    (200,100) -> (-100,200)
    (400,100) -> (-100,400)
    (300,200) -> (-200,300)

Graphics output:
    Original polygon is shown in red.
    Scaled-and-rotated polygon is shown in magenta.

10. RESULT
The program accepts a polygon, applies scaling and then rotation, and
displays both the original and transformed polygons.

11. CONCLUSION
Thus, scaling followed by rotation was implemented successfully. The
practical demonstrates scaling factors, rotation equations, angle
conversion and the importance of transformation order in 2D graphics.

12. PRECAUTIONS
- Enter at least three vertices.
- Enter vertices in boundary order.
- Convert degrees to radians before using sin() and cos().
- The transformations are performed about the origin.
- The viewing region includes negative coordinates for rotated output.
- GL_POLYGON is intended for simple convex polygons.

===============================================================================
HINGLISH EXPLANATION FOR VIVA
===============================================================================

A. PROGRAM KA BASIC IDEA
Is program mein polygon ko do steps mein transform kiya jata hai. First
scaling hoti hai, yani object ka size change hota hai. Second rotation hoti
hai, yani scaled object ko origin ke around angle se rotate karte hain.
Original polygon red aur final polygon magenta colour mein show hota hai.

B. FORMULA WITH EXAMPLE
Original point (100,100), scaling factors sx=2 aur sy=1:
    x1=100*2=200
    y1=100*1=100
Scaled point (200,100).

Ab 90 degree rotation:
    x'=200*cos90 - 100*sin90 = -100
    y'=200*sin90 + 100*cos90 = 200
Final point (-100,200).

C. CODE EXPLANATION
1. Header files:
   cmath mathematical calculations ke liye, iostream input-output ke liye,
   vector coordinates store karne ke liye aur glut.h graphics ke liye.

2. Global variables:
   edges vertices ki count hai. pntX,pntY polygon coordinates hain.
   sx,sy scaling factors hain. angleRad rotation angle radians mein hai.

3. rounded():
   Final coordinate decimal ho sakta hai. lround usko nearest integer
   banata hai, jisse glVertex2i use kar sake.

4. drawOriginal():
   Red colour set karke original polygon draw karta hai.

5. drawScaledRotated():
   Loop har vertex ke liye chalta hai. Pehle x1=x*sx aur y1=y*sy.
   Uske baad rotation formula apply hota hai. Final coordinate round
   karke OpenGL ko diya jata hai.

6. Angle conversion:
   Input degrees mein hota hai, lekin sin/cos radians use karte hain.
   Isliye angleRad=degrees*pi/180 calculate hota hai.

7. display():
   Window clear hoti hai, original polygon draw hota hai, phir transformed
   polygon draw hota hai. glFlush commands execute karta hai.

8. initialize():
   White background aur orthographic 2D coordinate range set karta hai.
   Negative coordinate range isliye rakha hai kyunki rotation ke baad
   object negative X ya Y side par ja sakta hai.

9. main():
   Vertices aur transformation parameters read karta hai, angle convert
   karta hai, GLUT initialize karta hai aur display callback set karta hai.

D. TRANSFORMATION ORDER
Is code mein scaling first aur rotation second hai. Matrix notation mein
column-vector convention ke saath combined matrix R*S hota hai. Agar order
R*S se S*R kar diya to generally output alag aayega. Isliye order ko
program ke formulas aur manual mein clearly samajhna zaroori hai.

E. VIVA QUESTIONS WITH ANSWERS
Q1. Scaling kya hai?
Ans: Object ka size X aur Y direction mein change karna scaling hai.

Q2. Rotation kya hai?
Ans: Object ko fixed point ke around given angle se turn karna rotation hai.

Q3. Scaling formula?
Ans: x'=x*sx aur y'=y*sy.

Q4. Rotation formula?
Ans: x'=x cos(theta)-y sin(theta), y'=x sin(theta)+y cos(theta).

Q5. Is program mein pehle kya hota hai?
Ans: Pehle scaling hoti hai, uske baad rotation.

Q6. sx aur sy ka role?
Ans: sx horizontal size change karta hai aur sy vertical size change karta hai.

Q7. Uniform scaling?
Ans: Jab sx=sy ho.

Q8. Non-uniform scaling?
Ans: Jab sx aur sy different ho.

Q9. Angle radians mein kyu convert hota hai?
Ans: C++ sin aur cos radians mein angle lete hain.

Q10. Positive rotation direction?
Ans: Standard Cartesian plane mein counter-clockwise.

Q11. Transformation order kyu important hai?
Ans: Scaling ke baad rotation aur rotation ke baad scaling ke results
generally same nahi hote.

Q12. Origin kya hai?
Ans: Coordinate system ka reference point (0,0).

Q13. glVertex2i kya karta hai?
Ans: OpenGL ko integer X aur Y coordinate deta hai.

Q14. gluOrtho2D kya karta hai?
Ans: 2D viewing window set karta hai.

Q15. std::vector ka use?
Ans: Runtime par variable number of polygon vertices store karne ke liye.

F. ONE-MINUTE VIVA SUMMARY
"Sir, this program performs scaling followed by rotation on a 2D polygon.
Each vertex is first multiplied by sx and sy. Then the scaled coordinates
are rotated using sine and cosine equations. The angle is converted from
degrees to radians. The original polygon is drawn in red and the final
transformed polygon in magenta. This practical demonstrates combined 2D
transformations and the effect of transformation order."
*/
