#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
using namespace std;

int algoChoice, lineStyle;
float X1, Y1, X2, Y2;

// DDA: calculates intermediate points using floating-point increments.
void drawLineDDA(float x1, float y1, float x2, float y2, int style)
{
    float dx = x2-x1, dy = y2-y1;
    float length = max(fabs(dx), fabs(dy));
    if (length == 0) {
        glBegin(GL_POINTS); glVertex2f(x1, y1); glEnd();
        glFlush();
        return;
    }

    float xInc = dx/length, yInc = dy/length;
    float x=x1, y=y1;
    glBegin(GL_POINTS);
    for (int i=0; i<=static_cast<int>(length); ++i) {
        bool plot = (style==1) || (style==2 && i%4==0) ||
                    (style==3 && i%15<10);
        if (plot) glVertex2f(x, y);
        x += xInc; y += yInc;
    }
    glEnd();
    glFlush();
}

// Bresenham: integer decision parameter; supports all line directions.
void drawLineBresenham(int x1, int y1, int x2, int y2, int style)
{
    int dx = abs(x2-x1), dy = abs(y2-y1);
    int xInc = (x2>=x1) ? 1 : -1;
    int yInc = (y2>=y1) ? 1 : -1;
    int x=x1, y=y1;

    glBegin(GL_POINTS);
    if (dx >= dy) {
        int p = 2*dy-dx;
        for (int i=0; i<=dx; ++i) {
            bool plot = (style==1) || (style==2 && i%4==0) ||
                        (style==3 && i%15<10);
            if (plot) glVertex2i(x,y);
            if (i==dx) break;
            x += xInc;
            if (p>=0) { y += yInc; p += 2*(dy-dx); }
            else p += 2*dy;
        }
    } else {
        int p = 2*dx-dy;
        for (int i=0; i<=dy; ++i) {
            bool plot = (style==1) || (style==2 && i%4==0) ||
                        (style==3 && i%15<10);
            if (plot) glVertex2i(x,y);
            if (i==dy) break;
            y += yInc;
            if (p>=0) { x += xInc; p += 2*(dx-dy); }
            else p += 2*dx;
        }
    }
    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f,1.0f,1.0f);
    if (algoChoice==1) drawLineDDA(X1,Y1,X2,Y2,lineStyle);
    else drawLineBresenham((int)round(X1),(int)round(Y1),
                           (int)round(X2),(int)round(Y2),lineStyle);
}

void init()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    gluOrtho2D(0.0,500.0,0.0,500.0);
}

int main(int argc, char** argv)
{
    cout << "Enter Start Point (x1 y1): "; cin >> X1 >> Y1;
    cout << "Enter End Point (x2 y2): "; cin >> X2 >> Y2;
    cout << "Select Algorithm (1=DDA, 2=Bresenham): "; cin >> algoChoice;
    cout << "Select Style (1=Simple, 2=Dotted, 3=Dashed): "; cin >> lineStyle;

    if (!cin || (algoChoice!=1 && algoChoice!=2) ||
        (lineStyle<1 || lineStyle>3) ||
        X1<0 || X1>500 || X2<0 || X2>500 ||
        Y1<0 || Y1>500 || Y2<0 || Y2>500) {
        cerr << "Invalid input. Use valid choices and coordinates from 0 to 500.\n";
        return 1;
    }

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Line Drawing Algorithms");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 2 : LINE DRAWING ALGORITHMS
===============================================================================

AIM:
To implement DDA and Bresenham's line drawing algorithms in C++ using
OpenGL and display simple, dotted and dashed lines.

OBJECTIVES:
1. To understand raster line generation.
2. To calculate the pixels that approximate a straight line.
3. To implement DDA using floating-point increments.
4. To implement Bresenham's algorithm using an integer decision parameter.
5. To compare the two algorithms and generate different line styles.

THEORY:
A line is specified by two endpoints (x1,y1) and (x2,y2). A raster display
contains discrete pixels, so a line drawing algorithm selects the pixels
that best represent the ideal mathematical line.

DDA (Digital Differential Analyzer):
dx = x2 - x1
dy = y2 - y1
length = max(|dx|, |dy|)
xIncrement = dx / length
yIncrement = dy / length
Starting from (x1,y1), the algorithm repeatedly adds the increments and
plots the resulting point. Floating-point values are rounded by the raster
system when converted to pixels. A zero-length line is handled separately.

BRESENHAM'S LINE ALGORITHM:
Bresenham's algorithm uses an integer decision parameter to choose the next
pixel. It avoids repeated floating-point calculations. The program handles
both cases: |slope| <= 1 and |slope| > 1, and uses xInc/yInc to support
positive and negative directions.
For the x-major case, the initial decision value is p = 2dy - dx.
For the y-major case, p = 2dx - dy. The sign of p determines whether the
minor coordinate also changes.

LINE STYLES:
1. Simple: Every generated pixel is plotted.
2. Dotted: A pixel is plotted at every fourth iteration (i % 4 == 0).
3. Dashed: Pixels are plotted for ten positions out of every fifteen
   (i % 15 < 10), creating repeated dash-and-gap sections.

ALGORITHM:
1. Start and accept the two endpoint coordinates.
2. Accept algorithm choice: 1 for DDA or 2 for Bresenham.
3. Accept style choice: 1 simple, 2 dotted or 3 dashed.
4. Initialize GLUT and create a 500 x 500 graphics window.
5. Clear the window and set the line colour to white.
6. If DDA is selected:
   a. Calculate dx and dy.
   b. Find length = max(|dx|,|dy|).
   c. Calculate x and y increments.
   d. Generate points by repeatedly adding the increments.
   e. Plot points according to the selected style.
7. If Bresenham is selected:
   a. Calculate absolute dx and dy and direction increments.
   b. Select the major axis.
   c. Initialize the decision parameter.
   d. Update the pixel and decision parameter for each iteration.
   e. Plot points according to the selected style.
8. Flush the drawing commands and display the line.
9. Stop when the window is closed.

PROGRAM DESCRIPTION:
drawLineDDA() implements the incremental DDA method. It uses floating-point
x and y values and calculates constant increments. drawLineBresenham()
uses integer arithmetic and a decision parameter. The major-axis check
ensures that shallow and steep lines are handled. The style condition is
applied in both algorithms, so the same line can be shown as simple,
dotted or dashed. display() chooses the selected algorithm. init() sets
a black background and a 0-to-500 coordinate system. main() accepts input
and creates the GLUT window.

COMPILE COMMAND (Ubuntu):
sudo apt update
sudo apt install g++ freeglut3-dev
g++ line_gemini2.cpp -o line_gemini2 -lGL -lGLU -lglut

RUN COMMAND:
./line_gemini2

SAMPLE INPUT:
Enter Start Point (x1 y1): 50 50
Enter End Point (x2 y2): 400 300
Select Algorithm (1=DDA, 2=Bresenham): 1
Select Style (1=Simple, 2=Dotted, 3=Dashed): 1

SAMPLE OUTPUT:
Terminal accepts the values shown above. A graphics window titled
"Line Drawing Algorithms" displays a white solid line from (50,50) to
(400,300) on a black background. For style 2 the line appears dotted;
for style 3 it appears dashed. Selecting algorithm 2 draws the line using
Bresenham's method.

COMPARISON:
- DDA uses floating-point increments; Bresenham uses integer decision values.
- DDA is easier to understand; Bresenham is generally faster for raster displays.
- DDA may accumulate rounding error; Bresenham avoids repeated floating-point
  rounding in its decision process.
- Both generate an approximation of a straight line on a pixel grid.

RESULT:
DDA and Bresenham's line drawing algorithms were implemented and used to
display simple, dotted and dashed line patterns.

CONCLUSION:
Thus, both line drawing algorithms were executed successfully. The practical
demonstrated how endpoint coordinates are converted into raster pixels and
how the two algorithms differ in their calculations and efficiency.

DETAILED HINGLISH VIVA EXPLANATION:
Socho screen ek graph paper hai jisme har chhota square ek pixel hai. Humein
do points diye gaye, jaise (50,50) aur (400,300). Mathematical line continuous
hoti hai, lekin monitor continuous nahi hota; usme pixels hote hain. Isliye
algorithm decide karta hai ki line ke sabse kareeb kaunse pixels light karne
hain. Isi process ko rasterization ya line generation bolte hain.

DDA ka simple idea:
Pehle x aur y ka total difference nikalte hain. dx batata hai horizontal
direction me kitna change hai aur dy vertical direction me kitna change hai.
Dono me jo bada difference hai, wahi length/steps hota hai. Agar dx bada hai
to har step me x roughly 1 se move karega; agar dy bada hai to y roughly 1
se move karega. Isliye xIncrement=dx/length aur yIncrement=dy/length.
Ab current x,y ko baar-baar increment karte hain aur point plot karte hain.
Example me dx=350, dy=250, length=350, xIncrement=1 aur yIncrement
approximately 0.714 hai. Matlab har step x ek badhega aur y around 0.714
badhega. DDA samajhna easy hai, lekin decimal calculations aur rounding
ki wajah se Bresenham ke comparison me less efficient ho sakta hai.

Bresenham ka simple idea:
Har next pixel ke liye do possible choices hoti hain. Algorithm decision
parameter se decide karta hai ki straight line ke zyada close kaunsa pixel
hai. Agar p ki value non-negative hai to minor axis bhi increment hota hai;
warna sirf major axis increment hota hai. Shallow line me x major axis hai,
isliye x har step badhta hai aur kabhi-kabhi y badhta hai. Steep line me y
major axis hai, isliye y har step badhta hai aur kabhi-kabhi x badhta hai.
xInc aur yInc negative slope ya reverse direction ko bhi handle karte hain.
Iska main advantage integer arithmetic hai, isliye raster graphics me
generally efficient hota hai.

Simple, dotted aur dashed ka difference algorithm ke main points calculate
karne ke baad aata hai. Simple style me har calculated point draw hota hai.
Dotted style me har fourth point draw hota hai, beech ke points skip hote
hain. Dashed style me 15 positions ke group me pehle 10 draw aur agle 5 skip
hote hain. Isliye pattern alag dikhta hai. Algorithm same rehta hai, sirf
plot karne ki condition badalti hai.

VIVA QUESTIONS:
1. DDA ka full form? Digital Differential Analyzer.
2. Bresenham kis arithmetic ka use karta hai? Mainly integer arithmetic.
3. length ka formula? max(|dx|,|dy|).
4. Decision parameter ka kaam? Next pixel choose karna.
5. Dotted line kaise banti hai? Fixed interval par pixels plot karke.
6. Dashed line kaise banti hai? Kuch consecutive pixels draw aur kuch skip karke.
7. Kaunsa algorithm generally faster hai? Bresenham, because integer operations.
8. Rasterization kya hai? Geometric shapes ko pixels me represent karna.
===============================================================================

/*
ADDITIONAL DETAILED THEORY AND EXAMINATION NOTES

1. LINE RASTERIZATION
A mathematical line is continuous, but a raster display is made of discrete
pixels. A line drawing algorithm decides which pixels should be activated
to make the displayed line look as close as possible to the ideal line.
The endpoints are supplied as (x1,y1) and (x2,y2). The algorithm must work
for horizontal, vertical, diagonal, steep, shallow and negative-slope lines.

2. MATHEMATICAL FOUNDATION
dx = x2 - x1
dy = y2 - y1
Slope m = dy/dx, when dx is not zero.
A positive slope rises from left to right; a negative slope falls.
A horizontal line has dy=0 and a vertical line has dx=0.
The program uses absolute differences to identify the major axis.
The major axis is the axis with the larger absolute change.

3. DDA - STEP-BY-STEP NUMERICAL EXAMPLE
Consider P1=(2,2) and P2=(8,5).
dx = 8-2 = 6
dy = 5-2 = 3
length = max(6,3) = 6
xIncrement = 6/6 = 1
yIncrement = 3/6 = 0.5
Start at (2,2). The successive ideal positions are approximately:
(2,2), (3,2.5), (4,3), (5,3.5), (6,4), (7,4.5), (8,5).
The raster system maps fractional positions to available pixels. This
illustrates why DDA performs incremental floating-point calculations.

4. BRESENHAM - DECISION PARAMETER
Bresenham avoids calculating the exact real-valued y-coordinate at every
step. For a shallow positive line, the next pixel is selected from two
neighbouring candidates. The decision parameter estimates which candidate
is closer to the ideal line. For dx >= dy:
p0 = 2dy - dx
If p < 0, choose the pixel that changes x only and update p by 2dy.
If p >= 0, change x and y and update p by 2(dy-dx).
For a steep line, interchange the roles of x and y. Direction increments
(+1 or -1) allow the line to be drawn in all directions.

5. WHY THE MAJOR-AXIS TEST IS IMPORTANT
If the line is shallow, x changes more frequently than y. Therefore x is
advanced at every iteration, and y changes only when the decision value
requires it. If the line is steep, y changes at every iteration and x is
updated conditionally. Without this distinction, steep lines could contain
gaps or be plotted incorrectly.

6. HANDLING SPECIAL CASES
A zero-length line has identical endpoints. The DDA routine plots one point
and returns to avoid division by zero. Horizontal and vertical lines are
handled because one of dx or dy can be zero. Negative direction is handled
using xInc and yInc. Input validation restricts the coordinates to the
window range and checks the algorithm/style choices.

7. LINE STYLE LOGIC
The geometry algorithm generates a sequence of candidate pixels. A style
condition decides whether each candidate is actually plotted:
Simple: always plot.
Dotted: plot only when i%4==0.
Dashed: plot when i%15<10.
The modulo operator (%) returns the remainder after division. For example,
i%4==0 is true at i values 0,4,8,12 and so on. For dashed mode, the
remainder cycles from 0 to 14; positions 0 through 9 are drawn and 10
through 14 are skipped. Thus style selection does not change the line
geometry calculation; it changes the visible pattern.

8. COMPARISON TABLE
DDA:
- Uses floating-point increments.
- Uses division to calculate xIncrement and yIncrement.
- Easy to understand and implement.
- Repeated fractional calculations can introduce rounding effects.
- Suitable for learning and simple line generation.

Bresenham:
- Uses an integer decision parameter in its standard form.
- Avoids repeated floating-point calculations.
- Efficient for raster displays.
- Needs careful handling of shallow/steep lines and direction.
- Commonly preferred when efficiency is important.

9. ADVANTAGES AND LIMITATIONS
DDA advantages: simple logic, easy mathematical explanation, supports
different slopes with one increment formula.
DDA limitations: floating-point operations and rounding can affect
accuracy and efficiency.
Bresenham advantages: efficient integer-based decisions and good raster
performance.
Bresenham limitations: logic is more involved; separate major-axis
handling is required. The displayed result still depends on the discrete
pixel grid and is an approximation of a mathematical line.

10. DETAILED HINGLISH VIVA EXPLANATION
Line ka formula hum school me padhte hain, lekin computer screen par line
draw karna thoda different hai. Mathematics me line ke infinite points
ho sakte hain, lekin screen par limited pixels hote hain. Humein decide
karna hota hai ki kaunse pixels on karne se line straight dikhegi. Isi ko
line rasterization bolte hain.

DDA me hum endpoints ka difference nikalte hain. dx horizontal distance
aur dy vertical distance batata hai. length = max(|dx|,|dy|) isliye lete
hain taaki major direction me har step ka movement approximately one
pixel ho. Phir dx/length aur dy/length se increments milte hain. Current
x,y me ye increments add karte jaate hain. Agar yIncrement decimal hai,
to intermediate y values fractional ho sakti hain. Raster display ko
finally integer pixel location chahiye, isliye rounding/mapping hoti hai.
DDA ka logic easy hai, lekin decimal operations ki wajah se Bresenham se
slow ho sakta hai.

Bresenham me hum exact decimal coordinate calculate nahi karte. Hum do
possible next pixels me se choose karte hain. Decision parameter p batata
hai ki line ke ideal path ke zyada close kaunsa pixel hai. p negative
ho to ek candidate choose hota hai; p zero ya positive ho to doosra.
Har iteration me p update hota hai. Shallow line me x major hota hai,
steep line me y major hota hai. Isliye code me dx>=dy check hai. xInc
aur yInc direction ko handle karte hain. Agar endpoint reverse direction
me ho, increment -1 bhi ho sakta hai.

Style ka concept alag hai. DDA/Bresenham decide karte hain line ke candidate
pixels. Simple/dotted/dashed condition decide karti hai unme se kaunse
pixels visible honge. Modulo operator remainder deta hai. Dotted mode me
har fourth point show hota hai. Dashed mode me 15-point cycle me 10 draw
aur 5 skip hote hain. Isliye same algorithm se different patterns milte hain.

11. PRACTICAL APPLICATIONS
Line algorithms are used in CAD drawings, graphs, charts, user-interface
borders, vector graphics, diagram editors, game rendering and plotting
geometric objects.

12. VIVA - DETAILED ANSWERS
Q1. DDA ka full form?
A. Digital Differential Analyzer. It generates a line by repeatedly
adding calculated x and y increments.

Q2. DDA me length kaise calculate hoti hai?
A. length = max(|dx|,|dy|), where dx=x2-x1 and dy=y2-y1.

Q3. DDA floating point kyun use karta hai?
A. Because x and y increments can be fractional. Repeated addition
generates intermediate positions between the endpoints.

Q4. Bresenham decision parameter ka role?
A. It chooses the next raster pixel by estimating which candidate is
closer to the ideal mathematical line.

Q5. Real-time raster drawing ke liye kaunsa preferred hai?
A. Standard Bresenham is generally preferred because it uses efficient
integer-based decisions.

Q6. Slope kya hota hai?
A. Slope is dy/dx and represents the vertical change per unit horizontal
change. A vertical line has undefined/infinite slope.

Q7. Dotted aur dashed me difference?
A. Dotted line contains separated individual points; dashed line contains
short continuous groups of pixels separated by gaps.

Q8. Modulo operator ka use?
A. It returns the remainder. The program uses it to repeat the dot/dash
pattern at regular intervals.

Q9. Major axis kya hai?
A. The coordinate axis with the greater absolute endpoint difference.

Q10. Zero-length line kaise handle hoti hai?
A. Both endpoints are identical, so the program plots one point and
returns without dividing by zero.
*/
*/
