#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <gl.h>
#include <glu.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <time.h>
#include "sdlglutils.h"




#define WIDTH 1540 // max 1540
#define HEIGHT 800 // max 800
#define ZNEAR 1
#define ZFAR 15000
#define PLANDESIMULATION 20
#define _60FPS 16.7
#define ECHELLE_DISTANCE pow(10, 6)
#define ECHELLE_RAYON 100 / 4


//les informations des planetes  // toutes les distances en km et les periodes en secondes

//mercure
#define DEMI_GRAND_AXE_MERCURE	57909176 
#define EXCENTRICITE_MERCURE 0.20563069
#define DEMI_PETIT_AXE_MERCURE DEMI_GRAND_AXE_MERCURE * sqrt(1 - pow(EXCENTRICITE_MERCURE, 2))
#define RAYON_MERCURE 2439.7 
#define PERIODE_MERCURE 87.96934 * 24 * 3600

//venus
#define DEMI_GRAND_AXE_VENUS	108208930  
#define EXCENTRICITE_VENUS 0.0068
#define DEMI_PETIT_AXE_VENUS DEMI_GRAND_AXE_VENUS * sqrt(1 - pow(EXCENTRICITE_VENUS, 2))
#define RAYON_VENUS 6051.8 
#define PERIODE_VENUS 224.701 * 24 * 3600

//terre
#define DEMI_GRAND_AXE_TERRE	160000000 //149597887 valeur reel
#define EXCENTRICITE_TERRE 0.01671022
#define DEMI_PETIT_AXE_TERRE DEMI_GRAND_AXE_TERRE* sqrt(1 - pow(EXCENTRICITE_TERRE, 2))
#define RAYON_TERRE 6378.137
#define PERIODE_TERRE 365.256363 * 24 * 3600

//mars
#define DEMI_GRAND_AXE_MARS		227936637
#define EXCENTRICITE_MARS 0.09341233
#define DEMI_PETIT_AXE_MARS DEMI_GRAND_AXE_MARS* sqrt(1 - pow(EXCENTRICITE_MARS, 2))
#define RAYON_MARS 3396.2
#define PERIODE_MARS 686.9601 * 24 * 3600 // 2 ans

//jupiter
#define DEMI_GRAND_AXE_JUPITER	778412027
#define EXCENTRICITE_JUPITER 0.04839266
#define DEMI_PETIT_AXE_JUPITER DEMI_GRAND_AXE_JUPITER* sqrt(1 - pow(EXCENTRICITE_JUPITER, 2))
#define RAYON_JUPITER 71492 // plus grand
#define PERIODE_JUPITER 4335.3545 * 24 * 3600 // 11 ans

//saturne
#define DEMI_GRAND_AXE_SATURNE	1429394069
#define EXCENTRICITE_SATURNE 0.05415060
#define DEMI_PETIT_AXE_SATURNE DEMI_GRAND_AXE_SATURNE* sqrt(1 - pow(EXCENTRICITE_SATURNE, 2))
#define RAYON_SATURNE 60268
#define PERIODE_SATURNE 10757.7365 * 24 * 3600 // 29 ans

//uranus
#define DEMI_GRAND_AXE_URANUS	2870658186
#define EXCENTRICITE_URANUS 0.04725744
#define DEMI_PETIT_AXE_URANUS DEMI_GRAND_AXE_URANUS* sqrt(1 - pow(EXCENTRICITE_URANUS, 2))
#define RAYON_URANUS 25559
#define PERIODE_URANUS 30687.15 * 24 * 3600 // 84 ans

//neptune
#define DEMI_GRAND_AXE_NEPTUNE 4503443661
#define EXCENTRICITE_NEPTUNE 0.00858587
#define DEMI_PETIT_AXE_NEPTUNE DEMI_GRAND_AXE_NEPTUNE* sqrt(1 - pow(EXCENTRICITE_NEPTUNE, 2))
#define RAYON_NEPTUNE 24764
#define PERIODE_NEPTUNE 60224.9036 * 24 * 3600 // 164 ans 

//Halley 
#define DEMI_GRAND_AXE_HALLEY 17.9 * 150 * pow(10, 6)
#define EXCENTRICITE_HALLEY 0.96727
#define DEMI_PETIT_AXE_HALLEY DEMI_GRAND_AXE_HALLEY * sqrt(1 - pow(EXCENTRICITE_HALLEY, 2))
#define RAYON_HALLEY 1737.4 // meme rayon que la terre 
#define PERIODE_HALLEY 76 * 365.256363 * 24 * 3600 // 76 ans 

//inforamations sur la lune 
#define DEMI_GRAND_AXE_LUNE 22000000 //384399 valeur reel
#define EXCENTRICITE_LUNE 0.05490
#define DEMI_PETIT_AXE_LUNE DEMI_GRAND_AXE_LUNE* sqrt(1 - pow(EXCENTRICITE_LUNE, 2))
#define RAYON_LUNE 1737.4
#define PERIODE_LUNE 27.321582 * 24 * 3600 // 27 jours 






