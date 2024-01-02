/* ***********************************************************************************************************************************************************
ESTE SKETCH FUNCIONA ÚNICAMENTE CON SERVOS MODIFICADOS PARA ROTAR DE FORMA CONTINUA
Los servos comúnmente están limitados a ángulos de entre 0 a 180 grados, pero hay modelos, como el TowerPro MG996R que giran sin detenerse.
De todas formas, existen tutoriales de cómo modificar los modelos comunes para que funcionen de esta forma
*********************************************************************************************************************************************************** */

// Inclusión de librerías ------------------------------------------------------------------------------------------------------------------------------------
#include <Servo.h>                                                                                // Servo.h es una librería dedicada al control de servos incluída en Arduino IDE

// Definición de los pines de los servos ---------------------------------------------------------------------------------------------------------------------
#define leftServoPin 9                                                                            // Pin del servo izquierdo
#define rightServoPin 10                                                                          // Pin del servo derecho

// Definición del switch del joystick ------------------------------------------------------------------------------------------------------------------------
#define joySwitchPin 11                                                                           // Pin para el click digital del joystick

// Definición del pin del buzzer -----------------------------------------------------------------------------------------------------------------------------
#define buzzerPin 8

// Definición de los pines de los LEDs -----------------------------------------------------------------------------------------------------------------------
#define ledDerecho 7
#define ledIzquierdo 4
#define ledPin 13                                                                                 // Pin del Built-in LED

// Definición del pin de la resistencia fotosensible ---------------------------------------------------------------------------------------------------------
#define pinFotoTras A2                                                                            // Pin A2 para la resistencia fotosensible trasera
#define pinFotoDela A3                                                                            // Pin A3 para la resistencia fotosensible delantera

// Definición de los pines del switch ON-OFF-ON --------------------------------------------------------------------------------------------------------------
#define pinSwOn1 2                                                                                // Pin de la primera posición On del switch
#define pinSwOn2 12                                                                               // Pin de la segunda posición On del switch

// Definición de los pines de los sensores de infrarrojos ----------------------------------------------------------------------------------------------------
#define pinIRDerecho 5
#define pinIRIzquierdo 6

// Definición del intervalo para el efecto intermitente ------------------------------------------------------------------------------------------------------
#define intervaloIntermitente 750                                                                 // Intervalo en milisegundos
#define intervaloPrecolision 1500

// Crea los objetos de los dos servos con sus constructores --------------------------------------------------------------------------------------------------
Servo leftServo;
Servo rightServo;

// Declaración e inicialización de variables para almacenar el tiempo de la iteración anterior de 'millis()' en los intermitentes ----------------------------
uint32_t previousMillisDerecho = 0;
uint32_t previousMillisIzquierdo = 0;

// Variables para el funcionamiento de la resistencia fotosensible -------------------------------------------------------------------------------------------
int valorSensor;                                                                                  // Variable que almacena los valores de la resistencia fotosensible
int sensorLow = 1023;                                                                             // Valor bajo del sensor (1023 porque funciona al revés al tratarse de una resistencia)
int sensorHigh = 0;                                                                               // Valor alto del sensor (0, por lo mismo)

// Toggles para la activación entre el modo manual y el modo sigue líneas ------------------------------------------------------------------------------------
bool modoManual = false;
bool modoSiguelineas = false;

// Función 'setup()' =========================================================================================================================================
void setup(){
  // Función 'servo.attach(uint8_t pin)' para asignar los pines a las variables donde escribir los cambios de sentido y velocidad ----------------------------
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);

  // Inicializar los servos en su posición neutra (90 grados) con 'servo.write(int angle)' -------------------------------------------------------------------
  leftServo.write(90);                                                                             // En 'servo.write()' se pueden escribir valores del rango 0 - 180 grados
  rightServo.write(90);

  // Establecer el modo I/O de cada uno de los periféricos comunes a ambos modos -----------------------------------------------------------------------------
  pinMode(ledDerecho, OUTPUT);
  pinMode(ledIzquierdo, OUTPUT);
  pinMode(pinSwOn1, INPUT);
  pinMode(pinSwOn2, INPUT);

  // Setup del modo manual -----------------------------------------------------------------------------------------------------------------------------------
  if(pinSwOn1 == HIGH && pinSwOn2 == LOW){
    // Establecer el modo I/O de cada uno de los periféricos del modo manual ---------------------------------------------------------------------------------
    pinMode(joySwitchPin, INPUT_PULLUP);                                                           // INPUT_PULLUP es un tipo de declaración de input que hace uso de una resistencia interna de tipo pull-up del Arduino UNO
    pinMode(ledPin, OUTPUT);                                                                       // El LED es una salida (muestra estado de la calibración)

    // Calibrar los sensores de precolisión dadas las condiciones lumínicas del lugar ------------------------------------------------------------------------
    calibrarPrecolision(pinFotoTras);
    delay(100);                                                                                    // delay para ver el cambio de qué sensor se está calibrando
    calibrarPrecolision(pinFotoDela);

    // Activamos el toggle manual y desactivamos el sigue líneas ---------------------------------------------------------------------------------------------
    modoManual = true;
    modoSiguelineas = false;
  }

  // Setup del modo sigue líneas -----------------------------------------------------------------------------------------------------------------------------
  if(pinSwOn1 == LOW && pinSwOn2 == HIGH){
    // Establecer el modo I/O de cada uno de los periféricos del modo sigue líneas ---------------------------------------------------------------------------
    pinMode(pinIRDerecho, INPUT);
    pinMode(pinIRIzquierdo, INPUT);

    // Desactivamos el toggle manual y activamos el sigue líneas ---------------------------------------------------------------------------------------------
    modoManual = false;
    modoSiguelineas = true;
  }

  // Se emite el sonido de inicio, dando a entender que el setup se ha completado ----------------------------------------------------------------------------
  sonidoIncio();
}

// Función 'loop()' ==========================================================================================================================================
void loop(){
  if(modoManual && !modoSiguelineas){
    controlManual();                                                                               // A cada iteración del loop se evalúa la función de control del robot, de la bocina, de los LEDs y de precolisiones
    sonidoBocina();
    lucesIntermitentes(ledDerecho, previousMillisDerecho);
    lucesIntermitentes(ledIzquierdo, previousMillisIzquierdo);
    precolision(pinFotoTras, 10);                                                                  // Si voy a chocar yendo hacia atrás, el offSet es positivo para ir hacia alante
    precolision(pinFotoDela, -10);                                                                 // Si voy a chocar yendo hacia adelante, el offSet es negativo para ir hacia atrás
  }
  if(!modoManual && modoSiguelineas){
    controlSiguelineas();
  }

  delay(20);                                                                                       // Se añade un delay de muestreo para evitar rebotes u otros problemas
}

// Función para el control sigue líneas ======================================================================================================================
void controlSiguelineas(){
  // Código para ir hacia adelante si ningún sensor detecta la línea negra -----------------------------------------------------------------------------------
  if((digitalRead(pinIRDerecho) == 0) && (digitalRead(pinIRIzquierdo) == 0)){
    leftServo.write(45);
    rightServo.write(135);
  }

  // Código para girar a la izquierda si el sensor derecho detecta la línea negra ----------------------------------------------------------------------------
  if((digitalRead(pinIRDerecho) == 1) && (digitalRead(pinIRIzquierdo) == 0)){
    leftServo.write(135);
    rightServo.write(135);
  }

  // Código para girar a la derecha si el sensor izquierdo detecta la línea negra ----------------------------------------------------------------------------
  if((digitalRead(pinIRDerecho) == 0) && (digitalRead(pinIRIzquierdo) == 0)){
    leftServo.write(45);
    rightServo.write(45);
  }

  // Código para detenerse si ambos sensores detectan la línea negra -----------------------------------------------------------------------------------------
  if((digitalRead(pinIRDerecho) == 0) && (digitalRead(pinIRIzquierdo) == 0)){
    leftServo.write(45);
    rightServo.write(135);
  }  
}

// Función sonido de inicio - PIÚ PIÚ (setup completo) =======================================================================================================
void sonidoIncio(){                                                                                // Función para imitar el sonido de un coche abriendo/cerrando con la llave a distancia
  for(int i = 750; i <= 1250; i += 20){                                                            // El bucle modifica la frecuencia, cuyo rango es de 750 a 1250Hz y aumenta en incrementos de 20Hz
    tone(buzzerPin, i);
    delay(10);                                                                                     // Aquí, la duración de cada frecuencia se da por este delay, no con el parámetro duración
  }

  for(int i = 750; i <= 1250; i += 20){                                                            // Se repite dos veces, igual que el efecto real
    tone(buzzerPin, i);
    delay(10);
  }
  noTone(buzzerPin);                                                                               // Aseguramos que la función acaba silenciando el buzzer
}

// Función para detener la marcha atrás en caso de colision y avanzar 2 segundos =============================================================================
void precolision(uint8_t pinFoto, int offSet){                                                     // Le pasamos el pin analógico que queramos leer y el offSet respecto del punto neutro (quieto) del robot
  valorSensor = analogRead(pinFoto);                                                               // Leemos el valor de la resistencia fotosensible elegida
  int distancia = map(valorSensor, sensorLow, sensorHigh, 0, 100);                                 // Mapeamos que los valores del rango de luminosidad de la resistencia se guarden en una variable escalada a un rango de 0 a 100

  if(distancia <= 20){                                                                             // Si la luz es menor a un 20%
    digitalWrite(ledPin, HIGH);                                                                    // Luz que indica el bloqueo de seguridad del sistema
    leftServo.write(90 - offSet);                                                                  // Movemos ambos servos hacia adelante durante el bloqueo
    rightServo.write(90 + offSet);
    delay(intervaloPrecolision);                                                                   // Durante 1500ms
    digitalWrite(ledPin, LOW);                                                                     // Apagamos la luz de bloqueo del sistema
  }
}

// Función para calibrar el sensor de pre-colisión ===========================================================================================================
void calibrarPrecolision(uint8_t pinFoto){                                                         // Le pasamos el pin analógico del sensor que queramos calibrar
  digitalWrite(ledPin, HIGH);                                                                      // Lo ponemos a brillar, HIGH

  uint32_t tiempoInicio = millis();                                                                // Guardamos el tiempo de inicio cuando se ejecuta la función

  // Código para calibrar el sensor del pin seleccionado -----------------------------------------------------------------------------------------------------
  while(millis() - tiempoInicio < 2500){                                                           // Bucle cuya condición es aguantar hasta que el contador de inicio dado por la función 'millis()' menos el tiempo de inicio de la función llegue a 2500ms. Se usa para CALIBRAR la resistencia fotosensible
    valorSensor = analogRead(pinFoto);                                                             // Asigno la variable del valor del sensor a la función de lectura analógica del pin donde conectamos la resistencia fotosensible

    if (valorSensor > sensorHigh){                                                                 // Si el valor analógico del sensor supera al umbralizado superior,
      sensorHigh = valorSensor;                                                                    // cambiamos el valor del umbralizado superior a ese valor en vez de 0
    }
    if (valorSensor < sensorLow){                                                                  // Si el valor analógico del sensor supera al umbralizado inferior,
      sensorLow = valorSensor;                                                                     // cambiamos el valor del umbralizado inferior a ese valor en vez de 1023
    }                                                                                              // IMPORTANTE: el calibrado reduce el intervalo de 0 - 1023 a uno más estrecho, adecuado a donde estemos. Para ello, durante los 5s de calibración, se debe exponer a la resistencia totalmente a la luz y taparlo completamente de forma intermitente con el dedo 3 o 4 veces
  }

  digitalWrite(ledPin, LOW);                                                                       // Hecha la calibración, apagamos el LED
}

// Función dirección luces ===================================================================================================================================
void lucesIntermitentes(uint8_t pinLED, uint32_t &previousMillis){
  int joyXValue = analogRead(A0);                                                                  // Leemos el valor del eje X del joystick

  // Código para el intermitente derecho ---------------------------------------------------------------------------------------------------------------------
  if((joyXValue <= 400 && pinLED == ledIzquierdo) || (joyXValue >= 600 && pinLED == ledDerecho)){  // Si estamos en el rango del eje X para el giro a la derecha
    unsigned long currentMillis = millis();                                                        // Se guarda en tiempo actual lo que vale 'millis()' para ese momento

    if(currentMillis - previousMillis >= intervaloIntermitente){                                   // Se comprueba que el tiempo actual menos el tiempo previo es mayor o igual que el intervalo
      previousMillis = currentMillis;                                                              // Se actualiza el tiempo previo con el tiempo actual más reciente
      digitalWrite(pinLED, !digitalRead(pinLED));                                                  // Se pone el LED al estado contrario al que estaba
    }
  }
  else{                                                                                            // Si no estamos en el rango del eje X para una de las direcciones de giro
    previousMillis = 0;                                                                            // Reseteamos el timer para ese intermitente
    digitalWrite(pinLED, LOW);                                                                     // Apagamos ese LED
  }
}

// Función bocina ============================================================================================================================================
void sonidoBocina(){
  uint8_t bocina = !digitalRead(joySwitchPin);                                                     // El switch digital del joystick usa lógica inversa. HIGH es no pulsado y, LOW, pulsado
  int joyYValue = analogRead(A1);
  
  static uint32_t previousMillisBocina = 0;                                                        // Variable para los tiempos del pitido trasero intermitente

  // Código para que la bocina suene cuando aprieto el switch del joystick -----------------------------------------------------------------------------------
  if(joyYValue <= 600 && bocina){                                                                  // Si pulso el botón,
    tone(buzzerPin, 100, 50);                                                                      // suena el tono, a cuya función se le pasan los parámetros 'pin', 'frecuencia(Hz)' y 'duración(ms)'
  }

  // Código para hacer pitar intermitentemente el buzzer al dar marcha atrás ---------------------------------------------------------------------------------
  else if(joyYValue > 600){                                                                        // Si se está en el rango de marcha atrás del joystick
    unsigned long currentMillis = millis();                                                        // Se guarda en tiempo actual el valor de 'millis()'

    if(currentMillis - previousMillisBocina >= intervaloIntermitente){                             // Se comprueba que el tiempo actual menos el tiempo previo es mayor o igual que el intervalo
      previousMillisBocina = currentMillis;                                                        // Se actualiza el tiempo previo con el tiempo actual más reciente
      tone(buzzerPin, 1500, 250);                                                                  // Se hace sonar el buzzer con la función tono a la que se le pasa 'pin', 'frecuencia(Hz)' y 'duración(ms)'
    }
  }

  // Código para hacer el efecto intermitente del buzzer -----------------------------------------------------------------------------------------------------
  else{
    noTone(buzzerPin);                                                                             // 'noTone()' es una función que cesa la onda cuadrada enviada al pin del buzzer
  }
}

// Función de control ========================================================================================================================================
void controlManual() {
  // Leemos los valores analógicos de los dos ejes del joystick ----------------------------------------------------------------------------------------------
  int joyXValue = analogRead(A0);
  int joyYValue = analogRead(A1);

  // Mapeamos los valores de los dos ejes del joystick a los valores del servo -------------------------------------------------------------------------------
  int servoYPosition = map(joyYValue, 0, 1023, 45, -45);                                           // Posición neutra del eje Y del joystick es 511, que corresponde a 0º en el servo. Respectivamente, 0 es 45º y 1023, -45º. Devuelve valores de -45 a 45
  int servoXPosition = map(joyXValue, 0, 1023, -45, 45);                                           // Lo para eL eje X del joystick es al revés, 0 es -45º y 1023, 45º. SE USA 45 PORQUE 90 PRODUCE ESPASMOS EN LOS MOTORES

  // Movimiento de los servos en función de los ejes del joystick - EJEMPLO COMENTADO, JOYSTICK A LA DERECHA. MOTOR IZQUIERDO HACIA ADELANTE, MOTOR DERECHO HACIA ATRÁS
  leftServo.write(90 - servoYPosition - servoXPosition);                                           // 90 + 0 - 45 = 45 -> Hacia adelante
  rightServo.write(90 + servoYPosition - servoXPosition);                                          // 90 - 0 - 45 = 45 -> Hacia atrás PORQUE ESTÁ PUESTO "ESPEJO" RESPECTO DEL OTRO EN EL ROBOT
}

/* ===========================================================================================================================================================

| -------------------------------------------------------------------------------------------------------- |
|                                           Tabla de movimientos                                           |
| -------------------------------------------------------------------------------------------------------- |
| joyYValue | joyXValue | servoYPosition | servoXPosition | leftServo.write | rightServo.write | RESULTADO |
| --------- | --------- | -------------- | -------------- | --------------- | ---------------- | --------- |
|    0      |    511    |       45       |       0        |       45        |       135        | ADELANTE  | 
|   1023    |    511    |      -45       |       0        |      135        |        45        |   ATRÁS   |
|   511     |     0     |       0        |      -45       |      135        |       135        | IZQUIERDA |
|   511     |   1023    |       0        |       45       |       45        |        45        |  DERECHA  |
| --------- | --------- | -------------- | -------------- | --------------- | ---------------- | --------- |

Teniendo en cuenta que el punto neutro es ambos a 90, ambos a 45 no significa moverse hacia adelante y, ambos a 135, tampoco hacia atrás. Por elcontrario,
estos dos casos representarían, respectivamente, el giro a derechas y el giro a izquierdas.
El hecho de que, por ejemplo, hacia adelante sea LEFT a 45 y RIGHT a 135 es porque los motores están cambiados de sentido cuando se montan en el chasis */