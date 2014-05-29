#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#define gridX 100
#define gridY 80
#define horRes 900
#define verRes 700

#define snakeMaxSize 100
#define snakeWidth 1

int isPause=0;

int xs[snakeMaxSize];
int ys[snakeMaxSize];

int foodX;
int foodY;

int pauseTime=59999;
int score = 0;
char *prevText;

int incrX=2; // 1,-1 or 0 
int incrY=0;	//1, -1 or 0

int snakeLength = 15;

void keyboard(unsigned char key, int x, int y);
void display(void);
void idleAnimiation(void);
void specialKeys(int key, int x, int y);
void printText(int x, int y, char *text);

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitWindowSize(horRes, verRes);
	glutCreateWindow("Snake");

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); //RGBA
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(-2, gridX+2,gridY+2,-6);
	int i;
	for(i=0;i<snakeLength;i++){
		ys[i] = gridY/2;
		xs[i]= gridX/4 + i*2*snakeWidth;
	}
	glutDisplayFunc(display);
	glutIdleFunc(idleAnimiation);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
}

void printText(int x, int y, char *text) {

	glColor3f(0, 0, 0);
	glRasterPos3f(x,y, 1);
	if(prevText==NULL);
	else{
	glRasterPos3f(x,y, 1);
	int i;
	for(i=0;i< strlen(text) ;i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, prevText[i]);
	}	
	}

	glColor3f(0.5,0.5,0);
	glRasterPos3f(x,y, 1);
	int i;
	for(i=0;i< strlen(text) ;i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

	}
	glColor3f(1.0,1.0,1.0);
	printf(" prev = %s curr = %s\n",prevText, text);
	if(prevText != NULL)
		strcpy(prevText,text);
	glFlush();

}

void drawCircle(int x, int y, int r){
//	printf("drawCicle\n");
//	glBegin(GL_LINE_LOOP);
//		glVertex2f(x-r,y-r);
//		glVertex2f(x+r,y-r);
//		glVertex2f(x+r,y+r);
//		glVertex2f(x-r,y+r);
	
	int i;
	float tx,ty;
	float thita=0;
	float incrThita = 12*3.1415/180; 
	glBegin(GL_LINE_LOOP);
	for(i=0;i<30;i++){
		tx =x+r*sin(thita+incrThita);
		ty = y+r*cos(thita+=incrThita);
		glVertex2f(tx,ty);
	}
	glEnd();
	glFlush();
}

void drawFood() {
	int r = 1;

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glVertex2f(foodX-r,foodY-r);
		glVertex2f(foodX+r,foodY-r);
		glVertex2f(foodX+r,foodY+r);
		glVertex2f(foodX-r,foodY+r);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	int x = rand()%gridX;
	int y = rand()%gridY;
//	printf("x = %d y = %d\n",x, y);
	glBegin(GL_POLYGON);
		glVertex2f(x-r,y-r);
		glVertex2f(x+r,y-r);
		glVertex2f(x+r,y+r);
		glVertex2f(x-r,y+r);
	glEnd();
	glColor3f(1.0,1.0,1.0);
	glFlush();
	foodX = x;
	foodY = y;
}

void display(void){
	int i=0;
	for(i=0;i<snakeLength;i++){
		drawCircle(xs[i],ys[i],snakeWidth);
	}

	glColor3f(0.5,0.5,0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-1,-2);
		glVertex2f(gridX+1,-2);
		glVertex2f(gridX+1, gridY);
		glVertex2f(-1,gridY);
	glEnd();
	glFlush();
	prevText = (char*) malloc(sizeof(char)*15);
	strcpy(prevText,"Score = 0");
	printText(10, -3,"Score = 0");
	glColor3f(1.0, 1.0, 1.0);

	char *txt="!  snake game using OpenGL  !";
	glRasterPos3f(gridX/2+5,-3, 1);
	for(i=0;i< strlen(txt) ;i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, txt[i]);
	}

	//srand(time(0));
	drawFood();
}

void idleAnimiation(void){
	if(!isPause){
	//glPushMatrix();
		int i=0;
		int newx, newy;
		newx = (gridX +xs[snakeLength-1]+incrX)%gridX;
		newy = (gridY +ys[snakeLength-1]+incrY)%gridY;


                for(i=0;i<snakeLength-1;i++){
                    if( abs(xs[i]-newx)<snakeWidth*2 && abs(ys[i]-newy)<snakeWidth*2) {
                        //dead condition
                    	printf("Sorry! Don't bite ownself\nYour are Dead");
                    	printText(2, gridY/2,"Sorry! Don't bite ownself$");
                    	sleep(1);
                    	exit(0);
                    }
		}

		if( abs(newx-foodX)<snakeWidth*2 && abs(newy-foodY)<snakeWidth*2){
				score++;
//				printf("Your Score = %d\n",score);
				char text[50];
				sprintf(text,"Score = %d",score);
				printText(10, -3,text);
				drawFood();
				drawCircle(newx,newy,snakeWidth);
				// what about making it longer
				xs[snakeLength]=newx;
				ys[snakeLength]=newy;
				snakeLength++;
				// assuming no one could reach 100 + 
				pauseTime -= 50;
		}else{
                     glColor3f(0.0,0.0,0.0);
                     drawCircle(xs[0], ys[0],snakeWidth); //erase
                     glColor3f(1.0,1.0,1.0);
                     drawCircle(xs[1],ys[1],snakeWidth);
                for(i=0;i<snakeLength-1;i++){
                       xs[i]=xs[i+1];
                       ys[i]=ys[i+1];
                  }
                  xs[snakeLength-1] = (gridX + xs[snakeLength-1]+incrX)%gridX;
                  ys[snakeLength-1] = (gridY + ys[snakeLength-1]+incrY)%gridY;
		}// end else

		drawCircle(xs[i],ys[i],snakeWidth);
		glFlush();
		}
		usleep(pauseTime);
		//glPopMatrix();
		//glutSwapBuffers();
}

void specialKeys(int key, int x, int y){
	//printf("%d is down \n",key);
	switch(key){
		case 103: // UP arrow
			if(incrY !=-snakeWidth*2)
				incrY=snakeWidth*2;
			incrX = 0;
			break;
		case 101:// down arrow
			if(incrY != snakeWidth*2)
				incrY = -snakeWidth*2;
			incrX = 0;
			break;
		case 102: //left arrow
			if(incrX != -snakeWidth*2)
				incrX = snakeWidth*2;
			incrY = 0;
			break;
		case 100: //right arrow
			if(incrX != snakeWidth*2)
				incrX = -snakeWidth*2;
			incrY = 0;
			break;
		default:
			break;
		}
}

void keyboard(unsigned char key, int x, int y) {
//printf("HEre key comes");
	switch(key){
		case 27:
			exit(0);
			break;
		case ' ':
				isPause = (isPause+1)%2;
				break;
		case 's':
			if(incrY !=-snakeWidth*2)
				incrY=snakeWidth*2;
			incrX = 0;
			break;
		case 'w':
			if(incrY != snakeWidth*2)
				incrY = -snakeWidth*2;
			incrX = 0;
			break;
		case 'a':
			if(incrX != snakeWidth*2)
				incrX = -snakeWidth*2;
			incrY = 0;
			break;
		case 'd':
			if(incrX != -snakeWidth*2)
				incrX = snakeWidth*2;
			incrY = 0;
			break;
		default:
			break;
	}
	
}
