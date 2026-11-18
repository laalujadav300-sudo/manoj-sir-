#include <GL/glut.h>
#include <iostream>
#include <stack>
using namespace std;

int algoChoice;
const int W=500, H=500;
const unsigned char boundaryColor[3]={255,0,0}; // Red
const unsigned char boundaryFillColor[3]={0,255,0}; // Green
const unsigned char floodOldColor[3]={0,0,0}; // Black interior
const unsigned char floodNewColor[3]={0,0,255}; // Blue

bool sameColor(const unsigned char a[3], const unsigned char b[3])
{
    return a[0]==b[0] && a[1]==b[1] && a[2]==b[2];
}

// Iterative 4-connected boundary fill. Stack avoids deep recursive calls.
void boundaryFill4(int sx, int sy)
{
    stack<pair<int,int>> pixels;
    pixels.push({sx,sy});
    while (!pixels.empty()) {
        auto p=pixels.top(); pixels.pop();
        int x=p.first, y=p.second;
        if (x<0 || x>=W || y<0 || y>=H) continue;

        unsigned char color[3];
        glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,color);
        if (!sameColor(color,boundaryColor) &&
            !sameColor(color,boundaryFillColor)) {
            glColor3ub(boundaryFillColor[0],boundaryFillColor[1],boundaryFillColor[2]);
            glBegin(GL_POINTS); glVertex2i(x,y); glEnd();
            pixels.push({x+1,y}); pixels.push({x-1,y});
            pixels.push({x,y+1}); pixels.push({x,y-1});
        }
    }
    glFlush();
}

// Iterative 4-connected flood fill: replaces the target interior color.
void floodFill4(int sx, int sy)
{
    stack<pair<int,int>> pixels;
    pixels.push({sx,sy});
    while (!pixels.empty()) {
        auto p=pixels.top(); pixels.pop();
        int x=p.first, y=p.second;
        if (x<0 || x>=W || y<0 || y>=H) continue;

        unsigned char color[3];
        glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,color);
        if (sameColor(color,floodOldColor)) {
            glColor3ub(floodNewColor[0],floodNewColor[1],floodNewColor[2]);
            glBegin(GL_POINTS); glVertex2i(x,y); glEnd();
            pixels.push({x+1,y}); pixels.push({x-1,y});
            pixels.push({x,y+1}); pixels.push({x,y-1});
        }
    }
    glFlush();
}

void drawPolygon()
{
    glLineWidth(3.0f);
    if (algoChoice==1) glColor3f(1.0f,0.0f,0.0f);
    else glColor3f(1.0f,1.0f,1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2i(150,100);
    glVertex2i(300,300);
    glVertex2i(450,100);
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
    // GLUT mouse coordinates start at top-left; OpenGL uses bottom-left.
    y=H-1-y;
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
        if (algoChoice==1) boundaryFill4(x,y);
        else floodFill4(x,y);
    }
}

void init()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    gluOrtho2D(0.0,500.0,0.0,500.0);
}

int main(int argc, char** argv)
{
    cout << "Select Algorithm:\n1. Boundary Fill\n2. Flood Fill\nEnter Choice: ";
    cin >> algoChoice;
    if (!cin || (algoChoice!=1 && algoChoice!=2)) {
        cerr << "Invalid choice. Enter 1 or 2.\n";
        return 1;
    }

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(W,H);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Boundary Fill and Flood Fill");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 4 : FLOOD FILL AND BOUNDARY FILL ALGORITHMS
===============================================================================

AIM:
To implement Flood Fill and Boundary Fill algorithms using C++ and OpenGL
to fill the interior of a closed polygon with a selected colour.

OBJECTIVES:
1. To understand region-filling techniques.
2. To draw a closed polygon.
3. To implement 4-connected Boundary Fill.
4. To implement 4-connected Flood Fill.
5. To perform filling on a mouse click inside the polygon.

THEORY:
Region filling means assigning a colour to the pixels inside a closed
graphical boundary. It is commonly used in paint applications, diagrams
and computer graphics.

BOUNDARY FILL:
Boundary Fill starts from a seed pixel and spreads to neighbouring pixels
until it reaches the specified boundary colour. In this program the
boundary is red and the fill colour is green. A pixel is filled only if it
is neither the boundary colour nor the already-filled colour.

FLOOD FILL:
Flood Fill starts from a seed pixel and replaces connected pixels having
the same old/interior colour with a new colour. Here the interior/background
target is black and the new fill colour is blue. It stops when a pixel is
not the old colour, so the white polygon boundary blocks the fill.

4-CONNECTIVITY:
A pixel has four direct neighbours: right (x+1,y), left (x-1,y),
up (x,y+1) and down (x,y-1). This is called 4-connected filling.
An 8-connected method would also include the four diagonal neighbours.

ALGORITHM - BOUNDARY FILL:
1. Start with a seed point selected by the mouse.
2. Read the current pixel colour using glReadPixels().
3. If the point is outside the window, stop.
4. If its colour is not the boundary colour and not the fill colour:
   a. Colour the pixel green.
   b. Add its four neighbours to the work stack.
5. Repeat until the stack becomes empty.

ALGORITHM - FLOOD FILL:
1. Start with a seed point selected by the mouse.
2. Read the pixel colour.
3. If the point is outside the window, stop.
4. If its colour equals the old colour (black):
   a. Change it to blue.
   b. Add its four neighbours to the work stack.
5. Repeat until no eligible pixels remain.

PROGRAM DESCRIPTION:
drawPolygon() draws a triangle using GL_LINE_LOOP. For Boundary Fill,
the triangle boundary is red; for Flood Fill, it is white. mouse() receives
the left mouse click and converts GLUT's top-left-origin y coordinate to
OpenGL's bottom-left-origin coordinate. boundaryFill4() checks for the
boundary and fill colours. floodFill4() checks the old colour and replaces
it. Both use a stack instead of deep recursion, which reduces the risk of
call-stack overflow on large filled regions. glReadPixels() reads the
colour of a pixel, while glColor3ub() sets an 8-bit RGB colour.

IMPORTANT USAGE:
- Select option 1 for Boundary Fill and click INSIDE the triangle.
- Select option 2 for Flood Fill and click INSIDE the triangle.
- Do not click outside the polygon: the outside region is also connected
  and can be filled if selected as the seed.
- Boundary Fill requires the specified boundary colour to be present.
- Flood Fill requires the inside region to match the specified old colour.
- The polygon must be closed; gaps can allow filling to leak out.

COMPILE COMMAND (Ubuntu):
sudo apt update
sudo apt install g++ freeglut3-dev
g++ polygonfill4.cpp -o polygonfill4 -lGL -lGLU -lglut

RUN COMMAND:
./polygonfill4

SAMPLE INPUT:
Select Algorithm:
1. Boundary Fill
2. Flood Fill
Enter Choice: 1

SAMPLE OUTPUT:
A 500 x 500 window opens with a triangular outline. For option 1 the
outline is red. Clicking inside it fills the connected interior green.
For option 2 the outline is white and the connected black interior becomes
blue. Filling is performed by clicking the left mouse button.

RESULT:
Flood Fill and Boundary Fill were implemented and used to fill the
interior region of a polygon using 4-connected neighbouring pixels.

CONCLUSION:
Thus, both region-filling algorithms were implemented successfully.
Flood Fill replaces pixels of a selected old colour, whereas Boundary Fill
fills until the specified boundary colour is encountered.

DETAILED HINGLISH VIVA EXPLANATION:
Is practical me hum ek closed triangle ke andar colour bhar rahe hain.
Screen par har pixel ka apna RGB colour hota hai. Program mouse click se
ek starting pixel, jise seed point bolte hain, leta hai. Us seed ke aas-paas
ke connected pixels ko check karke colour karta hai. Ye process tab tak
chalta hai jab tak fill karne layak connected pixels khatam nahi hote.

Boundary Fill ko simple words me samjho: hum boundary ka colour pehle se
jaante hain. Is program me boundary red hai aur fill green hai. Algorithm
seed point ka colour read karta hai. Agar pixel red boundary nahi hai aur
pehle se green bhi nahi hai, to usko green kar deta hai. Phir uske left,
right, up aur down neighbours check karta hai. Jaise hi red boundary milti
hai, us direction me filling ruk jaati hai. Isliye boundary continuous aur
closed honi chahiye. Agar outline me gap hoga to colour bahar leak kar sakta hai.

Flood Fill ka logic thoda different hai. Isme boundary ka exact colour
primary condition nahi hota; old/interior colour check hota hai. Yahan
interior black hai. Algorithm black pixels ko blue se replace karta hai.
Jab white boundary ya koi non-black pixel milta hai to us direction me
aage nahi badhta. Isliye Flood Fill ko old colour replacement method bhi
keh sakte hain. Dono algorithms me difference ye hai ki Boundary Fill
boundary colour ko dekhkar stop hota hai, jabki Flood Fill target old colour
ko dekhkar continue karta hai.

4-connected ka matlab ek pixel ke sirf chaar direct neighbours check
karna: right, left, up, down. Diagonal pixels include nahi hote. Stack ka
use pending pixels store karne ke liye hota hai. Recursive calls ke bajay
stack use karne se bahut bade region me function-call stack overflow ka
risk kam hota hai. glReadPixels current pixel ka RGB data read karta hai.
Mouse coordinate conversion zaroori hai kyunki GLUT mouse y top se count
karta hai, jabki OpenGL coordinate system bottom se count karta hai.

VIVA QUESTIONS:
1. Flood Fill kya hai? Same old-colour connected pixels ko new colour se replace karna.
2. Boundary Fill kya hai? Boundary colour milne tak interior fill karna.
3. Seed point kya hai? Filling start karne wala initial pixel.
4. 4-connected neighbours kaunse hain? Left, right, up and down.
5. 8-connected me extra kya hota hai? Four diagonal neighbours.
6. glReadPixels ka use? Screen ke selected pixel ka colour read karna.
7. Flood Fill me old colour kya hai? Is code me black.
8. Boundary Fill me boundary colour kya hai? Is code me red.
9. Polygon closed kyun hona chahiye? Taaki fill region ke bahar leak na kare.
10. Dono me main difference? Flood Fill old colour check karta hai; Boundary Fill boundary colour check karta hai.
===============================================================================

/*
ADDITIONAL DETAILED THEORY AND EXAMINATION NOTES

1. POLYGON AND REGION FILLING
A polygon is a closed two-dimensional shape made from three or more
straight edges. The meeting points are called vertices and the line
segments are called edges. A polygon can be convex or concave. In a
convex polygon, a line joining any two points inside it remains inside
the polygon. A concave polygon has at least one inward indentation.
Region filling means colouring the pixels enclosed by a boundary.

2. SEED POINT
A seed point is the starting pixel from which the filling process begins.
It must normally be selected inside the intended region. The mouse click
in this program provides the seed point. If the user clicks outside the
polygon, the outside connected region may be filled instead.

3. FOUR-CONNECTED AND EIGHT-CONNECTED PIXELS
For a pixel (x,y), 4-connected neighbours are:
Right: (x+1,y)
Left:  (x-1,y)
Up:    (x,y+1)
Down:  (x,y-1)
8-connected filling also includes diagonal neighbours:
(x+1,y+1), (x-1,y+1), (x+1,y-1), (x-1,y-1).
This program uses 4-connected filling. Connectivity determines which
pixels are considered connected to the seed.

4. BOUNDARY FILL - DETAILED WORKING
Boundary Fill begins at a seed pixel and expands outward. It checks
whether the current pixel is equal to the boundary colour or the fill
colour. If it is neither, the pixel is coloured with the fill colour
and its four neighbours are processed. If it matches the boundary,
that path stops. If it is already filled, it is not processed again.
In this program the boundary colour is red and the fill colour is green.
The boundary should be closed and consistently drawn; otherwise the
fill can escape through a gap.

5. FLOOD FILL - DETAILED WORKING
Flood Fill checks whether the current pixel has the target old colour.
If yes, it replaces that colour with the new fill colour and processes
the neighbours. If the pixel has another colour, processing stops in
that direction. In this program the old colour is black and the new
colour is blue. The polygon outline is white, so it does not match the
old black colour and blocks the fill. Flood Fill is useful when the
region is identified by a uniform interior colour rather than by a
particular boundary colour.

6. IMPORTANT DIFFERENCE
Boundary Fill asks: "Is this pixel NOT the boundary colour and NOT
already filled?" If yes, fill it.
Flood Fill asks: "Is this pixel equal to the old target colour?" If yes,
replace it.
Boundary Fill needs the boundary colour to be known. Flood Fill needs
the old interior colour to be known. Both need a valid seed point and
a region that is suitably enclosed.

7. STACK-BASED IMPLEMENTATION
A recursive implementation calls the function for each neighbour.
For a large region, many nested function calls can consume the program's
call stack. This implementation uses an explicit stack. A stack follows
LIFO: Last In, First Out. The algorithm pushes neighbouring coordinates
onto the stack and repeatedly pops one coordinate for processing.
This avoids very deep recursive function calls, although a large region
can still require substantial memory for pending pixels.

8. glReadPixels()
glReadPixels(x,y,width,height,format,type,data) reads pixel data from
the current read buffer. This program reads one pixel using width=1 and
height=1, format GL_RGB and type GL_UNSIGNED_BYTE. The resulting three
colour components are stored in an unsigned-char array. The program
compares these components with the expected boundary, fill or old colour.

9. MOUSE COORDINATE CONVERSION
GLUT mouse callbacks commonly report y measured from the top of the
window. OpenGL's 2D coordinate setup here uses the bottom as the lower
origin. Therefore the program converts y using H-1-y. Without this
conversion, the selected seed could appear vertically mirrored relative
to the polygon.

10. IMPORTANT PRACTICAL CONDITIONS
- Draw a closed polygon with no gaps.
- Select the correct algorithm before clicking.
- Click well inside the polygon, not on its outline.
- The expected interior colour must match the algorithm's configured colour.
- The viewport and read-buffer coordinates must correspond.
- Filling a large area can take time because many pixels are checked.
- The stack stores coordinates that are waiting to be processed.

11. DETAILED HINGLISH VIVA EXPLANATION
Is practical me hum polygon ke andar colour fill karte hain. Polygon ka
matlab closed shape hota hai jo straight line edges se banta hai. Triangle
ek polygon hai kyunki uske three edges aur three vertices hote hain.
Filling start karne ke liye ek seed point chahiye. User mouse se polygon
ke andar click karta hai. Us click ki location se algorithm colour spread
karna start karta hai.

Boundary Fill me algorithm ko boundary ka colour pata hota hai. Is code
me boundary red aur fill green hai. Current pixel read hota hai. Agar
pixel red nahi hai aur green bhi nahi hai, to usko green kar dete hain.
Phir uske four neighbours check karte hain. Jaise hi red boundary milti
hai, us direction me filling ruk jaati hai. Agar boundary me gap hai to
algorithm gap se bahar nikal sakta hai. Isliye closed boundary important hai.

Flood Fill me hum old colour identify karte hain. Yahan polygon ka inside
black hai aur fill colour blue hai. Algorithm sirf black pixels ko blue
me convert karta hai. White boundary black nahi hai, isliye uske aage
fill nahi hota. Isliye Flood Fill ka focus old colour par hota hai,
jabki Boundary Fill ka focus boundary colour par hota hai.

4-connected ka matlab current pixel ke left, right, up aur down neighbours.
Diagonal pixels include nahi hote. Agar 8-connected hota to diagonal
neighbours bhi check hote. Stack ek temporary storage hai. Neighbour
coordinates stack me push hote hain aur next iteration me pop hokar
process hote hain. Stack LIFO principle follow karta hai: jo last push
hua woh pehle pop hota hai.

glReadPixels screen se selected pixel ka RGB colour read karta hai.
Mouse y coordinate ko H-1-y se convert karte hain kyunki mouse ka origin
top side se aur OpenGL ka coordinate origin bottom side se hota hai.
Agar click polygon ke bahar kiya to outside area connected ho sakta hai
aur fill galat jagah spread ho sakta hai. Isliye seed point hamesha
polygon ke andar click karna chahiye.

12. FLOOD FILL VS BOUNDARY FILL TABLE
Flood Fill:
- Checks the old/interior colour.
- Replaces connected pixels of that colour.
- Does not need one specific boundary colour as its main test.
- Useful for replacing a uniform-colour region.

Boundary Fill:
- Checks the boundary colour.
- Fills until the boundary colour is encountered.
- Requires the boundary colour to be specified.
- Works well when the region has a clear, closed boundary.

13. APPLICATIONS
These algorithms are used in paint programs, image editors, colouring
applications, map colouring, game objects, diagram editors, GUI tools
and image-processing applications.

14. VIVA - DETAILED ANSWERS
Q1. Polygon kya hai?
A. A closed 2D figure formed by joining three or more straight line
segments. The meeting points are vertices and segments are edges.

Q2. Convex aur concave polygon me difference?
A. In a convex polygon, every internal angle is less than 180 degrees
and line segments between interior points remain inside. A concave
polygon has an inward indentation and at least one internal angle
greater than 180 degrees.

Q3. Region filling kya hai?
A. The process of assigning a colour to pixels inside a closed region.

Q4. Flood Fill kya hai?
A. It replaces connected pixels having a selected old colour with a
new fill colour.

Q5. Boundary Fill kya hai?
A. It fills from a seed point and stops when the specified boundary
colour is encountered.

Q6. Seed point kya hai?
A. The initial pixel from which the filling operation starts.

Q7. 4-connected pixels kaunse?
A. Left, right, top and bottom neighbours.

Q8. 8-connected pixels kaunse?
A. The four direct neighbours plus four diagonal neighbours.

Q9. Pixel colour read karne ka OpenGL function?
A. glReadPixels().

Q10. Kaunsa algorithm boundary colour use karta hai?
A. Boundary Fill.

Q11. Stack kyun use kiya?
A. To store pending neighbouring pixels and avoid very deep recursive
function calls.

Q12. Fill leak kyun hota hai?
A. A gap in the polygon boundary or an incorrect seed point can allow
the filling process to reach the outside region.
*/
*/
