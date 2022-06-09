

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <vector>

#include "GL/freeglut.h"
#include "Point.hpp"
#include "Triangle.hpp"
#include "Object.hpp"
#include "Camera.hpp"

int width = 500;
int height = 500;
double invWidth = 2.f / width; //2.f because frustrum from -1 to 1
double invHeight = 2.f / height;
std::vector<Object> objects = std::vector<Object>();
std::vector<Triangle> allTris = std::vector<Triangle>();
std::vector<Triangle> visibleTris = std::vector<Triangle>();
auto current = std::chrono::system_clock::now();
auto last = std::chrono::system_clock::now();
auto start = std::chrono::system_clock::now();
int frames = 0;
Camera cam = Camera(90, ((double)width)/ ((double)height), 0.01, 1000000);
bool rotate = false;
bool move = false;
Point rotVals;
Point moveVals;
double renderTime = 0;

auto renderStart = std::chrono::system_clock::now();
auto renderEnd = std::chrono::system_clock::now();

void idle() {


    current = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = current - last;
    if (elapsed_seconds.count() >= 1 / 60.f)
    {
        if (rotate)
        {
            cam.rotateX(rotVals.position.at(0));
            cam.rotateY(rotVals.position.at(1));
            cam.rotateZ(rotVals.position.at(2));
        }
        if (move)
        {
            cam.move(moveVals);
        }
        last = current;
        frames++;
        glutPostRedisplay();
    }
    elapsed_seconds = current - start;
    if (elapsed_seconds.count() >= 1) {

        std::cout << "FPS: " << frames / elapsed_seconds.count() << std::endl;
        std::cout << "Frametime: " << renderTime/frames*1000 << "ms" << std::endl;
        frames = 0;
        renderTime = 0;
        start = current;
    }
}
void display() {

    renderStart = std::chrono::system_clock::now();
    glClear(GL_COLOR_BUFFER_BIT);
    
    double buf = 1000000000;

    double heightOffset = -1;
    double widthOffset = -1;
    double currentX, currentY;
    Point rayDir = Point(0, 0, 1);
    Point rayOrigin = Point(0,0,0);
    double depth = -1;
    int last = -1;
    int t = -1;
    int collisionTri = -1;
    bool miss = true;
    allTris.clear();
    visibleTris.clear();
    for (int i = 0; i < objects.size(); i++)
    {
        objects.at(i).getTris(allTris);
    }
    Point min, max;
    Triangle tri;
    for (int i = 0; i < allTris.size(); i++)
    {
        cam.toScreenSpace(allTris.at(i),tri);
        if (tri.maxZ() > 1)
        {
            continue;
        }
        if (tri.normal.position.at(2) > 0)
        {
            min = tri.__boundingBox.at(0);
            max = tri.__boundingBox.at(1);
            if (min.position.at(0) > -widthOffset || max.position.at(0) < widthOffset)
            {
                continue;
            }
            if (min.position.at(1) > -heightOffset || max.position.at(1) < heightOffset)
            {
                continue;
            }
            visibleTris.push_back(tri);
        }
    }
    double minX, maxX, minY, maxY;
    minX = 1;
    maxX = -1;
    minY = 1;
    maxY = -1;
    for (int i = 0; i < visibleTris.size(); i++)
    {
        if (visibleTris.at(i).minX() < minX)
        {
            minX = visibleTris.at(i).minX();
        }

        if (visibleTris.at(i).maxX() > maxX)
        {
            maxX = visibleTris.at(i).maxX();
        }

        if (visibleTris.at(i).minY() < minY)
        {
            minY = visibleTris.at(i).minY();
        }

        if (visibleTris.at(i).maxY() > maxY)
        {
            maxY = visibleTris.at(i).maxY();
        }
    }


    glBegin(GL_POINTS);
    for (int y = 0; y < height; y++)
    {
        currentY = y * invHeight + heightOffset;
        if (currentY > maxY)
        {
            y = height;
            continue;
        }
        
        if (currentY < minY)
        {
            y = (minY-heightOffset) * height*0.5;
        }
        //currentY = y + heightOffset;
        //currentY *= invHeight;
        for (int x = 0; x < width; x++)
        {
            currentX = x * invWidth + widthOffset;
            if (currentX > maxX)
            {
                x = width;
                continue;
            }
            if (currentX < minX)
            {
                x = (minX - widthOffset) * width*0.5;
            }
            //currentX = x + widthOffset;
            //currentX *= invWidth;
            buf = 1000000000;
            miss = true;
            for (t = 0; t < visibleTris.size(); t++)
            {
                if (!visibleTris.at(t).withinBoundingBox(currentX, currentY))
                {
                    continue;
                }
                if (visibleTris.at(t).minDistance() >= buf)
                {
                    continue;
                }
                rayOrigin.setCoords(currentX, currentY, 0);
                rayDir.setCoords(0, 0, 1);
                if (visibleTris.at(t).rayIntersect(rayOrigin, rayDir, buf, depth))
                {
                    miss = false;
                    collisionTri = t;
                    buf = depth;
                }
            }
            if (miss)
            {
                continue;
            }
            if (last != collisionTri)
            {
                last = collisionTri;
                glColor3f(visibleTris.at(last).color.at(0), visibleTris.at(last).color.at(1), visibleTris.at(last).color.at(2));
            }
            glVertex2d(currentX, currentY);
        }
    }
    glEnd();
    glFlush();

    renderEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = renderEnd - renderStart;
    renderTime += elapsed_seconds.count();
}
void keyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
    case 'Q':
        glutLeaveMainLoop();
        break;
    case 'w':
    case 'W':
        moveVals.position.at(2) = 0.1f;
        move = true;
        break;
    case 's':
    case 'S':
        moveVals.position.at(2) = -0.1f;
        move = true;
        break;
    case 'a':
    case 'A':
        moveVals.position.at(0) = 0.1f;
        move = true;
        break;
    case 'd':
    case 'D':
        moveVals.position.at(0) = -0.1f;
        move = true;
        break;
    case 'r':
    case 'R':
        moveVals.position.at(1) = -0.1f;
        move = true;
        break;
    case 'f':
    case 'F':
        moveVals.position.at(1) = 0.1f;
        move = true;
        break;
    case 'i':
    case 'I':
        rotVals.position.at(0) = -1;
        rotate = true;
        break;
    case 'k':
    case 'K':
        rotVals.position.at(0) = 1;
        rotate = true;
        break;
    case 'j':
    case 'J':
        rotVals.position.at(1) = -1;
        rotate = true;
        break;
    case 'l':
    case 'L':
        rotVals.position.at(1) = 1;
        rotate = true;
        break;
    case 'o':
    case 'O':
        rotVals.position.at(2) = 1;
        rotate = true;
        break;
    case 'u':
    case 'U':
        rotVals.position.at(2) = -1;
        rotate = true;
        break;
    }
}
void keyboard_up(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 'W':
        moveVals.position.at(2) = 0;
        move = false;
        break;
    case 's':
    case 'S':
        moveVals.position.at(2) = 0;
        move = false;
        break;
    case 'a':
    case 'A':
        moveVals.position.at(0) = 0;
        move = false;
        break;
    case 'd':
    case 'D':
        moveVals.position.at(0) = 0;
        move = false;
        break;
    case 'r':
    case 'R':
        moveVals.position.at(1) = 0;
        move = false;
        break;
    case 'f':
    case 'F':
        moveVals.position.at(1) = 0;
        move = false;
        break;
    case 'i':
    case 'I':
        rotVals.position.at(0) = 0;
        rotate = false;
        break;
    case 'k':
    case 'K':
        rotVals.position.at(0) = 0;
        rotate = false;
        break;
    case 'j':
    case 'J':
        rotVals.position.at(1) = 0;
        rotate = false;
        break;
    case 'l':
    case 'L':
        rotVals.position.at(1) = 0;
        rotate = false;
        break;
    case 'o':
    case 'O':
        rotVals.position.at(2) = 0;
        rotate = false;
        break;
    case 'u':
    case 'U':
        rotVals.position.at(2) = 0;
        rotate = false;
        break;
    }
}

void initGlut(int argc, char* argv[]) {
    glutInitWindowSize(width, height);
    glutInitWindowPosition(500, 40);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutCreateWindow("RayTracing");

    glutKeyboardFunc(keyboardFunc);
    glutKeyboardUpFunc(keyboard_up);
    glutDisplayFunc(display);
    glutIdleFunc(idle);


    glutMainLoop();
}


void init()
{
    moveVals = Point(0, 0, 0);
    rotVals = Point(0, 0, 0);

    /*
    Triangle tri1, tri2, tri3;
    Point p1, p2, p3;
    std::vector<int> color;
    color.push_back(255);
    color.push_back(0);
    color.push_back(0);
    p1 = Point(0, 0, -1);
    p2 = Point(0.5, 0, -1.5);
    p3 = Point(0, 0.5, -1.5);
    tri1 = Triangle(p1, p2, p3, color);
    color.at(0) = 0;
    color.at(1) = 255;
    p1 = Point(0, 0, -1.5);
    p2 = Point(0.75, 0, -2);
    p3 = Point(0, 0.75, -2);
    tri2 = Triangle(p1, p2, p3, color);
    //tri1 = Triangle(Point(0 / 480.f * width, 0 / 270.f * height, 100), Point(150.f / 480.f * width, 10.f / 270.f * height, 100), Point(10.f / 480.f * width, 100.f / 270.f * height, 100),color);
    //tris.push_back(tri);

    color.at(0) = 0;
    color.at(1) = 255;
    //tri2 = Triangle(Point(0 / 480.f * width, 75.f / 270 * height, 99), Point(40 / 480.f * width, 10 / 270.f * height, 101), Point(250 / 480.f * width, 66 / 270.f * height, 99), color);
    ///tris.push_back(tri);

    color.at(1) = 0;
    color.at(2) = 255;
    //tri3 = Triangle(Point(200 / 480.f * width, 75 / 270.f * height, 10), Point(30 / 480.f * width, 10 / 270.f * height, 105), Point(70 / 480.f * width, -66 / 270.f * height, 80), color);
    
    //tris.push_back(tri);
    std::vector<Triangle> objs;
    objs.push_back(tri1);
    objs.push_back(tri2);
    //objs.push_back(tri3);
    objects.push_back(Object(objs));
    */

    std::vector<Point> p;
    std::vector<Triangle> t;
    std::vector<int> color;
    color.push_back(255);
    color.push_back(0);
    color.push_back(0);

    for (int i = -1; i < 2; i+=2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            for (int k = -1; k < 2; k += 2)
            {
                p.push_back(Point(i, j, k));
            }
        }
    }
    t.push_back(Triangle(p.at(0), p.at(1), p.at(2),color));
    t.push_back(Triangle(p.at(1), p.at(3), p.at(2),color));

    color.at(0) = 0;
    color.at(1) = 255;
    color.at(2) = 0;
    t.push_back(Triangle(p.at(4), p.at(6), p.at(5),color));
    t.push_back(Triangle(p.at(5), p.at(6), p.at(7), color));

    color.at(0) = 0;
    color.at(1) = 0;
    color.at(2) = 255;
    t.push_back(Triangle(p.at(0), p.at(2), p.at(4), color));
    t.push_back(Triangle(p.at(2), p.at(6), p.at(4), color));

    color.at(0) = 255;
    color.at(1) = 255;
    color.at(2) = 0;
    t.push_back(Triangle(p.at(3), p.at(1), p.at(7), color));
    t.push_back(Triangle(p.at(7), p.at(1), p.at(5), color));

    color.at(0) = 0;
    color.at(1) = 255;
    color.at(2) = 255;
    t.push_back(Triangle(p.at(2), p.at(3), p.at(6), color));
    t.push_back(Triangle(p.at(3), p.at(7), p.at(6), color));

    color.at(0) = 255;
    color.at(1) = 0;
    color.at(2) = 255;
    t.push_back(Triangle(p.at(0), p.at(4), p.at(1), color));
    t.push_back(Triangle(p.at(1), p.at(4), p.at(5), color));
    objects.push_back(Object(t));
    t.clear();
    objects.at(0).getTris(t);
    std::cout << t.size() << std::endl;



}

int main(int argc, char* argv[])
{
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    init();
    initGlut(argc, argv);
}