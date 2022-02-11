#include <Adafruit_NeoPixel.h>

// define physical pins on controller for 
// serial data connection on each strip of lights
#define PIN1 2
#define PIN2 14
#define PIN3 7
#define PIN4 8
#define PIN5 6
#define PIN6 20
#define PIN7 21
#define PIN8 5

#define LENGTH 150

#define BRIGHT_RAINBOW 25   // 0-255
#define BRIGHT_PULSE 0.1   // 0-1
#define BRIGHT_SNAKE 75     // 0-255
#define BRIGHT_PROGRESS 0.25  // 0-1
#define BRIGHT_WAVE 0.25    // 0-1

// initialize each physical strip of lights - 150 LEDs per 5 meter strip
Adafruit_NeoPixel leds1 = Adafruit_NeoPixel(LENGTH, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel(LENGTH, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds3 = Adafruit_NeoPixel(LENGTH, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds4 = Adafruit_NeoPixel(LENGTH, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds5 = Adafruit_NeoPixel(LENGTH, PIN5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds6 = Adafruit_NeoPixel(LENGTH, PIN6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds7 = Adafruit_NeoPixel(LENGTH, PIN7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds8 = Adafruit_NeoPixel(LENGTH, PIN8, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel leds[] = {leds1, leds2, leds3, leds4, leds5, leds6, leds7, leds8};

void setup() {
  for (int i=0; i<8; i++) {
    leds[i].begin();
  }
  clearAll();
}

void loop() {
  rotateRainbow();  // about 128sec   start 00:00 end (01:34) 8 -> 4
  clearAll();
  
  pulse();          // about 67sec    end 03:28 (1:54) 50 -> 20, removed steps
  clearAll();
  
  bouncingSnake();  // about 75sec    end 06:40 (3:12)  4 -> 1
  clearAll();

  progress();       // about 80sec    end 08:45 (2:05) 4 -> 1
  clearAll();
}

void clearAll() {
  for (int i=0; i<8; i++) {
    for (int j=0; j<LENGTH; j++) {
      leds[i].setPixelColor(j, 0);
    }
    leds[i].show();
  }
}

void progress() {
  // progress one row at a time gently
  int cgrn = 0;
  float bright = BRIGHT_PROGRESS;
  int delay_msec = 10;
  int steps = 2;

  for (int reps=0; reps<1; reps++) {
    for (int i=128+64; i<384+64; i+=steps) {
      cgrn = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
      writeSpanAll(0, cgrn, 0, 0, 75, 150, 1.0);
      delay(delay_msec);
    }
    clearAll();
    
    for (int i=128+64; i<384+64; i+=steps) {
      cgrn = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
      writeSpanAll(0, cgrn, 0, 0, 0, 75, 1.0);
      delay(delay_msec);
    }
    clearAll();

    for (int strip=1; strip<8; strip++) {
      for (int i=128+64; i<384+64; i+=steps) {
        cgrn = floor(leds[strip].sine8(i) * bright);   // factor of 4 quarters the brightness here
        writeSpanAll(0, cgrn, 0, strip, 0, 75, 1.0);
        delay(delay_msec);
      }
      clearAll();
      
      for (int i=128+64; i<384+64; i+=steps) {
        cgrn = floor(leds[strip].sine8(i) * bright);   // factor of 4 quarters the brightness here
        writeSpanAll(0, cgrn, 0, strip, 75, 150, 1.0);
        delay(delay_msec);
      }
      clearAll();
    }
  }
}

void progressRainbow() {
  int colors[3] = {0, 0, 0};
  float bright = BRIGHT_PROGRESS;

  for (int i=0; i<20; i++) {
    // test basic rainbow
    colorUpdate(colors, i, 20);
    writeSpanAll(colors[0], colors[1], colors[2], 0, 0, 75, bright);
    delay(500);
  }

}

void wave() {
  float bright = BRIGHT_WAVE;
  int steps = 2;
  int level[16] = {0};
  int level1 = 0;

    for (int i=0; i<512; i+=steps) {
      for (int strip=0; strip<8; strip++) {
        level1 = floor(leds[0].sine8(i + 64*strip) * bright);
        writeSpanAll(level1, 0, 0, strip, 0, 150, 1.0);
      }
      delay(20);
    }
  }

}

void colorUpdate(int *colors, int curr_step, int max_step) {
  colors[0] = 0;  // r
  colors[1] = 0;  // g
  colors[2] = 0;  // b

  int progress = floor(curr_step * 255 * 6 / max_step);
  int sw = floor(progress / 255);
  
  if (sw == 0) {
    colors[2] = 255;
    colors[0] = progress % 255;
  } else if (sw == 1) {
    colors[0] = 255;
    colors[2] = progress % 255;
  } else if (sw == 2) {
    colors[0] = 255;
    colors[1] = progress % 255;
  } else if (sw == 3) {
    colors[1] = 255;
    colors[0] = progress % 255;
  } else if (sw == 4) {
    colors[1] = 255;
    colors[2] = progress % 255;
  } else if (sw == 5) {
    colors[2] = 255;
    colors[1] = progress % 255;
  }
}

void progressRainbow1() {
  // progress one row at a time gently
  float bright = BRIGHT_PROGRESS;
  int delay_msec = 10;
  int steps = 2;
  int curr_step = 0;
  int max_step = 256;
  int level = 0;

  for (int reps=0; reps<4; reps++) {
    for (int i=128+64; i<384+64; i+=steps) {
      level = floor(leds[0].sine8(i) * bright);
      writeSpanAll(rainbow_r(curr_step, max_step), rainbow_g(curr_step, max_step), rainbow_b(curr_step, max_step), 0, 75, 150, level);
      curr_step = incStep(curr_step, max_step);
      delay(delay_msec);
    }
    clearAll();
    
    for (int i=128+64; i<384+64; i+=steps) {
      level = floor(leds[0].sine8(i) * bright);
      writeSpanAll(rainbow_r(curr_step, max_step), rainbow_g(curr_step, max_step), rainbow_b(curr_step, max_step), 0, 0, 75, level);
      curr_step = incStep(curr_step, max_step);
      delay(delay_msec);
    }
    clearAll();

    for (int strip=1; strip<8; strip++) {
      for (int i=128+64; i<384+64; i+=steps) {
        level = floor(leds[0].sine8(i) * bright);
        writeSpanAll(rainbow_r(curr_step, max_step), rainbow_g(curr_step, max_step), rainbow_b(curr_step, max_step), strip, 0, 75, level);
        curr_step = incStep(curr_step, max_step);
        delay(delay_msec);
      }
      clearAll();
      
      for (int i=128+64; i<384+64; i+=steps) {
        level = floor(leds[0].sine8(i) * bright);
        writeSpanAll(rainbow_r(curr_step, max_step), rainbow_g(curr_step, max_step), rainbow_b(curr_step, max_step), strip, 75, 150, level);
        curr_step = incStep(curr_step, max_step);
        delay(delay_msec);
      }
      clearAll();
    }
  }
}

int incStep(int step, int max_step) {
  step++;
  if (step >= max_step) {
    return 0;
  }
  return step;
}

int rainbow_r(int i, int steps) {
  int r = 0;
  int stepsize = floor(255 * 6 / steps);
  int progress = i * stepsize;

  if (floor(progress/255) == 0) {
    r = progress % 255;
  } else if (floor(progress/255) == 3) {
    r = progress % 255;
  } else if (floor(progress/255) == 1) {
    r = 255;
  } else if (floor(progress/255) == 2) {
    r = 255;
  }

  return r;
}

int rainbow_g(int i, int steps) {
  int g = 0;
  int stepsize = floor(255 * 6 / steps);
  int progress = i * stepsize;

  if (floor(progress/255) == 2) {
    g = progress % 255;
  } else if (floor(progress/255) == 5) {
    g = progress % 255;
  } else if (floor(progress/255) == 3) {
    g = 255;
  } else if (floor(progress/255) == 4) {
    g = 255;
  }

  return g;
}

int rainbow_b(int i, int steps) {
  int b = 0;
  int stepsize = floor(255 * 6 / steps);
  int progress = i * stepsize;

  if (floor(progress/255) == 1) {
    b = progress % 255;
  } else if (floor(progress/255) == 4) {
    b = progress % 255;
  } else if (floor(progress/255) == 0) {
    b = 255;
  } else if (floor(progress/255) == 5) {
    b = 255;
  }

  return b;
}

void pulse() {
  // want to pulse about 10/min (once every 6s)
  int cred = 0;
  int cgrn = 0;
  float bright = BRIGHT_PULSE;     // 0 to 1
  int delay_msec = 20;    // msec delay per step
  int steps = 2;          // number to increment for loop step

  for (int reps=0; reps<1; reps++) {    // 2 reps all red, 13sec
    for (int i=128+64; i<384+64; i+=steps) {
      // 1.28sec for steps of 4 at 20msec delay
      // 6.4sec for steps of 2 at 50msec delay
      cred = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
      writeMatrixAll(cred, 0, 0);
      delay(delay_msec);
    }    
  }
  
  // orange: 100%, 65%, 0%
  for (int reps=0; reps<1; reps++) {    // 1 rep orange, 6.5sec
    for (int i=128+64; i<384+64; i+=steps) {
      cred = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
      cgrn = floor(cred * .65);
      writeMatrixAll(cred, cgrn, 0);
      delay(delay_msec);
    }    
  }

  // yellow: 100%, 100%, 0%
  for (int reps=0; reps<1; reps++) {    // 1 rep yellow, 6.5sec
    for (int i=128+64; i<384+64; i+=steps) {
      cred = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
      cgrn = cred;
      writeMatrixAll(cred, cgrn, 0);
      delay(delay_msec);
    }    
  }

  for (int reps=0; reps<1; reps++) {    // 1 rep something, 6.5sec
    for (int i=128+64; i<384+64; i+=steps) {
      cgrn = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
      cred = floor(cred * .65);
      writeMatrixAll(cred, cgrn, 0);
      delay(delay_msec);
    }    
  }

  for (int reps=0; reps<2; reps++) {    // 4 rep green, 26sec
    for (int i=128+64; i<384+64; i+=steps) {
      cgrn = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
      writeMatrixAll(0, cgrn, 0);
      delay(delay_msec);
    }    
  }
 
  for (int i=384+64; i<384+64+128; i+=steps) {    // half rep green, 3sec
    cgrn = floor(leds[0].sine8(i) * bright);   // factor of 4 quarters the brightness here
    writeMatrixAll(0, cgrn, 0);
    delay(delay_msec);
  }    

  delay(5000);    // 5 sec hold

}

void writeMatrixAll(int cred, int cgrn, int cblu) {
  for (int i=0; i<8; i++) {
    for (int j=0; j<LENGTH; j+=5) {
      leds[i].setPixelColor(j, cred, cgrn, cblu);
    }
    leds[i].show();
  }
}

// writes all LEDs in a span of pixels on one strip with an RGB color
void writeSpanAll(int cred, int cgrn, int cblu, int strip, int px_start, int px_stop, float bright) {
  for (int i=px_start; i<px_stop; i++) {
    leds[strip].setPixelColor(i, floor(bright * cred), floor(bright * cgrn), floor(bright * cblu));
  }
  leds[strip].show();
}

void bouncingSnake() {
  int cgrn = BRIGHT_SNAKE;
  int delay_msec = 20;    // delay 20 should be 25sec
  
  for (int reps=0; reps<1; reps++) {
    for (int i=159; i>=0; i--) {
      leds[0].setPixelColor(i % 150, 0, 0, 0);
      leds[0].setPixelColor((i-10) % 150, 0, cgrn, 0);
      leds[0].show();
      delay(delay_msec);
    }

    for (int strip=1; strip<8; strip++) {
      for (int i=0; i<160; i++) {
        leds[strip].setPixelColor(i, 0, cgrn, 0);
        leds[strip].setPixelColor((i-10) % 150, 0, 0, 0);
        leds[strip].show();
        delay(delay_msec);
      }
    }

    for (int strip=7; strip>=1; strip--) {
      for (int i=159; i>=0; i--) {
        leds[strip].setPixelColor(i % 150, 0, 0, 0);
        leds[strip].setPixelColor((i-10) % 150, 0, cgrn, 0);
        leds[strip].show();
        delay(delay_msec);
      }
    }
    
    for (int i=0; i<160; i++) {
      leds[0].setPixelColor(i, 0, cgrn, 0);
      leds[0].setPixelColor((i-10) % 150, 0, 0, 0);
      leds[0].show();
      delay(delay_msec);
    }
  }
}

void rotateRainbow() {
  int bright = BRIGHT_RAINBOW;
  
  // should do about 128sec of continuous rotation
  for (int reps=0; reps<4; reps++) {
    for (int i=0; i<65536; i+=512) {
      leds[0].rainbow(i, -2, 255, bright, true);
      leds[0].show();
      for (int j=1; j<8; j++) {
        leds[j].rainbow((i + (j*4096)) % 65536, 2, 255, bright, true);
        leds[j].show();
      }
      delay(50);
    }    
  }
}
