#include "define.h"

using namespace std;



bool isRunning = true;
SDL_Window* win;
SDL_GLContext ctx;

class Point {
public:
	double x, y, z;

	Point() {
	}

	Point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	double distanceCarre(const Point& p) const {
		return sqrt((this->x - p.x)*(this->x - p.x) +
			(this->y - p.y)*(this->y - p.y));
	}

	/*void afficher(SDL_Renderer *renderer, int r, int g, int b, int taille) {
		SDL_Rect rect;
		rect.w = taille;
		rect.h = taille;
		rect.x = x - taille / 2;
		rect.y = y - taille / 2;

		SDL_SetRenderDrawColor(renderer, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);
	}*/

};







class Planete
{

public:

	static double revolutionDeTemps;

	GLuint idList;
	GLuint idTexture;
	GLUquadric* quadric;

	Point coordonnee;
	Point coordonneeFoyerDeRotation;
	double vitesseAngulaire;
	double demiGrandAxe;
	double demiPetitAxe;
	double l;
	double Excentricite;
	double distanceDemiFoyer;
	double rayon;

	//periode de revolution utour de soleil
	double periodeRevolution;

	//periode de revolution sur elle-meme
	double periodeRevolutionElleMeme;

	// abcisse curviligne, angle de la droite porte par la distance entre le foyer et la planete ete l'axe focal
	double omega;
	// angle de rotation sur elle-meme
	double alpha;

	//trajectoire de cette planete
	vector<Point> trajectoirePlanete;

	//static double  evolutionDeTemps;
	Planete() {

	}

	Planete(GLuint* idTexture, double demiGrandAxe, double demiPetitAxe, double rayon, Point coordonnee, double periodeRevolution, Point coordonneeFoyerDeRotation) {
		this->idTexture = *idTexture;
		this->coordonnee = coordonnee;
		this->coordonneeFoyerDeRotation = coordonneeFoyerDeRotation;
		this->demiGrandAxe = demiGrandAxe;
		this->demiPetitAxe = demiPetitAxe;
		this->rayon = rayon;
		this->distanceDemiFoyer = sqrt(pow(demiGrandAxe, 2) - pow(demiPetitAxe, 2));
		this->Excentricite = distanceDemiFoyer / demiGrandAxe;
		this->l = demiGrandAxe * (1 - pow(this->Excentricite, 2));
		this->periodeRevolution = periodeRevolution;
		this->vitesseAngulaire = 0;
		this->alpha = 0;
		this->omega = 0;
		this->periodeRevolutionElleMeme = 5;

		//evoultion de temps 
		//revolutionDeTemps = 100;

		//effecuter le trajectoire au complet
		effectuerTrajectoire(this->coordonneeFoyerDeRotation);

		//Enregistrement le facon de dessiner une sphere dans une list

		idList = glGenLists(1);
		glNewList(idList, GL_COMPILE);
		glPushMatrix();
		glTranslated(coordonnee.x, coordonnee.y, coordonnee.z);
		glRotated(this->alpha, 0, 0, 1);

		glColor3ub(255, 255, 255);
		glBindTexture(GL_TEXTURE_2D, this->idTexture);
		quadric = gluNewQuadric();
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricTexture(quadric, GL_TRUE);

		gluSphere(quadric, rayon, 30, 30);


		gluDeleteQuadric(quadric);
		glPopMatrix();
		glEndList();


	}

	~Planete() {
		glDeleteLists(idList, 1);
		glDeleteTextures(1, &idTexture);
	}

	void render() {
		glPushMatrix();
		glTranslated(coordonnee.x, coordonnee.y, coordonnee.z);

		glRotated(alpha, 0, 0, 1);

		//afficher l'axe 
		//drawAxis(15);

		//afficher la planete 
		glCallList(idList);

		glPopMatrix();
	}


	void renderTrajectoire(double r, double g, double b, float taille) {


		//afficher le trajectoire 
		glPointSize(taille);

		glPushMatrix();

		glBegin(GL_POINTS);

		glColor3ub(r, g, b);

		for (auto point : this->trajectoirePlanete) {
			glVertex3d(point.x, point.y, point.z);
		}

		glEnd();
		glPopMatrix();
	}


	//evolution des planetes autour de soleil
	void evolutionAutourSoleil() {

		//calcul centre de l"ellipse a partir de foyer 
		Point centreEllipse = Point(this->coordonneeFoyerDeRotation.x + this->distanceDemiFoyer, this->coordonneeFoyerDeRotation.y, this->coordonneeFoyerDeRotation.z);
		//calcul distance entre le foyer et la planete 
		double distanceFoyerPlanete = l / (1 + this->Excentricite* cos(this->omega));
		// mise a jour de coordonée polaire en fonction de sa periode
		this->omega += vitesseAngulaire / (this->periodeRevolution + revolutionDeTemps);
		//mise a jour de coordonnée en x et y de planete
		this->coordonnee.x = centreEllipse.x + this->demiGrandAxe * cos(this->omega);
		this->coordonnee.y = centreEllipse.y + this->demiPetitAxe * sin(this->omega);
		this->coordonnee.z = this->coordonneeFoyerDeRotation.z;

		// ajout de chaque point dans un tabelau de trajectoire 
		//this->trajectoirePlanete.push_back(coordonnee);

		// mise a jour de vitesse angulaire
		this->vitesseAngulaire = (pow(distanceFoyerPlanete, 2) * sqrt(pow(this->Excentricite, 2) + 2 * this->Excentricite * cos(this->omega) + 1) / this->l);

	}


	//evolution des satellites autour d'une planete donnée
	void evolutionSatellite(Point coordonneeFoyerDeRotation) {

		//calcul centre de l"ellipse a partir de foyer 
		Point centreEllipse = Point(coordonneeFoyerDeRotation.x + this->distanceDemiFoyer, coordonneeFoyerDeRotation.y, coordonneeFoyerDeRotation.z);
		//calcul distance entre le foyer et la planete 
		double distanceFoyerPlanete = l / (1 + this->Excentricite* cos(this->omega));
		// mise a jour de coordonée polaire en fonction de sa periode
		this->omega += vitesseAngulaire / (this->periodeRevolution + Planete::revolutionDeTemps);
		//mise a jour de coordonnée en x et y de planete
		this->coordonnee.x = centreEllipse.x + this->demiGrandAxe * cos(this->omega);
		this->coordonnee.y = centreEllipse.y + this->demiPetitAxe * sin(this->omega);
		this->coordonnee.z = coordonneeFoyerDeRotation.z;

		// ajout de chaque point dans un tabelau de trajectoire 
		//this->trajectoirePlanete.push_back(coordonnee);

		// mise a jour de vitesse angulaire
		this->vitesseAngulaire = (pow(distanceFoyerPlanete, 2) * sqrt(pow(this->Excentricite, 2) + 2 * this->Excentricite * cos(this->omega) + 1) / this->l);

	}

	void evolutionAutourElleMeme() {
		double vitesseRotation = 2 * M_PI / this->periodeRevolutionElleMeme;
		alpha -= vitesseRotation;
	}


	//enregistrement de trajectoire 

	void effectuerTrajectoire(Point coordonneeFoyerCentral) {

		//calcul centre de l"ellipse a partir de foyer 
		Point centreEllipse = Point(coordonneeFoyerCentral.x + this->distanceDemiFoyer, coordonneeFoyerCentral.y, coordonneeFoyerCentral.z);

		for (double i = 0; i < 2 * M_PI; i += (2 * M_PI / 200))
		{
			Point traject = Point(0, 0, PLANDESIMULATION);
			traject.x = centreEllipse.x + this->demiGrandAxe * cos(i);
			traject.y = centreEllipse.y + this->demiPetitAxe * sin(i);
			traject.z = coordonneeFoyerCentral.z;
			this->trajectoirePlanete.push_back(traject);
		}
	}


	//affichage de trajectoire de commet de halley 
	void renderTrajectoireHalley(Point coordonneeFoyerCentral, double angleActuelleHalley) {
		Point centreEllipse = Point(coordonneeFoyerCentral.x + this->distanceDemiFoyer, coordonneeFoyerCentral.y, coordonneeFoyerCentral.z);
		int taille = 200;
		glPointSize(8);
		glBegin(GL_POINTS);
		for (double i = angleActuelleHalley; i > angleActuelleHalley - M_PI / 4 / 3; i -= M_PI / 100) {
			Point traject = Point(0, 0, PLANDESIMULATION);
			traject.x = centreEllipse.x + this->demiGrandAxe * cos(i);
			traject.y = centreEllipse.y + this->demiPetitAxe * sin(i);
			traject.z = coordonneeFoyerCentral.z;
			//this->trajectoirePlanete.push_back(traject);

			//affichage de trajet de halley
			glPointSize(taille);
			glColor3ub(100, 100, 255);
			glVertex3d(traject.x, traject.y, traject.z);
			taille--;
		}
		glEnd();
	}

};

double Planete::revolutionDeTemps = 1;


void dessinerSkyBox(GLuint* buttom, GLuint* left, GLuint* front, GLuint* right, GLuint* back, GLuint* top, int x, int y, int z) {


	glBindTexture(GL_TEXTURE_2D, *buttom); // buttom
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2d(0, 0);
	glVertex3i(-x, -y, 0);
	glTexCoord2d(0, 1);
	glVertex3i(-x, y, 0);
	glTexCoord2d(1, 1);
	glVertex3i(x, y, 0);
	glTexCoord2d(1, 0);
	glVertex3i(x, -y, 0);
	glEnd();

	//les faces des cotes
	glBindTexture(GL_TEXTURE_2D, *left); //left
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2d(0, 0);
	glVertex3i(x, -y, 0);
	glTexCoord2d(0, 1);
	glVertex3i(x, -y, z);
	glTexCoord2d(1, 1);
	glVertex3i(x, y, z);
	glTexCoord2d(1, 0);
	glVertex3i(x, y, 0);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, *front); //front
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2d(0, 0);
	glVertex3i(x, -y, 0);
	glTexCoord2d(0, 1);
	glVertex3i(x, -y, z);
	glTexCoord2d(1, 1);
	glVertex3i(-x, -y, z);
	glTexCoord2d(1, 0);
	glVertex3i(-x, -y, 0);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, *right); //right
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2d(0, 0);
	glVertex3i(-x, -y, 0);
	glTexCoord2d(0, 1);
	glVertex3i(-x, -y, z);
	glTexCoord2d(1, 1);
	glVertex3i(-x, y, z);
	glTexCoord2d(1, 0);
	glVertex3i(-x, y, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, *back); //back
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2d(0, 0);
	glVertex3i(x, y, 0);
	glTexCoord2d(0, 1);
	glVertex3i(x, y, z);
	glTexCoord2d(1, 1);
	glVertex3i(-x, y, z);
	glTexCoord2d(1, 0);
	glVertex3i(-x, y, 0);
	glEnd();



	glBindTexture(GL_TEXTURE_2D, *top); //top
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2d(0, 0);
	glVertex3i(-x, -y, z);
	glTexCoord2d(0, 1);
	glVertex3i(-x, y, z);
	glTexCoord2d(1, 1);
	glVertex3i(x, y, z);
	glTexCoord2d(1, 0);
	glVertex3i(x, -y, z);
	glEnd();

}



int main(int argc, char** argv) {

	//initialisation de sdl et openGl
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	win = SDL_CreateWindow("Simulateur systeme solaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// create context to openGL
	ctx = SDL_GL_CreateContext(win);

	// initialisation de rand
	srand(time(NULL));

	//initiliastion de revolution de temps
	//Planete::revolutionDeTemps = 1;



	// le telechargement des textures des plantes
	GLuint idTextureSoleil = loadTexture("sun.jpg");
	GLuint idTextureMercure = loadTexture("mercury.jpg");
	GLuint idTextureVenus = loadTexture("venus.jpg");
	GLuint idTextureTerre = loadTexture("earth.jpg");
	GLuint idTextureLune = loadTexture("moon.jpg");
	GLuint idTextureMars = loadTexture("mars.jpg");
	GLuint idTextureJupiter = loadTexture("jupiter.jpg");
	GLuint idTextureSaturne = loadTexture("saturne.jpg");
	GLuint idTextureUranus = loadTexture("uranus.jpg");
	GLuint idTextureNeptune = loadTexture("neptune.jpg");
	//commet de halley
	GLuint idTextureHalley = loadTexture("halley.jpg");

	//telechargement textures skybox space
	GLuint idTextureButtom = loadTexture("bottom.png");
	GLuint idTextureLeft = loadTexture("left.png");
	GLuint idTextureFront = loadTexture("front.png");
	GLuint idTextureRight = loadTexture("right.png");
	GLuint idTextureBack = loadTexture("back.png");
	GLuint idTextureTop = loadTexture("top.png");


	//les planetes
	Point coordonneeSoleil = Point(0, 0, PLANDESIMULATION);
	Planete soleil = Planete(&idTextureSoleil, 0, 0, 25, coordonneeSoleil, 0, coordonneeSoleil);
	Planete mercure = Planete(&idTextureMercure, DEMI_GRAND_AXE_MERCURE / ECHELLE_DISTANCE, DEMI_PETIT_AXE_MERCURE / ECHELLE_DISTANCE, RAYON_MERCURE / ECHELLE_RAYON, Point(0, 0, PLANDESIMULATION), PERIODE_MERCURE / (3600), coordonneeSoleil);
	Planete venus = Planete(&idTextureVenus, DEMI_GRAND_AXE_VENUS / ECHELLE_DISTANCE, DEMI_PETIT_AXE_VENUS / ECHELLE_DISTANCE, RAYON_VENUS / ECHELLE_RAYON, Point(0, 0, PLANDESIMULATION), PERIODE_VENUS / (3600), coordonneeSoleil);
	Planete terre = Planete(&idTextureTerre, DEMI_GRAND_AXE_TERRE / ECHELLE_DISTANCE, DEMI_PETIT_AXE_TERRE / ECHELLE_DISTANCE, RAYON_TERRE / ECHELLE_RAYON, Point(0, 0, PLANDESIMULATION), PERIODE_TERRE / (3600), coordonneeSoleil);
	Planete lune = Planete(&idTextureLune, DEMI_GRAND_AXE_LUNE / ECHELLE_DISTANCE, DEMI_PETIT_AXE_LUNE / ECHELLE_DISTANCE, RAYON_LUNE / ECHELLE_RAYON, Point(0, 0, PLANDESIMULATION), 400, terre.coordonnee);
	Planete mars = Planete(&idTextureMars, DEMI_GRAND_AXE_MARS / ECHELLE_DISTANCE, DEMI_PETIT_AXE_MARS / ECHELLE_DISTANCE, RAYON_MARS / ECHELLE_RAYON, Point(0, 0, PLANDESIMULATION), PERIODE_MARS / (3600), coordonneeSoleil);
	Planete jupiter = Planete(&idTextureJupiter, DEMI_GRAND_AXE_JUPITER / ECHELLE_DISTANCE, DEMI_PETIT_AXE_JUPITER / ECHELLE_DISTANCE, RAYON_JUPITER / ECHELLE_RAYON / 3, Point(0, 0, PLANDESIMULATION), PERIODE_JUPITER / (3600), coordonneeSoleil);
	Planete saturne = Planete(&idTextureSaturne, DEMI_GRAND_AXE_SATURNE / ECHELLE_DISTANCE, DEMI_PETIT_AXE_SATURNE / ECHELLE_DISTANCE, RAYON_SATURNE / ECHELLE_RAYON / 3, Point(0, 0, PLANDESIMULATION), PERIODE_SATURNE / (3600), coordonneeSoleil);
	Planete uranus = Planete(&idTextureUranus, DEMI_GRAND_AXE_URANUS / ECHELLE_DISTANCE, DEMI_PETIT_AXE_URANUS / ECHELLE_DISTANCE, RAYON_URANUS / ECHELLE_RAYON / 2, Point(0, 0, PLANDESIMULATION), PERIODE_URANUS / (3600), coordonneeSoleil);
	Planete neptune = Planete(&idTextureNeptune, DEMI_GRAND_AXE_NEPTUNE / ECHELLE_DISTANCE, DEMI_PETIT_AXE_NEPTUNE / ECHELLE_DISTANCE, RAYON_NEPTUNE / ECHELLE_RAYON / 2, Point(0, 0, PLANDESIMULATION), PERIODE_NEPTUNE / (3600), coordonneeSoleil);

	//comete de halley 
	Planete halley = Planete(&idTextureHalley, DEMI_GRAND_AXE_HALLEY / ECHELLE_DISTANCE, DEMI_PETIT_AXE_HALLEY / ECHELLE_DISTANCE, RAYON_HALLEY / ECHELLE_RAYON, Point(0, 0, PLANDESIMULATION), PERIODE_HALLEY / (3600), coordonneeSoleil);


	//gestion de commande de camera 
	int gaucheDroite = 0;
	int hautBas = 0;
	bool isEvolution = true;
	Point eye = Point(0, 0, 0);

	// gestion de frame per seconde 
	Uint32 currentTime, lastTime = 0;

	//matrix de projection 3d
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (double)WIDTH / HEIGHT, ZNEAR, ZFAR);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// effacer l'ecran en noir 
	glClearColor(0, 0, 0, 1);

	const Uint8* states;
	SDL_Event event;
	while (isRunning) {

		//implementation d'un evenement non bloquant 
		SDL_PollEvent(&event);
		states = SDL_GetKeyboardState(NULL);

		// gestion de commande clavier / souris
		if (event.type == SDL_QUIT)
			isRunning = false;
		if (states[SDL_SCANCODE_ESCAPE])
			isRunning = false;
		if (states[SDL_SCANCODE_P]) {
			if (isEvolution) {
				isEvolution = false;
			}
			else {
				isEvolution = true;
			}
		}
		if (states[SDL_SCANCODE_LEFT]) {
			gaucheDroite += 1;
		}
		if (states[SDL_SCANCODE_RIGHT]) {
			gaucheDroite -= 1;
		}
		if (states[SDL_SCANCODE_UP]) {
			hautBas += 1;
		}
		if (states[SDL_SCANCODE_DOWN]) {
			hautBas -= 1;
		}
		if (states[SDL_SCANCODE_A]) {
			terre.alpha++;
		}

		if (states[SDL_SCANCODE_Q]) {
			Planete::revolutionDeTemps -= 10;
		}

		if (states[SDL_SCANCODE_W]) {
			Planete::revolutionDeTemps += 10;
		}

		if (states[SDL_SCANCODE_T]) {
			eye.x = terre.coordonnee.x;
			eye.y = terre.coordonnee.y;
			eye.z = terre.coordonnee.z;
		}
		if (states[SDL_SCANCODE_S]) {
			eye.x = soleil.coordonnee.x;
			eye.y = soleil.coordonnee.y;
			eye.z = soleil.coordonnee.z;
		}
		if (states[SDL_SCANCODE_J]) {
			eye.x = jupiter.coordonnee.x;
			eye.y = jupiter.coordonnee.y;
			eye.z = jupiter.coordonnee.z;
		}
		if (states[SDL_SCANCODE_N]) {
			eye.x = neptune.coordonnee.x;
			eye.y = neptune.coordonnee.y;
			eye.z = neptune.coordonnee.z;
		}
		if (states[SDL_SCANCODE_M]) {
			eye.x = mercure.coordonnee.x;
			eye.y = mercure.coordonnee.y;
			eye.z = mercure.coordonnee.z;
		}
		if (states[SDL_SCANCODE_N]) {
			eye.x = neptune.coordonnee.x;
			eye.y = neptune.coordonnee.y;
			eye.z = neptune.coordonnee.z;
		}
		if (states[SDL_SCANCODE_V]) {
			eye.x = venus.coordonnee.x;
			eye.y = venus.coordonnee.y;
			eye.z = venus.coordonnee.z;
		}
		if (states[SDL_SCANCODE_U]) {
			eye.x = uranus.coordonnee.x;
			eye.y = uranus.coordonnee.y;
			eye.z = uranus.coordonnee.z;
		}
		if (states[SDL_SCANCODE_R]) {
			eye.x = saturne.coordonnee.x;
			eye.y = saturne.coordonnee.y;
			eye.z = saturne.coordonnee.z;
		}
		if (states[SDL_SCANCODE_0]) {
			eye.x = mars.coordonnee.x;
			eye.y = mars.coordonnee.y;
			eye.z = mars.coordonnee.z;
		}
		if (states[SDL_SCANCODE_L]) {
			eye.x = terre.coordonnee.x;
			eye.y = terre.coordonnee.y;
			eye.z = terre.coordonnee.z;
		}
		if (states[SDL_SCANCODE_C]) {
			eye.x = halley.coordonnee.x;
			eye.y = halley.coordonnee.y;
			eye.z = halley.coordonnee.z;
		}




		//effacer ecran
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// matrix des transformations geometriques 
		glMatrixMode(GL_MODELVIEW);

		// l'etat d'orgine de repere 
		glLoadIdentity();




		// position de camera 
		gluLookAt(gaucheDroite, 50, hautBas + 50, eye.x, eye.y, eye.z, 0, 0, 1);

		// sauvegarder l'etat de repere 
		//glPushMatrix();

		// homotethie

		//glScaled(100, 100, 1);
		//cube.render();

		// loader la derniere etat de repere sauvegarder 
		//glPopMatrix();

		//sauvegarde 
		//glPushMatrix();

		//dessiner ici //affichge de planete tout le temps

		//commet de halley
		halley.render();

		//planetes
		soleil.render();
		mercure.render();
		venus.render();
		terre.render();
		lune.render();
		mars.render();
		jupiter.render();
		saturne.render();
		uranus.render();
		neptune.render();



		//dessiner le skybox
		//dessinerSkyBox(&idTextureButtom, &idTextureLeft, &idTextureFront, &idTextureRight, &idTextureBack, &idTextureTop, ZFAR, ZFAR, ZFAR);


		//affichage de trajectoire de chaque planete tout le temps 

		glPushMatrix();
		glTranslated(0, 0, PLANDESIMULATION);
		mercure.renderTrajectoire(100, 200, 230, 2);
		venus.renderTrajectoire(50, 200, 100, 2);
		terre.renderTrajectoire(100, 255, 200, 2);
		mars.renderTrajectoire(33, 80, 120, 2);
		jupiter.renderTrajectoire(35, 64, 200, 2);
		saturne.renderTrajectoire(45, 78, 99, 2);
		uranus.renderTrajectoire(255, 100, 100, 2);
		neptune.renderTrajectoire(0, 255, 90, 2);

		//commet de halley 
		halley.renderTrajectoire(100, 100, 100, 2);
		halley.renderTrajectoireHalley(coordonneeSoleil, halley.omega);

		glPopMatrix();



		//lune.renderTrajectoire(0, 255, 90, 2);



		// cracher derniere matrice sauvegarder
		//glPopMatrix();

		currentTime = SDL_GetTicks();
		if (currentTime >= lastTime + _60FPS) {

			//commande de pause 
			if (isEvolution) {
				//evolution des planete autour de soleil 
				mercure.evolutionAutourSoleil();
				venus.evolutionAutourSoleil();
				terre.evolutionAutourSoleil();
				mars.evolutionAutourSoleil();
				jupiter.evolutionAutourSoleil();
				saturne.evolutionAutourSoleil();
				uranus.evolutionAutourSoleil();
				neptune.evolutionAutourSoleil();

				//evolution de la lune autour de la terre
				lune.evolutionSatellite(terre.coordonnee);

				//rotation de la terre
				terre.evolutionAutourElleMeme();

				//evolution commet de halley
				halley.evolutionAutourSoleil();
			}
			lastTime = currentTime;
		}

		//dessiner le skybox
		glPushMatrix();
		glTranslated(0, 0, -2000);
		dessinerSkyBox(&idTextureButtom, &idTextureLeft, &idTextureFront, &idTextureLeft, &idTextureBack, &idTextureTop, ZFAR / 2, ZFAR / 2, ZFAR / 2);
		glPopMatrix();

		//mettre a jour l ecran
		SDL_GL_SwapWindow(win);
	}

	SDL_GL_DeleteContext(ctx);
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
	return 0;
}

