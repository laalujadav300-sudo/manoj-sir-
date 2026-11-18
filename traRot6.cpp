#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glut.h>

using namespace std;

int edges = 0;
vector<int> pntX, pntY;
int tx = 0, ty = 0;
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

void drawTranslatedRotated()
{
    glColor3f(1.0f, 0.65f, 0.0f);
    glBegin(GL_POLYGON);

    for (int i = 0; i < edges; ++i)
    {
        // Order: translate first, then rotate about the origin.
        double x1 = pntX[i] + tx;
        double y1 = pntY[i] + ty;

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
    drawTranslatedRotated();
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

    cout << "--- TRANSLATION FOLLOWED BY ROTATION ---\n";
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

    cout << "Enter translation factors (tx ty): ";
    cin >> tx >> ty;
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
    glutCreateWindow("Translation Followed by Rotation");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 6 - BASIC 2D TRANSFORMATIONS
PROGRAM: TRANSLATION FOLLOWED BY ROTATION
===============================================================================

1. AIM
To implement translation followed by rotation on a 2D polygon using
C++, OpenGL and GLUT, and display the original and transformed polygon.

2. OBJECTIVES
1. To understand translation and rotation in computer graphics.
2. To accept a polygon and transformation parameters from the user.
3. To move every vertex by translation factors tx and ty.
4. To rotate the translated vertices through a given angle.
5. To convert the angle from degrees to radians.
6. To compare the original and transformed polygon graphically.

3. REQUIREMENTS
Software: C++ compiler, OpenGL, GLU and GLUT/FreeGLUT development libraries.
Hardware: Computer with graphics display.

4. THEORY
A 2D transformation changes the coordinates of an object in the XY plane.
A polygon consists of vertices connected in sequence. The same operation
is applied to every vertex so that the polygon is transformed as a whole.

Translation:
For a point (x,y), translation by (tx,ty) gives:
    x1 = x + tx
    y1 = y + ty
It changes only the position of the object.

Rotation:
Rotation turns a point around the origin by an angle theta. For
counter-clockwise rotation in the standard Cartesian coordinate system:
    x2 = x1*cos(theta) - y1*sin(theta)
    y2 = x1*sin(theta) + y1*cos(theta)
The angle used by sin() and cos() in C++ is measured in radians.
Conversion:
    radians = degrees * pi / 180

Combined transformation in this program:
First translate, then rotate:
    x' = (x+tx)*cos(theta) - (y+ty)*sin(theta)
    y' = (x+tx)*sin(theta) + (y+ty)*cos(theta)
The rotation is performed about the origin after translation. The order
is important because translation followed by rotation is not generally
equal to rotation followed by translation.

Matrix representation:
Translation matrix:
    T = [ 1  0  tx ]
        [ 0  1  ty ]
        [ 0  0   1 ]

Counter-clockwise rotation matrix:
    R = [ cos(theta)  -sin(theta)  0 ]
        [ sin(theta)   cos(theta)  0 ]
        [     0            0      1 ]

Using column vectors, the combined operation is R*T.

5. ALGORITHM
1. Start.
2. Read the number of polygon vertices.
3. Validate that at least three vertices are entered.
4. Read all vertex coordinates.
5. Read translation factors tx and ty.
6. Read the rotation angle in degrees.
7. Convert degrees into radians using pi/180.
8. Initialize GLUT and create a graphics window.
9. Set the white background and 2D viewing coordinates.
10. Draw the original polygon in red.
11. For each vertex, calculate x1=x+tx and y1=y+ty.
12. Calculate the rotated coordinates using sine and cosine.
13. Round the results and draw the transformed polygon.
14. Flush the drawing commands.
15. Stop when the graphics window is closed.

6. WORKING OF THE PROGRAM
- pntX and pntY store polygon coordinates.
- tx and ty store translation distances.
- angleRad stores the rotation angle in radians.
- rounded() converts a calculated coordinate into an integer.
- drawOriginal() displays the input polygon in red.
- drawTranslatedRotated() first translates each point and then rotates it.
- display() draws both polygons.
- initialize() sets the background and coordinate system.
- main() reads inputs and registers the GLUT display callback.

7. IMPORTANT FUNCTIONS
sin() and cos(): calculate trigonometric values for the angle in radians.
acos(-1.0): obtains an accurate value of pi.
std::lround(): rounds a floating-point coordinate.
gluOrtho2D(): sets the 2D coordinate viewing region.
glVertex2i(): supplies a 2D integer vertex.
glBegin(GL_POLYGON)/glEnd(): draw the filled polygon.
glFlush(): executes pending drawing commands.

8. EXECUTION COMMAND
Install required library if necessary:
    sudo apt-get install freeglut3-dev
Compile:
    g++ traRot6.cpp -o traRot6 -lGL -lGLU -lglut
Run:
    ./traRot6

9. SAMPLE INPUT AND OUTPUT
Sample input:
    Number of vertices: 3
    Vertex 1: 100 100
    Vertex 2: 200 100
    Vertex 3: 150 200
    Translation factors: 20 10
    Rotation angle: 90

For a 90-degree counter-clockwise rotation:
    (x1,y1) becomes (-y1,x1)

Transformed vertices:
    (120,110) -> (-110,120)
    (220,110) -> (-110,220)
    (170,210) -> (-210,170)

Graphics output:
    Original triangle is displayed in red.
    Translated-and-rotated triangle is displayed in orange.

10. RESULT
The polygon is translated by the given X and Y distances and then rotated
by the specified angle. Both shapes are displayed in the graphics window.

11. CONCLUSION
Thus, translation followed by rotation was implemented successfully.
The practical demonstrates coordinate transformation, degree-to-radian
conversion, trigonometric calculations and polygon rendering.

12. PRECAUTIONS
- Enter at least three vertices.
- Enter polygon vertices in boundary order.
- sin() and cos() require radians, not degrees.
- This program rotates about the origin after translation.
- The viewing region includes negative coordinates so the rotated object
  can be seen when it moves to the negative side.
- GL_POLYGON is suitable for simple convex polygons.

===============================================================================
HINGLISH EXPLANATION FOR VIVA
===============================================================================

A. PROGRAM KA IDEA
Is practical mein user ek polygon ke vertices enter karta hai. Program
pehle original polygon draw karta hai. Phir polygon ko tx,ty se translate
karta hai aur translated coordinates ko theta angle se rotate karta hai.
Original polygon red aur transformed polygon orange colour mein dikhta hai.

B. FORMULA WITH EXAMPLE
Maan lo point (100,100) hai aur translation (20,10) hai.
Translation ke baad:
    x1=100+20=120
    y1=100+10=110

Ab 90 degree counter-clockwise rotation:
    x'=x1*cos90 - y1*sin90
    y'=x1*sin90 + y1*cos90
Since cos90=0 and sin90=1:
    x' = -110
    y' = 120
Final point (-110,120) hai.

C. CODE EXPLANATION
1. Header files:
   cmath sin, cos, acos aur lround ke liye hai.
   iostream input-output ke liye hai.
   vector vertices store karta hai.
   GL/glut.h OpenGL aur GLUT functions provide karta hai.

2. Global variables:
   edges polygon ke vertices ki count hai.
   pntX aur pntY X,Y coordinates store karte hain.
   tx,ty translation values hain.
   angleRad angle ko radians mein store karta hai.

3. rounded():
   Rotation ke baad coordinates decimal ho sakte hain. OpenGL ka glVertex2i
   integer values leta hai. Isliye lround se nearest integer banate hain.

4. drawOriginal():
   Red colour set karke saare original vertices GL_POLYGON ko pass karta hai.

5. drawTranslatedRotated():
   Har vertex ke liye pehle x1=x+tx aur y1=y+ty nikalta hai.
   Fir rotation formula apply karta hai. Result rounded karke draw hota hai.

6. Angle conversion:
   User angle degrees mein deta hai. C++ ke sin() aur cos() radians expect
   karte hain. Isliye angleRad = degrees*pi/180 calculate hota hai.
   acos(-1.0) se pi milta hai.

7. display():
   Screen clear hoti hai, original aur transformed polygon draw hote hain.

8. initialize():
   White background aur orthographic 2D projection set hota hai.
   Is program mein negative coordinate range bhi rakhi hai because rotation
   ke baad point negative X ya Y mein ja sakta hai.

9. main():
   User input read hota hai, vertex count validate hoti hai, radians
   calculate hote hain aur GLUT window create hoti hai.

D. ROTATION KE IMPORTANT POINTS
- Positive angle standard Cartesian coordinates mein counter-clockwise
  rotation deta hai.
- Negative angle clockwise rotation deta hai.
- 90 degree rotation: (x,y) -> (-y,x).
- 180 degree rotation: (x,y) -> (-x,-y).
- 360 degree rotation ke baad point original position par aa jata hai.
- Is code mein rotation origin ke about hoti hai, arbitrary pivot ke about
  nahi.

E. VIVA QUESTIONS WITH ANSWERS
Q1. Rotation kya hai?
Ans: Object ko ek fixed point, yahan origin, ke around angle theta se turn
karna rotation kehlata hai.

Q2. Translation kya hai?
Ans: Object ke har point mein tx aur ty add karke usko move karna.

Q3. Rotation formula kya hai?
Ans: x'=x cos(theta)-y sin(theta), y'=x sin(theta)+y cos(theta).

Q4. Degree ko radians mein kyu convert karte hain?
Ans: C++ ke sin() aur cos() functions angle radians mein accept karte hain.

Q5. Radians ka formula?
Ans: radians = degrees*pi/180.

Q6. 90 degree rotation ka result?
Ans: Standard Cartesian plane mein (x,y) point (-y,x) ban jata hai.

Q7. Positive rotation direction?
Ans: Standard Cartesian coordinate system mein positive angle counter-
clockwise hota hai.

Q8. Rotation origin ke about kyu hoti hai?
Ans: Formula directly origin-centred rotation define karta hai. Arbitrary
pivot ke liye pehle pivot ko origin par translate, rotate aur wapas
translate karna padta hai.

Q9. Transformation order important hai?
Ans: Haan. Translate then rotate aur rotate then translate generally
different output dete hain.

Q10. cos() aur sin() ka use?
Ans: Ye rotation ke naye X aur Y coordinates calculate karne ke liye hain.

Q11. std::vector kya hai?
Ans: Dynamic array jo runtime par vertices ki count ke according grow kar
sakta hai.

Q12. GL_POLYGON ka use?
Ans: Filled polygon draw karne ke liye.

Q13. gluOrtho2D() kya karta hai?
Ans: 2D viewing coordinates define karta hai.

Q14. Negative angle ka meaning?
Ans: Standard Cartesian system mein clockwise rotation.

Q15. Arbitrary pivot rotation kaise karenge?
Ans: Object ko pivot ke negative translation se origin par laayenge, rotate
karenge aur positive translation se wapas le jayenge.

F. ONE-MINUTE VIVA SUMMARY
"Sir, this program performs translation followed by rotation on a 2D
polygon. Each vertex is first translated using x1=x+tx and y1=y+ty.
Then the rotation equations are applied using sine and cosine. The input
angle is converted from degrees to radians because C++ trigonometric
functions use radians. The original polygon is red and the transformed
polygon is orange. The program demonstrates combined 2D transformations."
*/
