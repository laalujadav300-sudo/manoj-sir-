#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

int lineStyle;
float X1, Y1, X2, Y2;

// Digital Differential Analyzer (DDA) Line Drawing Algorithm
void drawLineDDA(float x1, float y1, float x2, float y2, int style)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = max(fabs(dx), fabs(dy));
    int steps = static_cast<int>(ceil(length));

    float xIncrement = (steps == 0) ? 0.0f : dx / steps;
    float yIncrement = (steps == 0) ? 0.0f : dy / steps;
    float x = x1, y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; ++i)
    {
        if (style == 1 || (style == 2 && i % 4 == 0) ||
            (style == 3 && i % 15 < 10))
            glVertex2i(static_cast<int>(round(x)), static_cast<int>(round(y)));

        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawLineDDA(X1, Y1, X2, Y2, lineStyle);
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
    glutCreateWindow("DDA Line Drawing");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL: DDA (DIGITAL DIFFERENTIAL ANALYZER) LINE DRAWING ALGORITHM
===============================================================================

1. AIM
To implement the Digital Differential Analyzer (DDA) Line Drawing Algorithm using C++ and OpenGL to generate straight raster lines in simple, dotted and dashed styles, and to understand incremental line generation.

2. OBJECTIVES
- To understand raster graphics and line generation.
- To accept the two endpoints of a line segment.
- To calculate dx, dy, number of steps and coordinate increments.
- To generate intermediate points using incremental calculations.
- To plot the nearest pixels between the endpoints.
- To display simple, dotted and dashed line patterns.

3. ASSOCIATED TASKS
1. Write a program to draw a line using the DDA line drawing algorithm.
2. Accept line endpoint coordinates from the user.
3. Calculate the intermediate points using x and y increments.
4. Display the generated line using OpenGL or another graphics library.
5. Observe different line styles and understand raster line generation.

4. SOFTWARE AND HARDWARE REQUIREMENTS
Software:
- C++ compiler such as GCC/G++ or MinGW.
- OpenGL, GLU and GLUT/freeglut libraries.
- Windows or Linux operating system with graphics support.
Hardware:
- Computer with display, keyboard and mouse.

5. THEORY
A point is the fundamental element of picture representation. Two points, (x1, y1) and (x2, y2), specify a line. A mathematical line extends infinitely in both directions, while a line segment is the portion between two endpoints. In computer graphics, line generation or vector generation is the process of turning on suitable pixels to represent a line segment on the screen.

A good line-drawing algorithm should make the line appear straight, start and end accurately, maintain consistent brightness along its length and draw rapidly.

DDA stands for Digital Differential Analyzer. It is an incremental scan-conversion algorithm. It calculates intermediate positions between the endpoints by repeatedly adding fixed increments to the x and y coordinates.

For endpoints (x1, y1) and (x2, y2):
dx = x2 - x1
dy = y2 - y1
steps = max(|dx|, |dy|)
xIncrement = dx / steps
yIncrement = dy / steps

The algorithm starts at (x1, y1), adds xIncrement and yIncrement in each iteration, and plots the nearest integer pixel. Floating-point values are used during calculation, so rounding is required when plotting. In this program, the number of steps is rounded upward using ceil(max(|dx|, |dy|)). A zero-length line is handled separately by using zero increments.

LINE STYLES
- Simple line: every generated point is plotted.
- Dotted line: one point is plotted at every fourth iteration.
- Dashed line: ten points are plotted in each group of fifteen iterations.

EXEMPLARS AND UTILITY
Line drawing algorithms are fundamental in raster graphics. They are used to display line segments in computer graphics applications, diagrams, charts, drawing tools, user interfaces and other graphical applications.

6. ALGORITHM
1. Start the program.
2. Read the starting point (x1, y1).
3. Read the ending point (x2, y2).
4. Read the required line style.
5. Calculate dx = x2 - x1 and dy = y2 - y1.
6. Calculate steps = ceil(max(|dx|, |dy|)).
7. Calculate xIncrement = dx / steps and yIncrement = dy / steps. If steps is zero, use zero increments.
8. Initialize x = x1 and y = y1.
9. Repeat from i = 0 to steps:
   a. Check the selected line-style condition.
   b. If required, plot the rounded coordinates (x, y).
   c. Add xIncrement to x and yIncrement to y.
10. Display the generated line in the OpenGL window.
11. Stop.

7. PROCEDURE
1. Open a C++ IDE or terminal configured with OpenGL and GLUT.
2. Save the program using the required .cpp filename.
3. Compile it with the OpenGL, GLU and GLUT/freeglut libraries.
4. Run the executable.
5. Enter the starting and ending coordinates.
6. Enter 1 for simple, 2 for dotted or 3 for dashed line.
7. Observe the output in the graphics window.
8. Repeat with different endpoints to study different slopes and directions.

8. IMPORTANT FUNCTIONS
- fabs(): returns the absolute value of a floating-point number.
- max(): selects the larger of the two coordinate differences.
- ceil(): rounds a value upward to the next integer.
- round(): rounds a calculated coordinate to the nearest integer.
- glBegin(GL_POINTS) and glEnd(): define a group of point primitives.
- glVertex2i(): specifies the integer pixel coordinate.
- glFlush(): sends drawing commands for execution.
- gluOrtho2D(): establishes the 2D coordinate system.

9. SAMPLE INPUT AND OUTPUT
Sample input:
Enter Start Point (x1 y1): 50 50
Enter End Point (x2 y2): 400 300
Select Style (1: Simple, 2: Dotted, 3: Dashed): 1

Expected graphics:
A white straight line is displayed from (50,50) to (400,300) on a black background. Style 2 displays a dotted line and style 3 displays a dashed line. The output is a raster approximation of the mathematical line.

10. RESULT
The DDA Line Drawing Algorithm was implemented successfully. The program generated a line between the entered endpoints by calculating incremental coordinate values and plotting the nearest pixels in simple, dotted or dashed style.

11. CONCLUSION
Thus, the DDA Line Drawing Algorithm was studied and executed successfully. The practical demonstrates raster line generation, calculation of dx and dy, selection of the number of steps, calculation of x and y increments, and plotting of rounded pixel positions. The different line styles were also understood.

11. PRECAUTIONS / LIMITATIONS
- Enter coordinates within the graphics window for a fully visible line.
- Choose only style 1, 2 or 3.
- Floating-point calculations can introduce rounding errors.
- DDA uses incremental floating-point calculations and is generally less
  efficient than integer-based Bresenham for basic raster line generation.
- The zero-length line case is handled separately by the program.

12. VIVA-VOCE: HINGLISH EXPLANATION AND QUESTIONS
Q1. DDA ka full form kya hai?
Ans. DDA ka full form Digital Differential Analyzer hai.

Q2. DDA algorithm kya karta hai?
Ans. DDA ek line drawing algorithm hai. Ye do endpoints ke beech intermediate
points calculate karta hai aur un points ko pixels ke form mein screen par
plot karta hai.

Q3. DDA mein steps kaise calculate hote hain?
Ans. Steps = max(|dx|, |dy|). Matlab x aur y direction mein jo difference
bada hai, uske according total increments decide hote hain. Isse har step
mein coordinate movement ek pixel ke aas-paas rehta hai.

Q4. xIncrement aur yIncrement kya hain?
Ans. xIncrement = dx/steps aur yIncrement = dy/steps. Har iteration mein x
aur y mein itna value add hota hai. Is tarah line ke next point tak pahunchte
hain.

Q5. Coordinates round kyun karte hain?
Ans. Calculated x-y values floating-point ho sakti hain, jaise 10.4 ya 15.7.
Screen par pixel position integer hoti hai, isliye nearest integer pixel
select karne ke liye round() use karte hain.

Q6. Agar dx zero ho toh?
Ans. Vertical line hoti hai. xIncrement zero hoga aur y coordinate change
hoga. Agar dy zero ho toh horizontal line hoti hai aur yIncrement zero hoga.

Q7. Agar dono endpoints same hon?
Ans. dx aur dy dono zero honge. Steps zero hoga. Isliye division by zero avoid
karne ke liye program zero increments set karta hai aur ek point plot karta
hai.

Q8. DDA aur Bresenham mein difference?
Ans. DDA floating-point increments aur rounding use karta hai. Bresenham
integer decision parameter use karta hai. Bresenham generally faster hota hai
aur rounding calculations avoid karta hai.

Q9. Slope ka formula?
Ans. m = (y2-y1)/(x2-x1), jab denominator zero na ho. DDA mein direct slope
calculate karne ke bajay dx, dy aur steps se increments nikalte hain.

Q10. Dotted line kaise draw hoti hai?
Ans. Har point draw nahi karte. Is code mein i % 4 == 0 hone par point draw
hota hai, baaki points skip hote hain.

Q11. Dashed line kaise draw hoti hai?
Ans. i % 15 < 10 condition pehle 10 iterations ke points draw karti hai aur
next 5 skip karti hai. Isse dash aur gap ka pattern banta hai.

Q12. ceil() kyun use kiya?
Ans. Maximum coordinate difference fractional ho sakta hai. ceil() usko
next integer tak round up karta hai, taaki endpoint tak sufficient steps
mil saken.

Q13. OpenGL mein GL_POINTS ka use?
Ans. GL_POINTS ka matlab har vertex ko individual point/pixel ke roop mein
render karna. DDA mein hum line ko points se approximate karte hain.

Q14. DDA ki limitation kya hai?
Ans. Isme floating-point addition aur rounding hoti hai. Long lines mein
rounding error accumulate ho sakta hai aur integer-based algorithms ke
comparison mein computation relatively slow ho sakta hai.

Q15. Practical ka output kya hai?
Ans. Black background par input endpoints ke beech white line draw hoti hai.
Style 1 simple, style 2 dotted aur style 3 dashed line display karta hai.
===============================================================================


13. DETAILED HINGLISH EXPLANATION (THEORY + CODE WALKTHROUGH)

A. DDA ka basic concept
DDA ka full form Digital Differential Analyzer hai. Ye line drawing ka
incremental algorithm hai. Screen pixels ka grid hota hai, isliye mathematical
line ke beech ke points ko nearest integer pixel par plot karna padta hai.
DDA pehle calculate karta hai ki total kitne steps chahiye, phir har step par
x aur y mein fixed increment add karta hai. Isse line ke points gradually
milte hain.

B. dx, dy aur steps
Maan lo endpoints (x1,y1) aur (x2,y2) hain. dx=x2-x1 horizontal difference
hai aur dy=y2-y1 vertical difference hai. steps=max(|dx|,|dy|) liya jata
hai, taaki major axis par har step approximately ek pixel move ho. Agar
horizontal distance zyada hai toh x dominant hai; vertical distance zyada
hai toh y dominant hai. ceil() length ko next integer tak round-up karta hai,
so fractional distance ke liye bhi enough steps milte hain.

C. Increment ka formula
xIncrement=dx/steps aur yIncrement=dy/steps. Har iteration mein x mein
xIncrement aur y mein yIncrement add hota hai. Isliye line ka direction aur
slope maintain rehta hai. Negative dx ya dy hone par increment negative ho
sakta hai, jisse line reverse direction mein draw hoti hai. Jab endpoints
same honge toh steps zero hota hai; code special condition se increments ko
0 set karta hai, taaki division by zero na ho aur ek point plot ho sake.

D. Rounding kyun hoti hai?
DDA ke x,y floating-point values ho sakte hain, lekin raster screen par pixel
coordinate integer hota hai. Isliye round(x), round(y) se nearest integer
pixel choose kiya jata hai. Rounding se line visually straight dikhti hai,
lekin repeated floating-point additions aur rounding ke kaaran small error
accumulate ho sakta hai.

E. Code ka flow
 drawLineDDA() endpoints leta hai. dx/dy aur steps calculate karta hai.
Initial x,y start point se set hote hain. GL_POINTS mode start hota hai.
for loop i=0 se steps tak chalta hai, yani starting point aur ending point
include hote hain. Style condition true ho toh rounded coordinate par
 glVertex2i() call hota hai. Har iteration ke end par x/y increments add
hote hain. glEnd() points batch close karta hai aur glFlush() drawing bhejta
hai.

F. Line styles
style 1: condition always true, har calculated point draw hota hai; simple
line banti hai. style 2: i%4==0 hone par point draw hota hai; beech ke points
skip hote hain, isliye dotted line. style 3: i%15<10 hone par 15-step group
mein pehle 10 points draw aur agle 5 skip hote hain; dashed pattern banta hai.
Ye styles line ke mathematical path ko nahi badalte, sirf plotted pixels
select karte hain.

G. OpenGL functions ka role
- glClear(): display refresh par purana frame clear karta hai.
- glColor3f(1,1,1): white drawing colour set karta hai (RGB 0.0 se 1.0).
- glBegin(GL_POINTS): points draw karne ka mode start karta hai.
- glVertex2i(): integer x-y location par point deta hai.
- gluOrtho2D(0,500,0,500): 2D coordinate mapping set karta hai.
- glutDisplayFunc(display): window ko redraw karne ke liye callback set karta
  hai.

H. DDA vs Bresenham
DDA floating point increments aur rounding use karta hai, isliye samajhna
simple hai. Bresenham integer decision parameter use karta hai aur generally
faster/more efficient hota hai. DDA ka main advantage straightforward formula
hai; limitation floating-point calculations aur rounding error hai.

I. Example
Input start (50,50), end (150,100). dx=100, dy=50, steps=max(100,50)=100.
X increment=1 aur Y increment=0.5. Starting at (50,50), each step x ek se
badhta hai aur y aadha se. Rasterization ke waqt round() nearest integer
pixel select karta hai. End tak line (150,100) par pahunchti hai.

14. COMPILATION AND EXECUTION
Linux (FreeGLUT installed):
 g++ dda2.cpp -o dda2 -lglut -lGLU -lGL
 ./dda2
Windows MinGW (FreeGLUT configured):
 g++ dda2.cpp -o dda2.exe -lfreeglut -lopengl32 -lglu32
 dda2.exe

15. SAMPLE INPUT / OUTPUT
Enter Start Point (x1 y1): 50 50
Enter End Point (x2 y2): 150 100
Select Style (1: Simple, 2: Dotted, 3: Dashed): 1
Output: Black graphics window par white straight line draw hoti hai. Style
2 par dotted aur style 3 par dashed pattern dikhta hai.

*/
