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
static void *cb_malloc(size_t s, int n)
{
    return malloc(s);
}

/**
 * @brief Stub or Callback for fsm_free that calls real free.
 *
 * @param[in] p Pointer to allocated memory to free
 * @param[in] n Amount of calls to this function
 *
 */
static void cb_free(void *p, int n)
{
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
    fsm_t *f = (fsm_t *)1;

    f = fsm_new(NULL);

    TEST_ASSERT_EQUAL(NULL, f);
}

/**
 * @brief Comprueba que la función de inicialización devuelve false si el puntero a la maquina de estado es NULL
 *
 */
void test_fsm_init_falseWhenNullFsm(void)
{
    fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}};
    fsm_t *f = (fsm_t *)1;
    f = fsm_new(NULL);

    fsm_init(f, tt);
    TEST_ASSERT_FALSE(f);
}

/**
 * @brief Función de inicializacion devuelve false si la tabla de transiciones es nula
 *
 */
void test_fsm_init_falseWhenNullTransitions(void)
{
    fsm_t *f = (fsm_t *)1;
    bool res = fsm_init(f, NULL);
    TEST_ASSERT_FALSE(res);
}

/**
 * @brief La máquina de estados devuelve NULL
 *        y no llama a fsm_malloc si el estado de origen
 *        de la primera transición es -1 (fin de la tabla)
 */
void test_fsm_nullWhenFirstOrigStateIsMinusOne(void)
{
    fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}};
    fsm_t *f = (fsm_t *)1;
    f = fsm_new(tt);

    // TEST_ASSERT_EQUAL (XXX);
    TEST_ASSERT_EQUAL(f, NULL);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transición es -1 (fin de la tabla)
 *
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne(void)
{
    fsm_trans_t tt[] = {{1, is_true, -1, do_nothing}};
    fsm_t *f = fsm_new(tt);

    TEST_ASSERT_EQUAL(f, NULL);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si la función de comprobación de la primera transición es NULL (fin de la tabla)
 *
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull(void)
{
    fsm_trans_t tt[] = {{1, NULL, 1, do_nothing}};
    fsm_t *f = fsm_new(tt);

    TEST_ASSERT_EQUAL(f, NULL);
}

/**
 * @brief Devuelve puntero no NULL y llama a fsm_malloc (Stub) al crear la maquina de estados con una transición válida con función de actualización (salida) NULL o no NULL.
 *        Hay que liberar la memoria al final llamando a free
 *
 */
TEST_CASE(NULL)
TEST_CASE(do_nothing)
void test_fsm_new_nonNullWhenOneValidTransitionCondition(fsm_output_func_t out)
{
    fsm_trans_t tt[] = {
        {1, is_true, 1, out},
        {-1, NULL, -1, NULL}};
    fsm_malloc_AddCallback(cb_malloc);
    int* MeLaPela = 0;
    fsm_malloc_ExpectAnyArgsAndReturn(MeLaPela);
    fsm_t *f = (fsm_t *)1;
    f = fsm_new(tt);

    TEST_ASSERT_NOT_EQUAL(f, NULL);
    free(f);
}

/**
 * @brief Estado inicial corresponde al estado de entrada de la primera transición de la lista al crear una maquiina de estados y es valido.
 *
 */
void test_fsm_new_fsmGetStateReturnsOrigStateOfFirstTransitionAfterInit(void)
{
    fsm_trans_t tt[] = {
        {1, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}};
    fsm_malloc_AddCallback(cb_malloc);
    fsm_malloc_ExpectAnyArgsAndReturn(0);

    fsm_t *f = fsm_new(tt);
    int estado = fsm_get_state(f);
    TEST_ASSERT_EQUAL(estado, 1);
}

/**
 * @brief La maquina de estado no transiciona si la funcion devuelve 0
 *
 */
void test_fsm_fire_isTrueReturnsFalseMeansDoNothingIsNotCalledAndStateKeepsTheSame(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}};

    fsm_t f;
    int res, res2;
    fsm_init(&f, tt);
    res = fsm_get_state(&f);
    is_true_ExpectAnyArgsAndReturn(false);
    fsm_fire(&f);
    res2 = fsm_get_state(&f);

    TEST_ASSERT_EQUAL(res,res2);
}

/**
 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la función de guarda cuando se comprueba una transición
 *
 */
void test_fsm_fire_checkFunctionCalledWithFsmPointerFromFsmFire(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}};
    fsm_t f;
    is_true_ExpectAndReturn(&f,true);
    fsm_init(&f,tt);
    fsm_fire(&f);
}

/**
 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la transición devuelve true (cambia) y cuando devuelve false (mantiene)
 *
 */
TEST_CASE(false, 0)
TEST_CASE(true, 1)
void test_fsm_fire_checkFunctionIsCalledAndResultIsImportantForTransition(bool returnValue, int expectedState)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}};
    fsm_t f;
    fsm_init(&f, tt);
    is_true_ExpectAndReturn(&f,returnValue);
    fsm_fire(&f);
    int state = fsm_get_state(&f);
    TEST_ASSERT_EQUAL(state, expectedState);
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
        {-1, NULL, -1, NULL}};
    
    fsm_malloc_ExpectAnyArgsAndReturn(NULL);
    fsm_t *f;
    f = fsm_new(tt);

    TEST_ASSERT_EQUAL(f, NULL);
}

/**
 * @brief Llamar a fsm_destroy provoca una llamada a fsm_free (Mock, no Stub)
 *
 */
void test_fsm_destroy_callsFsmFree(void)
{
    fsm_t *f = (fsm_t *)1;
    fsm_free_ExpectAnyArgs();
    fsm_destroy(f);
}

/**
 * @brief Comprueba que solo se llame a la función de guarda que toca según el estado actual
 *
 */
void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true2, 0, NULL},   //Descomentar cuando se haya declarado una nueva función para mock is_true2
        {-1, NULL, -1, NULL}};

    fsm_t f;
    int state1,state2;
    fsm_init(&f, tt);
    state1 = fsm_get_state(&f);
    is_true_ExpectAnyArgsAndReturn(true);
    fsm_fire(&f);
    is_true2_ExpectAnyArgsAndReturn(true);
    fsm_fire(&f);
    state2 = fsm_get_state(&f);

    TEST_ASSERT_EQUAL(state1,state2);
}

/**
 * @brief Comprueba que se pueden crear dos instancias de máquinas de estados simultánteas y son punteros distintos
 *
 */
void test_fsm_new_calledTwiceWithSameValidDataCreatesDifferentInstancePointer(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}};
    fsm_malloc_Stub(cb_malloc);
    fsm_t *f1 = fsm_new(tt);
    fsm_t *f2 =fsm_new(tt);
    TEST_ASSERT_NOT_EQUAL(f1,f2);
}
