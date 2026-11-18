#include <GL/glut.h>
#include <iostream>
using namespace std;

const int WINDOW_SIZE = 500;
const unsigned char fillColor[3] = {0, 255, 0};       // Green
const unsigned char boundaryColor[3] = {255, 0, 0};   // Red

// 4-connected Boundary Fill Algorithm
void boundaryFill4(int x, int y)
{
    if (x < 0 || x >= WINDOW_SIZE || y < 0 || y >= WINDOW_SIZE)
        return;

    unsigned char color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);

    bool isBoundary = color[0] == boundaryColor[0] &&
                      color[1] == boundaryColor[1] &&
                      color[2] == boundaryColor[2];

    bool isFilled = color[0] == fillColor[0] &&
                    color[1] == fillColor[1] &&
                    color[2] == fillColor[2];

    if (!isBoundary && !isFilled)
    {
        glColor3ub(fillColor[0], fillColor[1], fillColor[2]);
        glBegin(GL_POINTS);
            glVertex2i(x, y);
        glEnd();
        glFlush();

        boundaryFill4(x + 1, y);
        boundaryFill4(x - 1, y);
        boundaryFill4(x, y + 1);
        boundaryFill4(x, y - 1);
    }
}

void drawPolygon()
{
    glLineWidth(3.0f);
    glColor3ub(255, 0, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2i(150, 100);
        glVertex2i(300, 300);
        glVertex2i(450, 100);
    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPolygon();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        y = WINDOW_SIZE - 1 - y; // Convert mouse coordinates to OpenGL coordinates
        boundaryFill4(x, y);
    }
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_SIZE, 0.0, WINDOW_SIZE);
}

int main(int argc, char** argv)
{
    cout << "Boundary Fill Algorithm\n";
    cout << "Click inside the red triangle to fill it with green.\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Boundary Fill Algorithm");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL: BOUNDARY FILL ALGORITHM (4-CONNECTED)
===============================================================================

1. AIM
To implement the 4-connected Boundary Fill Algorithm using C++ and OpenGL/GLUT for filling the interior of a closed graphical object with a selected colour, and to understand the working of region-filling techniques used in computer graphics.

2. OBJECTIVES
- To understand the concept of polygon and region filling.
- To draw a closed graphical object and select an interior seed point.
- To identify the boundary colour and the fill colour.
- To read the colour of a screen pixel using glReadPixels().
- To fill connected interior pixels until the specified boundary colour is reached.
- To understand 4-connected pixel neighbourhood and recursive filling.

3. ASSOCIATED TASKS
1. Draw a closed polygon or enclosed graphical object using graphics functions.
2. Implement Boundary Fill to fill the selected region with a specified colour.
3. Start filling from a point located inside the polygon.
4. Check the colour of each pixel and stop when the boundary colour is found.
5. Understand the difference between 4-connected and 8-connected filling.

4. SOFTWARE AND HARDWARE REQUIREMENTS
Software:
- C++ compiler such as GCC/G++ or MinGW.
- OpenGL, GLU and GLUT/freeglut libraries.
- Windows or Linux operating system with graphics support.
Hardware:
- Computer with display, keyboard and mouse.

5. THEORY
A polygon is a closed two-dimensional geometrical figure formed by joining three or more straight line segments. The points where the segments meet are called vertices and the segments are called edges. A convex polygon has all interior angles less than 180 degrees, whereas a concave polygon has at least one interior angle greater than 180 degrees.

Region filling is the process of colouring the pixels inside a closed graphical object. Boundary Fill is a region-filling algorithm that begins at an interior point called the seed point and spreads outward. It checks the colour of the current pixel. If the pixel is neither the boundary colour nor the fill colour, it is changed to the fill colour and the algorithm is applied to its neighbours. The process stops when a boundary-coloured or already-filled pixel is reached.

In this program, a triangle is drawn with a red boundary on a black background. Green is used as the fill colour. A left mouse click inside the triangle provides the seed point. The program uses glReadPixels() to read the RGB colour of the selected pixel. The pixel is filled only if it is neither red nor green.

4-CONNECTED AND 8-CONNECTED NEIGHBOURHOOD
In 4-connected filling, only four direct neighbours are checked:
- Left: (x - 1, y)
- Right: (x + 1, y)
- Above: (x, y + 1)
- Below: (x, y - 1)

In 8-connected filling, the above four neighbours and four diagonal neighbours are checked:
- (x - 1, y - 1), (x + 1, y - 1)
- (x - 1, y + 1), (x + 1, y + 1)

This program uses only four neighbours; therefore, it implements 4-connected Boundary Fill.

EXEMPLARS AND UTILITY
Boundary Fill and other region-filling methods are useful in image-editing and paint applications, object colouring in games and animation, map colouring, GIS systems, GUI design, icon design and visualisation systems.

6. ALGORITHM
1. Start the program and initialize GLUT.
2. Create a 500 x 500 graphics window.
3. Set the background colour to black.
4. Set the 2D coordinate system using gluOrtho2D().
5. Draw a closed triangle using a red line loop.
6. Wait for a left mouse-button click.
7. Convert the mouse y-coordinate into the OpenGL coordinate system.
8. Use the clicked pixel as the seed point and check that it is inside the screen.
9. Read the pixel colour using glReadPixels().
10. If the pixel is neither red (boundary) nor green (already filled):
    a. Set the pixel colour to green.
    b. Plot the pixel.
    c. Recursively process its right, left, top and bottom neighbours.
11. Stop in each direction when a boundary pixel, filled pixel or screen limit is reached.
12. End.

7. PROCEDURE
1. Open a C++ IDE or terminal configured with OpenGL and GLUT.
2. Save the program using the required .cpp filename.
3. Compile it with the OpenGL, GLU and GLUT/freeglut libraries.
4. Run the executable.
5. A black graphics window containing a red triangle appears.
6. Click at a point inside the triangle.
7. Observe that the connected interior region is filled with green.
8. The red boundary remains visible.

8. IMPORTANT OPENGL FUNCTIONS
- glReadPixels(): reads the colour value of pixels from the frame buffer. Here it reads one RGB pixel.
- glColor3ub(): sets the current colour using unsigned-byte red, green and blue values in the range 0 to 255.
- glBegin(GL_POINTS) and glEnd(): define a group of points to be drawn.
- glVertex2i(x, y): specifies the integer coordinate of a point.
- GL_LINE_LOOP: draws connected edges and automatically joins the last vertex to the first, forming a closed outline.
- glutMouseFunc(): registers the mouse callback function.
- glFlush(): asks OpenGL to execute the drawing commands.
- gluOrtho2D(): defines the two-dimensional viewing coordinate system.

9. SAMPLE OUTPUT
Console:
Boundary Fill Algorithm
Click inside the red triangle to fill it with green.

Graphics output:
- Background: Black
- Triangle boundary: Red
- After clicking inside the triangle: the connected interior is Green
The exact number of filled pixels depends on the clicked seed position and the rasterised boundary.

10. RESULT
The 4-connected Boundary Fill Algorithm was implemented using C++ and OpenGL. When a seed point inside the closed red triangle is selected, the connected interior pixels are filled with green while filling stops at the red boundary.

11. CONCLUSION
Thus, the 4-connected Boundary Fill Algorithm was studied and executed successfully. The practical demonstrates polygon region filling, seed-point selection, pixel-colour checking using glReadPixels(), and recursive processing of four neighbouring pixels. It also explains how the fill colour and boundary colour control the filling process.

11. PRECAUTIONS / LIMITATIONS
- The polygon boundary must be completely closed. A gap can allow filling to
  escape outside the polygon.
- The seed point must be inside the intended region.
- Boundary and fill colours must be distinguishable and match the actual pixel
  colours. Anti-aliasing or colour blending can affect exact colour comparison.
- Recursive filling can use a large amount of call stack for large regions.
  An iterative stack/queue implementation is safer for very large images.
- This program uses 4-connectivity; diagonal pixels are not treated as direct
  neighbours.

12. VIVA-VOCE: HINGLISH EXPLANATION AND QUESTIONS
Q1. Boundary Fill Algorithm kya hota hai?
Ans. Boundary Fill ek region filling technique hai. Isme hum polygon ke andar
ek seed point select karte hain. Algorithm us pixel ka colour check karta hai.
Agar pixel boundary colour ka nahi hai aur pehle se fill colour ka bhi nahi hai,
toh us pixel ko fill colour se paint kar deta hai. Phir uske neighbours ko bhi
same tarike se check karta hai. Ye process boundary tak pahunchne par rukta hai.

Q2. Seed point kya hota hai?
Ans. Seed point woh starting pixel hota hai jahan se filling start hoti hai.
Is practical mein mouse se triangle ke andar click karna seed point select
karta hai.

Q3. 4-connected aur 8-connected mein difference?
Ans. 4-connected mein sirf left, right, top aur bottom neighbours check hote
hain. 8-connected mein in chaar ke saath chaar diagonal neighbours bhi check
hote hain. Is code mein 4-connected method use hua hai.

Q4. Boundary colour aur fill colour kya hain?
Ans. Boundary colour polygon ki outline ka colour hota hai. Yahan red hai.
Fill colour interior mein bharne wala colour hai. Yahan green hai.

Q5. glReadPixels() ka use kya hai?
Ans. Ye OpenGL framebuffer se selected pixel ka colour read karta hai. Code
RGB format mein ek pixel ka data read karke decide karta hai ki pixel fill
karna hai ya nahi.

Q6. Fill colour ko dobara check kyun karte hain?
Ans. Jo pixel already green ho chuka hai usko dobara process nahi karna chahiye.
Agar ye condition na ho toh recursion baar-baar same pixels par ja sakti hai
aur program hang ya stack overflow ho sakta hai.

Q7. Boundary Fill aur Flood Fill mein difference?
Ans. Boundary Fill boundary colour ko stopping condition banata hai. Flood Fill
starting pixel ke old/interior colour ko target karta hai aur us colour ke
connected pixels ko replace karta hai.

Q8. Is algorithm ki limitation kya hai?
Ans. Large area mein recursion bahut deep ho sakti hai aur stack overflow ho
sakta hai. Boundary mein gap ho toh fill region ke bahar bhi ja sakta hai.

Q9. GLUT ka full form kya hai?
Ans. OpenGL Utility Toolkit. Ye window create karne, mouse/keyboard input aur
display callbacks handle karne mein help karta hai.

Q10. glFlush() kya karta hai?
Ans. Ye OpenGL commands ko execute/display pipeline mein bhejta hai, taaki
draw kiye gaye pixels screen par update ho saken.

Q11. glBegin(GL_POINTS) kyun use kiya hai?
Ans. Kyunki algorithm ek-ek pixel position draw karta hai. GL_POINTS har
specified vertex par ek point render karta hai.

Q12. Mouse y-coordinate reverse kyun karte hain?
Ans. GLUT mouse coordinates generally window ke top-left se count hote hain,
jabki gluOrtho2D setup mein hum bottom-left origin use kar rahe hain. Isliye
y ko WINDOW_SIZE - 1 - y se convert karte hain.

Q13. Algorithm kab rukta hai?
Ans. Jab pixel boundary colour ka ho, already fill colour ka ho, ya screen
limits ke bahar ho, tab us direction ki recursion stop ho jaati hai.

Q14. Is practical ka final output kya hai?
Ans. Black background par red triangle show hota hai. Triangle ke andar click
karne par uska connected interior green colour se fill hota hai.
===============================================================================


13. DETAILED HINGLISH EXPLANATION (THEORY + CODE WALKTHROUGH)

A. Sabse pehle concept samjho
Boundary Fill ka simple meaning hai: kisi closed shape ke andar ka area ek
colour se bharna. Jaise drawing app mein bucket se kisi band shape ko colour
karte hain. Is algorithm ko ek starting pixel chahiye, jise seed point kehte
hain. User triangle ke andar mouse se click karta hai; wahi seed point ban
jata hai. Algorithm us pixel ka current RGB colour read karta hai. Agar pixel
red boundary nahi hai aur green se pehle fill bhi nahi hua hai, toh usko green
kar deta hai. Uske baad same rule uske neighbours par apply hota hai.

B. Is code mein exact colours aur shape
Window 500 x 500 pixels ki hai. Background black (RGB 0,0,0), triangle ki
boundary red (255,0,0), aur fill colour green (0,255,0) hai. drawPolygon()
function GL_LINE_LOOP use karke teen vertices (150,100), (300,300) aur
(450,100) ko join karta hai. GL_LINE_LOOP last vertex ko first vertex se bhi
join karta hai, isliye triangle closed rehta hai. Closed boundary important
hai; agar line mein gap ho toh fill shape ke bahar nikal sakta hai.

C. boundaryFill4() ko line-by-line samjho
1) Function ko x,y milte hain. Ye current pixel ka coordinate hai.
2) Pehle bounds check hota hai: x ya y 0 se chhota ho, ya 499 se bada/equal
   ho, toh return. Isse screen ke bahar pixel read nahi hota.
3) glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,color) framebuffer se ek
   pixel ke red, green, blue components read karta hai. 1,1 ka matlab sirf
   ek pixel read karna hai.
4) isBoundary condition compare karti hai ki pixel ka RGB exactly red ke
   equal hai ya nahi. isFilled condition check karti hai ki pixel already
   green toh nahi hai.
5) if (!isBoundary && !isFilled) ka matlab: sirf tab fill karo jab pixel
   boundary bhi na ho aur already filled bhi na ho. Dono conditions bahut
   important hain. Boundary condition shape ko rokhti hai; filled condition
   recursion ko same pixel par baar-baar jaane se rokhti hai.
6) glColor3ub() current drawing colour green set karta hai. GL_POINTS aur
   glVertex2i(x,y) us coordinate par ek point/pixel draw karte hain.
7) glFlush() pending OpenGL drawing commands ko execute karne ke liye bhejta
   hai, taaki next glReadPixels() ko updated framebuffer colour mil sake.
8) Last mein x+1, x-1, y+1, y-1 wale neighbours call hote hain. Ye right,
   left, up aur down directions hain. Isliye algorithm 4-connected hai.

D. 4-connected ka visual meaning
Agar current pixel ko centre samjho, toh direct neighbours cross (+) shape
mein hote hain: left, right, top, bottom. Diagonal corners check nahi hote.
8-connected algorithm mein in chaar ke saath top-left, top-right, bottom-left
and bottom-right bhi check hote. Exam mein yaad rakho: 4-connected = 4 direct
neighbours; 8-connected = 4 direct + 4 diagonal = total 8.

E. Mouse coordinate conversion kyun?
GLUT mouse callback ka y coordinate window ke top se count hota hai, jabki
is program ka OpenGL 2D coordinate setup bottom-left origin use karta hai.
Isliye y = WINDOW_SIZE - 1 - y kiya gaya hai. X coordinate same rehta hai.
Agar conversion na ho toh click ki vertical position ulat ho sakti hai.

F. Algorithm ka flow ek example se
Maan lo user triangle ke andar (300,150) par click karta hai. Mouse callback
coordinate convert karke boundaryFill4(300,...) call karta hai. Pixel black
hai, red nahi aur green nahi, isliye green draw hota hai. Phir uske chaar
neighbours check hote hain. Har valid black interior pixel green hota jata
hai. Jaise hi recursion red outline tak pahunchti hai, isBoundary true hota
hai aur us direction mein aage call nahi hota. Isi tarah poora connected
interior green ho jata hai.

G. Functions ka role
- main(): console instruction deta hai, GLUT initialize karta hai, window
  banata hai aur callbacks register karta hai.
- init(): black clear colour aur 2D projection set karta hai.
- display(): screen clear karke triangle draw karta hai.
- mouse(): left click detect karke y coordinate convert karta hai aur seed
  point se filling start karta hai.
- drawPolygon(): red closed triangle banata hai.
- boundaryFill4(): actual recursive filling karta hai.

H. Important exam points
Boundary Fill boundary colour ko stopping rule banata hai; Flood Fill old
interior colour ko target karta hai. Is code mein fill ek seed point se start
hota hai. Boundary ideally continuous/closed honi chahiye aur boundary/fill
colours clearly different hone chahiye. Recursion easy to understand hai,
lekin huge region mein stack overflow ho sakta hai. Large image ke liye
iterative stack/queue version better ho sakta hai.

14. COMPILATION AND EXECUTION
Linux (FreeGLUT installed):
 g++ boundary4.cpp -o boundary4 -lglut -lGLU -lGL
 ./boundary4
Windows MinGW (FreeGLUT configured):
 g++ boundary4.cpp -o boundary4.exe -lfreeglut -lopengl32 -lglu32
 boundary4.exe

15. SAMPLE INPUT / OUTPUT
Input: No keyboard input. User clicks inside the red triangle.
Console: Boundary Fill Algorithm; Click inside the red triangle to fill it
with green.
Graphics: Black window, red triangle; after an interior click, its connected
interior becomes green.

*/
