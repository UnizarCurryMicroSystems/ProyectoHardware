/*********************************************************************************************
 * Fichero:	main.c
 * Autor: Marta Frias y Juan Vela
 * Descrip:	punto de entrada de C
 * Version:  <P4-ARM.timer-leds>
 *********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "stdio.h"
#include "inttypes.h"
#include "def.h"

// Tamaños de la cuadricula
// Se utilizan 16 columnas para facilitar la visualización
enum {
    NUM_FILAS = 9, NUM_COLUMNAS = 16, PADDING = 7
};

/*--- variables globales ---*/
extern uint8_t switch_timer;
extern int8_t valor_actual;
extern uint8_t pulsado;
extern uint8_t btn_izq_pnd;
extern uint8_t btn_dch_pnd;
extern uint8_t alarma_1s;

/*--- funciones externas ---*/
extern void leds_off();
extern void led1_on();
extern void led2_on();
extern void leds_switch();
extern void timer_init();
extern void Eint4567_init();

extern void Timer2_Inicializar();
extern void Timer2_Empezar();
extern unsigned int Timer2_Leer();
extern void DelayMs(int);
extern void Delay(int);

extern void D8Led_init();
extern void D8Led_symbol();
extern void D8Led_define_rango(uint8_t, uint8_t);
extern void D8Led_activar_avance(void);
extern void D8Led_desactivar_avance(void);

// LCD
extern void Lcd_Init(void);
extern void Lcd_pantalla_inicial(void);
extern void Lcd_pantalla_final(void);
extern void Lcd_cuadricula_sudoku(void);
extern void Lcd_rellenar_celda(INT8U, INT8U, CELDA);
extern void Lcd_actualizar_tiempo_calculo(int);
extern void Lcd_actualizar_tiempo_total(int);
extern void Lcd_limpiar_celda(INT8U, INT8U);
extern void Lcd_marcar_celda(INT8U, INT8U);
extern void Lcd_desmarcar_celda(INT8U, INT8U);

extern void celda_poner_valor(CELDA*, uint8_t);
extern int sudoku9x9(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], int, char*);

void DoUndef(void);
void DoDabort(void);

/*--- declaracion de funciones ---*/
void Main(void);

/* variables */

uint8_t estado_juego = 0;

// cuadricula SUDOKU. Definida en espacio de memoria de la aplicacion. Alineada
static CELDA cuadricula_ini[NUM_FILAS][NUM_COLUMNAS] __attribute__((aligned(32)))
= { { 0x9800, 0x6800, 0x0000, 0x0000, 0x0000, 0x0000, 0x7800, 0x0000, 0x8800, 0,
        0, 0, 0, 0, 0, 0 }, { 0x8800, 0x0000, 0x0000, 0x0000, 0x0000, 0x4800,
        0x3800, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x1800, 0x0000, 0x0000,
        0x5800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, {
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1800, 0x7800, 0x6800,
        0, 0, 0, 0, 0, 0, 0 }, { 0x2800, 0x0000, 0x0000, 0x0000, 0x9800, 0x3800,
        0x0000, 0x0000, 0x5800, 0, 0, 0, 0, 0, 0, 0 }, { 0x7800, 0x0000, 0x8800,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, {
        0x0000, 0x0000, 0x7800, 0x0000, 0x3800, 0x2800, 0x0000, 0x4800, 0x0000,
        0, 0, 0, 0, 0, 0, 0 }, { 0x3800, 0x8800, 0x2800, 0x1800, 0x0000, 0x5800,
        0x6800, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x0000, 0x4800, 0x1800,
        0x0000, 0x0000, 0x9800, 0x5800, 0x2800, 0x0000, 0, 0, 0, 0, 0, 0, 0 } };

// cuadricula SUDOKU. Definida en espacio de memoria de la aplicacion. Alineada
static CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS] __attribute__((aligned(32)))
= { { 0x9800, 0x6800, 0x0000, 0x0000, 0x0000, 0x0000, 0x7800, 0x0000, 0x8800, 0,
        0, 0, 0, 0, 0, 0 }, { 0x8800, 0x0000, 0x0000, 0x0000, 0x0000, 0x4800,
        0x3800, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x1800, 0x0000, 0x0000,
        0x5800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, {
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1800, 0x7800, 0x6800,
        0, 0, 0, 0, 0, 0, 0 }, { 0x2800, 0x0000, 0x0000, 0x0000, 0x9800, 0x3800,
        0x0000, 0x0000, 0x5800, 0, 0, 0, 0, 0, 0, 0 }, { 0x7800, 0x0000, 0x8800,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, {
        0x0000, 0x0000, 0x7800, 0x0000, 0x3800, 0x2800, 0x0000, 0x4800, 0x0000,
        0, 0, 0, 0, 0, 0, 0 }, { 0x3800, 0x8800, 0x2800, 0x1800, 0x0000, 0x5800,
        0x6800, 0x0000, 0x0000, 0, 0, 0, 0, 0, 0, 0 }, { 0x0000, 0x4800, 0x1800,
        0x0000, 0x0000, 0x9800, 0x5800, 0x2800, 0x0000, 0, 0, 0, 0, 0, 0, 0 } };

/*--- codigo de funciones ---*/

/**
 * Devuelve el valor del bit 11 de la celda (inidica si es pista). Si el valor
 * es igual a 1 la celda tiene un valor fijado inicialmente
 */
inline uint8_t celda_comprueba_pista(CELDA *celdaptr) {
    return ((*celdaptr & 0x800) >> 11);
}

/**
 * Realiza una espera activa hasta que se pulsa y se suelta el boton [boton]
 *
 * boton = 0 -> cualquier boton
 * boton = 1 -> boton izquierdo
 * boton = 2 -> boton derecho
 */
void esperarPulsacion(uint8_t boton) {

    // espera mientras :
    // - no se pulsa nada
    // - no se pulsa el boton esperado
    while (pulsado == 0 || (boton > 0 && pulsado != boton))
        ;

    // espera mientras no se suelta el boton esperado
    while (pulsado != 0)
        ;
}

/**
 * Devuelve 1 si se ha pulsado el boton [boton]
 *
 * boton = 0 -> cualquier boton
 * boton = 1 -> boton izquierdo
 * boton = 2 -> boton derecho
 */
uint8_t comprobarPulsacion(uint8_t boton) {

    uint8_t resp = 0;

    if (boton == 0) {

        if (btn_izq_pnd == 1 || btn_dch_pnd == 1) {
            resp = 1;
        }
    } else if (boton == 1) {

        if (btn_izq_pnd == 1) {
            resp = 1;
        }
    } else if (boton == 2) {

        if (btn_dch_pnd == 1) {
            resp = 1;
        }
    }

    btn_izq_pnd = 0;
    btn_dch_pnd = 0;

    return resp;
}

int preparar_movimiento() {

    unsigned int t1, t2;
    t1 = t2 = 0;

    // calcular candidatos del tablero
    // (opcion 1 -> C + C)
    t1 = Timer2_Leer();

    // TODO : investigar opcion 5 (no pone error)

    int result = sudoku9x9(cuadricula, 5, 0);

    t2 = Timer2_Leer() - t1;

    rellenar_cuadricula();

    // para filas el rango de valores posibles es [1,9]
    // pero se permite el 0 para finalizar la partida
    D8Led_define_rango(1, 0xA);

    // mostrar la letra F (de FILA) en el 8led
    D8Led_symbol(0xF);

    // si tablero NO terminado
    if (result != 0) {
        return t2;
    }
    // si tablero terminado
    else {
        return -t2;
    }

}

void reiniciar_cuadricula(void) {
    uint8_t i, j;

    for (i = 0; i < NUM_FILAS; i++) {
        for (j = 0; j < NUM_COLUMNAS - PADDING; j++) {
            cuadricula[i][j] = cuadricula_ini[i][j];
        }
    }
}

void rellenar_cuadricula(void) {

    uint8_t i, j;
    i = j = 0;

    for (i = 0; i < NUM_FILAS; i++) {
        for (j = 0; j < NUM_COLUMNAS - PADDING; j++) {

            CELDA celda = cuadricula[i][j];
            Lcd_limpiar_celda(i, j);
            Lcd_rellenar_celda(i, j, celda);
        }
    }

}

void Main() {
    // Inicializa controladores
    sys_init();         // inicializar la placa, interrupciones y puertos
    Lcd_Init();
    timer_init();       // inicializar el temporizador
    Eint4567_init();    // inicializar los pulsadores
    D8Led_init();       // inicializar el 8led

    // configurar e iniciar el Timer2
    Timer2_Inicializar();
    Timer2_Empezar();

    // desactivar avance de 8Led (para boton izquierdo)
    D8Led_desactivar_avance();

    uint8_t fila, columna;
    uint8_t fin = 0;
    uint8_t ultima_modificada;

    unsigned int tiempo_total = 0;
    unsigned int tiempo_calculo = 0;
    unsigned int alarma_5s = 0;
    unsigned int ultimo_segundo = 0;
    uint8_t parpadeo = 1;

    Lcd_pantalla_inicial();

    while (1) {

        if (estado_juego > 0 && fin == 0 && alarma_1s == 1) {
            Lcd_actualizar_tiempo_total(tiempo_total++);
            alarma_1s = 0;
        }

        if (estado_juego == 0) {

            if (comprobarPulsacion(0) == 1) {

                tiempo_calculo = 0;
                tiempo_total = 0;

                Lcd_cuadricula_sudoku();
                tiempo_calculo += preparar_movimiento();
                estado_juego++;

                Lcd_actualizar_tiempo_calculo(tiempo_calculo);
            }

        } else if (estado_juego == 1) {

            if (comprobarPulsacion(1) == 1) {

                // mostrar posicion inicial (un uno)
                D8Led_symbol(0x1);

                // Activar avance de 8Led (para boton izquierdo)
                D8Led_activar_avance();

                estado_juego++;
            }

        } else if (estado_juego == 2) {

            if (fin == 1 && comprobarPulsacion(2) == 1) {
                Lcd_pantalla_final();
                reiniciar_cuadricula();
                estado_juego = 0;
                fin = 0;

            } else if (fin == 1 && comprobarPulsacion(1) == 1) {

                Lcd_cuadricula_sudoku();
                rellenar_cuadricula();
                Lcd_actualizar_tiempo_calculo(tiempo_calculo);

                // para filas el rango de valores posibles es [1,9]
                // pero se permite el 0 para finalizar la partida
                D8Led_define_rango(1, 0xA);

                // mostrar la letra F (de FILA) en el 8led
                D8Led_symbol(0xF);

                estado_juego = 1;
                fin = 0;

            } else if (comprobarPulsacion(2) == 1) {

                // Desactivar avance de 8Led (para boton izquierdo)
                D8Led_desactivar_avance();

                // obtener numero de fila a partir del valor del 8led
                fila = valor_actual;

                // si se introduce una A -> terminar (Abortar)
                if (fila == 0xA) {
                    Lcd_pantalla_confirmar();
                    fin = 1;
                } else {

                    // para columnas el rango de valores posibles es [1,9]
                    D8Led_define_rango(1, 9);

                    // mostrar la letra C (de COLUMNA) en el 8led
                    D8Led_symbol(0xC);

                    estado_juego++;
                }
            }
        } else if (estado_juego == 3) {

            if (comprobarPulsacion(1) == 1) {

                // mostrar posicion inicial (un uno)
                D8Led_symbol(0x1);

                // activar avance de 8Led (para boton izquierdo)
                D8Led_activar_avance();

                estado_juego++;
            }

        } else if (estado_juego == 4) {

            if (comprobarPulsacion(2) == 1) {

                // desactivar avance de 8Led (para boton izquierdo)
                D8Led_desactivar_avance();

                // obtener numero de columna
                columna = valor_actual;

                // para contenido de celdas el rango de valores posibles es [0,9]
                D8Led_define_rango(0, 9);

                // mostrar valor inicial (un uno)
                D8Led_symbol(0x1);

                // activar avance de 8Led (para boton izquierdo)
                D8Led_activar_avance();

                estado_juego++;
            }
        } else if (estado_juego == 5) {

            if (comprobarPulsacion(2) == 1) {

                // desactivar avance de 8Led (para boton izquierdo)
                D8Led_desactivar_avance();

                uint8_t modificado = 0;

                // almacenar el valor introducido para la celda (a partir del 8led)
                // si y solo si NO es una pista inicial
                if (celda_comprueba_pista(&cuadricula[fila - 1][columna - 1])
                        == 0) {

                    ultima_modificada = celda_leer_valor(
                            cuadricula[fila - 1][columna - 1]);
                    celda_poner_valor(&cuadricula[fila - 1][columna - 1],
                            valor_actual);

                    modificado = 1;
                }

                int t = preparar_movimiento();

                if (t > 0) {
                    tiempo_calculo += t;

                    if (modificado == 1) {
                        alarma_5s = tiempo_total + 5;
                        ultimo_segundo = tiempo_total;
                        Lcd_marcar_celda(fila - 1, columna - 1);
                        parpadeo = 1;
                        estado_juego = 6;
                    } else {
                        estado_juego = 1;
                    }

                } else {
                    tiempo_calculo += (-t);
                    Lcd_pantalla_final();
                    reiniciar_cuadricula();
                    estado_juego = 0;
                }

                Lcd_actualizar_tiempo_calculo(tiempo_calculo);
            }
        } else if (estado_juego == 6) {

            // parpadeo de recuadro de la celda seleccionada
            if (ultimo_segundo != tiempo_total) {
                ultimo_segundo = tiempo_total;
                if (parpadeo == 1) {
                    Lcd_desmarcar_celda(fila - 1, columna - 1);
                    parpadeo = 0;
                } else {
                    Lcd_marcar_celda(fila - 1, columna - 1);
                    parpadeo = 1;
                }
            }

            if (comprobarPulsacion(0) == 1) {
                //descartar valor, restaurar anterior
                //desactivar alarma
                //...

                celda_poner_valor(&cuadricula[fila - 1][columna - 1],
                        ultima_modificada);

                int t = preparar_movimiento();

                if (t > 0) {
                    tiempo_calculo += t;
                    estado_juego = 1;
                } else {
                    tiempo_calculo += (-t);
                    Lcd_pantalla_final();
                    reiniciar_cuadricula();
                    estado_juego = 0;
                }

                Lcd_actualizar_tiempo_calculo(tiempo_calculo);
            }

            // al cabo de 5s confirmar el valor
            if (alarma_5s > 0 && tiempo_total >= alarma_5s) {

                int t = preparar_movimiento();

                if (t > 0) {
                    tiempo_calculo += t;
                    estado_juego = 1;
                } else {
                    tiempo_calculo += (-t);
                    Lcd_pantalla_final();
                    reiniciar_cuadricula();
                    estado_juego = 0;
                }

                Lcd_actualizar_tiempo_calculo(tiempo_calculo);
                alarma_5s = 0;
            }
        }
    }
}

/*void Main() {

 // Inicializa controladores
 sys_init();         // inicializar la placa, interrupciones y puertos
 timer_init();	    // inicializar el temporizador
 Eint4567_init(); 	// inicializar los pulsadores
 D8Led_init();       // inicializar el 8led

 // configurar e iniciar el Timer2
 Timer2_Inicializar();
 Timer2_Empezar();

 // provocar excepciones
 //DoUndef();

 // variables para medicion de tiempos
 int t1, t2;
 t1 = t2 = 0;

 // desactivar avance de 8Led (para boton izquierdo)
 D8Led_desactivar_avance();

 // esperar pulsacion de un boton para empezar
 esperarPulsacion(0);

 /////////////////////  SUDOKU  /////////////////////////

 uint8_t fila, columna;
 uint8_t fin = 0;

 while (fin != 1) {

 // calcular candidatos del tablero
 // (opcion 1 -> C + C)
 t1 = Timer2_Leer();
 sudoku9x9(cuadricula, 5, 0);
 //Delay(100);
 t2 = Timer2_Leer() - t1;

 // para filas el rango de valores posibles es [1,9]
 // pero se permite el 0 para finalizar la partida
 D8Led_define_rango(0, 9);

 // mostrar la letra F (de FILA) en el 8led
 D8Led_symbol(0xF);

 // esperar boton izquierdo
 esperarPulsacion(1);

 // mostrar posicion inicial (un uno)
 D8Led_symbol(0x1);

 // Activar avance de 8Led (para boton izquierdo)
 D8Led_activar_avance();

 // esperar boton derecho (confirma)
 esperarPulsacion(2);

 // Desactivar avance de 8Led (para boton izquierdo)
 D8Led_desactivar_avance();

 // obtener numero de fila a partir del valor del 8led
 fila = valor_actual;

 // si se introduce un 0 -> terminar
 if (fila == 0) {
 fin = 1;
 }

 // si no -> continuar
 else {
 // para columnas el rango de valores posibles es [1,9]
 D8Led_define_rango(1, 9);

 // mostrar la letra C (de COLUMNA) en el 8led
 D8Led_symbol(0xC);

 // esperar boton izquierdo
 esperarPulsacion(1);

 // mostrar posicion inicial (un uno)
 D8Led_symbol(0x1);

 // activar avance de 8Led (para boton izquierdo)
 D8Led_activar_avance();

 // esperar boton derecho (confirma)
 esperarPulsacion(2);

 // desactivar avance de 8Led (para boton izquierdo)
 D8Led_desactivar_avance();

 // obtener numero de columna
 columna = valor_actual;

 // para contenido de celdas el rango de valores posibles es [0,9]
 D8Led_define_rango(0, 9);

 // mostrar valor inicial (un uno)
 D8Led_symbol(0x1);

 // activar avance de 8Led (para boton izquierdo)
 D8Led_activar_avance();

 // esperar boton derecho (confirma)
 esperarPulsacion(2);

 // desactivar avance de 8Led (para boton izquierdo)
 D8Led_desactivar_avance();

 // almacenar el valor introducido para la celda (a partir del 8led)
 // si y solo si NO es una pista inicial
 if (celda_comprueba_pista(&cuadricula[fila - 1][columna - 1]) == 0) {
 celda_poner_valor(&cuadricula[fila - 1][columna - 1],
 valor_actual);
 }
 }
 }
 }*/
