#include<GL/glut.h>
#include<math.h>

void drawAxes(float len) {
    glBegin(GL_LINES);

    // x axes-green
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(len,  0.0, 0.0);

    // y axes-red
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, len,  0.0);

    // z axes-blue
    glColor3f(0.0, 0.0, 1.0f);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, len);

    glEnd();
}

//rotate angle
float angle=0.0;
//Update angle using a timer
void timer(int value){
    angle+=1.0;
    if (angle >= 360.0f) angle -= 360.0;
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

//The light position
float lightPos[4]={4.0, 8.0, 4.0, 1.0};
//Build a planar shadow projection matrix for the plane y = 0.
void buildShadowMatrix(GLfloat mat[16]) {
    float lx = lightPos[0];
    float ly = lightPos[1];
    float lz = lightPos[2];

    GLfloat m[16] = {
         ly,  0.0f,  0.0f,  0.0f,
        -lx,  0.0f, -lz,   -1.0f,
         0.0f, 0.0f,  ly,   0.0f,
         0.0f, 0.0f,  0.0f,  ly
    };
    for (int i = 0; i < 16; ++i) mat[i] = m[i];
}

//Projects the car onto the ground plane y = 0
GLfloat shadowMat[16];

//draw wheel
void drawWheel(float x, float y, float z, float zoff) { // -> zoff----Offset in the z-axis direction
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90.0, 0.0, 0.0, 1.0);

    //tire
    glColor3f(0.1, 0.1, 0.1);
    glutSolidTorus(0.15, 0.3, 12, 24);

    //wheel hub
    glColor3f(0.8, 0.8, 0.85);
    float r = 0.25;
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0, 0.0, zoff);
        for (int i = 0; i <= 24; i++) {
            float a = 2.0 * 3.1415926 * i / 24.0;
            glVertex3f(r * cos(a), r * sin(a), zoff);
        }
    glEnd();

    glPopMatrix();
}

//==========================
//Free draw toolbox
typedef struct{
   float x, y, z;
}vec;
void drawPolygon(vec* points, int n){
      glBegin(GL_POLYGON);
      for(int i=0; i<n;i++){
        glVertex3f(points[i].x, points[i].y, points[i].z);
      }
      glEnd();
}

//==========================


//draw box
//x, y, z ----- is the box Central vertex location
//sx, sy, sz ---- is the size of box
void drawBox(float x, float y, float z,
             float angleIni,
              float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angleIni, 0.0, 0.0, 1.0);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

// 中网中心在原点，宽 W，高 H，朝 +z
void drawGrille()
{
    const float W     = 2.0f;    // 宽
    const float H     = 0.6f;    // 高
    const float barW  = 0.035f;  // 条宽
    const float barD  = 0.03f;   // 条向 +z 凸出的厚度
    const int   bars  = 22;      // 竖条数量

    //glEnable(GL_LIGHTING);
    glNormal3f(0.0f, 0.0f, 1.0f);   // 整个中网面朝 +z

    // ---------- 1. 深色底板 ----------
    glColor3f(0.06f, 0.06f, 0.08f);
    glBegin(GL_QUADS);
        glVertex3f(-W/2, -H/2, 0.0f);
        glVertex3f( W/2, -H/2, 0.0f);
        glVertex3f( W/2,  H/2, 0.0f);
        glVertex3f(-W/2,  H/2, 0.0f);
    glEnd();

    // ---------- 2. 竖条 ----------
    glColor3f(0.62f, 0.62f, 0.68f);   // 金属灰
    for (int i = 0; i <= bars; i++) {
        float x = -W/2 + i * (W / bars);
        glBegin(GL_QUADS);
            glVertex3f(x - barW/2, -H/2, barD);
            glVertex3f(x + barW/2, -H/2, barD);
            glVertex3f(x + barW/2,  H/2, barD);
            glVertex3f(x - barW/2,  H/2, barD);
        glEnd();
    }

    // ---------- 3. 横向分隔条（可选，做交叉网格） ----------
    glColor3f(0.55f, 0.55f, 0.6f);
    const int   hbars = 3;
    const float hbarH = 0.03f;
    for (int i = 1; i <= hbars; i++) {
        float y = -H/2 + i * (H / (hbars + 1));
        glBegin(GL_QUADS);
            glVertex3f(-W/2, y - hbarH/2, barD);
            glVertex3f( W/2, y - hbarH/2, barD);
            glVertex3f( W/2, y + hbarH/2, barD);
            glVertex3f(-W/2, y + hbarH/2, barD);
        glEnd();
    }

    // ---------- 4. 镀铬边框 ----------
    glColor3f(0.85f, 0.85f, 0.9f);
    const float bw = 0.05f;   // 边框宽度
    const float bd = 0.04f;   // 边框凸出
    // 上边
    glBegin(GL_QUADS);
        glVertex3f(-W/2, H/2,      bd); glVertex3f( W/2, H/2,      bd);
        glVertex3f( W/2, H/2 - bw, bd); glVertex3f(-W/2, H/2 - bw, bd);
    glEnd();
    // 下边
    glBegin(GL_QUADS);
        glVertex3f(-W/2, -H/2 + bw, bd); glVertex3f( W/2, -H/2 + bw, bd);
        glVertex3f( W/2, -H/2,      bd); glVertex3f(-W/2, -H/2,      bd);
    glEnd();
    // 左边
    glBegin(GL_QUADS);
        glVertex3f(-W/2,      -H/2, bd); glVertex3f(-W/2 + bw, -H/2, bd);
        glVertex3f(-W/2 + bw,  H/2, bd); glVertex3f(-W/2,       H/2, bd);
    glEnd();
    // 右边
    glBegin(GL_QUADS);
        glVertex3f( W/2 - bw, -H/2, bd); glVertex3f( W/2,      -H/2, bd);
        glVertex3f( W/2,       H/2, bd); glVertex3f( W/2 - bw,  H/2, bd);
    glEnd();
}

//car insdustry
void produceCar(){

    //car body
    glColor3f(1.0, 0.2, 0.2);
    drawBox(0.0, 0.8, 0.0,
            0.0,
            4.0, 1.0, 2.0);

    //Roof
    glColor3f(0.9, 0.1, 0.1);
    drawBox(-0.3, 1.6, 0.0,
            0.0,
            3.0, 0.8, 1.8);

    //side mirror right
    glColor3f(255,255,255);
    drawBox(1.5, 1.4, 1.18,
            0.0,
            0.1, 0.2, 0.4
            );
    //side mirror left
    glColor3f(255,255,255);
    drawBox(1.5, 1.4, -1.18,
            0.0,
            0.1, 0.2, 0.4
            );

    //Roof spoiler
    glColor3f(1.0, 0.0, 0.0);
    drawBox(-1.8, 2.05, 0.0,
            -10.0,
            0.2, 0.1, 1.8
            );

    //front light-left
    glColor3f(255, 255, 0);
    drawBox(1.9, 1.0, 0.8,
            0.0,
            0.4, 0.2, 0.5
            );
    //front light-right
     glColor3f(255, 255, 0);
    drawBox(1.9, 1.0, -0.8,
            0.0,
            0.4, 0.2, 0.5
            );
//中网
    glPushMatrix();
        glTranslatef(1.99f, 0.95f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.5f, 0.55f, 1.0f);
        drawGrille();
    glPopMatrix();
    //back light
     glColor3f(255, 255, 0);
    drawBox(-1.9, 1.0, 0.0,
            0.0,
            0.4, 0.2, 2.1
            );

    //front bumper
    glColor3f(0, 0, 0);
    drawBox(-1.9, 0.5, 0.0,
            0.0,
            0.4, 0.5, 2.1
            );
    //back bumper
    glColor3f(0, 0, 0);
    drawBox(1.9, 0.5, 0.0,
            0.0,
            0.4, 0.5, 2.1
            );

    //side window
    glColor3f(0.2, 0.3, 0.5);
    drawBox(-1.2, 1.7, 0.91,
            0.0,
            1.0, 0.6, 0.05);
    drawBox(-1.2, 1.7, -0.91,
            0.0,
            1.0, 0.6, 0.05);
        // ---- front windsheled ----
    glColor3f(0.2, 0.3, 0.5);
    drawBox( 1.41, 1.65, 0.0,
               30.0,               //rotate angle is 30
               0.05, 0.81, 1.9);

    // ---- back windsheiled ----
    drawBox(-1.8, 1.7, 0.0,
               0.0,
               0.05, 0.6, 1.6);

   //front side window
    vec points[4]={
        {0.0, 1.3, 0.95},
        {1.6, 1.3, 0.95},
        {1.22, 2.0, 0.95},
        {0.0, 2.0, 0.95}
    };
    drawPolygon(points, 4);
    vec pointsL[4] = {//front side window
    {0.0,  2.0,  -0.95},
    {1.22, 2.0, -0.95},
    {1.6, 1.3, -0.95},
    {0.0,  1.3,  -0.95}
};
drawPolygon(pointsL, 4);

    //4 wheel
    glColor3f(0.15, 0.15, 0.15);
    drawWheel(-1.3, 0.3,  1.0, 0.1);//right wheel
    drawWheel( 1.3, 0.3,  1.0, 0.1);//right wheel
    drawWheel(-1.3, 0.3, -1.0, -0.1);//left wheel
    drawWheel( 1.3, 0.3, -1.0, -0.1);//left wheel
}

//Draw the car's shadow
void drawShadow() {
    // 1. Mark the car's projected shape in the stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // 2. Draw the projected car without changing color or depth
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
        glMultMatrixf(shadowMat);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);//shadow rotate with car together
        produceCar();
    glPopMatrix();

    // 3. Restore normal drawing settings
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    // 4. Draw a transparent shadow only inside the marked area
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.45f);

    glBegin(GL_QUADS);
        glVertex3f(-5.0f, 0.01f, -5.0f);
        glVertex3f( 5.0f, 0.01f, -5.0f);
        glVertex3f( 5.0f, 0.01f,  5.0f);
        glVertex3f(-5.0f, 0.01f,  5.0f);
    glEnd();

    // 5. Disable the effects used for shadow rendering
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
}

//draw ground
void drawGround(){
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_QUADS);
        glVertex3f(-5.0, 0.0, -5.0);
        glVertex3f(5.0, 0.0, -5.0);
        glVertex3f(5.0, 0.0, 5.0);
        glVertex3f(-5.0, 0.0, 5.0);

        glEnd();
}

void display(){
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

     //set projection matrix
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(60.0, 1.0, 1.0, 100.0);

     //set camera
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt(6.0, 5.0, 8.0,
          0.0, 1.0, 0.0,
          0.0, 1.0, 0.0);

     //draw axexs
     drawAxes(5.0);

     //draw ground
     drawGround();

     //draw the car's shadow on the ground
     drawShadow();

     //draw car (rotating around the y axis)
     glPushMatrix();
     glRotatef(angle, 0.0, 1.0, 0.0);
     produceCar();
     glPopMatrix();

     glutSwapBuffers();
}

void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.15, 1.0);
    buildShadowMatrix(shadowMat);
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Car-Ultra Pro max");

    init();

    glutDisplayFunc(display);
    //glutTimerFunc(10, timer, 0);
    glutMainLoop();
    return 0;
}
