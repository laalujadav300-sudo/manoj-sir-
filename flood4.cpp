#include <GL/glut.h>
#include <iostream>
using namespace std;

const int WINDOW_SIZE = 500;
const unsigned char fillColor[3] = {0, 0, 255}; // Blue
const unsigned char oldColor[3] = {0, 0, 0};   // Black

// 4-connected Flood Fill Algorithm
void floodFill4(int x, int y)
{
    if (x < 0 || x >= WINDOW_SIZE || y < 0 || y >= WINDOW_SIZE)
        return;

    unsigned char color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);

    bool matchesOldColor = color[0] == oldColor[0] &&
                           color[1] == oldColor[1] &&
                           color[2] == oldColor[2];

    if (matchesOldColor)
    {
        glColor3ub(fillColor[0], fillColor[1], fillColor[2]);
        glBegin(GL_POINTS);
            glVertex2i(x, y);
        glEnd();
        glFlush();

        floodFill4(x + 1, y);
        floodFill4(x - 1, y);
        floodFill4(x, y + 1);
        floodFill4(x, y - 1);
    }
}

void drawPolygon()
{
    glLineWidth(3.0f);
    glColor3ub(255, 255, 255);
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
        y = WINDOW_SIZE - 1 - y;
        floodFill4(x, y);
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
    cout << "Flood Fill Algorithm\n";
    cout << "Click inside the white triangle to fill it with blue.\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Flood Fill Algorithm");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL: FLOOD FILL ALGORITHM (4-CONNECTED)
===============================================================================

1. AIM
To implement the 4-connected Flood Fill Algorithm using C++ and OpenGL/GLUT to replace the connected pixels of a selected interior colour with a new colour, and to understand region-filling techniques used in computer graphics.

2. OBJECTIVES
- To understand polygon representation and region filling.
- To draw a closed graphical object and select a seed point inside it.
- To identify pixels having a specified old colour.
- To replace connected old-colour pixels with a new fill colour.
- To understand 4-connected pixel neighbourhood and recursive filling.
- To distinguish Flood Fill from Boundary Fill.

3. ASSOCIATED TASKS
1. Draw a closed polygon or enclosed graphical object using graphics functions.
2. Implement Flood Fill to fill the selected region with a specified colour.
3. Select a seed point inside the polygon.
4. Check the pixel colour and replace connected pixels matching the old colour.
5. Compare Flood Fill and Boundary Fill based on their filling behaviour and boundary conditions.

4. SOFTWARE AND HARDWARE REQUIREMENTS
Software:
- C++ compiler such as GCC/G++ or MinGW.
- OpenGL, GLU and GLUT/freeglut libraries.
- Windows or Linux operating system with graphics support.
Hardware:
- Computer with display, keyboard and mouse.

5. THEORY
A polygon is a closed two-dimensional geometrical figure made by joining three or more straight line segments. The meeting points are called vertices and the line segments are called edges. A convex polygon has all interior angles less than 180 degrees. A concave polygon has at least one interior angle greater than 180 degrees.

Region filling is the process of colouring the pixels inside a closed graphical object. Flood Fill starts from a selected interior pixel called the seed point. It checks whether the current pixel has a specified old colour. If the colour matches, the pixel is replaced with the new fill colour and the algorithm checks its neighbours. It continues spreading through connected pixels of the old colour. Unlike Boundary Fill, Flood Fill checks the target interior colour rather than directly testing a particular boundary colour.

In this program, a triangle with a white outline is drawn on a black background. Black is the old colour and blue is the new fill colour. When the user clicks inside the triangle, the clicked pixel becomes the seed point. If the pixel is black, it is changed to blue and its four neighbours are processed. The white outline separates the interior region from the exterior.

4-CONNECTED AND 8-CONNECTED NEIGHBOURHOOD
In 4-connected filling, the algorithm checks:
- Left: (x - 1, y)
- Right: (x + 1, y)
- Above: (x, y + 1)
- Below: (x, y - 1)

In 8-connected filling, these four direct neighbours and four diagonal neighbours are checked:
- (x - 1, y - 1), (x + 1, y - 1)
- (x - 1, y + 1), (x + 1, y + 1)

This program checks only the four direct neighbours, so it is a 4-connected Flood Fill implementation.

FLOOD FILL AND BOUNDARY FILL
- Flood Fill replaces pixels that match a specified old/interior colour.
- Boundary Fill fills pixels until the specified boundary colour is encountered.
- Flood Fill can work with regions surrounded by multiple boundary colours, provided the target region colour is consistent.
- Boundary Fill generally expects a clearly defined boundary colour.
- Both methods can be implemented recursively or with an explicit stack. A stack-based approach can help avoid very deep recursive calls.

EXEMPLARS AND UTILITY
Region-filling methods are used in image-editing and paint systems, object colouring in games and animation, map colouring, GIS systems, GUI and icon designing, and visualisation applications.

6. ALGORITHM
1. Start and initialize the OpenGL/GLUT environment.
2. Create a 500 x 500 window with a black background.
3. Draw a closed triangle with a white outline.
4. Wait for a left mouse-button click.
5. Convert the mouse y-coordinate into OpenGL coordinates.
6. Use the clicked pixel as the seed point.
7. Check that the pixel lies inside the screen.
8. Read its RGB colour using glReadPixels().
9. If its colour matches the old colour (black):
   a. Change the pixel to the fill colour (blue).
   b. Plot the pixel.
   c. Recursively process right, left, top and bottom neighbours.
10. Stop processing a direction when a pixel does not match the old colour, has already been filled, or lies outside the screen.
11. End.

7. PROCEDURE
1. Open a C++ IDE or terminal configured with OpenGL and GLUT.
2. Save the program using the required .cpp filename.
3. Compile it using the OpenGL, GLU and GLUT/freeglut libraries.
4. Run the executable.
5. A black graphics window with a white triangle appears.
6. Click at a point inside the triangle.
7. Observe that the connected black interior region changes to blue.
8. The white boundary remains visible.

8. IMPORTANT OPENGL FUNCTIONS
- glReadPixels(): reads RGB colour data from the frame buffer for the selected pixel.
- glColor3ub(): sets the drawing colour using RGB values from 0 to 255.
- glVertex2i(): specifies the integer coordinate of a point.
- GL_POINTS: draws individual pixels/points.
- GL_LINE_LOOP: draws a connected closed outline and joins the last vertex to the first.
- glutMouseFunc(): registers the mouse callback used to obtain the seed point.
- glFlush(): sends drawing commands for execution.
- gluOrtho2D(): defines the 2D coordinate system.

9. SAMPLE OUTPUT
Console:
Flood Fill Algorithm
Click inside the white triangle to fill it with blue.

Graphics output:
- Background: Black
- Triangle boundary: White
- After clicking inside: connected black interior pixels become Blue
The result depends on selecting a seed point within the intended region.

10. RESULT
The 4-connected Flood Fill Algorithm was implemented using C++ and OpenGL. On selecting a seed point inside the white triangle, the connected black interior pixels were replaced with blue while the white outline remained visible.

11. CONCLUSION
Thus, the 4-connected Flood Fill Algorithm was studied and executed successfully. The practical demonstrates seed-point selection, checking the old pixel colour, replacing matching pixels with a new colour, and recursively processing four neighbours. The difference between Flood Fill and Boundary Fill was also understood.

11. PRECAUTIONS / LIMITATIONS
- The boundary should be closed so that the interior is separated from the
  exterior.
- The seed point should be inside the intended region.
- The target old colour must match the actual pixel colour.
- If the old colour is also present in a connected outside region due to a
  gap in the boundary, the fill may spread outside.
- Recursive filling can cause stack overflow for a very large region.
- This implementation uses 4-connectivity; diagonal neighbours are not
  directly checked.

12. VIVA-VOCE: HINGLISH EXPLANATION AND QUESTIONS
Q1. Flood Fill Algorithm kya hota hai?
Ans. Flood Fill ek region filling algorithm hai. Hum ek seed point select
karte hain aur us point ke same old colour wale connected pixels ko new colour
se replace kar dete hain. Ye process tab tak chalta hai jab tak matching
colour wale connected pixels milte rehte hain.

Q2. Seed point kya hota hai?
Ans. Seed point starting pixel hai. Is practical mein user mouse se triangle
ke andar click karta hai. Wahi se filling start hoti hai.

Q3. Old colour aur fill colour kya hain?
Ans. Old colour woh colour hai jise replace karna hai. Is code mein black
(0,0,0) old colour hai. Fill colour new colour hai, yahan blue (0,0,255).

Q4. Flood Fill mein boundary colour check hota hai?
Ans. Is implementation mein direct boundary-colour comparison nahi hota.
Algorithm old colour match karta hai. White outline old black region ko
surround karke rakhti hai, isliye fill outline par rukta hai.

Q5. 4-connected neighbourhood kya hai?
Ans. Current pixel ke left, right, top aur bottom neighbours. Is code mein
sirf ye chaar directions recursively check hoti hain.

Q6. 8-connected neighbourhood kya hota hai?
Ans. 4 direct neighbours ke alawa four diagonal neighbours bhi include hote
hain. Total eight neighbours check hote hain.

Q7. Flood Fill aur Boundary Fill ka difference?
Ans. Flood Fill old/interior colour ko target karta hai. Boundary Fill
boundary colour ko stopping condition banata hai. Dono region fill karte hain,
lekin stopping condition alag hoti hai.

Q8. glReadPixels() ka use?
Ans. Framebuffer se current pixel ka RGB colour read karta hai. Code check
karta hai ki pixel black hai ya nahi.

Q9. Pixel ko blue karne ke baad neighbours kyun check karte hain?
Ans. Taaki poora connected region fill ho sake. Ek pixel fill hone ke baad
algorithm uske four neighbours ko process karta hai.

Q10. Already filled pixel ko dobara kyun nahi bharte?
Ans. Fill hone ke baad pixel blue ho jata hai, black nahi rehta. Isliye
old-colour condition false ho jaati hai aur recursion wahan ruk jaati hai.
Ye repeated processing ko prevent karta hai.

Q11. Boundary mein gap ho toh kya hoga?
Ans. Agar white outline mein gap ho, toh inside black region outside black
background se connected ho sakta hai. Flood Fill us connected region ko bhi
blue kar sakta hai.

Q12. Flood Fill ka use kahan hota hai?
Ans. Paint applications ke bucket-fill tool, image editing, region colouring
aur connected-component processing mein iska concept use hota hai.

Q13. Recursive approach ki limitation?
Ans. Bahut bade region mein recursion depth badh sakti hai aur call stack
overflow ho sakta hai. Iterative stack ya queue approach large images ke liye
safer hoti hai.

Q14. glColor3ub() aur glColor3f() mein difference?
Ans. glColor3ub() unsigned byte values leta hai, usually 0 se 255. glColor3f()
floating-point RGB values leta hai, generally 0.0 se 1.0.

Q15. Practical ka output kya hai?
Ans. Black background par white triangle show hota hai. Triangle ke andar
click karne par connected black pixels blue ho jaate hain aur white boundary
visible rehti hai.
===============================================================================


13. DETAILED HINGLISH EXPLANATION (THEORY + CODE WALKTHROUGH)

A. Flood Fill ka basic concept
Flood Fill kisi connected region ke same old colour wale pixels ko ek new
colour se replace karta hai. Iska common example paint software ka bucket
fill tool hai. User ek seed point select karta hai. Algorithm us point ka
colour check karta hai. Agar colour target old colour ke equal hai toh pixel
new fill colour se replace hota hai aur uske neighbours bhi check hote hain.
Process tab tak chalta hai jab tak connected matching-colour pixels milte
hain.

B. Is program ke colours aur shape
Window 500 x 500 hai. Background black (0,0,0), triangle boundary white
(255,255,255), aur fill colour blue (0,0,255) hai. drawPolygon() GL_LINE_LOOP
se triangle ki closed white outline draw karta hai. oldColor black set hai.
Interior initially black hota hai, isliye interior click par filling start
ho sakti hai. White outline black interior ko black exterior se separate
rakhti hai, provided outline mein gap na ho.

C. floodFill4() step-by-step
1) Function x,y current pixel coordinate receive karta hai.
2) Bounds condition check karti hai ki pixel 0..499 ke andar hai. Bahar ho
   toh return karke recursion stop hoti hai.
3) glReadPixels() framebuffer se current pixel ka RGB colour read karta hai.
4) matchesOldColor tab true hota hai jab pixel ka RGB exactly black ho.
5) Agar black hai, glColor3ub() blue set karta hai aur GL_POINTS ke through
   current x,y par blue pixel draw hota hai.
6) glFlush() drawing commands execute karne ko bhejta hai. Ab wahi pixel blue
   hai, black nahi. Agar recursion dobara yahan aaye toh old-colour condition
   false hogi; is tarah already processed pixel par algorithm rukta hai.
7) Function right, left, top, bottom neighbours ko recursively call karta
   hai. Isliye 4-connected neighbourhood use hota hai.

D. Boundary Fill se difference
Dono algorithms seed point se region fill karte hain, lekin check alag hai.
Boundary Fill kehta hai: jab tak pixel boundary colour nahi hai aur fill
colour bhi nahi hai, fill karte jao. Flood Fill kehta hai: sirf woh pixels
replace karo jo specified old colour se match karte hain. Is code mein white
boundary ko direct compare nahi kiya jata; black old colour match na hone ki
wajah se white outline par recursion ruk jaati hai.

E. 4-connected aur 8-connected
Current pixel ke four direct neighbours right (x+1,y), left (x-1,y), top
(x,y+1), bottom (x,y-1) hain. Diagonal neighbours include nahi hain. Agar
8-connected version hota toh four diagonal positions bhi check hote. 4-way
connectivity mein diagonal-only touch karne wale pixels ek region ka hissa
nahi maane jaate.

F. Example se samjho
User white triangle ke andar black pixel par click karta hai. Ye seed point
hai. Pixel black hai, toh blue draw hota hai. Ab uske left/right/up/down
pixels check hote hain. Jo bhi black milta hai woh blue hota hai aur uske
neighbours check hote hain. Jab white outline milti hai toh pixel black nahi
hota, isliye us direction mein recursion aage nahi badhti. Is tarah connected
black interior blue ho jata hai. Agar outline mein gap hua toh inside aur
outside black connected ho sakte hain aur fill bahar bhi fail sakta hai.

G. Function aur library roles
- main(): instructions print karta hai, GLUT setup aur window creation karta
  hai, display/mouse callbacks register karta hai.
- init(): black background aur 2D projection define karta hai.
- display(): screen clear karke white triangle draw karta hai.
- mouse(): left click par GLUT ke top-origin y ko OpenGL bottom-origin y mein
  convert karta hai, phir floodFill4() ko seed coordinate deta hai.
- glReadPixels(): pixel ka actual framebuffer colour read karta hai.
- glColor3ub(): 0..255 RGB values se colour set karta hai.
- GL_POINTS: individual pixels plot karta hai.

H. Limitation aur practical precautions
Seed point intended region ke andar hona chahiye. Old colour exactly match
hona chahiye; anti-aliasing ya blended pixels exact RGB match ko affect kar
sakte hain. Boundary closed honi chahiye. Recursive approach large region
mein stack overflow kar sakti hai; iterative stack/queue implementation
large images ke liye safer hai. Is code ka fill 4-connected hai, diagonal
neighbours direct check nahi hote.

I. Exam mein yaad rakhne layak one-line answers
Flood Fill = old colour replace karta hai. Seed point = filling ka starting
pixel. 4-connected = left/right/up/down. 8-connected = four direct plus four
diagonal. glReadPixels = pixel colour read. glColor3ub = RGB colour set.
Boundary Fill vs Flood Fill = boundary colour stopping rule vs old colour
matching rule.

14. COMPILATION AND EXECUTION
Linux (FreeGLUT installed):
 g++ flood4.cpp -o flood4 -lglut -lGLU -lGL
 ./flood4
Windows MinGW (FreeGLUT configured):
 g++ flood4.cpp -o flood4.exe -lfreeglut -lopengl32 -lglu32
 flood4.exe

15. SAMPLE INPUT / OUTPUT
Input: No keyboard input. User clicks inside the white triangle.
Console: Flood Fill Algorithm; Click inside the white triangle to fill it
with blue.
Graphics: Black background and white triangle; after an interior click,
connected black interior pixels become blue.

*/
