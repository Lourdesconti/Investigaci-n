#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inicializa el LCD

// Pines para los LEDs
const int redPin = 2;
const int yellowPin = 3;
const int greenPin = 4;

// Duraciones de los colores en segundos
const int redDuration =90;
const int yellowDuration = 3;
const int greenDuration = 62;

// Pin del sensor infrarrojo
const int sensorPin = 7;  // Pin donde se conecta el FC-51
int autoCount = 0;        // Contador de autos
bool lastState = LOW;     // Estado anterior del sensor

void setup() {
  lcd.begin(16, 2);       // Inicia el LCD
  lcd.backlight();        // Activa la luz de fondo del LCD
  
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  pinMode(sensorPin, INPUT);  // Configura el pin del sensor como entrada
  
  lastState = digitalRead(sensorPin);  // Inicializa lastState con el estado actual del sensor
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Autos: 0");   // Muestra el contador inicial de autos
}

void loop() {
  // Semáforo rojo
  digitalWrite(redPin, HIGH);
  lcd.clear();
  countdown(redDuration, "Rojo");
  
  // Semáforo verde
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  lcd.clear();
  countdown(greenDuration, "Verde");
  
  // Semáforo amarillo
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, HIGH);
  lcd.clear();
  countdown(yellowDuration, "Amarillo");
  
  digitalWrite(yellowPin, LOW); // Apagar amarillo

  // Llamada para detectar autos en cada iteración
  detectAutos();
}

// Función de cuenta regresiva para el semáforo
void countdown(int duration, const char* color) {
  for (int i = duration; i > 0; i--) {
    lcd.setCursor(0, 0);
    lcd.print(color);   // Muestra el color actual del semáforo
    lcd.print(" Tiempo: ");
    lcd.print(i);
    lcd.print("s");
    lcd.setCursor(0, 1);
    lcd.print("Autos: ");
    lcd.print(autoCount);   // Muestra el contador de autos
    delay(1000); // Espera un segundo
    
    // Llama a la función de detección de autos dentro del loop
    detectAutos();
  }
}

// Función para detectar autos con el sensor infrarrojo
void detectAutos() {
  int currentState = digitalRead(sensorPin);  // Lee el estado actual del sensor

  // Solo contar si el estado anterior era LOW y el actual es HIGH
  if (lastState == LOW && currentState == HIGH) {
    autoCount++; // Incrementa el contador de autos
    delay(200);  // Espera un poco para evitar rebotes
  }

  lastState = currentState;  // Actualiza el estado anterior
}
