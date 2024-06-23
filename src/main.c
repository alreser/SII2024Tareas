#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <driver/gpio.h>
#include <esp_log.h>

//Declaración de variables globales e incializar funciones
static const char*  TAG = "SII2024Tareas";
#define PinMotor 23  

//#pragma region Interrupciones 
 

#define botonISR 27 //asigmo el pin 27 para recibir una interrupción externa. 
static int icontISR = 0; // contador de veces que se ejecutó la interrupción.  
static TickType_t ultimaISR; //tiempo en que se ejecutó por última vez la interrupción 
//función que ejecuta la rutina de interrrupción
void ISRAlarma(void *args)
{
 
 if (xTaskGetTickCountFromISR() > (ultimaISR + 20))
 {
    ultimaISR = xTaskGetTickCountFromISR() ;
    icontISR++; 
 };

}

/// @brief Función que inicializa la interrupción externa 
void InitISR()
{
    //Configuramos las propiedades de GPIO donde conectaremos la interrupción
    gpio_config_t gpioISR;
    gpioISR.pin_bit_mask = (1ULL << botonISR); 
    gpioISR.mode  = GPIO_MODE_DEF_INPUT;
    gpioISR.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioISR.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpioISR.intr_type = GPIO_INTR_POSEDGE;
    gpio_config(&gpioISR);

    //llamo a iniciar el servicio de interrupciones en Freertos 
    gpio_install_isr_service(0);
    gpio_isr_handler_add(botonISR, ISRAlarma, NULL);



}

//#pragma endregion Interrupciones

//#pragma region Tareas

TaskHandle_t handleATarea1, handleATarea2, handleATarea3; //aquí recibo el manejador para la tarea una vez creada

//función que ejecutará la Tarea 1 
void Tarea1(void *param)
{// aquí está el códigode la tarea 
unsigned long ulContador=0;

for (;;)
{
    /* code */
    ulContador++;
    printf("Estoy dentro de la tarea 1. Vuelta [%lu] y ejecuta en el nucleo [%lu]\n", ulContador, (unsigned long) xTaskGetCoreID(handleATarea1) ); 
    for ( long l = 0; l < 1000000; l++)
    {
        /* code */
        xTaskGetCoreID(handleATarea1);

    }
    

    vTaskDelay(30/ portTICK_PERIOD_MS); 
}


}


//funcion que eejecuta la tarea 2 
void Tarea2(void *param)
{// aquí está el códigode la tarea 
unsigned long ulContador=0;

for (;;)
{
    /* code */
    ulContador++;
    printf("Estoy dentro de la tarea 2. Vuelta [%lu] y ejecuta en el nucleo [%lu]\n", ulContador, (unsigned long) xTaskGetCoreID(handleATarea2) ); 
    for ( long l = 0; l < 1000000; l++)
    {
        /* code */
        xTaskGetCoreID(handleATarea2);

    }
    

    vTaskDelay(30/ portTICK_PERIOD_MS); 
}


}

//funcion de la tarea 3
void Tarea3(void *param)
{// aquí está el códigode la tarea 
unsigned long ulContador=0;

for (;;)
{
    /* code */
    ulContador++;
    printf("Estoy dentro de la tarea 3. Vuelta [%lu] y ejecuta en el nucleo [%lu]\n", ulContador, (unsigned long) xTaskGetCoreID(handleATarea3) ); 
    for ( long l = 0; l < 1000000; l++)
    {
        /* code */
        xTaskGetCoreID(handleATarea3);

    }
    

    vTaskDelay(30/ portTICK_PERIOD_MS); 
}


}

//#pragma endregion Tareas




void app_main() 
{

//creo 3 tareas 
//xTaskCreate(Tarea1, "Tarea 1", 2048, NULL,1, &handleATarea1 );
// Creo la tarea 1 y la asigno al segundo nucleo
//xTaskCreatePinnedToCore(Tarea1, "Tarea 1", 2048, NULL,1, &handleATarea1,0);

// Creo la tarea 2 y la asigno al segundo nucleo
//xTaskCreatePinnedToCore(Tarea2, "Tarea 2", 2048, NULL,1, &handleATarea2,0);

// Creo la tarea 3 y la asigno al segundo nucleo
//xTaskCreatePinnedToCore(Tarea3, "Tarea 3", 2048, NULL,1, &handleATarea3,0);


//Creamos una interruoción externa asociada al puerto 27. 
InitISR();

//Iniclizamos las propiedades del GPIO 23 
gpio_reset_pin(PinMotor); 
gpio_set_direction(PinMotor, GPIO_MODE_DEF_OUTPUT) ; 

bool bandera = true; 


    while (1)
    {



    if (bandera) 
    {
        bandera = !bandera;
        gpio_set_level(PinMotor, 0);

    }
    else
    {
        bandera = !bandera;
        gpio_set_level(PinMotor, 1);
    }
    
    

    //printf("Ejecutando el programa principaL ejecuta nucleo [%lu]\n", (unsigned long) xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
    printf("Contador de ISR=[%d]\n", icontISR);
    vTaskDelay(100/ portTICK_PERIOD_MS);



    }
}