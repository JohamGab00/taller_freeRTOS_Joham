TaskHandle_t Tarea0;
TaskHandle_t Tarea1;
TaskHandle_t Tarea2;
TaskHandle_t Tarea3;

const int pinMedicion = 34;     // Pin analógico para medir el voltaje
const int pinParlante = 32;     // Pin para el parlante 1
const int pinParlante2 = 27;     // Pin para el parlante 2
const int canalPWM = 0;         // Canal PWM para el parlante
const int canalPWMdos = 1;         // Canal PWM para el parlante2
const int pinEstado = 25;       // Pin para verificar el estado (0 o 1)

// Definición de las notas usando el formato que proporcionaste
const int NOTE_E4 = 329; // Mi
const int NOTE_E4S = 311; // Fa
const int NOTE_F4 = 349; // Fa#
const int NOTE_F4S = 370; // Sol
const int NOTE_G4 = 392; // Sol
const int NOTE_G4S = 415; // Sol#
const int NOTE_A4 = 440; // La
const int NOTE_A4S = 466; // La#
const int NOTE_B4 = 494; // Si
const int NOTE_C5 = 523; // Do (octava superior)
const int NOTE_C5S = 554; // Do#
const int NOTE_D5 = 587; // Re
const int NOTE_D5S = 622; // Re#
const int NOTE_E5 = 659; // Mi
const int NOTE_F5 = 698; // Fa
const int NOTE_F5S = 740; // Fa#
const int NOTE_G5 = 784; // Sol
const int NOTE_G5S = 831; // Sol#
const int NOTE_A5 = 880; // La
const int NOTE_A5S = 932; // La#
const int NOTE_B5 = 988; // Si
const int NOTE_P = 0;
const int NOTE_AS4 = 466; // La# / Si♭


// Melodía de Super Mario Bros
const int Melody[] = {
  NOTE_E5, NOTE_E5, NOTE_P, NOTE_E5, NOTE_P, NOTE_C5, NOTE_E5, NOTE_G5,
  NOTE_P, NOTE_G4, NOTE_P, NOTE_C5, NOTE_P, NOTE_G4, NOTE_E4, NOTE_A4,
  NOTE_B4, NOTE_P, NOTE_AS4, NOTE_A4, NOTE_P, NOTE_G4, NOTE_E5, NOTE_G5,
  NOTE_A5, NOTE_P, NOTE_F5, NOTE_G5, NOTE_P, NOTE_E5, NOTE_C5, NOTE_D5,
  NOTE_B4, NOTE_P, NOTE_C5, NOTE_P, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4,
  NOTE_P, NOTE_AS4, NOTE_A4, NOTE_P, NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5,
  NOTE_P, NOTE_F5, NOTE_G5, NOTE_P, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4,
  NOTE_P, NOTE_C5, NOTE_P, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_P,
  NOTE_AS4, NOTE_A4, NOTE_P, NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_P,
  NOTE_F5, NOTE_G5, NOTE_P, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_P
};


const int Durations[] = {
  50, 50, 70, 50, 70, 50, 50, 50,
  70, 50, 70, 50, 70, 50, 50, 50,
  50, 70, 50, 50, 70, 50, 50, 50,
  50, 70, 50, 50, 50, 50, 50, 50,
  50, 50, 50, 70, 70, 50, 50, 50,
  50, 50, 50, 50, 50, 70, 70, 50,
  50, 50, 50, 50, 50, 70, 50, 50,
  50, 50, 70, 50, 50, 50, 50, 70,
  50, 50, 50, 50, 50, 50, 50, 50,
  50, 50, 50, 50, 50, 70, 70, 50
};








// Duración de cada nota en milisegundos (aumentada para que dure más)
const int melodySize = sizeof(Melody) / sizeof(Melody[0]);

int melodyIndex = 0;


unsigned long lastAlarmTime = 0; // Variable para almacenar el tiempo en que se activó la alarma

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(loop0, "Tarea_0", 5000, NULL, 1, &Tarea0, 0); // Core 0
  xTaskCreatePinnedToCore(loop1, "Tarea_1", 5000, NULL, 1, &Tarea1, 1); // Core 1
  xTaskCreatePinnedToCore(loop2, "Tarea_2", 5000, NULL, 1, &Tarea2, 1); // Core 1
  xTaskCreatePinnedToCore(loop3, "Tarea_3", 5000, NULL, 1, &Tarea3, 0); // Core 0 

  pinMode(12, OUTPUT);   // Define Salida LEDs
  pinMode(14, OUTPUT);   // Define Salida LEDs
  pinMode(pinParlante, OUTPUT);  // Define el pin del parlante como salida
  pinMode(pinParlante2, OUTPUT);  // Define el pin del parlante2 como salida
  pinMode(pinEstado, INPUT_PULLUP); // Define el pin del estado como entrada con pull-up

  ledcSetup(canalPWM, 1000, 8); // Configura el generador PWM (frecuencia de 1000 Hz, resolución de 8 bits)
  ledcAttachPin(pinParlante, canalPWM); // Asocia el pin con el canal PWM
  ledcSetup(canalPWMdos, 1000, 8); // Configura el generador PWM (frecuencia de 1000 Hz, resolución de 8 bits)
  ledcAttachPin(pinParlante2, canalPWMdos); // Asocia el pin con el canal PWM
}

void loop0(void *parameter) {
  while (1) {
    digitalWrite(14, HIGH);
    Serial.println("tarea0 en core #: " + String(xPortGetCoreID()));
    delay(100);
    digitalWrite(14, LOW);
    Serial.println("tarea0 en core #: " + String(xPortGetCoreID()));
    delay(100);
  }
}

void loop1(void *parameter) {
  while (1) {
    digitalWrite(12, HIGH);
    Serial.println("\ttarea1 en core #: " + String(xPortGetCoreID()));
    delay(1000);
    digitalWrite(12, LOW);
    Serial.println("\ttarea1 en core #: " + String(xPortGetCoreID()));
    delay(1000);
  }
}

void loop2(void *parameter) {
  while (1) {
    int lectura = analogRead(pinMedicion); // Lee el valor analógico
    float voltaje = (lectura / 4095.0) * 3.3; // Convierte el valor a voltaje (0-3.3V)

    // Verifica si el voltaje es mayor o igual a 0.15 voltios
    if (voltaje >= 0.15) {
      // Si el voltaje es mayor o igual a 0.15 voltios, genera un tono en el parlante
      ledcWriteTone(canalPWMdos, 500); // Genera un tono de 1000 Hz (ajusta la frecuencia según sea necesario)
      delay(500);  // Mantiene el tono durante medio segundo (ajusta el tiempo según sea necesario)
      ledcWriteTone(canalPWMdos, 0); // Detiene el tono
      Serial.println("\t\t\t\t\ttarea2 en core #: " + String(xPortGetCoreID()) + "\tVoltaje : " + voltaje);
    }

    delay(1);
  }
}

void loop3(void *parameter) {
  while (1) {
    // Verifica el estado del pin de estado
    int estado = digitalRead(pinEstado);

    if (estado == 1) {
      int note = Melody[melodyIndex];
      int duration = Durations[melodyIndex];

      if (note == 0) {
        delay(duration);
      } else {
        ledcWriteTone(canalPWM, note);
        delay(duration);
        ledcWriteTone(canalPWM, 0);
      }
      
      melodyIndex++;
      if (melodyIndex >= melodySize) {
        melodyIndex = 0;
      }
    } else {
      melodyIndex = 0;
    }
    
    // Pequeña pausa para no saturar la CPU
    Serial.println("\t\t\t\t\ttarea3 en core #: " + String(xPortGetCoreID()));
    delay(100);
  }
}


void loop() {
  // El loop principal está vacío ya que estamos usando RTOS y las tareas se ejecutan en paralelo.
}
