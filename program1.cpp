#include <GL/glut.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_POINTS);
        glVertex2i(10,200);
        glVertex2i(24,47);
        glVertex2i(34,153);
        glVertex2i(245,199);
    glEnd();
    glFlush();
}

void myinit()
{
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glColor3f(1.0f,0.0f,0.0f);
    glPointSize(10.0f);
    gluOrtho2D(0.0,499.0,0.0,499.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(700,500);
    glutInitWindowPosition(0,10);
    glutCreateWindow("OpenGL Points");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
    return 0;
}

/*
===============================================================================
PRACTICAL NO. 1 : INSTALL AND EXPLORE OPENGL
===============================================================================

AIM:
To install and explore the basic features and working environment of OpenGL
and to write a simple C++ program to display points using OpenGL/GLUT.

OBJECTIVES:
1. To understand the OpenGL graphics programming environment.
2. To create a graphics window using GLUT.
3. To use basic OpenGL functions for clearing the screen, setting colour,
   changing point size and plotting points.
4. To execute a simple graphics program.

SOFTWARE / HARDWARE REQUIREMENTS:
- Ubuntu/Linux operating system
- C++ compiler (g++)
- OpenGL, GLU and FreeGLUT development libraries
- Any computer capable of running a graphical desktop

THEORY:
OpenGL (Open Graphics Library) is a cross-platform graphics API used to
render 2D and 3D graphics. It provides functions to specify geometric
primitives such as points, lines and polygons. OpenGL is an API, not a
programming language. C++ is used here to call its functions.

Main libraries used in this practical:
1. GL: Core OpenGL functions such as glClear(), glColor3f(),
   glBegin(), glVertex2i() and glFlush().
2. GLU: Utility functions built over OpenGL. gluOrtho2D() is used here
   to define a 2D coordinate system.
3. GLUT/FreeGLUT: Helps create windows, receive input and register
   callback functions such as the display callback.

IMPORTANT FUNCTIONS:
- glutInit(): Initializes the GLUT library.
- glutInitDisplayMode(): Selects the display mode. GLUT_SINGLE means
  single buffering and GLUT_RGB means RGB colour mode.
- glutInitWindowSize(w,h): Sets the initial window width and height.
- glutInitWindowPosition(x,y): Sets the initial window position.
- glutCreateWindow(title): Creates a window with the given title.
- glutDisplayFunc(display): Registers the function that draws the scene.
- glClearColor(r,g,b,a): Sets the background colour used when clearing.
- glClear(GL_COLOR_BUFFER_BIT): Clears the colour buffer.
- glColor3f(r,g,b): Sets the current drawing colour. Each RGB value is
  normally between 0.0 and 1.0.
- glPointSize(size): Sets the diameter of points in pixels.
- glBegin(GL_POINTS) / glEnd(): Marks a group of point vertices.
- glVertex2i(x,y): Specifies a 2D vertex using integer coordinates.
- gluOrtho2D(left,right,bottom,top): Defines the visible 2D coordinate range.
- glFlush(): Requests execution of the drawing commands in single-buffer mode.
- glutMainLoop(): Enters the event loop and keeps the window active.

ALGORITHM:
1. Start the program.
2. Initialize GLUT.
3. Select single-buffer RGB display mode.
4. Set window size and position.
5. Create the OpenGL window.
6. Register the display() callback.
7. Set the background colour, drawing colour, point size and 2D projection.
8. In display(), clear the screen and select red colour.
9. Begin GL_POINTS and specify four point coordinates.
10. End the primitive and flush the drawing commands.
11. Keep the window active using glutMainLoop().
12. Stop when the graphics window is closed.

PROGRAM DESCRIPTION:
The display() function clears the window and sets red as the drawing colour.
GL_POINTS tells OpenGL to interpret every supplied vertex as an individual
point. Four vertices are supplied: (10,200), (24,47), (34,153) and (245,199).
myinit() sets a white background, red drawing colour, point size 10 and a
2D coordinate range from 0 to 499 on both axes. main() initializes GLUT,
creates the window and registers display().

COMPILE COMMAND (Ubuntu):
sudo apt update
sudo apt install g++ freeglut3-dev
g++ program1.cpp -o program1 -lGL -lGLU -lglut

RUN COMMAND:
./program1

SAMPLE OUTPUT:
Terminal: No additional input is required. The program opens a window titled
"OpenGL Points".
Graphics window: White background with four red circular/square-looking
raster points at the specified coordinates. Each point has size 10 pixels.
(Exact appearance can vary with the graphics driver.)

RESULT:
The OpenGL environment was used successfully to create a graphics window
and display four points using the GL_POINTS primitive.

CONCLUSION:
Thus, a simple OpenGL program was implemented and executed successfully.
The practical demonstrated window creation, callback handling, colour
selection, point plotting and 2D coordinate mapping.

VIVA QUESTIONS WITH SHORT ANSWERS:
1. What is OpenGL?
   OpenGL is a cross-platform API used for rendering 2D and 3D graphics.
2. Is OpenGL a programming language?
   No. It is a graphics API that can be called from languages such as C++.
3. What is GLUT?
   GLUT is a utility toolkit used for window creation and input handling.
4. What is GL_POINTS?
   It is a primitive mode in which each vertex is drawn as an individual point.
5. What does glColor3f() do?
   It sets the current red, green and blue drawing components.
6. What is glFlush()?
   It asks OpenGL to execute the previously issued drawing commands.
7. What is gluOrtho2D()?
   It defines the 2D orthographic coordinate range.
8. What is a pixel?
   A pixel is the smallest addressable picture element on a raster display.
===============================================================================

/*
ADDITIONAL DETAILED THEORY AND EXAMINATION NOTES

1. COMPUTER GRAPHICS - MEANING AND IMPORTANCE
Computer Graphics is the branch of computer science concerned with creating,
storing, manipulating and displaying pictures with the help of a computer.
The picture may be a simple point, line, circle, chart, 2D object or a
complex 3D scene. The computer represents the picture using numerical
coordinates, colours and geometric information. A graphics program converts
this information into pixels that can be displayed on the monitor.

Applications include:
- CAD: engineers create and modify building, machine and product designs.
- Education: diagrams, simulations and visual explanations.
- Medical imaging: viewing and analysing scans.
- Games and animation: creating characters, scenes and motion.
- Graphical user interfaces: icons, buttons, windows and menus.
- Scientific visualisation: displaying complex data as graphs and models.

2. OPENGL, GLU AND GLUT - DETAILED DIFFERENCE
OpenGL is the main graphics API. It contains commands for specifying
geometric primitives, colours, transformations and rendering operations.
GLU means OpenGL Utility Library. It provides helper functions, including
gluOrtho2D(), which makes it convenient to define a 2D viewing area.
GLUT means OpenGL Utility Toolkit. It helps a program create a window,
receive keyboard or mouse input and run an event loop. FreeGLUT is a
commonly used open-source implementation of GLUT.

Remember this viva distinction:
OpenGL draws the graphics; GLU provides utility functions; GLUT manages
the window and user interaction.

3. COORDINATE SYSTEM AND VIEWING AREA
A 2D point is represented by an x-coordinate and a y-coordinate. The x-axis
controls horizontal position and the y-axis controls vertical position.
The call gluOrtho2D(0,499,0,499) defines the visible coordinate range.
The left and right values define the horizontal limits; bottom and top
define the vertical limits. A point outside this range may not be visible.
The window size in pixels and the coordinate range are related, but they
are not necessarily the same in every graphics program.

4. COLOUR REPRESENTATION
The RGB model represents colour using Red, Green and Blue components.
In glColor3f(), each component is a floating-point value generally between
0 and 1. For example:
- (1,0,0) = red
- (0,1,0) = green
- (0,0,1) = blue
- (1,1,1) = white
- (0,0,0) = black
The alpha component represents transparency in functions that accept it.
glClearColor() does not immediately paint the screen; it stores the colour
that will be used when glClear() clears the colour buffer.

5. GRAPHICS PRIMITIVES
A primitive is a basic geometric element from which more complex drawings
can be made. GL_POINTS draws independent points. GL_LINES interprets each
pair of vertices as a separate line segment. GL_LINE_STRIP joins successive
vertices. GL_LINE_LOOP joins successive vertices and also joins the last
vertex to the first, creating a closed outline. GL_TRIANGLES interprets
each group of three vertices as a triangle.

6. DETAILED EXECUTION FLOW
First, main() starts and calls glutInit() so GLUT can communicate with the
window system. It then selects the display mode and specifies the initial
window dimensions and position. glutCreateWindow() creates the window.
The display callback is registered through glutDisplayFunc(). When GLUT
needs the window to be drawn, it calls display(). The display function
clears the old frame, chooses the drawing colour, starts a primitive,
submits vertex coordinates, ends the primitive and flushes the commands.
The event loop remains active so that the window can continue to receive
system events. This callback-based design is common in interactive graphics.

7. WHY ARE glBegin() AND glEnd() USED?
They mark the beginning and end of a group of vertices that belong to a
specified primitive. The argument of glBegin() decides how OpenGL should
interpret the vertices. In this experiment GL_POINTS is used, so every
vertex becomes one point. glVertex2i() supplies an integer x and y value.
The z-coordinate is treated as zero in this 2D call.

8. SAMPLE COORDINATE INTERPRETATION
The program plots (10,200), (24,47), (34,153) and (245,199). These are four
separate locations, not four connected vertices. Because GL_POINTS is
selected, OpenGL does not join them with lines. The point size is set to
10 pixels, so each plotted point is larger than a default one-pixel point.
The white background is produced by glClearColor(1,1,1,1), while the red
points are produced by glColor3f(1,0,0).

9. COMMON ERRORS AND TROUBLESHOOTING
- If GL/glut.h is missing, install the FreeGLUT development package.
- If linker errors mention glBegin or glutInit, check the library flags:
  -lGL -lGLU -lglut.
- If no window appears, run the program in a graphical desktop session.
- If points are not visible, check that their coordinates are within the
  gluOrtho2D viewing range.
- If the background colour is unexpected, check glClearColor() and ensure
  glClear(GL_COLOR_BUFFER_BIT) is called.
- If the program closes immediately, check whether GLUT initialized
  successfully and whether the event loop is entered.

10. DETAILED HINGLISH CONCEPT EXPLANATION
Computer Graphics ka basic idea hai computer ki help se images banana.
Monitor par jo image dikhti hai woh pixels se milkar banti hai. Har pixel
ki ek location aur colour hota hai. Program me hum coordinates dete hain
aur graphics library un coordinates ko screen ke pixels ke saath map karti
hai. Is practical me hum koi complicated shape nahi bana rahe; hum basic
building block yani point draw kar rahe hain.

OpenGL ko drawing ke rules aur commands ka collection samjho. GLUT ko
window manager helper samjho, jo graphics window banata hai aur program
ko active rakhta hai. GLU extra utility functions deta hai. Isliye viva
me agar poocha jaye ki OpenGL aur GLUT same hain kya, to answer hoga:
nahi. OpenGL graphics rendering ke liye hai, jabki GLUT window aur events
manage karne me help karta hai.

main() se execution start hota hai. glutInit() graphics environment
initialize karta hai. glutInitDisplayMode() batata hai ki single buffer
aur RGB colour mode use karna hai. glutInitWindowSize() width-height
set karta hai. glutCreateWindow() actual window create karta hai.
glutDisplayFunc(display) GLUT ko batata hai ki screen draw karne ki
responsibility display() function ki hai. Isko callback bolte hain:
hum function ka naam register karte hain aur zaroorat padne par GLUT
use call karta hai.

display() me glClear() purani drawing hata kar screen ko fresh karta hai.
glColor3f(1,0,0) current drawing colour red set karta hai. glBegin(GL_POINTS)
batata hai ki aage aane wale vertices individual points hain. Har
glVertex2i(x,y) ek point ki position deta hai. glEnd() primitive group
complete karta hai. glFlush() drawing commands ko execute karne ke liye
send karta hai. Is tarah chaar independent red points white background
par show hote hain.

VIVA - DETAILED ANSWERS
Q1. Pixel kya hota hai?
A. Pixel ka full form picture element maana jaata hai. Digital image ka
smallest addressable display element pixel hota hai. Bahut saare pixels
milkar complete image banate hain.

Q2. RGB model kya hai?
A. RGB ek additive colour model hai. Red, Green aur Blue components ko
different intensity me combine karke colours banaye jaate hain.

Q3. Callback function kya hota hai?
A. Aisa function jiska address/name kisi library ko diya jaata hai aur
library kisi event ya requirement par us function ko call karti hai.
display() is practical ka callback hai.

Q4. Single buffering kya hai?
A. Single buffering me drawing ke liye ek colour buffer use hota hai.
glFlush() issued commands ko execute karne ke liye bhejta hai. Double
buffering me front aur back buffers use karke smooth presentation mil sakti hai.

Q5. GL_POINTS aur GL_LINES me difference?
A. GL_POINTS har vertex ko independent point banata hai. GL_LINES har
do vertices ko ek line segment ke endpoints ke roop me interpret karta hai.

Q6. glClearColor aur glColor3f me difference?
A. glClearColor background clear karte waqt use hone wala colour set karta
hai. glColor3f normal geometry draw karne ka current colour set karta hai.

Q7. glFlush aur glutMainLoop me difference?
A. glFlush drawing commands ko process karne ke liye bhejta hai.
glutMainLoop event processing loop chalata hai aur window ko active rakhta hai.
*/
*/
