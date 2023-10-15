TaskHandle_t Tarea0;
void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(loop0,"Tarea_0",1000,NULL,1,&Tarea0,0);
}

void loop() {
  Serial.println("proceso en core #: " + String(xPortGetCoreID()));
  delay(1000);
}


void loop0(void *parameter) {
  while(1==1){
  Serial.println("\t\t\tproceso en core #: " + String(xPortGetCoreID()));
  delay(300);
}}
