#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>

#define ARRIBA     72
#define IZQUIERDA  75
#define DERECHA    77
#define ABAJO      80
#define ESC        27

#define COLOR_BORDE 10       
#define COLOR_CUERPO 14       
#define COLOR_COMIDA 12        
#define COLOR_BONUS 13      
#define COLOR_TEXTO 15       

int cuerpo[200][2];
int segmento_actual = 1;     
int tamano_culebra = 10;     
int pos_x = 10, pos_y = 12;  
int direccion = 3;           
int comida_x = 30, comida_y = 15; 
int bonus_x = 0, bonus_y = 0; 
int velocidad = 200;         
int nivel = 1;               
int puntuacion = 0;          

char tecla_actual;
bool bonus_activo = false;

void cambiar_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        SetConsoleTextAttribute(hConsole, color);
    }
}

void mover_cursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        COORD pos;
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition(hConsole, pos);
    }
}

void dibujar_bordes() {
    cambiar_color(COLOR_BORDE);
    for (int i = 2; i < 78; i++) {
        mover_cursor(i, 3); std::cout << (char)205;
        mover_cursor(i, 23); std::cout << (char)205;
    }

    for (int i = 4; i < 23; i++) {
        mover_cursor(2, i); std::cout << (char)186;
        mover_cursor(77, i); std::cout << (char)186;
    }

    mover_cursor(2, 3); std::cout << (char)201;
    mover_cursor(2, 23); std::cout << (char)200;
    mover_cursor(77, 3); std::cout << (char)187;
    mover_cursor(77, 23); std::cout << (char)188;
    cambiar_color(COLOR_TEXTO);
}

void guardar_segmento() {
    cuerpo[segmento_actual][0] = pos_x;
    cuerpo[segmento_actual][1] = pos_y;
    segmento_actual++;
    if (segmento_actual == tamano_culebra) segmento_actual = 1;
}

void dibujar_culebra() {
    cambiar_color(COLOR_CUERPO);
    for (int i = 1; i < tamano_culebra; i++) {
        mover_cursor(cuerpo[i][0], cuerpo[i][1]); std::cout << "*";
    }
    cambiar_color(COLOR_TEXTO);
}

void borrar_cola() {
    mover_cursor(cuerpo[segmento_actual][0], cuerpo[segmento_actual][1]); std::cout << " ";
}

void leer_tecla() {
    if (_kbhit()) {
        tecla_actual = _getch();
        switch (tecla_actual) {
            case ARRIBA: if (direccion != 2) direccion = 1; break;
            case ABAJO: if (direccion != 1) direccion = 2; break;
            case DERECHA: if (direccion != 4) direccion = 3; break;
            case IZQUIERDA: if (direccion != 3) direccion = 4; break;
        }
    }
}

void gestionar_comida() {
    if (pos_x == comida_x && pos_y == comida_y) {
        mover_cursor(comida_x, comida_y); std::cout << " ";
        comida_x = (rand() % 73) + 4;
        comida_y = (rand() % 19) + 4;
        tamano_culebra++;
        puntuacion += 10;
        cambiar_color(COLOR_COMIDA);
        mover_cursor(comida_x, comida_y); std::cout << "o("; 
        cambiar_color(COLOR_TEXTO);
    }
}

void gestionar_bonus() {
    if (puntuacion % 100 == 0 && puntuacion != 0 && !bonus_activo) {
        bonus_x = (rand() % 73) + 4;
        bonus_y = (rand() % 19) + 4;
        bonus_activo = true;
        cambiar_color(COLOR_BONUS);
        mover_cursor(bonus_x, bonus_y); std::cout << "$";
        cambiar_color(COLOR_TEXTO);
    }

    if (pos_x == bonus_x && pos_y == bonus_y && bonus_activo) {
        mover_cursor(bonus_x, bonus_y); std::cout << " ";
        puntuacion += 50;
        tamano_culebra += 2;
        bonus_activo = false;
    }
}

bool verificar_fin() {
    if (pos_y == 3 || pos_y == 23 || pos_x == 2 || pos_x == 77) return false;
    for (int i = tamano_culebra - 1; i > 0; i--) {
        if (cuerpo[i][0] == pos_x && cuerpo[i][1] == pos_y) return false;
    }
    return true;
}

void mostrar_puntaje() {
    cambiar_color(COLOR_TEXTO);
    mover_cursor(3, 1); std::cout << "Puntaje: " << puntuacion;
}

void ajustar_velocidad() {
    if (puntuacion == nivel * 20) {
        velocidad -= 10;
        nivel++;
    }
}

int main() {
    system("cls");
    dibujar_bordes();
    cambiar_color(COLOR_COMIDA);
    mover_cursor(comida_x, comida_y); std::cout << "o("; 
    cambiar_color(COLOR_TEXTO);

    while (tecla_actual != ESC && verificar_fin()) {
        borrar_cola();
        guardar_segmento();
        dibujar_culebra();

        gestionar_comida();
        gestionar_bonus();
        mostrar_puntaje();
        ajustar_velocidad();

        leer_tecla();
        if (direccion == 1) pos_y--;
        if (direccion == 2) pos_y++;
        if (direccion == 3) pos_x++;
        if (direccion == 4) pos_x--;

        Sleep(velocidad);
    }

    mover_cursor(30, 12); std::cout << "GAME OVER";
    mover_cursor(30, 14); system("pause");
    return 0;
}

