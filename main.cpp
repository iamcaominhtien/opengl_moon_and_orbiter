//
// Created by iamca on 11/12/2022.
//

#include <GL/freeglut_std.h>
#include <valarray>

class Moon {
    int displayListId;
public:
    //Creating a display list.
    void create() {
        //glGenLists() returns a unique integer identifier for a display list.
        displayListId = glGenLists(1);
        //glNewList() creates a display list. Give it the unique identifier and the type of display list.
        glNewList(displayListId, GL_COMPILE);

        GLfloat direction[] = {-1.0, -1.0, -1.0, 0.0};
        //glLightfv() sets the value of a light source parameter. The light source is GL_LIGHT0. The parameter is GL_POSITION. The value is direction.
        glLightfv(GL_LIGHT0, GL_POSITION, direction);//Set the light position.
        //glutSolidSphere() draws a sphere. The first parameter is the radius. The second parameter is the number of slices. The third parameter is the number of stacks.
        //create a cube
        glutSolidSphere(1.0, 30, 30);
//        glutSolidCube (1);
        //glEndList() ends the definition of a display list.
        glEndList();
    }
    void draw() const {
        //Calling the display list
        glCallList(displayListId);
    }
};

// The one and only moon.
static Moon moon;

// An orbiter is an object that flies on a circle of a certain radius on the
// xz plane.  You supply the radius at construction time.
class Orbiter {
    //khoảng cách từ vệ tinh đến mặt trăng
    double radius;
    double u;
public:
    explicit Orbiter(double radius): radius(radius), u(0.0) {}
    // Thay đổi vị trí của vệ tinh theo số đo góc truyền vào (tăng lên). Số đo góc thay đổi càng lớn thì tốc độ tự quay quanh trục cũng càng lớn
    void advance(double delta) {u += delta;}

    // Trả về tọa độ x,y,z của quỹ đạo.
    void getPosition(double& x, double& y, double& z) const {
        x = radius * cos(u);
        y = 0;
        z = radius * sin(u);
    }
};

// The one and only orbiter.
static Orbiter orbiter(5);

// Clears the window (and the depth buffer) and draws the moon as viewed from
// the current position of the orbiter.
void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // It sets the current matrix mode. The matrix mode determines which matrix is the current matrix.
    glMatrixMode(GL_MODELVIEW);
    //It pushes the current matrix stack down by one, duplicating the current matrix. That way, when we call glPopMatrix(), we can get the original matrix back.
    glPushMatrix();
    //It replaces the current matrix with the identity matrix.
    glLoadIdentity();
    double x, y, z;
    orbiter.getPosition(x, y, z);

    //Thay đổi vị trí quan sát của mắt (hay camera)
    // 3 tham số đầu tiên biểu diễn tọa độ đặt mắt
    // 3 tham số tiếp theo biểu diễn tọa độ mà mắt đang nhìn vào (ở đây là tâm gốc tọa độ 3 chiều)
    // 3 tham số cuối biểu diễn hướng của mắt (ở đây là hướng trên của mắt)
    gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    moon.draw();
    //It pops the current matrix stack, replacing the current matrix with the one below it on the stack.
    glPopMatrix();
    //It swaps the front and back buffers.
    glutSwapBuffers();
}

void timer(int v) {
    orbiter.advance(0.03);
    glutPostRedisplay();
    glutTimerFunc(20, timer, v);
}

// reshape() fixes up the projection matrix so that we always see a sphere
// instead of an ellipsoid.
void reshape(GLint w, GLint h) {
    //It sets the viewport. The first parameter is the x coordinate of the lower left corner of the viewport. The second parameter is the y coordinate of the lower left corner of the viewport. The third parameter is the width of the viewport. The fourth parameter is the height of the viewport.
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //It sets the perspective projection matrix. The first parameter is the field of view angle in degrees. The second parameter is the aspect ratio. The third parameter is the near clipping plane. The fourth parameter is the far clipping plane.
    gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 10.0);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    GLfloat yellow[] = {1.0, 1.0, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    moon.create();
}

// The usual application code.
int main(int argc, char** argv) {
    //glutInit() initializes the GLUT library.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("The Moon");
    glutDisplayFunc(display);
    //glutTimerFunc sets a timer. The first parameter is the number of milliseconds. The second parameter is the function to call. The third parameter is the value to pass to the function.
    glutTimerFunc(20, timer, 0);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
}