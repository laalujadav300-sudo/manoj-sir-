#include <GL/glut.h>
#include <iostream>
using namespace std;

const float xmin = 150.0f, ymin = 150.0f;
const float xmax = 350.0f, ymax = 350.0f;

float px1, py1, px2, py2;
float originalX1, originalY1, originalX2, originalY2;
bool clipped = false;
bool processed = false;

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

int computeCode(float x, float y)
{
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;

    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

void cohenSutherlandClip()
{
    float x, y;
    float x1 = originalX1, y1 = originalY1;
    float x2 = originalX2, y2 = originalY2;
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true)
    {
        if (code1 == 0 && code2 == 0)
        {
            accept = true;
            break;
        }
        if (code1 & code2)
            break;

        int codeOut = (code1 != 0) ? code1 : code2;

        if (codeOut & TOP)
        {
            if (y2 == y1) break;
            x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
            y = ymax;
        }
        else if (codeOut & BOTTOM)
        {
            if (y2 == y1) break;
            x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
            y = ymin;
        }
        else if (codeOut & RIGHT)
        {
            if (x2 == x1) break;
            y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
            x = xmax;
        }
        else
        {
            if (x2 == x1) break;
            y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
            x = xmin;
        }

        if (codeOut == code1)
        {
            x1 = x; y1 = y;
            code1 = computeCode(x1, y1);
        }
        else
        {
            x2 = x; y2 = y;
            code2 = computeCode(x2, y2);
        }
    }

    if (accept)
    {
        px1 = x1; py1 = y1;
        px2 = x2; py2 = y2;
        clipped = true;
    }
    else
    {
        px1 = originalX1; py1 = originalY1;
        px2 = originalX2; py2 = originalY2;
        clipped = false;
    }
    processed = true;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
    glEnd();

    if (processed && clipped)
        glColor3f(0.0f, 1.0f, 0.0f); // Accepted/clipped line
    else
        glColor3f(1.0f, 0.0f, 0.0f); // Original or rejected line

    glBegin(GL_LINES);
        glVertex2f(px1, py1);
        glVertex2f(px2, py2);
    glEnd();
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        cohenSutherlandClip();
        glutPostRedisplay();
    }
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

int main(int argc, char** argv)
{
    cout << "Enter line coordinates (x1 y1 x2 y2): ";
    cin >> originalX1 >> originalY1 >> originalX2 >> originalY2;
    px1 = originalX1; py1 = originalY1;
    px2 = originalX2; py2 = originalY2;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL: COHEN-SUTHERLAND LINE CLIPPING ALGORITHM
===============================================================================

1. AIM
To implement the Cohen-Sutherland Line Clipping Algorithm using C++ and OpenGL for clipping line segments against a rectangular clipping window, and to understand completely visible, completely invisible and partially visible line segments.

2. OBJECTIVES
- To understand the concept and purpose of clipping in computer graphics.
- To create a rectangular clipping window.
- To assign a 4-bit region code to each line endpoint.
- To identify whether a line is accepted, rejected or partially clipped.
- To calculate intersection points with the edges of the clipping window.
- To display only the visible portion of a line segment.

3. ASSOCIATED TASKS
1. Create a rectangular clipping window on the screen.
2. Draw line segments at different positions relative to the clipping window.
3. Implement Cohen-Sutherland clipping using region codes.
4. Identify completely visible, completely invisible and partially visible lines.
5. Calculate intersection points and display the accepted line portion.

4. SOFTWARE AND HARDWARE REQUIREMENTS
Software:
- C++ compiler such as GCC/G++ or MinGW.
- OpenGL, GLU and GLUT/freeglut libraries.
- Windows or Linux operating system with graphics support.
Hardware:
- Computer with display, keyboard and mouse.

5. THEORY
Clipping is the process of removing the portions of a graphical object that lie outside a specified viewing window. The clipping window defines the area that should remain visible. Only the portion of a line, point or other object inside the window is displayed; the outside portion is discarded.

The Cohen-Sutherland algorithm is a line-clipping algorithm for a rectangular window. In this program, the clipping window is defined by:
xmin = 150, ymin = 150, xmax = 350, ymax = 350.

The algorithm divides the area around the rectangular window into regions and assigns a 4-bit outcode to each endpoint. A point inside the window has code 0000. The four region bits represent LEFT, RIGHT, BOTTOM and TOP:
- LEFT = 0001
- RIGHT = 0010
- BOTTOM = 0100
- TOP = 1000

The program uses bitwise operations on the outcodes:
1. TRIVIAL ACCEPTANCE: If both endpoint codes are 0000, the complete line lies inside the window and is accepted.
2. TRIVIAL REJECTION: If the bitwise AND of the two endpoint codes is non-zero, both endpoints lie outside in at least one common region, so the line is rejected.
3. PARTIAL CLIPPING: If the AND result is zero but one or both endpoints are outside, an outside endpoint is selected. Its intersection with the relevant window edge is calculated. The endpoint is replaced by the intersection point, its code is recalculated, and the test is repeated.

Intersection calculations use the line equation. For example, at the left or right edge, x is fixed to xmin or xmax and the corresponding y is calculated. At the top or bottom edge, y is fixed to ymax or ymin and the corresponding x is calculated.

EXEMPLARS AND UTILITY
Line clipping is useful in image editing, video games, graphical user interfaces, digital maps, animation and simulation software. It avoids displaying portions of lines that lie outside the required viewing region.

6. ALGORITHM
1. Start and read the endpoints (x1, y1) and (x2, y2).
2. Store the original endpoints.
3. Calculate a 4-bit region code for each endpoint.
4. If both codes are zero, accept the line.
5. Otherwise, perform the bitwise AND of the two codes.
6. If the AND result is non-zero, reject the line.
7. If the AND result is zero, select an endpoint whose code is non-zero.
8. Identify the boundary indicated by its outcode: TOP, BOTTOM, RIGHT or LEFT.
9. Calculate the intersection point with that boundary.
10. Replace the outside endpoint with the intersection point and recalculate its code.
11. Repeat the tests until the line is accepted or rejected.
12. Display the rectangular window and the resulting line.
13. Use the left mouse button to perform clipping, as provided by the program.
14. Stop.

7. PROCEDURE
1. Open a C++ IDE or terminal configured with OpenGL and GLUT.
2. Save the program using the required .cpp filename.
3. Compile it with the OpenGL, GLU and GLUT/freeglut libraries.
4. Run the executable.
5. Enter four coordinates in the order x1 y1 x2 y2.
6. The graphics window displays the rectangular clipping window and original line.
7. Click the left mouse button to perform clipping.
8. Observe the accepted line portion or the rejection of a fully invisible line.

8. IMPORTANT FUNCTIONS AND OPERATORS
- computeCode(): calculates the 4-bit region code of a point.
- Bitwise OR (|): sets the appropriate region bit when a point is outside a boundary.
- Bitwise AND (&): checks whether the two endpoints share an outside region.
- glutMouseFunc(): registers the mouse callback.
- glutPostRedisplay(): requests the display callback to redraw the scene.
- GL_LINES: draws a line segment between two vertices.
- gluOrtho2D(): defines the 2D viewing coordinate system.

9. SAMPLE INPUT AND OUTPUT
Sample input:
Enter line coordinates (x1 y1 x2 y2): 50 250 450 250

Expected graphics:
- A rectangular clipping window with corners (150,150) and (350,350).
- Before clipping: the original line is displayed.
- After clipping: only the portion from (150,250) to (350,250) is accepted and displayed inside the window.

Another example:
Input: 50 50 100 100
Both endpoints lie in the bottom-left outside region. Their common outcode has a non-zero AND result, so the line is trivially rejected.

10. RESULT
The Cohen-Sutherland Line Clipping Algorithm was implemented successfully. The program assigns region codes and identifies whether the entered line is completely visible, completely invisible or partially visible against the rectangular clipping window.

11. CONCLUSION
Thus, the Cohen-Sutherland Line Clipping Algorithm was studied and executed successfully. The practical demonstrates rectangular-window clipping, 4-bit region coding, trivial acceptance, trivial rejection and intersection-point calculation. Only the visible portion of a partially intersecting line is retained.

11. PRECAUTIONS / LIMITATIONS
- Enter coordinates in the same 2D coordinate system as the window.
- The clipping rectangle is fixed in this program.
- Lines outside the graphics viewport may not be visible before clipping.
- The program uses a rectangular clipping window.
- A line parallel to a clipping edge must be handled carefully to avoid
  division by zero. The code includes checks for such cases.

12. VIVA-VOCE: HINGLISH EXPLANATION AND QUESTIONS
Q1. Clipping kya hoti hai?
Ans. Clipping ek process hai jisme viewing window ke bahar wale graphical
object ke parts remove ya hide kiye jaate hain. Sirf window ke andar ka visible
portion display kiya jata hai.

Q2. Cohen-Sutherland algorithm kis liye use hota hai?
Ans. Ye 2D graphics mein line segment ko rectangular clipping window ke
against clip karne ke liye use hota hai.

Q3. Region code kya hota hai?
Ans. Har endpoint ko 4-bit code diya jata hai. Ye batata hai ki point window
ke left, right, top, bottom mein hai ya andar. Inside point ka code 0000 hota
hai.

Q4. Four bits ka meaning kya hai?
Ans. LEFT bit 0001, RIGHT bit 0010, BOTTOM bit 0100 aur TOP bit 1000 hai.
Ek point corner ke bahar ho toh ek se zyada bits set ho sakte hain.

Q5. Trivial acceptance kya hai?
Ans. Jab dono endpoints ka region code zero hota hai, toh dono points window
ke andar hain. Isliye poori line accept ho jaati hai; intersection calculate
karne ki zaroorat nahi hoti.

Q6. Trivial rejection kya hai?
Ans. Jab dono region codes ka bitwise AND non-zero ho, toh dono endpoints
ek common outside region mein hain. Is condition mein line clipping window ko
cross nahi kar sakti, isliye reject hoti hai.

Q7. Bitwise AND kyun use hota hai?
Ans. AND se check hota hai ki dono codes mein koi common outside bit set hai
ya nahi. Common bit milne par line trivially reject ho jaati hai.

Q8. Partial clipping kya hai?
Ans. Jab ek ya dono endpoints outside hote hain lekin line rectangle ko cross
karti hai, toh outside endpoint ko boundary intersection se replace karte
hain. Repeat karne ke baad inside segment milta hai.

Q9. Clipping window kya hai?
Ans. Screen ka selected rectangular area jiske andar ka content visible
rakhna hai. Is program mein x aur y limits 150 se 350 hain.

Q10. Intersection ka formula kya hai?
Ans. Top ya bottom edge ke liye x = x1 + (x2-x1)*(yEdge-y1)/(y2-y1).
Left ya right edge ke liye y = y1 + (y2-y1)*(xEdge-x1)/(x2-x1).

Q11. Agar dono endpoints inside hain?
Ans. Dono codes 0000 honge aur line directly accept ho jayegi.

Q12. Agar line poori tarah bahar hai?
Ans. Agar endpoint codes ka AND non-zero hai toh line reject ho jaati hai.
Kuch cases mein iterative intersection checks ke baad bhi reject hoti hai.

Q13. Original coordinates store kyun kiye?
Ans. Clipping ke dauran endpoints update hote hain. Original values preserve
karne se rejected line ko galti se partially modified state mein display
karne se bachaya ja sakta hai.

Q14. glutPostRedisplay() kya karta hai?
Ans. Ye GLUT ko request karta hai ki display callback dobara execute ho aur
updated graphics window redraw ho.

Q15. Practical ka output kya hai?
Ans. White rectangular window aur line display hoti hai. Mouse click par
algorithm line ko clip karta hai. Accepted clipped portion green dikhaya jata
hai; rejected line red rehti hai.
===============================================================================


13. DETAILED HINGLISH EXPLANATION (THEORY + CODE WALKTHROUGH)

A. Clipping ka matlab
Computer graphics mein kabhi line ya object ka kuch part display window ke
bahar hota hai. Clipping ka matlab hai display window ke bahar wale portion
ko remove karna aur sirf andar wala visible part rakhna. Cohen-Sutherland
algorithm rectangular clipping window ke liye line clipping technique hai.
Is program ki clipping window xmin=150, ymin=150, xmax=350, ymax=350 hai.

B. Region codes / outcodes
Har endpoint ko 4-bit region code diya jata hai. INSIDE=0000 ka matlab point
window ke andar ya boundary par hai. LEFT, RIGHT, BOTTOM, TOP bits batate
hain ki point kis side outside hai. Code mein constants bit values 1,2,4,8
hain. computeCode(x,y) x/y ko limits se compare karke relevant bit set karta
hai. Ek point ek se zyada side ke bahar ho toh multiple bits set ho sakte
hain, jaise top-left corner.

C. Trivial acceptance aur rejection
Algorithm ke do fast decisions yaad rakho:
1) Agar code1==0 aur code2==0, dono endpoints inside hain; poori line accept.
2) Agar (code1 & code2) non-zero hai, dono endpoints kisi common outside
   region mein hain; line completely outside hai, isliye reject.
3) Baaki case mein line window ko cross kar sakti hai. Outside endpoint
   choose karke boundary intersection calculate karte hain.

D. Intersection kaise calculate hota hai?
Agar selected endpoint TOP ke bahar hai toh y=ymax set karke line equation
se x calculate hota hai: x=x1+(x2-x1)*(ymax-y1)/(y2-y1). BOTTOM ke liye
y=ymin. RIGHT ke liye x=xmax set karke y calculate hota hai. LEFT ke liye
x=xmin set karke y calculate hota hai. Ye formulas line ke slope/parametric
relation se aate hain. Horizontal ya vertical division-by-zero cases ko
code y2==y1 ya x2==x1 check karke break karta hai.

E. Iteration ka flow
codeOut decide karta hai kaunsa endpoint outside hai. Intersection point
nikalne ke baad us endpoint ko intersection se replace karte hain. Phir
computeCode dobara chalta hai, kyunki naya point ab inside ho sakta hai ya
kisi doosri side se outside ho sakta hai. while loop tab tak repeat hota hai
jab tak line accept ya reject na ho jaye. Ye repeated clipping process hai.

F. Is program mein display aur mouse
Window mein white rectangle clipping window show karta hai. Original line
input values se draw hoti hai. Program ke mouse callback par click karne ke
baad clipping calculation run hoti hai. Accepted portion green colour mein
show hota hai; rejected line ke liye clipped flag false hota hai aur original
line red display ho sakti hai (display code ke according). processed flag
batata hai ki user ne clipping action perform kiya hai ya nahi. px1,py1,px2,
py2 clipped endpoints store karte hain.

G. Ek example
Maan lo line ka first point rectangle ke left side outside hai aur second
point right side outside hai, lekin line rectangle ko cross karti hai. Dono
outcodes ka AND zero ho sakta hai, isliye trivial rejection nahi hota.
Algorithm ek outside endpoint leta hai, jis side ka bit set hai us boundary
par intersection find karta hai, endpoint update karta hai, aur repeat karta
hai. Aakhir mein dono endpoints rectangle ke andar aa jaate hain; sirf beech
ka segment display hota hai.

H. Important terms
- Clipping window: rectangular visible area.
- Outcode: endpoint ki position batane wala 4-bit code.
- Trivial accept: dono endpoints inside.
- Trivial reject: dono endpoints same outside half-plane mein.
- Intersection: line aur clipping boundary ka meeting point.
- Bitwise AND (&): common outside-region bit check karta hai.
- Bitwise OR (|): outcode mein region bit set karta hai.

I. Algorithm ki strength aur limitation
Outcodes ki wajah se many lines ko quickly accept/reject kiya ja sakta hai.
Ye rectangular clipping window ke liye suitable hai. Complex polygonal
clipping ke liye different algorithms ki zaroorat ho sakti hai. Floating
point calculations ki wajah se boundary ke paas precision issues aa sakte
hain. Input endpoints ko graphics window ke useful coordinate range mein
rakhna chahiye.

14. COMPILATION AND EXECUTION
Linux (FreeGLUT installed):
 g++ cohen.cpp -o cohen -lglut -lGLU -lGL
 ./cohen
Windows MinGW (FreeGLUT configured):
 g++ cohen.cpp -o cohen.exe -lfreeglut -lopengl32 -lglu32
 cohen.exe

15. SAMPLE OUTPUT
Graphics window: white rectangular clipping window aur input line show hoti
hai. Mouse click karne par clipping hoti hai. Accepted segment green colour
mein show hota hai; completely rejected line ka visible clipped segment nahi
hota. Exact line position input coordinates par depend karti hai.

*/
