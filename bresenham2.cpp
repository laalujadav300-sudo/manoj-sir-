#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
using namespace std;

int lineStyle;
int X1, Y1, X2, Y2;

// Bresenham Line Drawing Algorithm for every octant
void drawLineBresenham(int x1, int y1, int x2, int y2, int style)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int xInc = (x2 >= x1) ? 1 : -1;
    int yInc = (y2 >= y1) ? 1 : -1;
    int x = x1, y = y1;

    glBegin(GL_POINTS);

    if (dx >= dy)
    {
        int p = 2 * dy - dx;
        for (int i = 0; i <= dx; ++i)
        {
            if (style == 1 || (style == 2 && i % 4 == 0) ||
                (style == 3 && i % 15 < 10))
                glVertex2i(x, y);

            if (i == dx) break;

            if (p >= 0)
            {
                y += yInc;
                p += 2 * dy - 2 * dx;
            }
            else
                p += 2 * dy;

            x += xInc;
        }
    }
    else
    {
        int p = 2 * dx - dy;
        for (int i = 0; i <= dy; ++i)
        {
            if (style == 1 || (style == 2 && i % 4 == 0) ||
                (style == 3 && i % 15 < 10))
                glVertex2i(x, y);

            if (i == dy) break;

            if (p >= 0)
            {
                x += xInc;
                p += 2 * dx - 2 * dy;
            }
            else
                p += 2 * dx;

            y += yInc;
        }
    }

    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawLineBresenham(X1, Y1, X2, Y2, lineStyle);
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

int main(int argc, char** argv)
{
    cout << "Enter Start Point (x1 y1): ";
    cin >> X1 >> Y1;
    cout << "Enter End Point (x2 y2): ";
    cin >> X2 >> Y2;
    cout << "Select Style (1: Simple, 2: Dotted, 3: Dashed): ";
    cin >> lineStyle;

    if (lineStyle < 1 || lineStyle > 3)
    {
        cout << "Invalid style. Please select 1, 2 or 3.\n";
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line Drawing");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL: BRESENHAM LINE DRAWING ALGORITHM
===============================================================================

1. AIM
To implement Bresenham's Line Drawing Algorithm using C++ and OpenGL to generate straight raster lines in simple, dotted and dashed styles, and to understand its integer-based pixel selection technique and efficiency.

2. OBJECTIVES
- To understand raster graphics and line generation.
- To accept the two endpoints of a line segment.
- To calculate the coordinate differences and decision parameter.
- To select the next pixel using integer arithmetic.
- To draw lines with different slopes and directions.
- To generate simple, dotted and dashed line patterns.
- To compare the working principles of Bresenham and DDA algorithms.

3. ASSOCIATED TASKS
1. Write a program to draw a line using Bresenham's line drawing algorithm.
2. Accept line endpoint coordinates from the user.
3. Calculate the next pixel using the decision parameter.
4. Display the generated line using OpenGL or another graphics library.
5. Understand and compare line generation methods and their efficiency.

4. SOFTWARE AND HARDWARE REQUIREMENTS
Software:
- C++ compiler such as GCC/G++ or MinGW.
- OpenGL, GLU and GLUT/freeglut libraries.
- Windows or Linux operating system with graphics support.
Hardware:
- Computer with display, keyboard and mouse.

5. THEORY
A point is the fundamental element of picture representation. Two points, (x1, y1) and (x2, y2), specify a line. A mathematical line extends infinitely in both directions; the portion between two endpoints is called a line segment. In raster graphics, line generation means selecting and turning on suitable screen pixels to represent the line.

A good line-drawing algorithm should produce a straight-looking line, start and end at the correct positions, maintain consistent brightness and draw rapidly.

Bresenham's Line Drawing Algorithm is an efficient raster line generation algorithm. It uses an integer decision parameter to select the next pixel from the available candidate pixels. The main calculations use integer arithmetic, so the algorithm avoids repeated floating-point operations and is suitable for efficient graphics drawing.

Let the endpoints be (x1, y1) and (x2, y2):
dx = |x2 - x1|
dy = |y2 - y1|

The program calculates xInc and yInc as +1 or -1 depending on the direction of the line. If dx is greater than or equal to dy, x is the major direction and the initial decision parameter is:
p = 2dy - dx

Otherwise, y is the major direction and:
p = 2dx - dy

The decision parameter determines whether the minor coordinate must also be changed. This program handles all octants by considering the absolute differences and the signs of the coordinate increments.

LINE STYLES
- Simple line: every generated point is plotted.
- Dotted line: one point is plotted at every fourth iteration.
- Dashed line: ten points are plotted in each group of fifteen iterations.

COMPARISON WITH DDA
- DDA uses floating-point calculations; Bresenham uses integer calculations in its pixel-selection loop.
- DDA calculates xIncrement and yIncrement; Bresenham uses a decision parameter.
- DDA requires rounding calculated coordinates; Bresenham selects pixels using an integer decision.
- DDA is generally easier to understand and implement.
- Bresenham is generally more efficient for raster line generation because it avoids repeated floating-point calculations.

EXEMPLARS AND UTILITY
Line drawing algorithms are basic techniques in raster graphics and are used by drawing tools, diagrams, charts, user interfaces and computer graphics applications. Efficient line generation is important when many line segments must be displayed.

6. ALGORITHM
1. Start and read the first endpoint (x1, y1).
2. Read the second endpoint (x2, y2).
3. Read the line style: 1 for simple, 2 for dotted or 3 for dashed.
4. Calculate dx = |x2 - x1| and dy = |y2 - y1|.
5. Set xInc and yInc according to the direction of the line.
6. Initialize the current point as (x1, y1).
7. If dx >= dy:
   a. Set p = 2dy - dx.
   b. Repeat dx + 1 times.
   c. Plot the current point according to the selected style.
   d. Update the decision parameter and coordinates.
8. Otherwise:
   a. Set p = 2dx - dy.
   b. Repeat dy + 1 times.
   c. Plot the current point according to the selected style.
   d. Update the decision parameter and coordinates.
9. Display the generated line in the OpenGL window.
10. Stop.

7. PROCEDURE
1. Open a C++ IDE or terminal configured with OpenGL and GLUT.
2. Save the program using the required .cpp filename.
3. Compile it with the OpenGL, GLU and GLUT/freeglut libraries.
4. Run the executable.
5. Enter the starting and ending coordinates.
6. Enter 1, 2 or 3 for the required line style.
7. Observe the generated line in the graphics window.
8. Repeat with different coordinates, slopes and styles.

8. IMPORTANT FUNCTIONS AND TERMS
- abs(): obtains the positive difference between integer coordinates.
- Decision parameter (p): decides which candidate pixel is closer to the ideal line.
- xInc and yInc: determine the direction in which the coordinates move.
- glBegin(GL_POINTS): starts drawing individual raster points.
- glVertex2i(x, y): specifies an integer pixel position.
- glEnd(): ends the point drawing section.
- glFlush(): sends drawing commands for execution.
- gluOrtho2D(): establishes the 2D coordinate system.

9. SAMPLE INPUT AND OUTPUT
Sample input:
Enter Start Point (x1 y1): 50 50
Enter End Point (x2 y2): 400 300
Select Style (1: Simple, 2: Dotted, 3: Dashed): 1

Expected graphics:
A white straight line is displayed from (50,50) to (400,300) on a black background. Style 2 displays a dotted line and style 3 displays a dashed line. The output is a raster approximation of the ideal line.

10. RESULT
Bresenham's Line Drawing Algorithm was implemented successfully. The program generated a line between the entered endpoints using an integer decision parameter and displayed it in simple, dotted or dashed style.

11. CONCLUSION
Thus, Bresenham's Line Drawing Algorithm was studied and executed successfully. The practical demonstrates raster line generation, calculation of coordinate differences, use of a decision parameter, selection of suitable pixels and generation of different line styles. Its integer-based calculations and efficiency were understood and compared with the DDA algorithm.

11. PRECAUTIONS / LIMITATIONS
- Enter coordinates within the 0 to 500 graphics window for a fully visible
  line.
- Select only style 1, 2 or 3.
- The algorithm draws a raster approximation of the ideal mathematical line.
- Dotted and dashed appearance depends on the iteration intervals used.

12. VIVA-VOCE: HINGLISH EXPLANATION AND QUESTIONS
Q1. Bresenham algorithm kya hai?
Ans. Ye line drawing algorithm hai jo screen par straight line ko pixels ke
form mein draw karta hai. Ye decision parameter use karke decide karta hai ki
next pixel kaunsa hoga. Isme main calculation integer arithmetic se hoti hai,
isliye ye efficient hai.

Q2. Raster scan display kya hota hai?
Ans. Raster display mein screen pixels ki rows aur columns se bani hoti hai.
Image ko pixels illuminate karke display kiya jata hai.

Q3. Pixel kya hota hai?
Ans. Pixel ka full form picture element hai. Ye digital image ka smallest
addressable display element hota hai.

Q4. Decision parameter p ka role kya hai?
Ans. p decide karta hai ki next step mein sirf x ya y coordinate change karna
hai, ya dono coordinate change karne hain. Isse ideal line ke closest pixel
select karne mein help milti hai.

Q5. dx aur dy ka meaning?
Ans. dx = x direction mein total difference aur dy = y direction mein total
difference. Dono absolute values hote hain.

Q6. Major axis kya hota hai?
Ans. Jis direction mein coordinate difference zyada hota hai, woh major axis
hota hai. Agar dx bada hai toh x major direction hai; agar dy bada hai toh y.

Q7. Bresenham floating point avoid kyun karta hai?
Ans. Integer calculations generally fast aur simple hoti hain. Bresenham ka
decision parameter integer form mein update hota hai, isliye raster drawing
efficient hoti hai.

Q8. Slope kya hota hai?
Ans. Slope line ki steepness batata hai. Formula m = (y2-y1)/(x2-x1) hai,
jab x2 aur x1 equal na hon. Positive slope line upar jaati hai aur negative
slope neeche jaati hai.

Q9. Is program mein xInc aur yInc kyun hain?
Ans. Ye direction decide karte hain. Agar endpoint coordinate bada hai toh
increment +1 hota hai, otherwise -1. Isliye reverse direction aur negative
slope wali lines bhi draw ho sakti hain.

Q10. Dotted aur dashed line kaise banti hai?
Ans. Algorithm har iteration mein point calculate karta hai, lekin style
condition decide karti hai ki point draw karna hai ya skip. Dotted style mein
har chauthe point ko draw karte hain. Dashed style mein 15 ke group mein pehle
10 points draw karte hain.

Q11. Bresenham aur DDA mein difference?
Ans. DDA floating-point increments use karta hai, jabki Bresenham decision
parameter aur integer updates use karta hai. Bresenham generally faster aur
integer raster implementation ke liye efficient hota hai.

Q12. dx = dy ho toh kya hota hai?
Ans. Line ka slope magnitude 1 hota hai. Is program mein dx >= dy condition
true hoti hai aur X-major branch execute hoti hai.

Q13. Endpoints same hon toh?
Ans. dx aur dy dono zero honge. Loop ek point draw karega, isliye output ek
single pixel hoga.

Q14. glVertex2i() ka use?
Ans. Ye OpenGL ko batata hai ki integer x-y coordinate par point draw karna
hai.

Q15. Practical ka output?
Ans. Black background par user ke diye endpoints ke beech white line draw
hoti hai. User simple, dotted ya dashed pattern choose kar sakta hai.
===============================================================================


13. DETAILED HINGLISH EXPLANATION (THEORY + CODE WALKTHROUGH)

A. Bresenham ka basic idea
Computer screen pixels ka grid hota hai. Mathematical straight line continuous
hoti hai, lekin screen par sirf integer pixel positions draw kar sakte hain.
Bresenham algorithm har step par decide karta hai ki next pixel current row
par lena hai ya next row par. Is decision ke liye decision parameter (p) use
hota hai. Iski khas baat ye hai ki calculations integer arithmetic se hoti
hain; isliye DDA ki tarah har step par floating-point increment aur rounding
ki zaroorat nahi padti.

B. dx, dy aur direction
Code mein dx = abs(x2-x1), dy = abs(y2-y1) hai. Ye line ki horizontal aur
vertical distance batate hain. xInc aur yInc decide karte hain ki x/y ko
positive direction mein badhana hai ya negative direction mein. Isse code
sirf first octant tak limited nahi rehta; endpoints kisi bhi direction mein
ho sakte hain. x=x1, y=y1 se plotting start hoti hai.

C. Do cases kyun hain?
Agar dx >= dy, line x direction mein zyada lambi/steepness kam hai. Is case
mein x har iteration mein change hota hai aur y kabhi-kabhi change hota hai.
Initial decision parameter p = 2*dy - dx hota hai. Agar p >= 0, diagonal
choice better hoti hai, isliye y += yInc aur p mein 2*dy - 2*dx add hota
hai. Otherwise sirf x direction mein move karke p mein 2*dy add hota hai.

Agar dy > dx, line y direction mein zyada lambi hai. Ab y har iteration
mein change hota hai aur x decision ke hisaab se change hota hai. Initial
p = 2*dx - dy hota hai. p >= 0 par x += xInc aur p mein 2*dx - 2*dy add
hota hai; warna p mein 2*dx add hota hai. Dono branches ka goal ideal
mathematical line ke sabse paas pixel select karna hai.

D. Decision parameter ko simple tarike se samjho
Har current pixel ke baad do possible next pixels socho. Algorithm error ko
measure karta hai ki ideal line se kaunsa candidate zyada close hai. p ka
sign decision deta hai. p >= 0 hone par secondary axis bhi change hoti hai;
p < 0 hone par major axis ke along hi move karte hain. Exact formulas exam
mein likho: x-major: p0=2dy-dx; y-major: p0=2dx-dy.

E. Line styles kaise bante hain?
style 1 condition har point ko draw karti hai, so simple continuous line.
style 2 mein i%4==0 hone par selected points draw hote hain, isliye dotted
appearance aati hai. style 3 mein i%15<10 hone par 15-step pattern mein
10 points draw aur baaki skip hote hain, isliye dashed appearance milti hai.
Ye visual pattern algorithm ke point counter i se control hota hai.

F. Code ke OpenGL parts
GL_POINTS individual pixel positions plot karta hai. glBegin(GL_POINTS) aur
glEnd() ke beech glVertex2i(x,y) har selected integer coordinate deta hai.
glFlush() drawing commands bhejta hai. display() screen clear karta hai,
white colour set karta hai aur input endpoints ke liye line function call
karta hai. init() black background aur 0..500 2D coordinate plane set karta
hai. main() endpoints aur style keyboard se leta hai; invalid style 1,2,3
ke bahar ho toh program window banane se pehle exit karta hai.

G. Example samjho
Input start (50,50), end (300,200), style 1. dx=250 aur dy=150, isliye
x-major branch chalegi. p0=2(150)-250=50. Algorithm x ko har step badhata
hai; p ke sign ke according kuch steps par y bhi badhta hai. Is tarah
(50,50) se (300,200) tak nearest pixels plot hote hain. style 2/3 select
karne par same calculated path ke kuch points skip kiye jaate hain.

H. DDA se comparison
DDA floating-point x/y increments calculate karta hai aur round karta hai.
Bresenham decision parameter aur integer updates use karta hai. Isliye
Bresenham generally efficient aur raster line drawing ke liye popular hai.
Dono algorithms endpoints ke beech approximate straight line banate hain.

14. COMPILATION AND EXECUTION
Linux (FreeGLUT installed):
 g++ bresenham2.cpp -o bresenham2 -lglut -lGLU -lGL
 ./bresenham2
Windows MinGW (FreeGLUT configured):
 g++ bresenham2.cpp -o bresenham2.exe -lfreeglut -lopengl32 -lglu32
 bresenham2.exe

15. SAMPLE INPUT / OUTPUT
Input example:
Enter Start Point (x1 y1): 50 50
Enter End Point (x2 y2): 300 200
Select Style (1: Simple, 2: Dotted, 3: Dashed): 1
Output: 500x500 black graphics window par white line start point se end
point tak draw hoti hai. Style 2 dotted aur style 3 dashed line deta hai.

*/
