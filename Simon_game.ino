//porte pin (buzzer e pulsante start)
#define BUZZER_PIN 2
#define START_BUTTON_PIN 13

//numero di livelli
#define LEVELS 10

//fasi del gioco
#define STATE_STOPPED 0
#define STATE_SHOW 1
#define STATE_REPEAT 2

//array di pin (pulsanti, led e note)
int buttons[4] = {11, 9, 7, 5};
int led[4] = {10, 8, 6, 4};
int notes[4] = {523, 587, 659, 698};

int sequence[LEVELS];         //mostra la sequenza

//inizializzazione livello e indice
int level = 0;                //livello del gioco
int index = 0;

int state = STATE_STOPPED;    //stato attuale



void setup() {

  //impostiamo i pin dei pulsanti in input e i pin dei led in output
  for(int i=0; i<4; i++){
    pinMode(buttons[i], INPUT);
    pinMode(led[i], OUTPUT);
    }
    
  pinMode(START_BUTTON_PIN, INPUT);   //pin del pulsante start in input
  pinMode(BUZZER_PIN, OUTPUT);        //pin del buzzer in output

  randomSeed(analogRead(A0));         //sequenza casuale di numeri, A0 è un pin analogico non collegato
  }



void loop() {

  //se il pulsante start viene premuto inizia il gioco
  if(digitalRead(START_BUTTON_PIN) == HIGH)
    welcome();

  //siamo nella fase di visualizzazione della sequenza
  if(state == STATE_SHOW)
    showNextLevel();

  //deve essere ripetuta la sequenza
  if(state == STATE_REPEAT)
    repeatSequence();

  delay(10);
  }



//funzione per suonare una delle 4 note musicali
void playNote(int note){
  tone(BUZZER_PIN, notes[note], 150);
  digitalWrite(led[note], HIGH);
  delay(150);
  digitalWrite(led[note], LOW);
  }


//funzione che legge tutti e 4 i pulsanti e sa quale è stato premuto
int readButtons(){
  for(int i=0; i<4; i++)
    if(digitalRead(buttons[i]) == HIGH)
      return i;

  return -1;
  }


//funzione che indica l'inizio di una nuova partita
void welcome(){
  int randomNumber;

  for(int i=0; i<7; i++){
    randomNumber = random(4);                       //genera un numero casuale
    tone(BUZZER_PIN, notes[randomNumber], 150);
    digitalWrite(led[randomNumber], HIGH);
    delay(75);
    digitalWrite(led[randomNumber], LOW);
    delay(75);
    }

  level = 0;
  state = STATE_SHOW;       //stato successivo
  delay(1200);
  }


//funzione che genera una nuova nota e visualizza tutta la sequenza creata fin adesso
void showNextLevel(){
  sequence[level] = random(4);      //genera una nota casuale al livello 'level'

  for(int i=0; i<=level; i++){
    playNote(sequence[i]);
    delay(200);
    }

  index = 0;
  level++;
  state = STATE_REPEAT;      //stato successivo
  }


//funzione di errore
void error(int note){
  tone(BUZZER_PIN, 200, 1000);     //nota bassa
  
  digitalWrite(led[note], HIGH);
  delay(1000);
  digitalWrite(led[note], LOW);
    
  state = STATE_STOPPED;          //ferma il gioco
  }


//funzione di vittoria
void win(){
  int randomNumber;
  delay(200);

  for(int i=0; i<12; i++){
    randomNumber = random(4);
    tone(BUZZER_PIN, notes[randomNumber], 150);

    //accende tutti i led
    for(int j=0; j<4; j++)
      digitalWrite(led[j], HIGH);
    delay(75);

    //spegne tutti i led
    for(int j=0; j<4; j++)
      digitalWrite(led[j], LOW);
    delay(75);
    }

  state = STATE_STOPPED;      //ferma il gioco
  }


//funzione di ripetizione della sequenza
void repeatSequence(){
  int selectedNote = readButtons();     //stato dei pulsanti

  //se un pulsante è stato premuto, e corrisponde alla nota aspettata possiamo suonarla
  if(selectedNote >= 0){
    if(selectedNote == sequence[index]){
      playNote(selectedNote);
      while(readButtons() != -1);
      index++;

      //se non abbiamo raggiunto il livello massimo ritorniamo alla visualizzazione della sequenza
      if(index >= level){
        if(level < LEVELS){
          state = STATE_SHOW;
          delay(1000);
          }
        else  win();            //altrimenti finisce il gioco
        }
        
      }
    else  error(selectedNote);  //se il pulsante premuto non corrisponde alla sequenza si genera un errore
    }
  }
