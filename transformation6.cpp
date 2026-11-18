#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glut.h>

using namespace std;

int edges = 0;
int choice = 0;
vector<int> pntX, pntY;
int transX = 0, transY = 0;
double scaleX = 1.0, scaleY = 1.0;
double angleRad = 0.0;

int rounded(double value)
{
    return static_cast<int>(std::lround(value));
}

void drawPolygon()
{
    glColor3f(1.0f, 0.0f, 0.0f); // Original polygon: red
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; ++i)
        glVertex2i(pntX[i], pntY[i]);
    glEnd();
}

void drawTranslatedPolygon()
{
    glColor3f(0.0f, 0.65f, 0.0f); // Translated polygon: green
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; ++i)
        glVertex2i(pntX[i] + transX, pntY[i] + transY);
    glEnd();
}

void drawScaledPolygon()
{
    glColor3f(0.0f, 0.0f, 1.0f); // Scaled polygon: blue
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; ++i)
        glVertex2i(rounded(pntX[i] * scaleX),
                   rounded(pntY[i] * scaleY));
    glEnd();
}

void drawRotatedPolygon()
{
    glColor3f(1.0f, 0.55f, 0.0f); // Rotated polygon: orange
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; ++i)
    {
        double x = pntX[i];
        double y = pntY[i];
        double newX = x * cos(angleRad) - y * sin(angleRad);
        double newY = x * sin(angleRad) + y * cos(angleRad);
        glVertex2i(rounded(newX), rounded(newY));
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawPolygon();

    if (choice == 1)
        drawTranslatedPolygon();
    else if (choice == 2)
        drawScaledPolygon();
    else if (choice == 3)
        drawRotatedPolygon();

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

    cout << "===== BASIC 2D TRANSFORMATIONS =====\n";
    cout << "1. Translation\n";
    cout << "2. Scaling\n";
    cout << "3. Rotation\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (!cin || choice < 1 || choice > 4)
    {
        cerr << "Invalid menu choice.\n";
        return 1;
    }

    if (choice == 4)
        return 0;

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

    if (choice == 1)
    {
        cout << "Enter translation factors (tx ty): ";
        cin >> transX >> transY;
    }
    else if (choice == 2)
    {
        cout << "Enter scaling factors (sx sy): ";
        cin >> scaleX >> scaleY;
    }
    else if (choice == 3)
    {
        cout << "Enter rotation angle in degrees: ";
        cin >> angleDegrees;
        const double pi = acos(-1.0);
        angleRad = angleDegrees * pi / 180.0;
    }

    if (!cin)
    {
        cerr << "Invalid transformation values.\n";
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Basic 2D Transformations");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 6 - BASIC 2D TRANSFORMATIONS
PROGRAM: MENU-DRIVEN TRANSLATION, SCALING AND ROTATION
===============================================================================

1. AIM
To implement the basic 2D transformations translation, scaling and rotation
on a user-defined polygon and display the original and transformed objects
using C++, OpenGL and GLUT.

2. OBJECTIVES
1. To understand the concept of 2D geometric transformations.
2. To create a polygon using user-entered vertices.
3. To implement translation, scaling and rotation.
4. To use a menu to select the required transformation.
5. To apply the correct mathematical formula for each operation.
6. To display the original and transformed polygon in different colours.

3. REQUIREMENTS
Software: C++ compiler, OpenGL, GLU and GLUT/FreeGLUT development libraries.
Hardware: Computer with a graphics display.

4. THEORY
A transformation changes the coordinates of an object. In 2D computer
graphics, transformations are applied on the XY plane. They are used to
change an object's position, size and orientation. A polygon is represented
by a list of vertices, and the same transformation is applied to each vertex.

A. Translation
Translation moves an object by a fixed distance:
    x' = x + tx
    y' = y + ty
Here tx is the horizontal movement and ty is the vertical movement.
The shape, size and orientation remain unchanged.

B. Scaling
Scaling changes the size of an object:
    x' = x * sx
    y' = y * sy
If sx=sy, uniform scaling takes place. If the factors are different,
non-uniform scaling takes place. Factors greater than one enlarge the
object, while factors between zero and one reduce it. Scaling in this
program is performed about the origin.

C. Rotation
Rotation turns the object around the origin by angle theta:
    x' = x*cos(theta) - y*sin(theta)
    y' = x*sin(theta) + y*cos(theta)
The angle must be converted into radians:
    radians = degrees*pi/180
A positive angle gives counter-clockwise rotation in the standard Cartesian
coordinate system.

Homogeneous coordinates:
A 2D point can be represented as (x,y,1). This allows translation, scaling
and rotation to be written using 3x3 matrices. It is useful when multiple
transformations are combined.

5. ALGORITHM
1. Start.
2. Display the menu: Translation, Scaling, Rotation and Exit.
3. Read the user's choice.
4. If Exit is selected, terminate the program.
5. Read the number of polygon vertices and validate the input.
6. Read all X and Y coordinates.
7. If Translation is selected, read tx and ty.
8. If Scaling is selected, read sx and sy.
9. If Rotation is selected, read the angle and convert it to radians.
10. Initialize GLUT and create a graphics window.
11. Set the background and 2D coordinate viewing region.
12. Draw the original polygon in red.
13. Apply only the selected transformation to every original vertex.
14. Draw the transformed polygon in the selected colour.
15. Flush the drawing commands and stop when the window closes.

6. WORKING OF THE PROGRAM
- choice stores the selected menu option.
- edges stores the number of polygon vertices.
- pntX and pntY store the original coordinates.
- transX and transY store translation values.
- scaleX and scaleY store scaling factors.
- angleRad stores the rotation angle in radians.
- drawPolygon() draws the original polygon in red.
- drawTranslatedPolygon() applies translation and draws green output.
- drawScaledPolygon() applies scaling and draws blue output.
- drawRotatedPolygon() applies rotation and draws orange output.
- display() draws the original shape and calls the selected function.
- initialize() sets the white background and orthographic projection.

7. IMPORTANT OPENGL / C++ FUNCTIONS
glBegin(GL_POLYGON): begins a filled polygon.
glVertex2i(): passes a 2D vertex to OpenGL.
glColor3f(): sets the RGB colour.
glClear(): clears the selected buffer.
gluOrtho2D(): defines a 2D viewing region.
glFlush(): executes pending OpenGL commands.
std::vector: stores a variable number of vertices.
std::lround(): rounds calculated coordinates.
sin()/cos(): are used in the rotation equations.

8. EXECUTION COMMAND
Install required libraries if necessary:
    sudo apt-get install freeglut3-dev
Compile:
    g++ transformation6.cpp -o transformation6 -lGL -lGLU -lglut
Run:
    ./transformation6

9. SAMPLE INPUT AND OUTPUT
Example for Translation:
    Choice: 1
    Number of vertices: 3
    Vertex 1: 100 100
    Vertex 2: 200 100
    Vertex 3: 150 200
    Translation factors: 20 30
Transformed vertices:
    (120,130), (220,130), (170,230)

Example for Scaling:
    Choice: 2
    Number of vertices: 3
    Vertices: (100,100), (200,100), (150,200)
    Scaling factors: 2 2
Transformed vertices:
    (200,200), (400,200), (300,400)

Example for Rotation:
    Choice: 3
    Number of vertices: 3
    Vertices: (100,100), (200,100), (150,200)
    Angle: 90 degrees
Transformed vertices:
    (-100,100), (-100,200), (-200,150)

Graphics output:
    The original polygon is red. Translation output is green, scaling
    output is blue and rotation output is orange.

10. RESULT
The menu-driven program accepts a polygon and performs the selected
transformation: translation, scaling or rotation. The original and
transformed polygons are displayed together.

11. CONCLUSION
Thus, basic 2D transformations were implemented successfully using C++ and
OpenGL. The program demonstrates how translation changes position, scaling
changes size and rotation changes orientation. It also demonstrates menu
selection and coordinate-based graphics programming.

12. PRECAUTIONS
- Select a valid menu option.
- Enter at least three vertices.
- Enter vertices in boundary order.
- sin() and cos() require radians.
- Transformations in this program are performed about the origin.
- Keep the transformed polygon inside the viewing region.
- GL_POLYGON is intended for simple convex polygons.

===============================================================================
HINGLISH EXPLANATION FOR VIVA
===============================================================================

A. PROGRAM KYA KARTA HAI?
Ye menu-driven program hai. User ke paas teen transformations ka option
hai: Translation, Scaling aur Rotation. User pehle option choose karta hai,
phir polygon ke vertices enter karta hai. Uske baad selected transformation
ki values input deta hai. Screen par original polygon red colour mein aur
transformed polygon selected colour mein display hota hai.

B. TEENO TRANSFORMATIONS KO EXAMPLE SE SAMAJHO

1. Translation:
   Original point (100,100), tx=20, ty=30.
   x'=100+20=120
   y'=100+30=130
   New point (120,130).
   Matlab object move hua, lekin size aur shape same rahe.

2. Scaling:
   Original point (100,100), sx=2, sy=2.
   x'=100*2=200
   y'=100*2=200
   New point (200,200).
   Matlab object origin se door scale hua aur size double hua.

3. Rotation:
   Original point (100,100), angle=90 degree.
   x'=100*cos90 - 100*sin90 = -100
   y'=100*sin90 + 100*cos90 = 100
   New point (-100,100).
   Matlab point origin ke around counter-clockwise rotate hua.

C. CODE EXPLANATION
1. Header files:
   cmath maths ke functions ke liye, iostream input/output ke liye, vector
   polygon coordinates store karne ke liye aur glut.h graphics ke liye.

2. Global variables:
   choice user ka menu selection store karta hai. edges vertices ki count
   hai. pntX,pntY coordinates store karte hain. transX,transY translation
   factors hain. scaleX,scaleY scaling factors hain. angleRad radians mein
   rotation angle hai.

3. rounded():
   Rotation aur scaling ke baad decimal coordinate aa sakte hain.
   std::lround nearest integer deta hai, jise glVertex2i use kar sakta hai.

4. drawPolygon():
   Original vertices ko red colour mein draw karta hai.

5. drawTranslatedPolygon():
   Har vertex mein transX aur transY add karta hai. Isse polygon move hota hai.

6. drawScaledPolygon():
   Har X coordinate ko scaleX aur Y coordinate ko scaleY se multiply karta
   hai. Isse size change hota hai.

7. drawRotatedPolygon():
   Rotation equations apply karta hai. cos aur sin se new X,Y values
   calculate hoti hain.

8. display():
   Screen clear karta hai aur original polygon draw karta hai. Choice ke
   according ek transformation function call hota hai.

9. initialize():
   White background set karta hai. gluOrtho2D negative aur positive
   coordinate range define karta hai, taki rotated points visible ho saken.

10. main():
    Menu print hota hai. Choice 4 par program exit hota hai. Other choices
    par polygon input hota hai. Selected transformation ke parameters read
    hote hain. GLUT window create hoti hai aur display callback register hota hai.

D. IMPORTANT CONCEPTS
- Uniform scaling: sx aur sy same.
- Non-uniform scaling: sx aur sy different.
- Positive rotation: counter-clockwise.
- Negative rotation: clockwise.
- Origin: coordinate system ka point (0,0).
- Homogeneous coordinates: (x,y) ko (x,y,1) form mein represent karna.
- Transformation matrix: coordinate transformation ko matrix multiplication
  se represent karna.
- Composite transformation: do ya zyada transformations ko sequence mein
  apply karna.

E. VIVA QUESTIONS WITH ANSWERS
Q1. 2D transformation kya hai?
Ans: 2D plane mein object ki position, size ya orientation change karna.

Q2. Basic 2D transformations kaunse hain?
Ans: Translation, scaling aur rotation. Shearing aur reflection bhi
common 2D transformations hain.

Q3. Translation formula?
Ans: x'=x+tx, y'=y+ty.

Q4. Scaling formula?
Ans: x'=x*sx, y'=y*sy.

Q5. Rotation formula?
Ans: x'=x cos(theta)-y sin(theta), y'=x sin(theta)+y cos(theta).

Q6. Uniform scaling kya hai?
Ans: Jab sx aur sy equal hote hain.

Q7. Non-uniform scaling kya hai?
Ans: Jab sx aur sy alag hote hain.

Q8. Homogeneous coordinates kya hain?
Ans: 2D point ko third coordinate ke saath (x,y,1) represent karna.

Q9. Rotation mein radians kyu use karte hain?
Ans: C++ ke trigonometric functions radians expect karte hain.

Q10. Which transformation changes only position?
Ans: Translation.

Q11. Which transformation changes size?
Ans: Scaling.

Q12. Which transformation changes orientation?
Ans: Rotation.

Q13. Scaling kis point ke about hoti hai?
Ans: Is program mein origin (0,0) ke about.

Q14. Rotation kis point ke about hoti hai?
Ans: Is program mein origin (0,0) ke about.

Q15. Multiple transformations combine kar sakte hain?
Ans: Haan. Lekin order important hota hai because matrix multiplication
generally commutative nahi hoti.

Q16. GL_POLYGON ka use?
Ans: Filled polygon draw karne ke liye.

Q17. Vector ka benefit?
Ans: Runtime par vertices ki count ke according storage manage hota hai.

Q18. glFlush() kya karta hai?
Ans: Pending OpenGL commands ko execute karwata hai.

F. ONE-MINUTE VIVA SUMMARY
"Sir, this is a menu-driven 2D transformation program. It accepts polygon
vertices and lets the user select translation, scaling or rotation.
Translation uses x'=x+tx and y'=y+ty. Scaling uses x'=x*sx and y'=y*sy.
Rotation uses sine and cosine equations after converting degrees into
radians. The original polygon is drawn in red, while the transformed
polygon is drawn in a different colour. All transformations are performed
about the origin."
*/
