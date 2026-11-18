#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glut.h>

using namespace std;

int edges = 0;
vector<int> pntX, pntY;
int tx = 0, ty = 0;
double sx = 1.0, sy = 1.0;

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

void drawTranslatedScaled()
{
    glColor3f(0.0f, 0.65f, 0.85f);
    glBegin(GL_POLYGON);

    for (int i = 0; i < edges; ++i)
    {
        // Order: first translate, then scale the translated coordinates.
        double translatedX = pntX[i] + tx;
        double translatedY = pntY[i] + ty;

        double newX = translatedX * sx;
        double newY = translatedY * sy;

        glVertex2i(rounded(newX), rounded(newY));
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawOriginal();
    drawTranslatedScaled();
    glFlush();
}

void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    cout << "--- TRANSLATION FOLLOWED BY SCALING ---\n";
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
    cout << "Enter scaling factors (sx sy): ";
    cin >> sx >> sy;

    if (!cin)
    {
        cerr << "Invalid transformation values.\n";
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Translation Followed by Scaling");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 6 - BASIC 2D TRANSFORMATIONS
PROGRAM: TRANSLATION FOLLOWED BY SCALING
===============================================================================

1. AIM
To implement translation followed by scaling on a 2D polygon and display
both the original and transformed polygon using C++, OpenGL and GLUT.

2. OBJECTIVES
1. To understand the meaning of 2D geometric transformations.
2. To accept polygon vertices from the user.
3. To move a polygon using translation factors tx and ty.
4. To change the polygon size using scaling factors sx and sy.
5. To apply the two operations in a specified order.
6. To display the original and transformed polygon in one graphics window.

3. REQUIREMENTS
Software: C++ compiler, OpenGL, GLU and GLUT/FreeGLUT development libraries.
Hardware: Computer with a graphics display.

4. THEORY
A 2D transformation changes the position, size or orientation of an object
on the two-dimensional XY plane. A polygon is represented by an ordered
collection of vertices. Applying the same transformation to every vertex
produces the transformed polygon.

Translation:
Translation moves an object from one position to another without changing
its dimensions or orientation. If the original point is P(x, y) and the
translation factors are tx and ty, then:
    x1 = x + tx
    y1 = y + ty

Scaling:
Scaling changes the size of an object. The scaling factors sx and sy control
the change along the X and Y axes:
    x2 = x1 * sx
    y2 = y1 * sy
If a factor is greater than 1, the object expands in that direction. If it
lies between 0 and 1, the object becomes smaller in that direction. A
negative factor reflects the object as well as scaling it.

Combined transformation used in this program:
The program first translates a vertex and then scales the translated point:
    x' = (x + tx) * sx
    y' = (y + ty) * sy
The order matters. Scaling first and translation second would give:
    x' = x * sx + tx
    y' = y * sy + ty
These results are generally different.

Matrix representation:
Using column-vector notation:
    T = [ 1  0  tx ]
        [ 0  1  ty ]
        [ 0  0   1 ]

    S = [ sx  0  0 ]
        [  0 sy  0 ]
        [  0  0  1 ]

For translation followed by scaling, the combined matrix is S*T.
The homogeneous coordinate 1 allows translation to be represented as
matrix multiplication.

5. ALGORITHM
1. Start.
2. Read the number of polygon vertices.
3. Check that the number is at least three.
4. Read the X and Y coordinates of every vertex.
5. Read translation factors tx and ty.
6. Read scaling factors sx and sy.
7. Initialize GLUT and create a 640 by 480 graphics window.
8. Set a white background and a 2D orthographic projection.
9. Draw the original polygon in red.
10. For each vertex, calculate x1 = x + tx and y1 = y + ty.
11. Calculate x' = x1*sx and y' = y1*sy.
12. Round the final coordinates to integer pixel coordinates.
13. Draw the transformed polygon in cyan.
14. Flush the drawing commands and stop when the window is closed.

6. WORKING OF THE PROGRAM
- The two vectors pntX and pntY store the X and Y coordinates of the polygon.
- edges stores the number of vertices.
- tx and ty store translation distances.
- sx and sy store scaling factors.
- drawOriginal() draws the input polygon in red.
- drawTranslatedScaled() calculates each new vertex and draws the result.
- display() clears the window and calls both drawing functions.
- initialize() sets the background and coordinate system.
- main() accepts input and initializes GLUT.

7. IMPORTANT FUNCTIONS
glBegin(GL_POLYGON): starts polygon vertex specification.
glVertex2i(x,y): supplies an integer 2D vertex.
glEnd(): finishes the polygon.
glColor3f(r,g,b): selects the drawing color.
glClear(): clears the window.
gluOrtho2D(): defines the 2D coordinate viewing area.
glFlush(): forces pending drawing commands to execute.
vector: dynamic array used to store a variable number of vertices.
std::lround(): rounds a floating-point value to the nearest integer.

8. EXECUTION COMMAND
Install libraries on Ubuntu if required:
    sudo apt-get install freeglut3-dev
Compile:
    g++ traSca6.cpp -o traSca6 -lGL -lGLU -lglut
Run:
    ./traSca6

9. SAMPLE INPUT AND OUTPUT
Sample input:
    Number of polygon vertices: 4
    Vertex 1: 100 100
    Vertex 2: 200 100
    Vertex 3: 200 200
    Vertex 4: 100 200
    Translation factors: 20 10
    Scaling factors: 1.5 1.5

Calculated transformed vertices:
    (100+20, 100+10) * (1.5, 1.5) = (180, 165)
    (200+20, 100+10) * (1.5, 1.5) = (330, 165)
    (200+20, 200+10) * (1.5, 1.5) = (330, 315)
    (100+20, 200+10) * (1.5, 1.5) = (180, 315)

Graphics output:
    Original square is shown in red.
    Translated-and-scaled square is shown in cyan.

10. RESULT
The program accepts a polygon and applies translation followed by scaling.
The original and transformed polygons are displayed in the same window.

11. CONCLUSION
Thus, translation and scaling of a 2D polygon were implemented successfully.
The practical demonstrates coordinate-based transformation, transformation
order, rounding and polygon rendering using OpenGL.

12. PRECAUTIONS
- Enter at least three vertices for a polygon.
- Give vertices in boundary order.
- GL_POLYGON is intended for simple convex polygons.
- Keep the transformed coordinates inside the viewing window to see the
  complete polygon.
- The program scales coordinates about the origin (0,0), not about the
  polygon's centre.

===============================================================================
HINGLISH EXPLANATION FOR VIVA
===============================================================================

A. SIMPLE CONCEPT
Is program mein ek polygon input lete hain. Pehle uska original shape red
colour mein draw hota hai. Phir same polygon par do transformations apply
hote hain: pehle translation aur uske baad scaling. Final transformed
polygon cyan colour mein show hota hai.

B. FORMULA KO ACHHE SE SAMAJHO
Maan lo ek vertex (100,100) hai.
Translation factors tx=20 aur ty=10 hain.
Pehle translation:
    x1 = 100+20 = 120
    y1 = 100+10 = 110
Ab scaling factors sx=1.5 aur sy=1.5:
    x' = 120*1.5 = 180
    y' = 110*1.5 = 165
Final point (180,165) hoga.

Important: yahan scaling translated point par ho rahi hai. Isliye formula
(x+tx)*sx hai, na ki x*sx+tx. Transformation order badalne se result badal
sakta hai.

C. CODE EXPLANATION
1. #include <cmath>
   Mathematical functions aur lround ke liye use hota hai.

2. #include <vector>
   Polygon ke vertices store karne ke liye dynamic arrays milte hain.

3. vector<int> pntX, pntY
   X coordinates alag aur Y coordinates alag store hote hain. Agar polygon
   mein 4 vertices hain to dono vectors mein 4 values hongi.

4. rounded()
   Calculation double mein hoti hai. glVertex2i integer coordinates leta
   hai. std::lround floating value ko nearest integer mein round karta hai.

5. drawOriginal()
   GL_POLYGON start karta hai, red colour set karta hai aur loop ke through
   har original vertex OpenGL ko deta hai. glEnd polygon complete karta hai.

6. drawTranslatedScaled()
   Har vertex ke liye pehle translatedX aur translatedY calculate hote hain.
   Uske baad sx aur sy multiply hote hain. Result round karke glVertex2i ko
   diya jata hai. Cyan colour transformed polygon ko identify karta hai.

7. display()
   glClear screen clean karta hai. Original aur transformed dono shapes
   draw hote hain. glFlush drawing commands execute karwata hai.

8. initialize()
   White background set hota hai. gluOrtho2D(0,640,0,480) coordinate
   system define karta hai. Isme x 0 se 640 aur y 0 se 480 tak visible hai.

9. main()
   User se number of vertices, har vertex, translation factors aur scaling
   factors input liye jate hain. Invalid vertex count reject hota hai.
   GLUT window create hoti hai aur display callback register hota hai.

D. MATRIX AUR HOMOGENEOUS COORDINATES
2D transformations ko matrix form mein likhna convenient hota hai.
Translation ko normal 2x2 matrix se represent nahi kar sakte, isliye
homogeneous coordinates use hote hain. Point (x,y) ko (x,y,1) likhte hain.
Isse translation, scaling aur rotation sab 3x3 matrix se represent hote hain.

E. VIVA QUESTIONS WITH ANSWERS
Q1. 2D transformation kya hai?
Ans: XY plane mein kisi object ki position, size ya orientation change
karna 2D transformation kehlata hai.

Q2. Translation kya hai?
Ans: Object ko ek jagah se doosri jagah move karna without changing its
shape and size. Formula x'=x+tx, y'=y+ty.

Q3. Scaling kya hai?
Ans: Object ka size increase ya decrease karna. Formula x'=x*sx,
y'=y*sy.

Q4. sx aur sy kya represent karte hain?
Ans: sx X direction ka scaling factor aur sy Y direction ka scaling factor.

Q5. Uniform scaling kya hai?
Ans: Jab sx aur sy equal hote hain, to dono directions mein same ratio se
size change hota hai. Shape ka proportion preserve rehta hai.

Q6. Non-uniform scaling kya hai?
Ans: Jab sx aur sy different hote hain, to X aur Y direction mein different
amount se scaling hoti hai.

Q7. Transformation order important kyu hai?
Ans: Kyunki generally transformations commutative nahi hote. Translation
ke baad scaling aur scaling ke baad translation alag results de sakte hain.

Q8. Homogeneous coordinates kya hain?
Ans: 2D point ko third coordinate add karke (x,y,1) represent karte hain,
jisse translation ko matrix multiplication se show kar sakte hain.

Q9. std::vector kyu use kiya?
Ans: Polygon ke vertices ki count runtime par user decide karta hai.
Vector automatically required storage manage karta hai.

Q10. GL_POLYGON kya karta hai?
Ans: Given vertices ko connect karke filled polygon draw karta hai. Ye
simple convex polygons ke liye suitable hai.

Q11. glFlush() ka use?
Ans: Pending OpenGL commands ko execute karne ke liye force karta hai.

Q12. Scaling kis point ke about hoti hai?
Ans: Is program mein origin (0,0) ke about scaling hoti hai.

Q13. Agar sx=1 aur sy=1 ho to kya hoga?
Ans: Size change nahi hoga. Sirf translation ka effect dikhega.

Q14. Agar sx=2 aur sy=2 ho to?
Ans: Object dono directions mein double scale hoga.

Q15. Negative scaling factor ka effect?
Ans: Scaling ke saath corresponding axis ke across reflection bhi ho sakta hai.

F. ONE-MINUTE VIVA SUMMARY
"Sir, this program implements translation followed by scaling on a 2D
polygon. The polygon vertices are stored in two vectors. First, each point
is translated using x1=x+tx and y1=y+ty. Then scaling is applied using
x'=x1*sx and y'=y1*sy. The original polygon is drawn in red and the
transformed polygon in cyan using GL_POLYGON. The program demonstrates
basic 2D transformations and the importance of transformation order."
*/
