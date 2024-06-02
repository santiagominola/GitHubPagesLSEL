#include "unity.h"

#define TEST_CASE(...)
#define TEST_RANGE(...)

#include "fsm.h"
#include "mock_test_fsm.h"

#include <stdlib.h>

/**
 * @file test_fsm_legacy.c
 * @author 
 * @author 
 * @brief Tests for existing fsm module
 * @version 0.1
 * @date 2024-04-09
 * 
 */

/**
 * @brief Stub or Callback for fsm_malloc that calls real malloc. 
 * 
 * @param[in] s Amount of bytes to allocate
 * @param[in] n Amount of calls to this function
 * 
 * @return pointer to allocated memory if success; NULL if fails
 * 
 */
static void* cb_malloc(size_t s, int n) {
    return malloc(s);
}

/**
 * @brief Stub or Callback for fsm_free that calls real free. 
 * 
 * @param[in] p Pointer to allocated memory to free
 * @param[in] n Amount of calls to this function
 * 
 */
static void cb_free(void* p, int n) {
    return free(p);
}

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * @brief Comprueba que la funcion de fsm_new devuelve NULL 
 *        y no llama a fsm_malloc si la tabla de transiciones es NULL 
 */
void test_fsm_new_nullWhenNullTransition(void)
{
    fsm_t *f = (fsm_t*)1; //inicializas la fsm y apuntas el puntero al valor de dirección 1

    f = fsm_new(NULL); //metes en la tabla de transiciones NULL

    TEST_ASSERT_EQUAL (NULL, f); //compruebas que el puntero que te da al meter NULL tambien te devuelve NULL
	//fsm_malloc es guardar memoria
}

/**
 * @brief Comprueba que la función de inicialización devuelve false si el puntero a la maquina de estado es NULL 
 *
 */
void test_fsm_init_falseWhenNullFsm(void)
{
    //mirar el fsm_init la funcion en el fsm.c
	fsm_trans_t tt = (fsm_trans_t*)1; //para tablas de transiones inicialización
	bool resultado = fsm_init(NULL,tt); // 
	TEST_ASSERT_EQUAL (false, f); //compruebas que devuelve false
	
	//tambien se puede poner int en lugar de bool y en lugar de false pones 0
	
	
}

/**
 * @brief Función de inicializacion devuelve false si la tabla de transiciones es nula
 * 
 */
void test_fsm_init_falseWhenNullTransitions(void)
{
    fsm_t *f = (fsm_t*)1; 
	bool resultado = fsm_init(f,NULL);  
	TEST_ASSERT_EQUAL (false, resultado); 
}

/**
* @brief La máquina de estados devuelve NULL 
*        y no llama a fsm_malloc si el estado de origen 
*        de la primera transición es -1 (fin de la tabla)
*/
void test_fsm_nullWhenFirstOrigStateIsMinusOne (void) {
  fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}}; //un array es como un puntero en c
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
 
  TEST_ASSERT_EQUAL (NULL,f);
  TEST_FAIL_MESSAGE("Implement the test");
  //mocks son funciones que simulan otras funciones, cuando quieres que una funcion que no se va a utilizar quieres que te devuleva algo concreto
  //en test_fsm.h ponemos las funciones que queremos mockear
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transición es -1 (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne (void) {
  fsm_trans_t tt[] = {{1, is_true, -1, do_nothing}}; //un array es como un puntero en c
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
 
  TEST_ASSERT_EQUAL (NULL,f);
  TEST_FAIL_MESSAGE("Implement the test");
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si la función de comprobación de la primera transición es NULL (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull (void) {
	
  fsm_trans_t tt[] = {{1, NULL, 1, do_nothing}}; //un array es como un puntero en c
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
 
  TEST_ASSERT_EQUAL (NULL,f);
}

/**
 * @brief Devuelve puntero no NULL y llama a fsm_malloc (AddCallback) al crear la maquina de estados con una transición válida con función de actualización (salida) NULL o no NULL.
 *        Hay que liberar la memoria al final llamando a free
 * 
 */
TEST_CASE(NULL)
TEST_CASE(do_nothing) //tiene que salir las dos opciones con el tick verde al pasar el test
void test_fsm_new_nonNullWhenOneValidTransitionCondition(fsm_output_func_t out)
{
	//el stub es el mock que simula la funcion y devuelve lo que tu quieres simular, no reserva las posiciones de memoria
	//el Callback si que reserva la dirección de memoria porque si que llama la función 
	//en esta hay que comprobar que se va a reservar bien las posiciones de memoria ya que la función si que lo hará
    fsm_trans_t tt[] = {
        {1, is_true, 1, out}, //para comprobar las dos opciones (NULL y do_nothing) con el TEST_CASE, variable entrada función
        {-1, NULL, -1, NULL}
    };
	
	// ver mocks en ceedling->build->test->mocks (mejor ir al .h) para ver las funciones de stub y callback (tambien estan en las diapos)
	 fsm_malloc_AddCallback(cb_malloc); //para llamar al malloc con Callback (tiene que hacerse antes de llamar a la funicón fsm)
	 fsm_malloc_ExpectAnyArgsAndReturn(0); //espera a que la llame, no cal especificar los valores de entrada, 
	//0 es el valor que quieres que te devuelva, aunque te da igual ya que al hacer Callback ya te devuelve lo que quieres
     fsm_t *f = (fsm_t*)1; //puntero de la fsm
	 f = fsm_new(tt);//f es lo que te devuleve de fsm_new al meter la tabla de transiciones
		
	
     TEST_ASSERT_NOT_EQUAL (NULL,f); //comprobación de que NO da NULL, transición buena, por lo que va a llamar a malloc

     free(f); //porque la tabla de transición no es NULL por lo que va a llamar a fsm_malloc (mirar fsm_new.c) por lo que hay que liberarlo
	 //poner tantas veces como llamas a malloc
}


/**
 * @brief Estado inicial corresponde al estado de entrada de la primera transición de la lista al crear una maquiina de estados y es valido. 
 * 
 */
void test_fsm_new_fsmGetStateReturnsOrigStateOfFirstTransitionAfterInit(void)
{
   fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing}, //para no confundirse entre los dos estados
        {-1, NULL, -1, NULL}
    };
	//si no se hace un malloc da error porque la llama ya que no da NULL por lo que hay que hacer un Callback
	 fsm_malloc_AddCallback(cb_malloc); 
	 fsm_malloc_ExpectAnyArgsAndReturn(0);
	 fsm_t *f = (fsm_t*)1; //puntero de la fsm
	 f = fsm_new(tt);
	 int state = fsm_get_state(f); //te da el estado del puntero f y se guarda en state
	
    TEST_ASSERT_EQUAL (0,sate); //que es el primer estado
	//TEST_ASSERT_EQUAL (0,fsm_get_state(f)); //tambien es correcto
}

/**
 * @brief La maquina de estado no transiciona si la funcion devuelve 0
 * 
 */
void test_fsm_fire_isTrueReturnsFalseMeansDoNothingIsNotCalledAndStateKeepsTheSame(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}
    };
  //hay que obligar con un mock a que is_true sea  para que no transicione, hay que hacer un stub ya que is_true no es una funcion como tal
    is_true_ExpectAnyArgsAndReturn(0); //is_true ya es un mock de por si
	fsm_t f;
    int res;
    fsm_init(&f, tt);
    res = fsm_get_state(&f); //estado actual
	fsm_fire(&f); //obligas a que transicione, a que se active la maquina
	int res2 = fsm_get_state(&f); //estado al transicionar
	TEST_ASSERT_EQUAL (res,res2); //compara los dos estados para ver si ha transicionado

   
}

/**
 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la función de guarda cuando se comprueba una transición
 * 
 */
void test_fsm_fire_checkFunctionCalledWithFsmPointerFromFsmFire(void)
{

    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };
	
	// fsm_new y fsm_init hacen lo mismo pero lo que cambia es como le das los argumentos
	//para comprobar que ha pasado una transicion hay que hacer un fsm_fire
	fsm_t f;
	is_true_ExpectAndReturn(&f,true);  //tiene que ser f lo que le llegue y si le llega que haga true
	fsm_init(&f,tt)
	fsm_fire(&f); //no cal test assert ya que si falla ya sabes que no pasa test

}

/** 
 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la transición devuelve true (cambia) y cuando devuelve false (mantiene)
 * 
 */
TEST_CASE(false, 0)
TEST_CASE(true, 1)
void test_fsm_fire_checkFunctionIsCalledAndResultIsImportantForTransition(bool returnValue, int expectedState) //returnValue es el valor que quieres de is_true y expectedState es el estado que quieres al transicionar
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };
	is_true_ExpectAnyArgsAndReturn(returnValue); //para que is_true haga lo que queremos del case
    fsm_t f;
    fsm_init(&f, tt);
	fsm_fire(&f); //hacer que transicione
	int state = fsm_get_state(&f)//coger el estado despues de transicionar
	TEST_ASSERT_EQUAL(expectedState, sta)
    
}


/**
 * @brief La creación de una máquina de estados devuelve NULL si la reserva de memoria falla (Mock, no Stub)
 * 
 */
void test_fsm_new_nullWhenFsmMallocReturnsNull(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };

    TEST_IGNORE();
}

/**
 * @brief Llamar a fsm_destroy provoca una llamada a fsm_free (Mock, no Stub)
 * 
 */
void test_fsm_destroy_callsFsmFree(void)
{
    TEST_IGNORE();
}

/**
 * @brief Comprueba que solo se llame a la función de guarda que toca según el estado actual
 * 
 */
void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        //{1, is_true2, 0, NULL},   //Descomentar cuando se haya declarado una nueva función para mock is_true2 //significa que lo metamos el is_true2 en el test_fsm.h
        {-1, NULL, -1, NULL}
    };

    fsm_t f;
    int res;
    fsm_init(&f, tt);
    res = fsm_get_state(&f);
	
	is_true_ExpectAnyArgsAndReturn(true);
	fsm_fire(&f); //primera transicion de is_true
	is_true2_ExpectAnyArgsAndReturn(true);
	fsm_fire(&f); //para la trasnsicion de is_true2

    
}

/**
 * @brief Comprueba que se pueden crear dos instancias de máquinas de estados simultánteas y son punteros distintos
 * 
 */
void test_fsm_new_calledTwiceWithSameValidDataCreatesDifferentInstancePointer(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };

    TEST_IGNORE();
}

//----------------------TDD-----------------------------------------
TEST_CASE(0,0);//para comprobar las 3 opciones que nos piden, y pones lo que quieres que te devuelva si son esos casos
TEST_CASE(128,128);
TEST_CASE(129,0);
void test_fsm_init_returniIntNumValidTransitionsEqualFSM_MAX_TRANSITIONS(int numTransition, int expectedTransition)
{
	fsm_trans_t tt[numTransition+1]; //el +1 es por la última fila no valida (el -1,NULL,-1,NULL)
    for(i=0;i<numTransition;i++) {
        tt[i].orig_state = 0;
        tt[i].in = is_true;
        tt[i].dest_state = 1;
        tt[i].out = do_nothing;
    }
	
	//es la última fila de -1,NULL,-1,NULL que se pone al final de la maquina de estado   
	tti[i].orig_state = -1;
    tti[i].in = NULL;
    tti[i].dest_state = -1;
    tti[i].out = NULL;
	
	fsm_t f;
    fsm_init(&f, tt);
	
	int num = fsm_init(&f, tt)
	
	TEST_ASSERT_EQUAL(num, expectedTransition);
	
}



