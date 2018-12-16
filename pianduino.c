#define NUMBUTTONS 10
#define NUMFREQS 8

typedef struct node {
    //t represents both 
    //time held and time not held
    int t;
    int f;
    struct node *next;
} NODE;

//list
typedef struct list {
    NODE *head;
    NODE *tail;
} LIST;

//declare list
LIST l;


//long count = 0;
//speaker to pin assignment
const int speaker = 9;

//button to pin assignment
const int oUp = 11;
const int oDown = 10;
const int reco = 13;
const int play = 12;
const int key1 = 24;
const int key2 = 22;
const int key3 = 2;
const int key4 = 3;
const int key5 = 4;
const int key6 = 6;
const int key7 = 7;
const int key8 = 5;
const int key9 = 10;
const int key10 = 11;
const int key11 = 12;
const int key12 = 13;

//button states
int oUpState = 0;
int oDownState = 0;
int recoState = 0;
int playState = 0;
int key1State = 0;
int key2State = 0;
int key3State = 0;
int key4State = 0;
int key5State = 0;
int key6State = 0;
int key7State = 0;
int key8State = 0;
int key9State = 0;
int key10State = 0;
int key11State = 0;
int key12State = 0;

//prior button (key) states
int lkey1State = 0;
int lkey2State = 0;
int lkey3State = 0;
int lkey4State = 0;
int lkey5State = 0;
int lkey6State = 0;
int lkey7State = 0;
int lkey8State = 0;
int lkey9State = 0;
int lkey10State = 0;
int lkey11State = 0;
int lkey12State = 0;


//start presses
int recoStartPressed = 0;
int playStartPressed = 0;
int key1StartPressed = 0;
int key2StartPressed = 0;
int key3StartPressed = 0;
int key4StartPressed = 0;
int key5StartPressed = 0;
int key6StartPressed = 0;
int key7StartPressed = 0;
int key8StartPressed = 0;
int key9StartPressed = 0;
int key10StartPressed = 0;
int key11StartPressed = 0;
int key12StartPressed = 0;

//end presses
int recoEndPressed = 0;
int playEndPressed = 0;

//since we will be playing notes one at a time, 
//because chords sound horrible on the speaker, 
//we only need one general endPress, just to keep track
//of the last note pressed
int generalEndPressed = 0;

//int key1EndPressed = 0;
//int key2EndPressed = 0;

//time held
int recoTimeHold = 0;
int playTimeHold = 0;
int key1TimeHold = 0;
int key2TimeHold = 0;
int key3TimeHold = 0;
int key4TimeHold = 0;
int key5TimeHold = 0;
int key6TimeHold = 0;
int key7TimeHold = 0;
int key8TimeHold = 0;
int key9TimeHold = 0;
int key10TimeHold = 0;
int key11TimeHold = 0;
int key12TimeHold = 0;

//time released
int recoTimeReleased = 0;
int playTimeReleased = 0;
int key1TimeReleased = 0;
int key2TimeReleased = 0;
int key3TimeReleased = 0;
int key4TimeReleased = 0;
int key5TimeReleased = 0;
int key6TimeReleased = 0;
int key7TimeReleased = 0;
int key8TimeReleased = 0;
int key9TimeReleased = 0;
int key10TimeReleased = 0;
int key11TimeReleased = 0;
int key12TimeReleased = 0;

//keep track of reco timing
int recoTimeStamp = 0;

//boolean to check if we're recording
bool recording = false;

//boolean to check if we're playing back
bool playing = false;

//boolean to check when we should empty list
bool firstTime = false;


//, key2, key3, key4, key5, key6, key7, key8, key9, key10, key11, key12
int buttons[NUMBUTTONS] = { reco, play, key1, key2, key3, key4, key5, key6, key7, key8 };

//array of frequencies
//key1, key2, key3, in order
//261, 277, 293, 311, 329, 349, 369, 392, 415, 440, 466, 493

int freqs[NUMFREQS] = { 261, 293, 329, 349, 392, 440, 494, 523 };





void setup() {
    // put your setup code here, to run once:



    //begin console output
    Serial.begin(9600);

    //initialize reco button separately
    pinMode(reco, INPUT);

    //for loop to intialize keypin modes
    int i = 0;
    for (i = 0; i<NUMBUTTONS; i++) {
        pinMode(buttons[i], INPUT);
    }

    //initialize speaker output
    pinMode(speaker, OUTPUT);

    //initialize list
    l.head = NULL;
    l.tail = NULL;

}

//check if we should add to list function
void checkToAddToList(int key_time, int freq) {
    if (key_time != -1) {
        if (l.head == NULL) {
            l.head = (NODE*)malloc(sizeof(NODE));
            //will be general frequency later
            l.head->f = freq;
            l.head->t = key_time;
            l.head->next = NULL;
            l.tail = l.head;
        }
        else {
            l.tail->next = (NODE*)malloc(sizeof(NODE));
            //will be general frequency later
            l.tail->next->f = freq;
            l.tail->next->t = key_time;
            l.tail->next->next = NULL;
            l.tail = l.tail->next;
        }
    }
}

//check whether octave buttons have been pressed function
void checkOctaveButtons(int up, int down) {
    //check whether octave button has been pressed
    if (up == HIGH) {
        //the octave up button was just pressed
        int q = 0;
        tone(speaker, 1500);
        for (q = 0; q<NUMFREQS; q++) {
            freqs[q] = freqs[q] * 2;
        }
        delay(250);
        noTone(speaker);
    }
    if (down == HIGH) {
        int q = 0;
        tone(speaker, 400);
        for (q = 0; q<NUMFREQS; q++) {
            if (freqs[q] % 2 == 0) {
                freqs[q] = freqs[q] / 2;
            }
            else {
                freqs[q] = freqs[q] - 1;
                freqs[q] = freqs[q] / 2;
            }
        }
        delay(250);
        noTone(speaker);
    }

}


//generate tone depending on what key is pressed
void checkKeyStateToPlay(int key_state, int freq) {
    if (key_state == HIGH) {
        tone(speaker, freq);
        delay(25);
        noTone(speaker);
    }
}

//function to figure out the button time held down and button time not held down
//need to pass by value so it changes every time
int buttonTime(/*int *buttonPin, */int *buttonState, int *lastButtonState, int *startPressed, int *endPressed, int *timeHold, int *timeReleased) {
    // read the pushbutton input pin:

    //buttonState = digitalRead(buttonPin);
    // button state changed
    if (*buttonState != *lastButtonState) {
        Serial.print("button state changed\n");
        // the button was just pressed
        if (*buttonState == HIGH) {
            Serial.print("button just pressed\n");
            //millis() returns the number of milliseconds that have passed since the arduino booted up
            *startPressed = millis();
            *timeReleased = *startPressed - *endPressed;
            *lastButtonState = *buttonState;
            return *timeReleased;
            Serial.print("time released\n");
            Serial.print(*timeReleased);
            Serial.print("\n");

            // the button was just released
        }
        else {
            Serial.print("button just released\n");
            *endPressed = millis();
            Serial.print(*endPressed);
            Serial.print("\n");
            Serial.print(*startPressed);
            Serial.print("\n");
            *timeHold = *endPressed - *startPressed;
            *lastButtonState = *buttonState;
            return *timeHold;
        }
    }
    *lastButtonState = *buttonState;
    return -1;
}


void loop() {
    /*
    pos+=1;
    myservo.write(pos);
    */
    key1State = digitalRead(key1);
    key2State = digitalRead(key2);
    key3State = digitalRead(key3);
    key4State = digitalRead(key4);
    key5State = digitalRead(key5);
    key6State = digitalRead(key6);
    key7State = digitalRead(key7);
    key8State = digitalRead(key8);

    //Serial.print(key1State);
    //Serial.print(key2State);
    /*
    key3State = digitalRead(key3);
    key4State = digitalRead(key4);
    key5State = digitalRead(key5);
    key6State = digitalRead(key6);
    key7State = digitalRead(key7);
    key8State = digitalRead(key8);
    key9State = digitalRead(key9);
    key10State = digitalRead(key10);
    key11State = digitalRead(key11);
    key12State = digitalRead(key12);
    */

    //read record button
    recoState = digitalRead(reco);



    //read octave buttons
    oUpState = digitalRead(oUp);
    oDownState = digitalRead(oDown);




    //read play button
    playState = digitalRead(play);

    //delay just to be safe----???-----
    //delay(100);

    //this if statement is needed to see if we're recording at the moment
    if (recoState == HIGH) {
        //if the recording button has been pushed...
        //change recording to true if it was previously false...
        //...or change it to false if it was previously true
        Serial.print("in here\n");
        recording = !recording;

        if (recording) recoTimeStamp = millis();
        //
        firstTime = recording;
        //test to see if recording button is being pushed
        tone(speaker, 1000);

        //this delay is VERY important.  otherwise the "recording = !recording" will execute an unexpected amount
        //because the loop executes extremely fast, recoState will stay high and "recording = !recording" will happen more than once
        //If audience members want to try this, they cannot hold the button for too long either.  This will, again, cause 
        //"recording = !recording" to execute an unexpected amount.  The quicker the press, the less risky.  But 250ms is a good # for 
        //a normal press.  
        delay(250);
        noTone(speaker);
    }


    if (playState == HIGH) {
        //if the play button has been pushed
        playing = !playing;
        tone(speaker, 50);
        //delay's also important here for the same reasons as recoState comments
        delay(250);
        noTone(speaker);
    }


    //call function to change freq array depending on 
    //whether octave buttons have been pressed
    checkOctaveButtons(oUpState, oDownState);



    //***********************we'll do this part whether we're recording or not******************//

    checkKeyStateToPlay(key1State, freqs[0]);
    checkKeyStateToPlay(key2State, freqs[1]);
    checkKeyStateToPlay(key3State, freqs[2]);
    checkKeyStateToPlay(key4State, freqs[3]);
    checkKeyStateToPlay(key5State, freqs[4]);
    checkKeyStateToPlay(key6State, freqs[5]);
    checkKeyStateToPlay(key7State, freqs[6]);
    checkKeyStateToPlay(key8State, freqs[7]);
    /*
    checkKeyStateToPlay(key9State, freqs[8]);
    checkKeyStateToPlay(key10State, freqs[9]);
    checkKeyStateToPlay(key11State, freqs[10]);
    checkKeyStateToPlay(key12State, freqs[11]);
    */

    //******************************************************************************************//


    if (recording) {

        //checkOctaveButtons(oUpState, oDownState);
        //delay(100);

        //Delete all nodes
        if (firstTime) {
            NODE *temp;
            while (l.head != NULL) {
                temp = l.head;
                l.head = l.head->next;
                free(temp);
            }

            //************fixes timing stuff..VERY important*************
            generalEndPressed = recoTimeStamp;
            //**********************************************************//

            //set firstTime equal to false so we only delete nodes the first time around
            firstTime = false;
        }
        //Serial.print("\nFinished deleting all nodes\n");
        //Finished deleting all nodes

        //if we are currently recording
        int key1t = buttonTime(&key1State, &lkey1State, &key1StartPressed, &generalEndPressed, &key1TimeHold, &key1TimeReleased);
        int key2t = buttonTime(&key2State, &lkey2State, &key2StartPressed, &generalEndPressed, &key2TimeHold, &key2TimeReleased);
        int key3t = buttonTime(&key3State, &lkey3State, &key3StartPressed, &generalEndPressed, &key3TimeHold, &key3TimeReleased);
        int key4t = buttonTime(&key4State, &lkey4State, &key4StartPressed, &generalEndPressed, &key4TimeHold, &key4TimeReleased);
        int key5t = buttonTime(&key5State, &lkey5State, &key5StartPressed, &generalEndPressed, &key5TimeHold, &key5TimeReleased);
        int key6t = buttonTime(&key6State, &lkey6State, &key6StartPressed, &generalEndPressed, &key6TimeHold, &key6TimeReleased);
        int key7t = buttonTime(&key7State, &lkey7State, &key7StartPressed, &generalEndPressed, &key7TimeHold, &key7TimeReleased);
        int key8t = buttonTime(&key8State, &lkey8State, &key8StartPressed, &generalEndPressed, &key8TimeHold, &key8TimeReleased);
        /*
        int key9t = buttonTime(&key9State, &lkey9State, &key9StartPressed, &generalEndPressed, &key9TimeHold, &key9TimeReleased);
        int key10t = buttonTime(&key10State, &lkey10State, &key10StartPressed, &generalEndPressed, &key10TimeHold, &key10TimeReleased);
        int key11t = buttonTime(&key11State, &lkey11State, &key11StartPressed, &generalEndPressed, &key11TimeHold, &key11TimeReleased);
        int key12t = buttonTime(&key12State, &lkey12State, &key12StartPressed, &generalEndPressed, &key12TimeHold, &key12TimeReleased);
        //...
        //...
        //.future keys..
        //...
        */
        Serial.print("in recording\n");
        checkToAddToList(key1t, freqs[0]);
        checkToAddToList(key2t, freqs[1]);
        checkToAddToList(key3t, freqs[2]);
        checkToAddToList(key4t, freqs[3]);
        checkToAddToList(key5t, freqs[4]);
        checkToAddToList(key6t, freqs[5]);
        checkToAddToList(key7t, freqs[6]);
        checkToAddToList(key8t, freqs[7]);
        /*
        checkToAddToList(key9t, freqs[8]);
        checkToAddToList(key10t, freqs[9]);
        checkToAddToList(k bvxcney11t, freqs[10]);
        checkToAddToList(key12t, freqs[11]);
        //add future keys here...
        //...
        //...
        */
    }

    if (playing) {


        Serial.print("in playing\n");
        //iterate through linked list and play notes
        if (l.head == NULL) {
            Serial.print("No notes to play\n");
            playing = false;
            return;
        }

        //list bool
        //parity = word for even/odd
        //i've defined it so even = true, odd = false
        bool indexParity = true;
        NODE *cur = l.head;
        while (cur != NULL) {
            if (indexParity == false) {
                //if we're in the position where 
                //t is a "timeHold" number
                tone(speaker, cur->f);

                //**changed to delay here instead of duration..duration screws things up a lot***//
                delay(cur->t);

                //to be safe..
                noTone(speaker);
            }
            else {
                //we're in a position were t is a 
                //"timeReleased" number
                noTone(speaker);
                //since the noTone function has no duration,
                //we need to delay for the "timeReleased" time
                //"timeReleased" is just the amount of time the button was
                //not pushed
                delay(cur->t);
            }
            //move along in list
            cur = cur->next;
            //switch value of indexParity
            indexParity = !indexParity;
        }

        //after the playback finishes, we need to set "playing" to false
        playing = false;
    }

}
