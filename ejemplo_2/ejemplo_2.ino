TaskHandle_t Tarea0;    
TaskHandle_t Tarea1;   

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(loop0, "Tarea_0", 5000, NULL, 1, &Tarea0, 0); // Core 0
  xTaskCreatePinnedToCore(loop1, "Tarea_1", 5000, NULL, 1, &Tarea1, 1); // Core 1
  pinMode(12, OUTPUT); // Define Salida LEDs
  pinMode(14, OUTPUT); // Define Salida LEDs
}

void loop0(void *parameter) {  // Tarea0 parpadeo LED 0.1 segundos
  while (1) {
    digitalWrite(14, HIGH);
    Serial.println("\t\t\tproceso en core #: " + String(xPortGetCoreID()));
    delay(100);
    digitalWrite(14, LOW);
    Serial.println("\t\t\tproceso en core #: " + String(xPortGetCoreID()));
    delay(100);
  }
}

void loop1(void *parameter) {  // Tarea1 parpadeo LED 1 Segundo
  while (1) {
    digitalWrite(12, HIGH);
    Serial.println("proceso en core #: " + String(xPortGetCoreID()));
    delay(1000);
    digitalWrite(12, LOW);
    Serial.println("proceso en core #: " + String(xPortGetCoreID()));
    delay(1000);
  }
}

void loop() {
  // El loop principal está vacío ya que estamos usando RTOS y las tareas se ejecutan en paralelo.
}
