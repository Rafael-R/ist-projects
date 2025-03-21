// Bakeoff #2 - Seleção de Alvos e Fatores Humanos
// IPM 2019-20, Semestre 2
// Bake-off: durante a aula de lab da semana de 20 de Abril
// Submissão via Twitter: exclusivamente no dia 24 de Abril, até às 23h59

// Processing reference: https://processing.org/reference/

import java.util.Collections;

// Target properties
float PPI, PPCM;
float SCALE_FACTOR;
float TARGET_SIZE;
float TARGET_PADDING, MARGIN, LEFT_PADDING, TOP_PADDING;

// Study properties
ArrayList<Integer> trials  = new ArrayList<Integer>();    // contains the order of targets that activate in the test
ArrayList<Float> performance  = new ArrayList<Float>();
int trialNum               = 0;                           // the current trial number (indexes into trials array above)
final int NUM_REPEATS      = 3;                           // sets the number of times each target repeats in the test - FOR THE BAKEOFF NEEDS TO BE 3!
boolean ended              = false;

// Performance variables
int startTime              = 0;      // time starts when the first click is captured
int finishTime             = 0;      // records the time of the final click
int hits                   = 0;      // number of successful clicks
int misses                 = 0;      // number of missed clicks

// Class used to store properties of a target
class Target
{
  int x, y;
  float w;
  
  Target(int posx, int posy, float twidth) 
  {
    x = posx;
    y = posy;
    w = twidth;
  }
}

// Setup window and vars - runs once
void setup()
{
  //size(900, 900);    // window size in px (use for debugging)
  fullScreen();   // USE THIS DURING THE BAKEOFF!
  
  // The text from the file is loaded into an array. 
  SCALE_FACTOR    = 1.0 / displayDensity(); 
  String[] ppi_string = loadStrings("ppi.txt");
  PPI = float(ppi_string[1]);      // set PPI, we assume the ppi value is in the second line of the .txt
  PPCM           = PPI / 2.54 * SCALE_FACTOR;          // do not change this!
  TARGET_SIZE    = 1.5 * PPCM;     // set the target size in cm; do not change this!
  TARGET_PADDING = 1.5 * PPCM;     // set the padding around the targets in cm; do not change this!
  MARGIN         = 1.5 * PPCM;     // set the margin around the targets in cm; do not change this!
  LEFT_PADDING   = width/2 - TARGET_SIZE - 1.5*TARGET_PADDING - 1.5*MARGIN;        // set the margin of the grid of targets to the left of the canvas; do not change this!
  TOP_PADDING    = height/2 - TARGET_SIZE - 1.5*TARGET_PADDING - 1.5*MARGIN;       // set the margin of the grid of targets to the top of the canvas; do not change this!
  
  noStroke();        // draw shapes without outlines
  frameRate(60);     // set frame rate
  
  // Text and font setup
  textFont(createFont("Arial", 16));    // sets the font to Arial size 16
  textAlign(CENTER);                    // align text
  
  randomizeTrials();    // randomize the trial order for each participant
}

// Updates UI - this method is constantly being called and drawing targets
void draw()
{
	if(hasEnded()) return; // nothing else to do; study is over

	background(30);       // set background to fark grey

	// Print trial count
  	fill(255);          // set text fill color to white
  	text("Trial " + (trialNum + 1) + " of " + trials.size(), 50, 20);    // display what trial the participant is on (the top-left corner)

	cursor(CROSS);	// set cursor to a cross

	// Draw targets
	for (int i = 0; i < 16; i++) drawTarget(i);

	// Draw arrow to target
	drawArrow(trialNum);

	// Changes target color when hovered
	mouseHover(trialNum);
}

boolean hasEnded() {
	if(ended) return true;    // returns if test has ended before

  // Check if the study is over
  if (trialNum >= trials.size())
  {
    float timeTaken = (finishTime-startTime) / 1000f;     // convert to seconds - DO NOT CHANGE!
    float penalty = constrain(((95f-((float)hits*100f/(float)(hits+misses)))*.2f),0,100);    // calculate penalty - DO NOT CHANGE!
    
    printResults(timeTaken, penalty);    // prints study results on-screen
    ended = true;
  }
  
  return ended;
}

// Randomize the order in the targets to be selected
// DO NOT CHANGE THIS METHOD!
void randomizeTrials()
{
  for (int i = 0; i < 16; i++)             // 4 rows times 4 columns = 16 target
    for (int k = 0; k < NUM_REPEATS; k++)  // each target will repeat 'NUM_REPEATS' times
		trials.add(i);
  Collections.shuffle(trials);             // randomize the trial order

  System.out.println("trial order: " + trials);    // prints trial order - for debug purposes
}

// Print results at the end of the study
void printResults(float timeTaken, float penalty)
{
  background(30);       // clears screen

  	fill(255);    //set text fill color to white
  	text(day() + "/" + month() + "/" + year() + "  " + hour() + ":" + minute() + ":" + second() , 100, 20);   // display time on screen

	text("Finished!", width / 2, 40); 
	text("Hits: " + hits, width / 2, 40 + 20);
	text("Misses: " + misses, width / 2, 40 + 40);
	text("Accuracy: " + (float)hits*100f/(float)(hits+misses) +"%", width / 2, 40 + 60);
	text("Total time taken: " + timeTaken + " sec", width / 2, 40 + 80);
	text("Average time for each target: " + nf((timeTaken)/(float)(hits+misses),0,3) + " sec", width / 2, 40 + 100);
	text("Average time for each target + penalty: " + nf(((timeTaken)/(float)(hits+misses) + penalty),0,3) + " sec", width / 2, 40 + 130);
	text("Fitts Index of Performance", width / 2, 40 + 160);

  for (int i = 1; i <= trials.size(); ++i) {
		printPerformance(i);
	}

  saveFrame("results-######.png");    // saves screenshot in current folder
}

// Print index of performance for given target
void printPerformance(int index)
{
	int printWidth, printHeight;

	if (index < trials.size() / 2 + 1)
	{
		printWidth = width / 4;
		printHeight = 210 + index * 20;
	}
	else
	{
		printWidth = (width / 4) * 3;
		printHeight = 210 + (index % (trials.size() / 2 + 1) * 20) + 20;
	}

	if (index == 1) 
	{
		text("Target 1: ---", printWidth, printHeight);
	} 
	else
	{
		float indexOfPerformance = performance.get(index - 2);

		if (indexOfPerformance != 0)
		{
			text("Target " + index + ": " + nf(indexOfPerformance,0,3), printWidth, printHeight);
		}
		else
		{
			text("Target " + index + ": MISSED", printWidth, printHeight);
		}
	}
}

// Mouse button was released - lets test to see if hit was in the correct target
void mouseReleased() 
{
  if (trialNum >= trials.size()) return;      // if study is over, just return
  if (trialNum == 0) startTime = millis();    // check if first click, if so, start timer
  if (trialNum == trials.size() - 1)          // check if final click
  {
    finishTime = millis();    // save final timestamp
    println("We're done!");
  }
  
  Target target = getTargetBounds(trials.get(trialNum));    // get the location and size for the target in the current trial
  
  // Check to see if mouse cursor is inside the target bounds
  if(dist(target.x, target.y, mouseX, mouseY) < target.w/2)
  {
    System.out.println("HIT! " + trialNum + " " + (millis() - startTime));     // success - hit!
    hits++; // increases hits counter 
  }
  else
  {
    System.out.println("MISSED! " + trialNum + " " + (millis() - startTime));  // fail
    misses++;   // increases misses counter
    if (trialNum > 1) performance.set(trialNum - 1, 0.0);
  }

  if (trialNum < trials.size() - 1)
	{
		target = getTargetBounds(trials.get(trialNum + 1));
		float indexOfPerformance = log((dist(target.x, target.y, mouseX, mouseY) / target.w) + 1) / log(2);
		performance.add(indexOfPerformance);
	}

  trialNum++;   // move on to the next trial; UI will be updated on the next draw() cycle
}  

// For a given target ID, returns its location and size
Target getTargetBounds(int i)
{
  int x = (int)LEFT_PADDING + (int)((i % 4) * (TARGET_SIZE + TARGET_PADDING) + MARGIN);
  int y = (int)TOP_PADDING + (int)((i / 4) * (TARGET_SIZE + TARGET_PADDING) + MARGIN);
  
  return new Target(x, y, TARGET_SIZE);
}

// Draw target on-screen
// This method is called in every draw cycle; you can update the target's UI here //<>// //<>//
void drawTarget(int i)
{
	Target target = getTargetBounds(i);   // get the location and size for the circle with ID:i

	// check whether current circle is the intended target
	if (trials.get(trialNum) == i) 
	{ 
		// if so ...
		fill(200, 0, 0);           // fill red
	}
	else  
	{
		fill(65);           // fill dark gray
	}

	circle(target.x, target.y, target.w);   // draw target

	if (trialNum == 0  && trials.get(trialNum) == i)
	{
		// if so ...
		fill(255); 		// fill white
		text("HIT", target.x, target.y + 7);
    	noFill();
	}

	if (trialNum < trials.size() - 1 && trials.get(trialNum + 1) == i) 
	{ 
		// if so ...
		stroke(255);     // stroke white
		strokeWeight(0.1 * PPCM);
		circle(target.x, target.y, target.w);
		noStroke();    // next targets won't have stroke
	}
}

void drawArrow(int trialNum)
{
	if (trialNum < trials.size() - 1) 
	{
		Target origin = getTargetBounds(trials.get(trialNum));    // get the location and size for the target in the current trial
		Target target = getTargetBounds(trials.get(trialNum + 1));    // get the location and size for the target in the next trial

		if (target.x != origin.x || target.y != origin.y) 
		{
			fill(255);		// fill white
			arrow(origin.x, origin.y, target.x, target.y);
		}
	}
}

void arrow(int x1, int y1, int x2, int y2)
{
	float a = dist(x1, y1, x2, y2) / 50;
	float width = 0.5 * PPCM;
	float height = 0.6 * PPCM;
	float distance = 0.95 * PPCM;
	pushMatrix();
	translate(x1, y1);
	rotate(atan2(y2 - y1, x2 - x1));
	triangle(distance, -width/2, distance + height, 0, distance, width/2);
	popMatrix();
} 

void mouseHover(int trialNum)
{
	Target target = getTargetBounds(trials.get(trialNum));    // get the location and size for the target in the current trial

  	// Check to see if mouse cursor is inside the target bounds
	if(dist(target.x, target.y, mouseX, mouseY) < target.w/2)
	{
    fill(0, 200, 0); // fill green
		circle(target.x, target.y, target.w);
	}
}
