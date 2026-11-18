#include <GL/glut.h>
#include <iostream>
using namespace std;

int Xc, Yc, R;

// Plots all eight symmetric points for one calculated (x,y) pair.
void plot8Pixels(int xc, int yc, int x, int y)
{
    glVertex2i(xc+x, yc+y);
    glVertex2i(xc+y, yc+x);
    glVertex2i(xc+y, yc-x);
    glVertex2i(xc+x, yc-y);
    glVertex2i(xc-x, yc-y);
    glVertex2i(xc-y, yc-x);
    glVertex2i(xc-y, yc+x);
    glVertex2i(xc-x, yc+y);
}

void drawCircleMidpoint(int xc, int yc, int r)
{
    int x=0, y=r;
    float d=1.25f-r; // Initial decision parameter used in the lab manual.

    glBegin(GL_POINTS);
    while (x<y) {
        plot8Pixels(xc,yc,x,y);
        if (d<0) {
            d = d + 2*x + 1;
            x++;
        } else {
            d = d + 2*x - 2*y + 1;
            x++; y--;
        }
    }
    if (x==y) plot8Pixels(xc,yc,x,y);
    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f,1.0f,1.0f);
    drawCircleMidpoint(Xc,Yc,R);
}

void init()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    gluOrtho2D(0.0,500.0,0.0,500.0);
}

int main(int argc, char** argv)
{
    cout << "Enter Center of Circle (Xc Yc): ";
    cin >> Xc >> Yc;
    cout << "Enter Radius of Circle (R): ";
    cin >> R;

    if (!cin || R<=0 || Xc-R<0 || Xc+R>500 || Yc-R<0 || Yc+R>500) {
        cerr << "Invalid input. Use positive radius and keep the circle inside 0..500.\n";
        return 1;
    }

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Midpoint Circle Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 3 : MIDPOINT CIRCLE DRAWING ALGORITHM
===============================================================================

AIM:
To implement the Midpoint Circle Drawing Algorithm in C++ using OpenGL
and display a circle for a user-provided centre and radius.

OBJECTIVES:
1. To understand raster circle generation.
2. To use a decision parameter to select the next pixel.
3. To apply eight-way symmetry to reduce calculations.
4. To display a complete circle using OpenGL.

THEORY:
A circle is a closed 2D shape whose every point is at a fixed distance
(radius r) from its centre (xc,yc). Its equation is:
(x-xc)^2 + (y-yc)^2 = r^2

A raster screen cannot draw a mathematically continuous curve directly.
The midpoint algorithm chooses pixels that approximate the circle.
It calculates points in only one octant and reflects them into the
remaining seven octants. This is called 8-way symmetry.

For a circle centred at origin, the eight points are:
(x,y), (y,x), (y,-x), (x,-y), (-x,-y), (-y,-x), (-y,x), (-x,y).
For a centre (xc,yc), add xc to every x coordinate and yc to every y.

INITIAL VALUES:
x = 0
y = r
d = 1.25 - r

DECISION:
- If d < 0, choose the pixel directly to the right:
  d = d + 2x + 1; then x = x + 1.
- Otherwise, choose the diagonal-down/right pixel:
  d = d + 2x - 2y + 1; then x = x + 1 and y = y - 1.
The old x and y are used in the update expression. Continue until x
reaches or crosses y. The equality point is plotted once at the end.

ALGORITHM:
1. Start.
2. Read centre coordinates xc,yc and radius r.
3. Set x=0, y=r and d=1.25-r.
4. While x<y:
   a. Plot all eight symmetric points.
   b. If d<0, update d=d+2x+1 and increment x.
   c. Otherwise update d=d+2x-2y+1, increment x and decrement y.
5. If x==y, plot the final symmetric point.
6. Flush the OpenGL commands.
7. Stop.

PROGRAM DESCRIPTION:
plot8Pixels() plots the eight reflected positions around the centre.
drawCircleMidpoint() maintains x, y and the decision parameter d. Only
one octant is calculated; the helper function draws the other seven
points. display() clears the screen, sets the circle colour to white and
calls the circle routine. init() creates a black background and 2D
coordinate range. main() accepts the centre and radius, validates that
the circle fits inside the 500 by 500 window, and starts GLUT.

COMPILE COMMAND (Ubuntu):
sudo apt update
sudo apt install g++ freeglut3-dev
g++ Midpoint_Circle3.cpp -o Midpoint_Circle3 -lGL -lGLU -lglut

RUN COMMAND:
./Midpoint_Circle3

SAMPLE INPUT:
Enter Center of Circle (Xc Yc): 250 250
Enter Radius of Circle (R): 100

SAMPLE OUTPUT:
A graphics window titled "Midpoint Circle Algorithm" opens. It shows a
white circle centred approximately at (250,250), with radius 100 pixels,
on a black background. No further terminal output is expected.

RESULT:
The circle was generated using the Midpoint Circle Drawing Algorithm and
eight-way symmetry.

CONCLUSION:
Thus, the Midpoint Circle Algorithm was implemented successfully. The
decision parameter selected the next raster pixel and symmetry reduced
the calculations by generating eight points for each calculated point.

DETAILED HINGLISH VIVA EXPLANATION:
Circle draw karne ke liye har angle ka sin-cos calculate karna zaroori nahi.
Midpoint algorithm smartly sirf ek octant calculate karta hai. Circle ke
shape me symmetry hoti hai: agar ek point top-right side par mila, to uske
mirror points baaki directions me bhi mil jaate hain. Ek point se total
eight symmetric points milte hain. Isi ko 8-way symmetry kehte hain.

x=0 aur y=radius se start karte hain, kyunki topmost point centre se radius
distance par hota hai. Decision parameter d batata hai ki next pixel right
side wala lena hai ya diagonal wala. d negative hai to midpoint circle ke
andar hota hai, isliye right pixel choose karte hain. d zero ya positive
hai to diagonal pixel choose karte hain aur y ko bhi ek kam karte hain.
Har iteration me x badhta hai, aur kabhi y kam hota hai. Jab x, y ke barabar
ho jaata hai, hum ek octant ke end par pahunch jaate hain. Uske baad doosre
octant ki zaroorat nahi hoti because symmetry se sab points pehle hi plot
ho rahe hote hain.

Code me plot8Pixels() actual symmetry ka kaam karta hai. drawCircleMidpoint()
mathematical decision logic handle karta hai. display() drawing ko screen
par show karta hai. User centre aur radius deta hai, jisse circle ki jagah
aur size decide hota hai. Example me centre (250,250) aur radius 100 hai,
to circle ka leftmost point x=150, rightmost x=350, bottom/top y approx
150 aur 350 honge depending on coordinate orientation.

VIVA QUESTIONS:
1. Midpoint Circle Algorithm kya hai? Pixel-based circle generation method.
2. Radius kya hota hai? Centre se circle ke kisi point tak ka fixed distance.
3. 8-way symmetry kya hai? Ek calculated point se 8 reflected points plot karna.
4. Initial x,y kya hote hain? x=0 and y=r.
5. Initial decision parameter? d=1.25-r (is implementation/manual version).
6. Algorithm kab stop hota hai? Jab x<y condition false ho jaati hai.
7. Decision parameter kyun chahiye? Next pixel selection ke liye.
8. Circle equation? (x-xc)^2+(y-yc)^2=r^2.
===============================================================================

/*
ADDITIONAL DETAILED THEORY AND EXAMINATION NOTES

1. CIRCLE IN COMPUTER GRAPHICS
A circle is the set of all points at a fixed distance r from a centre
(xc,yc). Its Cartesian equation is:
(x-xc)^2 + (y-yc)^2 = r^2
The radius controls the size. The centre controls the position.
The diameter is 2r. The circumference is 2*pi*r and the area is pi*r*r.
A raster display cannot show a continuous mathematical curve directly,
so a circle algorithm chooses nearby pixels.

2. EIGHT-WAY SYMMETRY
A circle is symmetric in eight octants. If one point (x,y) is calculated
for a circle centred at the origin, its reflected points are:
(x,y), (y,x), (y,-x), (x,-y), (-x,-y), (-y,-x), (-y,x), (-x,y).
For a circle centred at (xc,yc), add xc to each x coordinate and yc to
each y coordinate. Therefore the program calculates only one octant and
plots the corresponding seven reflected points. This reduces repeated
calculation and makes the circle symmetric.

3. MIDPOINT DECISION IDEA
At each step, the algorithm considers candidate pixels near the ideal
circle. The midpoint between the candidates is tested against the circle
equation. A decision value indicates which candidate should be selected.
The implementation in this practical uses the initial value:
d = 1.25 - r
Initial coordinates are x=0 and y=r. If d<0, the next point is chosen
horizontally and d is updated by d+2x+1. Otherwise the diagonal point is
chosen, so x increases and y decreases; d is updated by d+2x-2y+1.
The decision value is updated incrementally rather than recalculating
the full circle equation for every pixel.

4. WORKED EXAMPLE
Suppose centre=(250,250), radius=100.
Initial point relative to centre: x=0, y=100.
The first symmetric points include (250,350), (350,250), (250,150)
and (150,250), along with diagonal reflections that may overlap at axes.
The initial decision value is d=1.25-100=-98.75. Since it is negative,
the algorithm chooses the horizontal candidate and updates the decision
value. The process continues until x reaches y. Every calculated point
is reflected into the other octants, forming the full circle.

5. STOPPING CONDITION
Only the first octant needs to be calculated. While x<y, the point is
within the region where the next symmetric set is required. When x
becomes equal to y, the diagonal boundary between octants is reached.
The final point is plotted once. Continuing beyond this point would
repeat already generated parts of the circle.

6. ROLE OF EACH FUNCTION
plot8Pixels(xc,yc,x,y): calculates and plots the eight symmetric positions.
drawCircleMidpoint(xc,yc,r): initializes x, y and d, runs the decision
logic and calls plot8Pixels().
display(): clears the window, sets the drawing colour and calls the
circle function.
init(): sets the background and 2D viewing coordinates.
main(): accepts input, validates radius and centre, initializes GLUT,
creates the window and registers the display callback.

7. ADVANTAGES
- Uses symmetry to reduce calculation.
- Uses an incremental decision parameter.
- Does not require trigonometric calculations for every circle point.
- Produces a visually balanced raster circle.
- Suitable for understanding efficient raster graphics.

8. LIMITATIONS
- The output is an approximation on a pixel grid.
- The program requires the circle to fit inside the defined viewing area.
- A fixed coordinate window limits the maximum visible radius.
- The displayed quality depends on pixel resolution and point rendering.

9. APPLICATIONS
Circle generation is used in CAD, drawing software, diagrams, games,
circular indicators, charts, icons and graphical simulations.

10. DETAILED HINGLISH VIVA EXPLANATION
Circle ka mathematical equation hota hai (x-xc)^2+(y-yc)^2=r^2. Iska
meaning hai ki circle ke har point ki centre se distance same, yani
radius hoti hai. Computer screen pixels ki grid hoti hai, isliye exact
continuous circle possible nahi hota. Algorithm nearest pixels choose
karke circle ka raster representation banata hai.

Midpoint algorithm ki sabse important property 8-way symmetry hai.
Circle ko centre se dekho to woh eight octants me divide ho sakta hai.
Agar ek octant ka point calculate kar liya, to baaki seven positions
coordinate swap aur sign change se mil jaati hain. Example: (x,y) se
(y,x), (x,-y), (-x,y) jaise points milte hain. Centre origin par na ho
to har point me xc aur yc add kar dete hain. Isliye calculation kam hoti
hai aur circle ke points balanced rehte hain.

Starting point x=0,y=r hota hai kyunki top point centre se radius distance
par hai. Decision parameter d=1.25-r se start hota hai. d negative hone
par horizontal next pixel choose hota hai aur d update hota hai. d
positive ya zero hone par diagonal pixel choose hota hai, x increase
aur y decrease hota hai. Is tarah algorithm decide karta hai ki ideal
circle ke close kaunsa pixel hai. Jab x aur y equal hote hain, octant
boundary aa jaati hai aur loop stop hota hai.

Code me plot8Pixels symmetry handle karta hai. drawCircleMidpoint decision
calculation karta hai. display drawing start karta hai. User centre aur
radius enter karta hai. Agar circle window ke bahar chala jaaye to poora
circle visible nahi hoga; isliye input validation rakha gaya hai.
Viva me ye bhi bol sakte ho ki midpoint algorithm har point ke liye
sin/cos calculate nahi karta, isliye efficient hai.

11. VIVA - DETAILED ANSWERS
Q1. Midpoint Circle Algorithm kya hai?
A. It is a raster circle-generation algorithm that uses a decision
parameter to select pixels close to the mathematical circle.

Q2. Decision parameter ka purpose?
A. It decides which of the candidate pixels should be plotted next.

Q3. Initial decision parameter?
A. This practical uses d=1.25-r, with x=0 and y=r.

Q4. Eight-way symmetry kya hai?
A. One calculated point is reflected into eight octants using coordinate
swapping and sign changes.

Q5. Stopping condition?
A. The main loop continues while x<y. The diagonal point x==y is plotted
once and then the process stops.

Q6. Radius aur diameter me difference?
A. Radius is centre-to-circumference distance; diameter is twice the radius.

Q7. Circle ke liye trigonometry kyun avoid karte hain?
A. The midpoint decision method uses incremental arithmetic and symmetry,
so repeated trigonometric calculations are unnecessary.

Q8. glVertex2i ka role?
A. It supplies integer x and y coordinates for a vertex/pixel location.

Q9. Centre shift kaise hota hai?
A. Add xc to every calculated x offset and yc to every calculated y offset.

Q10. Algorithm ka main advantage?
A. It uses eight-way symmetry and incremental decision updates to generate
a circle efficiently.
*/
*/
