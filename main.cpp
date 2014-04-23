#include <Arduino.h>

#define DEBOUNCE_CYCLES 5
#define COL_BASE 2
#define COLUMNS 4
#define ROWS 4
int buttons[ROWS][COLUMNS];

void noteOn(int cmd, int pitch, int velocity) {
	Serial.write(cmd);
	Serial.write(pitch);
	Serial.write(velocity);
}

void pullUpInput(int pin) {
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH); // Enable pull-up
}

int readButton(int row, int col) {
	digitalWrite(col + COL_BASE, LOW);
	int val = digitalRead(row+COL_BASE+COLUMNS) == LOW ? 1 : 0;
	digitalWrite(col + COL_BASE, HIGH);
	return val;
}

void doNoteButton(int row, int col, int note) {
	if(readButton(row,col)) {
		if(buttons[row][col] < DEBOUNCE_CYCLES) {
			buttons[row][col]++;
		}
	} else {
		buttons[row][col] = 0;
	}

	if(buttons[row][col] == DEBOUNCE_CYCLES) {
		//Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
		noteOn(0x90, note, 0x45);
		buttons[row][col]++;
	} else if(!buttons[row][col]) {
		//Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
		noteOn(0x90, note, 0x00);
	}
}

int main(void) {
	init(); // Init Arduino library

	for(int i = 0; i < ROWS; i++) {
		for(int j = 0; j < COLUMNS; j++) {
			buttons[i][j] = 0;
		}
	}

	Serial.begin(31250); // For MIDI
	pinMode(13, OUTPUT); // Debug LED

	for(int i = 0; i < COLUMNS; i++) {
		pinMode(i + COL_BASE, OUTPUT);
		digitalWrite(i + COL_BASE, HIGH);
	}

	for(int i = 0; i < ROWS; i++) {
		pullUpInput(i + COL_BASE + COLUMNS);
	}

	while(1) {
		int note = 0x24;
		for(int i = 0; i < ROWS; i++) {
			for(int j = 0; j < COLUMNS; j++) {
				doNoteButton(i, j, note++);
			}
		}
	}
	return 0;
}
