#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "shape.h"
#include "main.h"

//Global variables
static int FPS = 60;
static int DT = 1000 / 60;
const int height = 20;
const int width = 12;
int score = 0;
int mouseLastx = 350;
int mouseLasty = 500;
int board[height][width];
int currentY = 2;
int currentX = 6;
int currentShape = 0;
int currentRotation = 0;
int fullscreen = 0;
int color = 0;
float counter = 0.0f;
float limit = 1.0f;
float currentLimit = limit;
float fastLimit = 0.02f;
float dt = 1.0f / FPS;
float angley = 0.0f;
float anglex = 0.0f;
float size = 0.95f;
static char stringBuffer[10];

void drawScene(void){
	int i;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    gluLookAt(0.0f, 10.0f, 10.0f, 0.0f, 5.0f, -8.0f, 0.0f, 20.0f, -10.0f); // eye, at, up

    drawHud();

	glRotatef(angley/10, 0.0f, 1.0f, 0.0f);
	glRotatef(anglex/10, 1.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(-5.0f, -0.5f, 0.0f);

	//glScalef(1.0f, 1.0f, 1.5f);

	// draw floor
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.5f, 0.0f, 0.5f);
		glVertex3f(10.5f, 0.0f, 0.5f);
		glVertex3f(10.5f, -0.0f, -0.5f);
		glVertex3f(0.5f, -0.0f, -0.5f);
	glEnd();

	glTranslatef(0.0f, -0.5f, 0.0f);
	drawCubes();

	glTranslatef(currentX, height-2-currentY, 0.0f);
	drawShape();

	glutSwapBuffers();
}

void drawHud(void){
    glRasterPos3f(-8.0, 15.0, 0.0);
	writeString(GLUT_BITMAP_9_BY_15, "3dtris v.0.1");
	glRasterPos3f(-8.1, 14.6, 0.0);
	writeString(GLUT_BITMAP_9_BY_15, "by tvh33");
    sprintf(stringBuffer, "%d", score);
    glRasterPos3f(-8.30, 13.8, 0.0);
    writeString(GLUT_BITMAP_9_BY_15, "score: ");
    writeString(GLUT_BITMAP_9_BY_15, stringBuffer);
}

void drawCubes(void){
    int i,j;
    int tmp, tmp2;

    for (i=height-2; i>=0; i--){
        glTranslatef(0.0f, 1.0f, 0.0f);
        tmp = 1;
        tmp2 = 0;
        for (j=1; j<11; j++){
            //glTranslatef(1.0f, 0.0f, 0.0f);
            if (board[i][j] > 0){
                glTranslatef(tmp, 0.0f, 0.0f);
                if (color == 1){
                    setColor(board[i][j]);
                }
                glutWireCube(size);
                tmp2 += tmp;
                tmp = 1;
            }else{
                tmp++;
            }
        }
        if (tmp2 == 0){
            glTranslatef(0.0f, -(height-2-i), 0.0f);
            return;
        }
        glTranslatef(-tmp2, 0.0f, 0.0f);
    }
    glTranslatef(0.0f, -(height-2), 0.0f);
}

void drawShape(void){
    int i,j;
    if (color == 1){
        setColor(currentShape+1);
    }else{
        glColor3f(0.5f, 0.5f, 1.0f);
    }
    glTranslatef(-3.0f, 3.0f, 0.0f);
    for (i=0; i<5; i++){
        glTranslatef(0.0f, -1.0, 0.0);
        for (j=0; j<5; j++){
            glTranslatef(1.0f, 0.0f, 0.0f);
            if (shape[currentShape][currentRotation][i][j] == 1){
                glutWireCube(size);
            }
        }
        glTranslatef(-5.0f, 0.0f, 0.0f);
    }
    glColor3f(0.0f, 0.0f, 0.0f);
}

void setColor(int n){
    switch (n-1){
        case 0: glColor3f(1.0f, 0.5f, 0.5f); break;
        case 1: glColor3f(0.5f, 1.0f, 0.5f); break;
        case 2: glColor3f(0.5f, 0.5f, 1.0f); break;
        case 3: glColor3f(1.0f, 1.0f, 0.5f); break;
        case 4: glColor3f(1.0f, 0.5f, 1.0f); break;
        case 5: glColor3f(0.0f, 1.0f, 1.0f); break;
        case 6: glColor3f(0.5f, 0.5f, 0.5f); break;
        default: glColor3f(0.0f, 0.0f, 0.0f); break;
    }
}

void update(int value){
	//angle += 20*dt;

	counter += dt;
	if (counter >= currentLimit){
	    tick();
        counter = 0.0f;
	}

	glutTimerFunc(DT, update, 1);
	glutPostRedisplay();
}

void setup(void){
    int i,j;

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

    // variables
    srand (time(NULL));

    for (i=0; i<height; i++){
        for (j=0; j<width; j++){
            if (j==0 || j==width-1 || i==height-1){
                board[i][j] = 1;
            }else{
                board[i][j] = 0;
            }
        }
    }
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glutWarpPointer(w/2, h/2);
	anglex = 0.0f;
	angley = 0.0f;
	mouseLastx = w/2;
	mouseLasty = h/7*5;
}

void keyInput(unsigned char key, int x, int y){
	//switch
	switch(key){
		case 'a':
			angley += 10.0f;
			break;
		case 'd':
			angley -= 10.0f;
			break;
        case 'z':
            drawGame();
            break;
        case ' ':
            currentLimit = fastLimit;
            break;
        case 'r':
            restart();
            break;
        case 'c':
            color = (color + 1) % 2;
            break;
        case 27:
            exit(0);
            break;
		default:
			break;
	}

}

void keyUp(unsigned char key, int x, int y){
    switch(key){
        case ' ':
            currentLimit = limit;
            break;
        default:
            break;
    }
}

void specialKeyInput(int key, int x, int y){
    if (key == GLUT_KEY_LEFT){
        if (nextStepAllowed(-1, 0, currentRotation) == 0){
            currentX -= 1;
        }
    }else if (key == GLUT_KEY_RIGHT){
        if (nextStepAllowed(1, 0, currentRotation) == 0){
            currentX += 1;
        }
    }else if (key == GLUT_KEY_UP){
        rotate(1);
    }else if (key == GLUT_KEY_DOWN){
        rotate(-1);
    }else if (key == GLUT_KEY_F11){
        if (fullscreen == 0){
            glutFullScreen();
            fullscreen = 1;
        }else{
            glutReshapeWindow(700, 700);
            glutPositionWindow(20,20);
            fullscreen = 0;
        }
    }
}

void mouse(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if (nextStepAllowed(-1, 0, currentRotation) == 0){
            currentX -= 1;
        }
    }else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        if (nextStepAllowed(1, 0, currentRotation) == 0){
            currentX += 1;
        }
    }else if (button == 3 && state == GLUT_DOWN){
        rotate(1);
    }else if (button == 4 && state == GLUT_DOWN){
        rotate(-1);
    }else if (button == GLUT_MIDDLE_BUTTON){
        if (state == GLUT_DOWN){
            currentLimit = fastLimit;
        }else if (state == GLUT_UP){
            currentLimit = limit;
        }
    }
}

void move(int x, int y){
    angley += (x - mouseLastx) * 2;
    anglex += (y - mouseLasty);
    mouseLastx = x;
    mouseLasty = y;
}

void tick(void){
    if (nextStepAllowed(0, 1, currentRotation) == 0){
        currentY++;
    }else{
        lockShape();
    }
    drawGame();
}

void rotate(int n){
    int tmp;
    tmp = (currentRotation + 4 + n) % 4;
    if (nextStepAllowed(0, 0, tmp) == 0){
        currentRotation = tmp;
    }
}

void drawGame(void){
    int i,j;
    for (i=0; i<height; i++){
        for (j=0; j<width; j++){
            if (i >= currentY-2 && i <= currentY+2 && j >= currentX-2 && j <= currentX+2 && shape[currentShape][currentRotation][i-currentY+2][j-currentX+2] == 1){
                printf("1");
            }else{
                printf("%d", board[i][j]);
            }

        }
        printf("\n");
    }
    printf("\n\n");
}

void removeRows(void){
    int i,j,p,q;
    int tmp;
	int rows = 0;

	for (i=1; i<height-1; i++){
		tmp = 0;
		for (j=1; j<11; j++){
			if (board[i][j] == 0){
				tmp = 1;
				break;
			}
		}
		if (tmp == 0){
			for (p=i; p>=1; p--){
				for (q=1; q<11; q++){
					board[p][q] = board[p-1][q];
				}
			}
			rows = rows + 1;
		}
	}
	score += rows;
}

int nextStepAllowed(int dx, int dy, int rot){
    int i,j;
    int yy = currentY + dy - 2;
    int xx = currentX + dx - 2;

    for (i=0; i<5; i++){
        for (j=0; j<5; j++){
            if (shape[currentShape][rot][i][j] == 1
                    && board[yy+i][xx+j] > 0){
                return -1;
            }
        }
    }
    return 0;
}

void lockShape(void){
    int i,j;
    int yy = currentY - 2;
    int xx = currentX - 2;

    for (i=0; i<5; i++){
        for (j=0; j<5; j++){
            if (shape[currentShape][currentRotation][i][j] == 1){
                board[yy+i][xx+j] = currentShape + 1;
            }
        }
    }

    currentY = 2;
    currentX = 6;
    currentShape = newShape();
    currentRotation = 0;

    removeRows();
}

int newShape(void){
    return rand() % 7;
}

void writeString(void *font, char *string){
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void restart(void){
    int i,j;

    for (i=0; i<height; i++){
        for (j=0; j<width; j++){
            if (j==0 || j==width-1 || i==height-1){
                board[i][j] = 1;
            }else{
                board[i][j] = 0;
            }
        }
    }

    currentY = 2;
    currentX = 6;
    currentRotation = 0;
    currentShape = newShape();

    score = 0;
}

int main(int argc, char **argv){
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(700, 700);
   glutInitWindowPosition(20, 20);
   glutCreateWindow("3dtris");
   setup();

   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutKeyboardUpFunc(keyUp);
   glutSpecialFunc(specialKeyInput);
   glutMouseFunc(mouse);
   glutPassiveMotionFunc(move);
   glutMotionFunc(move);
   glutTimerFunc(5, update, 1);
   glutMainLoop();
   return 0;
}
