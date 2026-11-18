#include <GL/glut.h>
#include <cstdlib>

// Class encapsulates the car's state (position) and its drawing behaviour.
class AnimatedCar
{
private:
    float yPosition;

    void drawWheel(float x) const
    {
        glPushMatrix();
        glTranslatef(x, -0.08f, 0.05f);
        glColor3f(0.12f, 0.12f, 0.12f);
        glutSolidSphere(0.22, 24, 24);
        glPopMatrix();
    }

public:
    AnimatedCar() : yPosition(0.0f) {}

    void update()
    {
        yPosition -= 0.004f;
        if (yPosition < -1.5f)
            yPosition = 1.5f;
    }

    void draw() const
    {
        // Move the complete car in the scene.
        glTranslatef(0.0f, yPosition, -6.0f);

        // Car body.
        glColor3f(0.85f, 0.10f, 0.65f);
        glBegin(GL_POLYGON);
            glVertex3f(-1.0f, 0.00f, 0.0f);
            glVertex3f(-0.75f, 0.55f, 0.0f);
            glVertex3f(0.35f, 0.55f, 0.0f);
            glVertex3f(1.0f, 0.00f, 0.0f);
        glEnd();

        // Two wheels are positioned independently using push/pop.
        drawWheel(-0.60f);
        drawWheel(0.60f);
    }
};

AnimatedCar car;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    car.draw();
    glutSwapBuffers();
}

void update(int value)
{
    car.update();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Approximately 60 updates per second.
}

void reshape(int width, int height)
{
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(width) / height, 0.1, 50.0);

    glMatrixMode(GL_MODELVIEW);
}

void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Animated Moving Car");

    initialize();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return EXIT_SUCCESS;
}

/*
===============================================================================
PRACTICAL NO. 8 (AS PER THE UPLOADED LAB MANUAL)
TITLE: Animated Graphics Application - Moving Car
===============================================================================

1. AIM
To design and develop an interactive graphics application that demonstrates
basic animation principles by displaying a car moving continuously on the
screen using C++, OpenGL and GLUT.

2. OBJECTIVES
1. To understand how an OpenGL scene is created and displayed.
2. To draw a simple car using graphical primitives.
3. To understand translation as a way of changing an object's position.
4. To use a timer callback to update the position repeatedly.
5. To understand double buffering and smooth animation.
6. To use object-oriented style decomposition through separate functions
   for drawing the wheels, displaying the scene, updating animation and
   initializing the graphics environment.

3. SOFTWARE AND HARDWARE REQUIREMENTS
Software:
- C++ compiler such as g++.
- OpenGL, GLU and GLUT/FreeGLUT development libraries.
- Linux/Ubuntu or another system configured for OpenGL.
Hardware:
- Computer with a display and graphics support.
- Keyboard and mouse for normal system interaction.

4. THEORY
Computer graphics is the branch of computing concerned with creating,
storing and displaying visual information. A graphics application creates
objects from points, lines, polygons and other primitives.

Animation means displaying a sequence of slightly different frames so that
the viewer observes continuous motion. In this program, the car is drawn
again after its vertical position is changed. The position variable carY
stores the current vertical displacement.

Translation:
Translation moves an object without changing its shape, size or orientation.
For a point (x, y), translation by (tx, ty) is:
    x' = x + tx
    y' = y + ty
The program applies vertical translation through:
    glTranslatef(0.0f, carY, -6.0f)

Timer-based animation:
GLUT provides glutTimerFunc(milliseconds, callback, value). It schedules a
callback after the specified delay. The update() function changes carY,
requests a new frame using glutPostRedisplay(), and schedules itself again.
This repeated update-and-redraw process creates animation.

OpenGL transformations:
glTranslatef() changes the current coordinate system's position.
glPushMatrix() saves the current matrix.
glPopMatrix() restores the saved matrix.
These two matrix functions allow each wheel to be positioned independently
without disturbing the position of the next wheel.

Double buffering:
The program uses GLUT_DOUBLE and glutSwapBuffers(). One buffer is displayed
while the other buffer is prepared. Swapping the buffers after drawing helps
reduce flickering and partially drawn frames.

Depth buffer:
GLUT_DEPTH requests a depth buffer. glEnable(GL_DEPTH_TEST) enables depth
comparison so that objects are drawn according to their depth values.

5. ALGORITHM
1. Start the program.
2. Initialize GLUT and create a graphics window.
3. Select RGBA, double-buffer and depth-buffer display modes.
4. Set the background color and enable depth testing.
5. Register display, reshape and timer callback functions.
6. In display(), clear the color and depth buffers.
7. Call the car object's draw() method.
8. Apply the object's current translation.
9. Draw the car body using GL_POLYGON and the wheels as spheres.
10. Swap the display buffers.
11. In update(), call the object's update() method.
12. The object changes its vertical position and resets at the lower limit.
13. Request another frame and schedule the next timer event.
14. Continue until the graphics window is closed.

6. FUNCTIONAL DESCRIPTION
- AnimatedCar class: encapsulates the car's data and behaviour.
- yPosition: private data member that stores the car's vertical position.
- drawWheel(x): class helper that draws one wheel at the required position.
- car.draw(): draws the complete car after applying its current translation.
- car.update(): changes the car's position and resets it at the lower limit.
- display(): clears the screen, calls car.draw(), and swaps the buffers.
- update(value): calls car.update() and schedules the next timer event.
- reshape(width, height): updates the viewport and perspective projection
  whenever the window size changes.
- initialize(): sets the background color and enables depth testing.
- main(): initializes GLUT, creates the window and registers callbacks.

7. IMPORTANT OPENGL FUNCTIONS
glClear(): clears selected buffers.
glClearColor(): sets the background color.
glTranslatef(): applies translation.
glBegin()/glEnd(): specify vertices of a primitive.
glVertex3f(): specifies a 3D vertex.
glColor3f(): selects an RGB drawing color.
glutSolidSphere(): draws a solid sphere.
glPushMatrix()/glPopMatrix(): save and restore transformation matrices.
glutSwapBuffers(): displays the completed frame.
glutTimerFunc(): schedules a timer callback.
glutPostRedisplay(): asks GLUT to redraw the window.
gluPerspective(): creates a perspective projection.

8. EXECUTION COMMAND
On Ubuntu, install the required development package if it is not installed:
    sudo apt-get install freeglut3-dev
Compile:
    g++ vehicle8.cpp -o vehicle8 -lGL -lGLU -lglut
Run:
    ./vehicle8

9. SAMPLE OUTPUT
Console:
    (No keyboard input is required. The program opens a graphics window.)

Graphics window:
    A coloured car with a body and two dark wheels is displayed.
    The car continuously moves vertically and reappears near the top after
    reaching the lower limit. The motion continues until the window closes.

10. RESULT
The animated car application is created using OpenGL and GLUT. The car body
and wheels are displayed, and timer-based translation produces continuous
movement.

11. CONCLUSION
Thus, a simple animated graphics application was successfully developed.
The practical demonstrates drawing primitives, translation, matrix handling,
timer callbacks, double buffering and continuous redrawing. These concepts
form the basis of animation in computer graphics.

12. PRECAUTIONS / NOTES
- Install the OpenGL, GLU and GLUT development libraries before compilation.
- Use GLUT_DEPTH, not GL_DEPTH, in glutInitDisplayMode().
- Keep the window height non-zero while calculating the aspect ratio.
- Use push/pop matrix around independent object transformations.
- Timer callbacks should request redisplay instead of drawing directly.
- AnimatedCar demonstrates encapsulation: position data and related
  drawing/update operations are grouped inside one class.
- This program uses legacy immediate-mode OpenGL, suitable for introductory
  laboratory exercises.

===============================================================================
HINGLISH EXPLANATION FOR VIVA
===============================================================================

A. PROGRAM KA BASIC IDEA
Is program mein hum ek simple car draw karte hain aur usko screen par move
karwate hain. Car ka body polygon se banta hai aur wheels sphere se bante
hain. Animation ka simple meaning hai ki object ki position ko baar-baar
change karke screen ko redraw kiya jaye. Jab har frame mein car thodi neeche
move hoti hai, to hume continuous movement dikhta hai.

B. CODE KO STEP-BY-STEP SAMAJHO
1. #include <GL/glut.h>
   Ye GLUT/OpenGL ki declarations provide karta hai. Iske through window,
   graphics primitives, callbacks aur transformations use kar sakte hain.

2. float carY = 0.0f;
   Ye global variable car ki vertical position store karta hai. Starting
   mein value zero hai. Update function is value ko change karta rehta hai.

3. drawWheel(float x)
   Ye reusable function hai. Isko x coordinate diya jata hai jahan wheel
   banana hai. glPushMatrix current transformation ko save karta hai.
   glTranslatef wheel ko required jagah le jata hai. glutSolidSphere wheel
   draw karta hai. glPopMatrix purani matrix restore karta hai.
   Isliye ek wheel ki translation doosre wheel ko affect nahi karti.

5. display()
   GLUT jab frame draw karna chahta hai tab display callback call hota hai.
   glClear color aur depth buffer clean karta hai. glLoadIdentity purani
   model-view transformation reset karta hai. car.draw() object ka drawing
   method call karta hai. Last mein glutSwapBuffers complete frame visible
   banata hai.

6. update(int value)
   Ye animation ka main callback hai. car.update() object ki vertical
   position change karta hai. glutPostRedisplay() GLUT ko new frame draw
   karne ko bolta hai. glutTimerFunc(16, update, 0) approx 16 milliseconds
   baad update ko phir call karta hai.

6. reshape(width, height)
   Window resize hone par ye callback call hota hai. glViewport drawing
   area set karta hai. gluPerspective 3D perspective view banata hai.
   Height zero ho to division by zero avoid karne ke liye height ko 1
   kar diya gaya hai.

7. initialize()
   glClearColor white background set karta hai. glEnable(GL_DEPTH_TEST)
   depth testing on karta hai.

8. main()
   GLUT initialize hota hai, display mode set hota hai, window create hoti
   hai aur callbacks register hote hain. glutMainLoop() event loop start
   karta hai. Iske baad GLUT display, timer aur resize callbacks chalata hai.

C. IMPORTANT CONCEPTS
- Frame: animation ka ek single displayed image.
- Callback: aisa function jo event hone par library call karti hai.
- Timer callback: fixed delay ke baad execute hone wala callback.
- Redisplay: screen ko dobara draw karne ki request.
- Double buffering: ek buffer display hota hai aur doosre mein next frame
  prepare hota hai; swap karne se flicker kam hota hai.
- Translation: object ko ek jagah se doosri jagah move karna.
- Matrix stack: transformations ko temporarily save aur restore karna.
- Class: data aur member functions ka blueprint.
- Object: class ka instance; yahan AnimatedCar class ka car object hai.
- Encapsulation: data aur related functions ko ek class ke andar group karna.
- Perspective projection: 3D scene ko 2D screen par depth effect ke saath
  dikhana.

D. VIVA QUESTIONS WITH ANSWERS
Q1. Animation kya hota hai?
Ans: Animation mein multiple frames ko rapidly display kiya jata hai.
Har frame mein object ki position ya appearance thodi different hoti hai.

Q2. Is program mein car ka movement kaise hota hai?
Ans: carY variable ko timer callback mein repeatedly decrease kiya jata hai.
Har update ke baad redisplay request hoti hai.

Q3. GLUT kya hai?
Ans: GLUT ek utility toolkit hai jo OpenGL window creation, input handling
aur callback management ko simple banata hai.

Q4. glutMainLoop() ka use kya hai?
Ans: Ye main event loop start karta hai aur registered callbacks ko execute
karne mein help karta hai.

Q5. glutTimerFunc() kya karta hai?
Ans: Ye given milliseconds ke baad specified callback ko call karta hai.

Q6. glutPostRedisplay() ka use?
Ans: Ye GLUT ko batata hai ki window ko dobara draw karna hai.

Q7. Double buffering kya hai?
Ans: Do buffers use hote hain. Ek screen par show hota hai aur doosre mein
next frame draw hota hai. Swap ke baad completed frame show hota hai.

Q8. glutSwapBuffers() kab use hota hai?
Ans: Double-buffered window mein drawing complete hone ke baad buffers swap
karne ke liye.

Q9. glPushMatrix aur glPopMatrix ka kya kaam hai?
Ans: Push current transformation save karta hai aur pop saved transformation
restore karta hai. Independent objects ko position karne mein useful hai.

Q10. Translation ka formula kya hai?
Ans: x' = x + tx aur y' = y + ty.

Q11. GLUT_DEPTH aur GL_DEPTH mein kya difference hai?
Ans: Display mode request mein GLUT_DEPTH use hota hai. GL_DEPTH valid
GLUT display-mode flag nahi hai.

Q12. Depth testing kyu use hoti hai?
Ans: 3D scene mein objects ki depth compare karke visible surface decide
karne ke liye.

Q13. Is program mein wheels kaise draw kiye hain?
Ans: glutSolidSphere() function se solid spheres draw kiye hain.

Q14. Animation kab stop hoti hai?
Ans: Jab graphics window close hoti hai ya application terminate hoti hai.

Q15. Is program mein class kaunsi hai?
Ans: AnimatedCar class car ki position aur drawing/update behaviour ko
encapsulate karti hai.

Q16. Object kya hai?
Ans: Class ka instance object hota hai. Is program mein car, AnimatedCar
class ka object hai.

Q17. Encapsulation kya hai?
Ans: Data aur us data par kaam karne wale functions ko ek class mein group
karna encapsulation kehlata hai.

Q18. Is program ka practical use kya hai?
Ans: Animation basics, moving objects, simple games, simulations aur
interactive graphics applications mein ye concepts use hote hain.

E. ONE-MINUTE VIVA SUMMARY
"Sir, this program demonstrates an animated car using OpenGL and GLUT.
The AnimatedCar class encapsulates the car's position and drawing behaviour.
The body is drawn using GL_POLYGON and wheels using glutSolidSphere. A timer
callback updates the object's vertical position and glutPostRedisplay
requests a new frame. Double buffering reduces flickering. Thus, the program
demonstrates classes, encapsulation, translation and basic animation."
*/
