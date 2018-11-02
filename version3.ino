#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include "pitches.h"

#define VOLTAGEINPUTVALUE 5.0 // I'm using 5 Volts

int temperatureSensorPin = 0; // Analog pin 0
int buzzerPin = 9; // Digital pin 9
float temperature = -5; // Hold temperature value
float pertinence;
long valor;
long valor2;
unsigned long timeNow;
unsigned long timeLast;

Fuzzy* fuzzy = new Fuzzy();// Instantiating an object library

// CREO CONJUNTOS DIFUSAS PARA LA ENTRADA DE TEMPERATURA
FuzzySet* baja = new FuzzySet(10, 10, 10, 25); // cold temperature
FuzzySet* media = new FuzzySet(20, 35, 55, 70); // fresh temperature
FuzzySet* alta = new FuzzySet(65, 100, 100, 100); // hot temperature

// CREO CONJUNTOS DIFUSAS PARA LA ENTRADA DE LUZ 
FuzzySet* poca = new FuzzySet(0, 0, 0, 40); // cold temperature
FuzzySet* normal = new FuzzySet(30, 50, 60, 75); // fresh temperature
FuzzySet* mucha = new FuzzySet(70, 100, 100, 100); // hot temperature

// Creating FuzzySets to compond FuzzyOutput danger
FuzzySet* freezingDanger = new FuzzySet(0, 20, 20, 30); // freezingDanger danger
FuzzySet* small = new FuzzySet(20, 40, 50, 70); // good small
FuzzySet* medium = new FuzzySet(30, 50, 60, 75); // meltingDanger danger
FuzzySet* fuego = new FuzzySet(200, 200, 200, 200); // meltingDanger danger

//FuzzyRule* fuzzyRule01;
//FuzzyRule* fuzzyRule02;
//FuzzyRule* fuzzyRule03;

void setup(){
   pinMode(buzzerPin, OUTPUT);
   digitalWrite(buzzerPin, LOW);
   Serial.begin(9600);
   FuzzyInput* temperature = new FuzzyInput(1);// Creating a FuzzyInput temperature, (as param, its ID)
   FuzzyInput* light = new FuzzyInput(2);// Creating a FuzzyInput temperature, (as param, its ID)
 
  temperature->addFuzzySet(baja); // Add FuzzySet cold to FuzzyInput temperature
  temperature->addFuzzySet(media); // Add FuzzySet fresh to FuzzyInput temperature
  temperature->addFuzzySet(alta); // Add FuzzySet hot to FuzzyInput temperature

  light->addFuzzySet(poca); // Add FuzzySet cold to FuzzyInput temperature
  light->addFuzzySet(normal); // Add FuzzySet fresh to FuzzyInput temperature
  light->addFuzzySet(mucha); // Add FuzzySet hot to FuzzyInput temperature

  fuzzy->addFuzzyInput(temperature); // Add FuzzyInput to Fuzzy object
  fuzzy->addFuzzyInput(light); // Add FuzzyInput to Fuzzy object
  
  FuzzyOutput* danger = new FuzzyOutput(1);// Creating FuzzyOutput danger, With its ID in param

  danger->addFuzzySet(freezingDanger); // Add FuzzySet freezingDanger to danger
  danger->addFuzzySet(small); // Add FuzzySet small to danger
   danger->addFuzzySet(medium); // Add FuzzySet small to danger
  danger->addFuzzySet(fuego); // Add FuzzySet meltingDanger to danger
  
  fuzzy->addFuzzyOutput(danger); // Add FuzzyOutput to Fuzzy object
  
  //Assembly the Fuzzy rules

    // FuzzyRule "IF temperature = hot THEN danger = meltingDanger"

  
  // FuzzyRule "IF temperature = hot THEN danger = meltingDanger"
  FuzzyRuleAntecedent* ifTemperatureHot = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureHot->joinSingle(alta); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerMeltingDanger = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerMeltingDanger->addOutput(medium);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifTemperatureHot, thenDangerMeltingDanger); // Passing the Antecedent and the Consequent of expression
 
  fuzzy->addFuzzyRule(fuzzyRule01); // Adding FuzzyRule to Fuzzy object

   // FuzzyRule "IF light = alta THEN danger = meltingDanger"
  FuzzyRuleAntecedent* ifLightHot = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifLightHot->joinSingle(mucha); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerMeltingDanger2 = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerMeltingDanger2->addOutput(medium);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule04 = new FuzzyRule(4, ifLightHot, thenDangerMeltingDanger2); // Passing the Antecedent and the Consequent of expression
 
  fuzzy->addFuzzyRule(fuzzyRule04); // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF temperature = fresh THEN danger = small"
  FuzzyRuleAntecedent* ifTemperatureFresh = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureFresh->joinSingle(media); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerSmall = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerSmall->addOutput(small); // Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifTemperatureFresh, thenDangerSmall); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule02); // Adding FuzzyRule to Fuzzy object

   // FuzzyRule "IF light = fresh THEN danger = small"
  FuzzyRuleAntecedent* ifLightFresh = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifLightFresh->joinSingle(normal); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerSmall2 = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerSmall2->addOutput(small); // Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule05 = new FuzzyRule(5, ifLightFresh, thenDangerSmall2); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule05); // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF temperature = cold THEN danger = freezingDanger"
  FuzzyRuleAntecedent* ifTemperatureCold = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureCold->joinSingle(baja); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerFreezingDanger = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerFreezingDanger->addOutput(freezingDanger);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifTemperatureCold, thenDangerFreezingDanger); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule03); // Adding FuzzyRule to Fuzzy object

   // FuzzyRule "IF temperature = cold THEN danger = freezingDanger"
  FuzzyRuleAntecedent* ifLightCold = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifLightCold->joinSingle(poca); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerFreezingDanger2 = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerFreezingDanger2->addOutput(freezingDanger);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule06 = new FuzzyRule(6, ifLightCold, thenDangerFreezingDanger2); // Passing the Antecedent and the Consequent of expression
  fuzzy->addFuzzyRule(fuzzyRule06); // Adding FuzzyRule to Fuzzy object

  
  
  FuzzyRuleAntecedent* ifTemperatureHot2 = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureHot2->joinSingle(alta); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleAntecedent* ifLightHot2 = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifLightHot2->joinSingle(mucha); // Adding corresponding FuzzySet to Antecedent object  
  FuzzyRuleAntecedent* ifTemperatureHotAndLightHot = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifTemperatureHotAndLightHot->joinWithAND(ifTemperatureHot2,ifLightHot2); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenDangerMeltingDanger7 = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenDangerMeltingDanger7->addOutput(fuego);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule07 = new FuzzyRule(7, ifTemperatureHotAndLightHot, thenDangerMeltingDanger7); // Passing the Antecedent and the Consequent of expression
 
  fuzzy->addFuzzyRule(fuzzyRule07); // Adding FuzzyRule to Fuzzy object
 
}

void loop(){
  timeNow = millis();// Get time since the program started in miliseconds
  if( (timeNow - timeLast) >= 1000 ){// Every 1 minute check the temperature
    timeLast = timeNow;
    // Read temperature
   // temperature = getTemperature(temperatureSensorPin);
    
 
  //Imprimimos por el monitor serie

    valor = analogRead(A0);
    valor2 = analogRead(A1);
    Serial.println(""); 
    Serial.println(""); 
    Serial.print("ENTRADA Tº: ");
    
    int entrada=(valor * 100.0 ) / 1023.0;
    Serial.print(entrada);
    Serial.println("");
    Serial.print("ENTRADA LUZ: ");
    int entrada2=(valor2 * 100.0 ) / 1023.0;
    Serial.println(entrada2);
    //Report teh temperature value, passing its ID and value
    fuzzy->setInput(1, entrada);
    fuzzy->setInput(2, entrada2);
  
     
    // Executing the fuzzification
    fuzzy->fuzzify();
     Serial.print("FUSIFICACION:"); 
      Serial.println("Pertenencia Conjutos:"); 
    Serial.print("Tº BAJA: ");    
    Serial.print(baja->getPertinence());
    Serial.print(" Tº MEDIA: ");    
    Serial.print(media->getPertinence());
    Serial.print(" Tº ALTA ");    
    Serial.print(alta->getPertinence());
    Serial.println(""); 
      Serial.print("LUZ BAJA: ");    
    Serial.print(poca->getPertinence());
    Serial.print(" LUZ MEDIA: ");    
    Serial.print(normal->getPertinence());
    Serial.print(" LUZ ALTA ");    
    Serial.print(mucha->getPertinence());
    Serial.println(""); 
    Serial.print("REGLAS QUE CUMPLE:");   
    /*
      Now, in this example, id dont want to know the crisp output value,
      I just want to know if fuzzyRule01 or fuzzyRule03 was fired, tu buz
    */
    //if( (fuzzy->isFiredRule(1) == true) or (fuzzy->isFiredRule(3) == true) ){
    if( (fuzzy->isFiredRule(1) == true) ){
       Serial.print(" REGLA 1 ");
    }
    if( (fuzzy->isFiredRule(2) == true)  ){
     Serial.print(" REGLA 2 ");     
    }
     if( (fuzzy->isFiredRule(3) == true)  ){
       Serial.print(" REGLA 3 ");
    }
     if( (fuzzy->isFiredRule(4) == true) ){
       Serial.print(" REGLA 4 ");
    }
    if( (fuzzy->isFiredRule(5) == true)  ){
     Serial.print(" REGLA 5 ");     
    }
     if( (fuzzy->isFiredRule(6) == true)  ){
       Serial.print(" REGLA 6 ");
    }
     if( (fuzzy->isFiredRule(7) == true)  ){
       Serial.print(" REGLA 7 ");
        tone(7, NOTE_C4, 1000);
    }

      Serial.print("\nSALIDA: ");
      float out = fuzzy->defuzzify(1);
     Serial.println(out);
 
   
  
  
     
       
  }
//  Serial.println("Risk output: ");
 
}

float getCurrent(float input){
  return ((input * VOLTAGEINPUTVALUE) / 1023.0);
}

float getTemperature(int pin){
  float lecture = analogRead(pin);
  float current = getCurrent(lecture);
  return current * 100;
}

void buzzerAlert(int buzzerDelay){
  digitalWrite(buzzerPin, HIGH);
  delay(buzzerDelay);
  digitalWrite(buzzerPin, LOW);
}
