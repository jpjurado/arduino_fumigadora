
//PINES DE ENTRADA DE CONTROLADOR

  //REALIMENTACION ACTUADORES
    int PDMenos = 17;  
    int PDMas = 18;  
    int PIMenos = 19;  
    int PIMas = 20;  
    int PAMenos = 19;  
    int PAMas = 20;  
 
  
  //BOTONES TABLERO
    int bontnContraer = 2;  //Pin Interrupcion
    int botonExtender = 3;  //Pin Interrupcion
    int botonAltura = 21;
    int PotAltura = 0;         //Pin Analogo
    int PotSensorAltura = 1;         //Pin Analogo

//PINES DE SALIDA DE CONTROLADOR

  //ELECTROVALVULAS
    int pistonDerechaMas = 6;
    int pistonDerechaMenos = 7;
    int pistonIzquierdaMas = 8;
    int pistonIzquierdaMenos = 9;
    int pistonAlturaMas = 8; //10
    int pistonAlturaMenos = 9;  //11
  
    //INDICADORES PILOTOS
    int pilotoencendido = 53;
    
    int pilotoextender = 51;
    int pilotocontraer = 52;

    int pilotopistonDerechaMas = 49;
    int pilotopistonDerechaMenos = 48;
    int pilotopistonIzquierdaMas = 47;
    int pilotopistonIzquierdaMenos = 46;
    int pilotopistonAlturaMas =45;
    int pilotopistonAlturaMenos =44;

//VARAIBLES
    boolean contraerPistones = false;
    boolean extenderPistones = false;
    boolean alturaPiston = false;

    boolean PMenosEstado = false;
    boolean PMasEstado = false;

    float ReadAlturaNew = 0;
    float ReadAlturaOld = 0;
    
    
    
// Configuracion Inicial
void setup() {

    Serial.begin(9600);
 
  //ENTRADAS
    pinMode(PDMenos, INPUT);
    pinMode(PDMas, INPUT);
    pinMode(PIMenos, INPUT);
    pinMode(PIMas, INPUT);
    
    pinMode(bontnContraer, INPUT_PULLUP); //Pines con Interrupcion
    pinMode(botonExtender, INPUT_PULLUP); //Pines con Interrupcion
    pinMode(botonAltura, INPUT_PULLUP); //Pines con Interrupcion

    attachInterrupt(digitalPinToInterrupt(bontnContraer), contraer, CHANGE);
    attachInterrupt(digitalPinToInterrupt(botonExtender), extender, CHANGE);
    attachInterrupt(digitalPinToInterrupt(botonAltura), altura, CHANGE);

  //SALIDAS
    pinMode(pistonDerechaMas, OUTPUT);
    pinMode(pistonDerechaMenos, OUTPUT);
    pinMode(pistonIzquierdaMas, OUTPUT);
    pinMode(pistonIzquierdaMenos, OUTPUT);
    pinMode(pistonAlturaMas, OUTPUT);
    pinMode(pistonAlturaMenos, OUTPUT);
    
    pinMode(pilotoencendido, OUTPUT);
    pinMode(pilotoextender, OUTPUT);
    pinMode(pilotocontraer, OUTPUT);
    pinMode(pilotopistonDerechaMas, OUTPUT);
    pinMode(pilotopistonDerechaMenos, OUTPUT);
    pinMode(pilotopistonIzquierdaMas, OUTPUT);
    pinMode(pilotopistonIzquierdaMenos, OUTPUT);
    pinMode(pilotopistonAlturaMas, OUTPUT);
    pinMode(pilotopistonAlturaMenos, OUTPUT);

    digitalWrite(pistonDerechaMas, HIGH);
    digitalWrite(pistonDerechaMenos, HIGH);
    digitalWrite(pistonIzquierdaMas, HIGH);
    digitalWrite(pistonIzquierdaMenos, HIGH);
    digitalWrite(pistonAlturaMas, HIGH);
    digitalWrite(pistonAlturaMenos, HIGH);
    
    
    digitalWrite(pilotoencendido, HIGH); 
    delay(1000); 
}

boolean extenderBrazos(){
  Serial.println("entro extender Brazos");
  boolean ret = false;
  //Se verifica que los pistones este en Contraidos
  Serial.print("PIMenos: ");    Serial.println(digitalRead(PIMenos));
  Serial.print("PDMenos: ");    Serial.println(digitalRead(PDMenos));
  //if(digitalRead(PIMenos) && digitalRead(PDMenos)){
    PMenosEstado= true;   

  //Si ambos pistones estan Contraidos se activan hasta llegar a extenderse
    while(PMenosEstado){
      if(!digitalRead(PDMas)){
        Serial.println("Señal extender Derecho");
        digitalWrite(pistonDerechaMas, LOW);      //SALIDA DEL PISTON DERECHO
      }else{
         Serial.println("Termino extender Derecho");
        digitalWrite(pistonDerechaMas, HIGH);}

      if(!digitalRead(PIMas)){
        Serial.println("Señal extender Izquierdo");
        digitalWrite(pistonIzquierdaMas, LOW);    //SALIDA DEL PISTON IZQUIERDO
      }else{
        Serial.println("Termino extender Izquierdo");
        digitalWrite(pistonIzquierdaMas, HIGH);}

      if(digitalRead(PDMas) && digitalRead(PIMas)){   //TERMINO DE EXTENDER LOS DOS PISTONES
        Serial.println("Termino extenderBrazos");
        digitalWrite(pistonDerechaMas, HIGH);
        digitalWrite(pistonIzquierdaMas, HIGH);
        PMenosEstado = false;
        ret = true;
        break;
      }
    }
  //}else{}
  return ret;
}

boolean contraerBrazos(){
  Serial.println("entro contraerBrazos");
  boolean ret = false;
  //Se verifica que los pistones esten Extendidos
  Serial.print("PIMas: ");    Serial.println(digitalRead(PIMas));
  Serial.print("PDMas: ");    Serial.println(digitalRead(PDMas));
  //if(digitalRead(PIMas) && digitalRead(PDMas)){
    PMasEstado = true;

  digitalWrite(pistonDerechaMenos, LOW);    //ENTRADA DEL PISTON DERECHO
  digitalWrite(pistonIzquierdaMenos, LOW);  //ENTRADA DEL PISTON IZQUIERDO
    
  //Si ambos pistones estan Extraidos se activan hasta llegar a contraerse
  while(!(digitalRead(PDMenos) && digitalRead(PIMenos))){
    if(digitalRead(PDMenos)){
      Serial.println("Termino contraerBrazo Derecho");
      digitalWrite(pistonDerechaMenos, HIGH);
    }
    if(digitalRead(PIMenos)){
      Serial.println("Termino contraerBrazo Izquierdo");
      digitalWrite(pistonIzquierdaMenos, HIGH);
    }
    Serial.println("Esperando condicion");  
  }
  Serial.println("Termino contraer"); 
  ret = true;
  digitalWrite(pistonDerechaMenos, HIGH);
  digitalWrite(pistonIzquierdaMenos, HIGH);
  //}else{}
  return ret;
}


boolean configurarAltura(){
  Serial.println("entro configurar Altura");
  boolean ret = false;
  int type = -1;  //0 arriba  ,  1 abajo
  int rango = 10;
  
  int setPointAlturaValue = analogRead(PotAltura);
  Serial.print("Voltaje setPoint"); Serial.println(setPointAlturaValue);

  int sensorAlturaValue = analogRead(PotSensorAltura);
  Serial.print("Voltaje sensor"); Serial.println(sensorAlturaValue);

  type = (setPointAlturaValue - sensorAlturaValue < 0)?1:0;

  switch (type) {
    case 0:
      digitalWrite(pistonAlturaMenos, LOW);    //ENTRADA DEL PISTON ALTURA
    //do something when var equals 1
    break;
    case 1:
      digitalWrite(pistonAlturaMas, LOW);    //SALIDA DEL PISTON ALTURA
    //do something when var equals 2
    break;
  }

  while(abs(setPointAlturaValue - sensorAlturaValue) > rango && digitalRead(PAMas) == 0 && digitalRead(PAMenos) == 0){
    setPointAlturaValue = analogRead(PotAltura);
    sensorAlturaValue = analogRead(PotSensorAltura);
    Serial.println(setPointAlturaValue);
    Serial.print("  -  ");
    Serial.print(sensorAlturaValue);
    Serial.print("  -  ");
    Serial.println(setPointAlturaValue - sensorAlturaValue);
    
    delay(50);
  }

  digitalWrite(pistonAlturaMenos, HIGH);
  digitalWrite(pistonAlturaMas, HIGH);
  ret = true;

  
  return ret;
}



//PROGRAMA PRINCIPAL
void loop() {
//---------------------------------------------------------------------------------------------
  //Funcionalidad para Contraer los pistones
  if(contraerPistones  && !extenderPistones && !alturaPiston ){
    contraerBrazos();
    contraerPistones = false;
    extenderPistones = false;
    alturaPiston = false;
  }else {}

//---------------------------------------------------------------------------------------------
  //Funcionalidad para Extender los pistones
  if(extenderPistones && !contraerPistones && !alturaPiston){
    extenderBrazos();
    extenderPistones = false;
    contraerPistones = false;
    alturaPiston = false;
  }else{}
//---------------------------------------------------------------------------------------------
 //Funcionalidad para Ajustar la altura de los brazos
  if(alturaPiston && !contraerPistones  && !extenderPistones){
    configurarAltura();
    extenderPistones = false;
    contraerPistones = false;
    alturaPiston = false;
  }else{}
    

  delay(1000); 
} //FIN DEL PROGRAMA PRINCIPAL


//FUNCIONES DE LAS INTERRUPCIONES
void contraer(){
  Serial.println("interrupcion contraer");
  contraerPistones = true;
}
void extender(){
  Serial.println("interrupcion extender");
  extenderPistones = true;
}

void altura(){
  Serial.println("interrupcion altura");
  alturaPiston = true;
}


