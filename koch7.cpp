#include <iostream>
#include <GL/glut.h>
#include <cmath>
using namespace std;

int n;

// Recursively generates one Koch segment.
void koch(double x1, double y1, double x2, double y2, int level)
{
    if (level == 0) {
        glBegin(GL_LINES);
        glVertex2d(x1, y1);
        glVertex2d(x2, y2);
        glEnd();
        return;
    }

    const double s = 0.866025403784; // sin(60 degrees)
    double x[5], y[5];
    x[0] = x1; y[0] = y1;
    x[4] = x2; y[4] = y2;

    double dx = (x2 - x1) / 3.0;
    double dy = (y2 - y1) / 3.0;
    x[1] = x1 + dx;       y[1] = y1 + dy;
    x[3] = x1 + 2.0*dx;   y[3] = y1 + 2.0*dy;

    double vx = x[3] - x[1];
    double vy = y[3] - y[1];
    x[2] = x[1] + 0.5*vx + s*vy;
    y[2] = y[1] + 0.5*vy - s*vx;

    koch(x[0], y[0], x[1], y[1], level-1);
    koch(x[1], y[1], x[2], y[2], level-1);
    koch(x[2], y[2], x[3], y[3], level-1);
    koch(x[3], y[3], x[4], y[4], level-1);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    koch(50.0, 300.0, 600.0, 300.0, n);
    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(0.0, 650.0, 650.0, 0.0);
}

int main(int argc, char **argv)
{
    cout << "Enter recursion level (0 to 6 recommended): ";
    cin >> n;
    if (!cin || n < 0 || n > 8) {
        cerr << "Invalid level. Enter a value from 0 to 8.\n";
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Koch Curve");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 7 : FRACTAL PATTERN GENERATION USING KOCH CURVE
===============================================================================

AIM:
To implement fractal pattern generation using the Koch Curve algorithm
and understand recursive graphics concepts.

OBJECTIVES:
1. To understand fractals and self-similarity.
2. To construct the Koch Curve from a straight line.
3. To implement the construction using recursion.
4. To observe how recursion depth changes the pattern complexity.

THEORY:
A fractal is a geometric pattern that shows detail at different scales.
A common property is self-similarity: a small part resembles the overall
shape. Fractals are useful for representing complex natural forms such as
coastlines, mountains and clouds.

KOCH CURVE CONSTRUCTION:
1. Start with a straight line segment.
2. Divide it into three equal parts.
3. Remove the middle third.
4. Replace the middle third with two sides of an equilateral triangular
   bump, keeping the original segment's endpoints.
5. Apply the same operation to each of the four new segments.
6. Repeat for the selected recursion level.

At every recursion level, one segment becomes four smaller segments.
Therefore, after n levels the number of segments is 4^n. The length of
each segment becomes one-third of its previous length. This program draws
one Koch curve segment, not the complete three-sided Koch Snowflake.

RECURSION:
Recursion means a function calls itself to solve smaller versions of the
same problem. The base case is level==0: draw the four small straight
segments. Otherwise, call koch() four times for the four new segments,
using level-1.

ALGORITHM:
1. Start.
2. Read recursion level n.
3. Set the initial line endpoints.
4. In koch(x1,y1,x2,y2,level), divide the segment into three equal parts.
5. Calculate the two division points.
6. Calculate the apex of an equilateral triangular bump using sin(60°).
7. If level>0, recursively call the function for all four new segments.
8. Otherwise draw the four resulting line segments using GL_LINES.
9. Clear the window, set colour and display the generated curve.
10. Stop when the graphics window is closed.

PROGRAM DESCRIPTION:
koch() calculates five points: the two original endpoints, two one-third
division points and the triangle apex. The constant sin(60°) is used to
calculate the height/direction of the equilateral bump. Each recursive
call processes one of the four smaller segments. At level zero, GL_LINES
draws the final straight pieces. display() starts the curve from
(50,300) to (600,300). init() sets a black background and 2D projection.
main() accepts the recursion depth and creates the GLUT window.

COMPILE COMMAND (Ubuntu):
sudo apt update
sudo apt install g++ freeglut3-dev
g++ koch7.cpp -o koch7 -lGL -lGLU -lglut

RUN COMMAND:
./koch7

SAMPLE INPUT:
Enter recursion level (0 to 6 recommended): 2

SAMPLE OUTPUT:
A window titled "Koch Curve" displays a white fractal curve on a black
background. At level 0 it is a straight line. At level 1 it has one
triangular bump. At level 2 every one of the four segments is subdivided
again, so the pattern has 16 small segments. Higher levels create more
detail. The number of final segments at level 2 is 4^2 = 16.

RESULT:
The Koch Curve fractal was generated successfully using recursion and
OpenGL line primitives.

CONCLUSION:
Thus, the Koch Curve algorithm was implemented successfully. The experiment
demonstrated recursion, self-similarity and the increase in fractal detail
with increasing recursion depth.

DETAILED HINGLISH VIVA EXPLANATION:
Fractal ek aisa pattern hota hai jisme chhote parts ka shape overall pattern
jaisa dikhta hai. Is property ko self-similarity bolte hain. Koch Curve
fractal ka classic example hai. Hum ek simple straight line se start karte
hain, phir us line ko teen equal parts me divide karte hain. Middle part ko
remove karke uski jagah equilateral triangle ka upper bump banate hain.
Is process se ek line ke badle chaar chhote line segments milte hain.

Ab recursion ka role aata hai. Har chhote segment par same rule dobara
apply hota hai. Function apne aap ko four times call karta hai, aur har
call recursion level ko one kam kar deta hai. Jab level zero hota hai,
to function aur call nahi karta; woh base case hai aur final straight
segments draw kar deta hai. Base case recursion ko infinite hone se rokta hai.

Level 0 par 1 straight segment hota hai. Level 1 par 4 segments, level 2
par 16, level 3 par 64. General formula 4^n hai. Isliye level badhane par
detail bahut fast badhti hai. Isi wajah se program me practical limit
rakhi gayi hai. Code me double coordinates use kiye hain taaki har division
par integer truncation se precision lose na ho. sin(60 degree) ki value
approximately 0.866 hai, jo triangular bump ka perpendicular component
calculate karne me help karti hai.

Ek important viva point: Koch Curve aur Koch Snowflake same nahi hain.
Koch Curve ek initial line segment par construction karta hai. Koch
Snowflake me equilateral triangle ki teeno sides par Koch Curve apply
kiya jaata hai. Is program me ek Koch Curve draw ho raha hai, complete
snowflake nahi.

VIVA QUESTIONS:
1. Fractal kya hota hai? Different scales par detail/self-similarity dikhane wala pattern.
2. Koch Curve kya hai? Recursive rule se banti fractal curve.
3. Self-similarity kya hai? Pattern ke parts ka whole jaise dikhna.
4. Recursion kya hai? Function ka khud ko call karna.
5. Base case kya hai? Condition jahan recursion stop hota hai.
6. Har segment kitne segments me badalta hai? Four.
7. Level n par segments kitne? 4^n.
8. sin 60 degree kyun use hota hai? Equilateral triangle bump ki height/direction calculate karne ke liye.
9. Koch Curve aur Snowflake me difference? Snowflake triangle ki all three sides par curve lagata hai.
===============================================================================

/*
ADDITIONAL DETAILED THEORY AND EXAMINATION NOTES

1. FRACTAL - MEANING
A fractal is a geometric pattern that can show similar structure at
different scales. A characteristic property is self-similarity: a small
part resembles the overall pattern. Fractals can be generated by
repeating a simple rule many times. They are useful for modelling or
representing complex-looking natural patterns.

2. KOCH CURVE CONSTRUCTION IN DETAIL
Start with one straight line segment. Divide it into three equal parts.
The middle third is replaced by two sides of an equilateral triangular
bump. The original middle segment is therefore replaced by three new
segments, while the two outer segments remain. The total becomes four
smaller segments. Apply the same rule to each of those four segments.
Repeating the rule creates the Koch Curve.

3. NUMBER OF SEGMENTS
At level 0: 1 segment.
At level 1: 4 segments.
At level 2: 4 x 4 = 16 segments.
At level 3: 4 x 4 x 4 = 64 segments.
At level n: 4^n segments.
Each new segment has one-third the length of the segment from the
previous level. If the initial length is L, then after n levels each
small segment has length L/(3^n). The total curve length is
L*(4/3)^n, so it increases as the recursion depth increases.

4. GEOMETRY OF THE TRIANGULAR BUMP
The middle third is replaced using an equilateral triangle. Each angle
of an equilateral triangle is 60 degrees. The perpendicular component
uses sin(60 degrees), whose approximate value is 0.8660254. The program
uses this constant to calculate the apex of the bump relative to the
segment direction. The coordinate calculations use double values to
preserve fractional positions during repeated subdivision.

5. RECURSION AND BASE CASE
Recursion means a function calls itself. In this program koch() calls
itself four times for the four new subsegments. Every call receives
level-1, so the recursion depth decreases. When level becomes zero,
the base case draws a straight segment and returns. Without a correct
base case, recursion would not stop and the program could fail.

6. KOCH CURVE VS KOCH SNOWFLAKE
The Koch Curve is generated from one initial line segment. The Koch
Snowflake starts with an equilateral triangle and applies the Koch Curve
construction to all three sides. This practical draws one Koch Curve,
not the complete three-sided snowflake. This distinction is important
in viva.

7. ROLE OF PROGRAM FUNCTIONS
koch(): divides the current segment into three parts, calculates the
triangular apex and recursively processes four smaller segments.
display(): clears the window, sets the colour and starts the curve from
the initial endpoints.
init(): sets the background and 2D coordinate system.
main(): accepts recursion level, validates the value, initializes GLUT
and creates the graphics window.

8. COMPLEXITY AND EFFECT OF LEVEL
The number of final segments grows exponentially as 4^n. Increasing the
level creates more detail but also increases the number of drawing
operations. For example, level 5 produces 1024 segments and level 6
produces 4096 segments. A high recursion level can therefore take more
time and generate a dense pattern. The program restricts the input range
to avoid excessive work.

9. ADVANTAGES AND LIMITATIONS
Advantages:
- Demonstrates recursion visually.
- Uses a simple repeated geometric rule.
- Shows self-similarity.
- Generates complex patterns from a basic line.
Limitations:
- Number of segments grows quickly.
- Floating-point coordinates may become very close together at high levels.
- The viewing window limits the visible region.
- The curve is a 2D fractal approximation displayed using raster lines.

10. APPLICATIONS OF FRACTALS
Fractal concepts are used in natural-scene modelling, terrain and
mountain generation, coastline representation, cloud-like patterns,
procedural textures, animation, visual effects and mathematical modelling.

11. DETAILED HINGLISH VIVA EXPLANATION
Fractal ko simple language me aisa pattern samjho jisme ek chhota part
overall pattern jaisa dikhta hai. Is property ko self-similarity bolte
hain. Koch Curve ek famous fractal hai. Hum ek straight line se start
karte hain. Line ko three equal parts me divide karte hain. Beech wale
part ko hata kar uski jagah equilateral triangle ka bump banate hain.
Ab original ek line ke badle four smaller line segments milte hain.

Ab same rule har segment par repeat karte hain. Isi repetition ko recursion
ke through implement kiya gaya hai. Function koch() apne aap ko four
times call karta hai, kyunki ek segment four subsegments me convert hota
hai. Har call me level one kam hota hai. Jab level zero ho jaata hai,
function aur recursive call nahi karta aur straight line draw kar deta
hai. Ye base case hai. Base case recursion ko stop karta hai.

Level 0 par 1 segment, level 1 par 4, level 2 par 16 aur level 3 par 64
segments hote hain. Formula 4^n hai. Har chhote segment ki length previous
segment ki one-third hoti hai. Isliye level badhne par curve me detail
increase hoti hai aur total segments bahut jaldi badhte hain. Isi wajah
se program me recursion level ki limit rakhi gayi hai.

Triangle bump calculate karne ke liye sin 60 degree ka use hota hai.
Equilateral triangle ke angles 60 degree hote hain aur sin 60 ka value
approximately 0.866 hai. Code me double datatype use kiya hai kyunki
subdivision ke baad coordinates decimal ho sakte hain. Agar int use
karte to fractional parts truncate ho jaate aur shape inaccurate ho sakti thi.

Ek common viva question hai: Koch Curve aur Koch Snowflake same hain kya?
Answer: nahi. Koch Curve ek initial line segment se banta hai. Koch
Snowflake me starting equilateral triangle ki teeno sides par Koch Curve
apply hota hai. Hamara program sirf ek curve draw karta hai.

12. VIVA - DETAILED ANSWERS
Q1. Fractal kya hota hai?
A. A geometric pattern that can show self-similar details at different
scales and is often generated by repeating a rule.

Q2. Koch Curve kya hai?
A. A fractal curve formed by repeatedly replacing the middle third of
each line segment with two sides of an equilateral triangular bump.

Q3. Self-similarity kya hai?
A. The property in which smaller parts resemble the overall structure.

Q4. Recursion kya hai?
A. A programming technique in which a function calls itself to solve
smaller versions of the same problem.

Q5. Base case kya hai?
A. The stopping condition of recursion. Here, level zero draws a line
segment and returns.

Q6. Har segment kitne segments me convert hota hai?
A. Four smaller segments.

Q7. Level n par total segments?
A. 4^n.

Q8. Har new segment ki length?
A. One-third of the previous segment's length.

Q9. sin 60 degree kyun use hota hai?
A. It helps calculate the perpendicular component/height of the
equilateral triangular bump.

Q10. Koch Curve aur Koch Snowflake me difference?
A. Koch Curve starts from one line segment; Koch Snowflake applies the
curve construction to all three sides of an initial equilateral triangle.

Q11. Recursion level badhane par kya hota hai?
A. The pattern becomes more detailed and the number of segments grows
by a factor of four at each level.

Q12. Program me double kyun use kiya?
A. Repeated subdivision produces fractional coordinates, so double
preserves more precision than integer coordinates.
*/
*/
