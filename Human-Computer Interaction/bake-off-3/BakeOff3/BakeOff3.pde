// Bakeoff #3 - Escrita de Texto em Smartwatches
// IPM 2019-20, Semestre 2
// Entrega: exclusivamente no dia 22 de Maio, até às 23h59, via Discord

// Processing reference: https://processing.org/reference/

import java.util.Arrays;
import java.util.Collections;
import java.util.Random;

// Screen resolution vars;
float PPI, PPCM;
float SCALE_FACTOR;

// Finger parameters
PImage fingerOcclusion;
int FINGER_SIZE;
int FINGER_OFFSET;

// Arm/watch parameters
PImage arm;
int ARM_LENGTH;
int ARM_HEIGHT;

// Arrow parameters
PImage leftArrow, rightArrow;
int ARROW_SIZE;

// Study properties
String[] phrases;                   // contains all the phrases that can be tested
int NUM_REPEATS            = 2;     // the total number of phrases to be tested
int currTrialNum           = 0;     // the current trial number (indexes into phrases array above)
String currentPhrase       = "";    // the current target phrase
String currentTyped        = "";    // what the user has typed so far
String[] typed = new String[NUM_REPEATS];

// Suggestions variables
String[] oneWords;
String[] twoWords;
String[] suggestions = new String[3];
float suggestionWidth;
float suggestionHeight     = 0.75;
int count 				   = 0;

// Keyboard properties
int keyRows                = 3;
int keyColumns             = 10;
float keyWidth             = 4.0 / keyColumns;
float keyHeight            = (3 - suggestionHeight) / keyRows;
char[] keys                = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
							  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '`',
							  'z', 'x', 'c', 'v', 'b', 'n', 'm', '_', '_', '_'};
boolean pressedKey         = false;

// Performance variables
float startTime            = 0;     // time starts when the user clicks for the first time
float finishTime           = 0;     // records the time of when the final trial ends
float lastTime             = 0;     // the timestamp of when the last trial was completed
float lettersEnteredTotal  = 0;     // a running total of the number of letters the user has entered (need this for final WPM computation)
float lettersExpectedTotal = 0;     // a running total of the number of letters expected (correct phrases)
float errorsTotal          = 0;     // a running total of the number of errors (when hitting next)

//Setup window and vars - runs once
void setup()
{
	//size(900, 900);
	fullScreen();
	textFont(createFont("Arial", 24));  // set the font to arial 24
	noCursor();                         // hides the cursor to emulate a watch environment

	// Load images
	arm = loadImage("arm_watch.png");
	fingerOcclusion = loadImage("finger.png");

	// Load phrases
	phrases = loadStrings("phrases.txt");                       // load the phrase set into memory
	Collections.shuffle(Arrays.asList(phrases), new Random());  // randomize the order of the phrases with no seed

	//Load suggestions
	oneWords = loadStrings("count_1w.txt");
	twoWords = loadStrings("count_2w.txt");

	// Scale targets and imagens to match screen resolution
	SCALE_FACTOR = 1.0 / displayDensity();          // scale factor for high-density displays
	String[] ppi_string = loadStrings("ppi.txt");   // the text from the file is loaded into an array.
	PPI = float(ppi_string[1]);                     // set PPI, we assume the ppi value is in the second line of the .txt
	PPCM = PPI / 2.54 * SCALE_FACTOR;               // do not change this!

	FINGER_SIZE = (int)(11 * PPCM);
	FINGER_OFFSET = (int)(0.8 * PPCM);
	ARM_LENGTH = (int)(19 * PPCM);
	ARM_HEIGHT = (int)(11.2 * PPCM);
	ARROW_SIZE = (int)(2.2 * PPCM);
}

// Get the key index
int getIndex()
{
	float clickX = (mouseX - width/2  + 2.0*PPCM) / PPCM;
	float clickY = (mouseY - height/2 + (1 - suggestionHeight)*PPCM) / PPCM;
	int x = floor(clickX / keyWidth);
	int y = floor(clickY / keyHeight);

	if (x > keyColumns - 1)
		x = keyColumns - 1;
	else if (x < 0)
		x = 0;

	if (y > keyRows - 1)
		y = keyRows - 1;
	else if (y < 0)
		y = 0;

	return x + keyColumns * y;
}

int getIndex(int x, int y)
{
	return x + keyColumns * y;
}


void draw()
{
	// Check if we have reached the end of the study
	if (finishTime != 0)  return;

	background(255);                                                         // clear background

	// Draw arm and watch background
	imageMode(CENTER);
	image(arm, width/2, height/2, ARM_LENGTH, ARM_HEIGHT);

	// Check if we just started the application
	if (startTime == 0 && !mousePressed)
	{
		fill(0);
		textAlign(CENTER);
		text("Tap to start time!", width/2, height/2);
	}
	else if (startTime == 0 && mousePressed) nextTrial();                    // show next sentence

	// Check if we are in the middle of a trial
	else if (startTime != 0)
	{
		textAlign(LEFT);
		fill(100);
		text("Phrase " + (currTrialNum + 1) + " of " + NUM_REPEATS, width/2 - 4.0*PPCM, height/2 - 8.1*PPCM);   // write the trial count
		text("Target:    " + currentPhrase, width/2 - 4.0*PPCM, height/2 - 7.1*PPCM);                           // draw the target string
		fill(0);
		text("Entered:  " + currentTyped + "|", width/2 - 4.0*PPCM, height/2 - 6.1*PPCM);                      // draw what the user has entered thus far

		// Draw very basic ACCEPT button - do not change this!
		textAlign(CENTER);
		noStroke();
		fill(0, 250, 0);
		rect(width/2 - 2*PPCM, height/2 - 5.1*PPCM, 4.0*PPCM, 2.0*PPCM);
		fill(0);
		text("ACCEPT >", width/2, height/2 - 4.1*PPCM);

		// Draw screen areas
		// simulates text box - not interactive
		fill(20);
		noStroke();
		rect(width/2 - 2.0*PPCM, height/2 - 2.0*PPCM, 4.0*PPCM, 1.0*PPCM);
		drawCurrentTyped();

		// THIS IS THE ONLY INTERACTIVE AREA (4cm x 4cm); do not change size
		fill(30);
		noStroke();
		rect(width/2 - 2.0*PPCM, height/2 - 1.0*PPCM, 4.0*PPCM, 3.0*PPCM);

		drawSuggestions();

		drawKeyboard();

		drawKeyIndicator();

		textSize(24);  // set the font to arial 24
	}

	// Draw the user finger to illustrate the issues with occlusion (the fat finger problem)
	imageMode(CORNER);
	image(fingerOcclusion, mouseX - FINGER_OFFSET, mouseY - FINGER_OFFSET, FINGER_SIZE, FINGER_SIZE);
}

void drawCurrentTyped()
{
	String cuttedText = currentTyped + "|";

	textSize(16);  // set the font to arial 16

	while (textWidth(cuttedText) > 3.8*PPCM) {
		cuttedText = cuttedText.substring(1);
	}

	fill(255);
	text(cuttedText, width/2, height/2 - 1.4 * PPCM);
}

void drawSuggestions()
{
	if (suggestions[0] != null) {

		pushMatrix();
		translate(width/2 - 2.0*PPCM, height/2 - 1.0*PPCM);

		fill(255);
		for (int i = 0; i < count; i++) {
			text(suggestions[i], (4.0/(count*2))*((i*2)+1)*PPCM, ((suggestionHeight/2) + 0.1)*PPCM);
		}

		popMatrix();
	}
}

void drawKeyboard()
{
	pushMatrix();
	translate(width/2 - 2.0*PPCM, height/2 - (1 - suggestionHeight)*PPCM);

	for (int x = 0; x < keyColumns; x++) {
		for (int y = 0; y < keyRows; y++) {
			noFill();
			stroke(50);
			rect(x*keyWidth*PPCM, y*keyHeight*PPCM, keyWidth*PPCM, keyHeight*PPCM);

			fill(255);
			textSize(12);  // set the font to arial 12
			text(keys[getIndex(x, y)], (x*keyWidth + (keyWidth / 2))*PPCM, (y*keyHeight + (keyHeight / 2) + 0.1)*PPCM);
		}
	}

	popMatrix();
}

void drawKeyIndicator()
{
	if (pressedKey)
	{
		int index = getIndex();
		float x, y;
		float indicatorWidth = keyWidth * 1.5;
		float indicatorHeight = keyHeight * 1.1;

		if (index % keyColumns == 0)
			x = (index % keyColumns) * keyWidth;
		else if (index % keyColumns == keyColumns - 1)
			x = (index % keyColumns) * keyWidth - (indicatorWidth - keyWidth);
		else
			x = (index % keyColumns) * keyWidth - ((indicatorWidth - keyWidth) / 2);

		y = (index / keyColumns) * keyHeight - indicatorHeight;

		pushMatrix();
		translate(width/2 - 2.0*PPCM, height/2 - (1 - suggestionHeight)*PPCM);

		fill(200);
		noStroke();
		rect(x*PPCM, y*PPCM, indicatorWidth*PPCM, indicatorHeight*PPCM, 5.0);

		fill(0);
		textSize(16);  // set the font to arial 16
		text(keys[index], (x + (indicatorWidth / 2))*PPCM, (y + (indicatorHeight / 2) + 0.1)*PPCM);

		popMatrix();
	}
}

// Check if mouse click was within certain bounds
boolean didMouseClick(float x, float y, float w, float h)
{
	return (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h);
}

void mousePressed()
{
	if (didMouseClick(width/2 - 2*PPCM, height/2 - 5.1*PPCM, 4.0*PPCM, 2.0*PPCM)) nextTrial();  // Test click on 'accept' button - do not change this!
	else if(didMouseClick(width/2 - 2.0*PPCM, height/2 - 1.0*PPCM, 4.0*PPCM, 3.0*PPCM)) // Test click on 'keyboard' area - do not change this condition!
	{
		// YOUR KEYBOARD IMPLEMENTATION NEEDS TO BE IN HERE! (inside the condition)
		if (startTime != 0)
		{
			if(didMouseClick(width/2 - 2.0*PPCM, height/2 - 1.0*PPCM, 4.0*PPCM, suggestionHeight*PPCM))
			{
				float clickX = mouseX - width/2  + 2.0*PPCM;
				int index = floor(clickX / suggestionWidth);

				if (currentTyped.length() == 0 || currentTyped.charAt(currentTyped.length() - 1) == ' ')
				{
					currentTyped += suggestions[index] + " ";
				}
				else
				{
					while (currentTyped.charAt(currentTyped.length() - 1) != ' ') {
						currentTyped = currentTyped.substring(0, currentTyped.length() - 1);
						if (currentTyped.length() == 0) break;
					}
					currentTyped += suggestions[index] + " ";
				}
			}
			else
			{
				pressedKey = true;
			}
		}

	}
	else System.out.println("debug: CLICK NOT ACCEPTED");
}

void mouseReleased()
{
	if(pressedKey)
	{
		char key = keys[getIndex()];

		if (key == '_') currentTyped += ' ';
		else if (key == '`' && currentTyped.length() > 0)    // if `, treat that as a delete command
			currentTyped = currentTyped.substring(0, currentTyped.length() - 1);
		else if (key != '_' && key != '`')
			currentTyped += key;  // if not any of the above cases, add the clicked letter to the typed string

		pressedKey = false;
	}
	getSuggestions();
}


void getSuggestions()
{
	String currentWord = null;
	String previousWord = null;
	String typedString = currentTyped + "-";
	String[] words = typedString.split(" ");

	count = 0;

	if (words[words.length - 1].equals("-") && words.length > 1)
	{
		previousWord = words[words.length - 2];
	}
	else
	{
		currentWord = words[words.length - 1];
		currentWord = currentWord.substring(0, currentWord.length() - 1);
	}

	if (currentWord != null)
	{
		for (String word : oneWords)
		{
			if (word.startsWith(currentWord))
				suggestions[count++] = word;
			if (count >= 3)
				break;
		}
	}
	else if (previousWord != null)
	{
		for (String word : twoWords)
		{
			words = word.split(" ");
			if (words[0].equals(previousWord))
				suggestions[count++] = words[1];
			if (count >= 3)
				break;
		}
	}
	while(count < 3) {
		suggestions[count] = oneWords[count++];
	}

	for (int i = 0; i < count; i++) {
		if (textWidth(suggestions[i]) > (4.0/count)*PPCM) {
			count--;
		}
	}

	suggestionWidth = (4.0/count)*PPCM;
}


void nextTrial()
{
	if (currTrialNum >= NUM_REPEATS) return;                                            // check to see if experiment is done

	// Check if we're in the middle of the tests
	else if (startTime != 0 && finishTime == 0)
	{
		System.out.println("==================");
		System.out.println("Phrase " + (currTrialNum+1) + " of " + NUM_REPEATS);
		System.out.println("Target phrase: " + currentPhrase);
		System.out.println("Phrase length: " + currentPhrase.length());
		System.out.println("User typed: " + currentTyped);
		System.out.println("User typed length: " + currentTyped.length());
		System.out.println("Number of errors: " + computeLevenshteinDistance(currentTyped.trim(), currentPhrase.trim()));
		System.out.println("Time taken on this trial: " + (millis() - lastTime));
		System.out.println("Time taken since beginning: " + (millis() - startTime));
		System.out.println("==================");
		lettersExpectedTotal += currentPhrase.trim().length();
		lettersEnteredTotal += currentTyped.trim().length();
		errorsTotal += computeLevenshteinDistance(currentTyped.trim(), currentPhrase.trim());
		typed[currTrialNum] = currentTyped;
	}

	// Check to see if experiment just finished
	if (currTrialNum == NUM_REPEATS - 1)
	{
		finishTime = millis();
		System.out.println("==================");
		System.out.println("Trials complete!"); //output
		System.out.println("Total time taken: " + (finishTime - startTime));
		System.out.println("Total letters entered: " + lettersEnteredTotal);
		System.out.println("Total letters expected: " + lettersExpectedTotal);
		System.out.println("Total errors entered: " + errorsTotal);

		float wpm = (lettersEnteredTotal / 5.0f) / ((finishTime - startTime) / 60000f);   // FYI - 60K is number of milliseconds in minute
		float freebieErrors = lettersExpectedTotal * .05;                                 // no penalty if errors are under 5% of chars
		float penalty = max(0, (errorsTotal - freebieErrors) / ((finishTime - startTime) / 60000f));
		float cps = lettersEnteredTotal / ((finishTime - startTime) / 1000f);             // characters per second

		System.out.println("Raw WPM: " + wpm);
		System.out.println("Freebie errors: " + freebieErrors);
		System.out.println("Penalty: " + penalty);
		System.out.println("WPM w/ penalty: " + (wpm - penalty));                         // yes, minus, because higher WPM is better
		System.out.println("CPS: " + cps);                                                // CPS
		System.out.println("==================");

		printResults(wpm, freebieErrors, penalty, cps);

		currTrialNum++;                                                                   // increment by one so this mesage only appears once when all trials are done
		return;
	}

	else if (startTime == 0)                                                            // first trial starting now
	{
		System.out.println("Trials beginning! Starting timer...");
		startTime = millis();                                                             // start the timer!
	}
	else currTrialNum++;                                                                // increment trial number

	lastTime = millis();                                                                // record the time of when this trial ended
	currentTyped = "";                                                                  // clear what is currently typed preparing for next trial
	currentPhrase = phrases[currTrialNum];                                              // load the next phrase!
}

// Print results at the end of the study
void printResults(float wpm, float freebieErrors, float penalty, float cps)
{
	background(0);       // clears screen

	textFont(createFont("Arial", 16));    // sets the font to Arial size 16
	fill(255);    //set text fill color to white
	text(day() + "/" + month() + "/" + year() + "  " + hour() + ":" + minute() + ":" + second(), 100, 20);   // display time on screen

	text("Finished!", width / 2, height / 2);

	int h = 20;
	for(int i = 0; i < NUM_REPEATS; i++, h += 40 ) {
		text("Target phrase " + (i+1) + ": " + phrases[i], width / 2, height / 2 + h);
		text("User typed " + (i+1) + ": " + typed[i], width / 2, height / 2 + h+20);
	}

	text("Raw WPM: " + wpm, width / 2, height / 2 + h+20);
	text("Freebie errors: " + freebieErrors, width / 2, height / 2 + h+40);
	text("Penalty: " + penalty, width / 2, height / 2 + h+60);
	text("WPM with penalty: " + max((wpm - penalty), 0), width / 2, height / 2 + h+80);
	text("CPS: " + cps, width / 2, height / 2 + 100);

	saveFrame("ist1XXXXX_GYY_###.png");    // saves screenshot in current folder
}

// This computes the error between two strings (i.e., original phrase and user input)
int computeLevenshteinDistance(String phrase1, String phrase2)
{
	int[][] distance = new int[phrase1.length() + 1][phrase2.length() + 1];

	for (int i = 0; i <= phrase1.length(); i++) distance[i][0] = i;
	for (int j = 1; j <= phrase2.length(); j++) distance[0][j] = j;

	for (int i = 1; i <= phrase1.length(); i++)
		for (int j = 1; j <= phrase2.length(); j++)
			distance[i][j] = min(min(distance[i - 1][j] + 1, distance[i][j - 1] + 1), distance[i - 1][j - 1] + ((phrase1.charAt(i - 1) == phrase2.charAt(j - 1)) ? 0 : 1));

	return distance[phrase1.length()][phrase2.length()];
}
