import ddf.minim.analysis.*;
import ddf.minim.*;
import processing.serial.*; 
 
Serial port1; 
 
Minim minim;
AudioInput in;
FFT fft;

int buffer_size = 4096; 
float sample_rate = 200000;

float scale = 1;

float redValue = 0;
float greenValue = 0;
float blueValue = 0;

float slopeMode = 0;

int freq_width = 250; // <-O-> set the frequency range for each band over 400hz. larger bands will have less intensity per band. smaller bands would result in the overall range being limited

//arrays to hold 2 bands
int[] freq_array = {0,0,0,0};
float[] freq_height = {0,0,0,0};

float[] freq_height_prev = {0,0,0,0};


void setup()
{
  size(200 , 200); 

  minim = new Minim(this);
 
  in = minim.getLineIn(Minim.MONO,buffer_size,sample_rate);
  //port1 = new Serial(this, "/dev/cu.usbmodem1462201" , 115200); // <-O-> set baud rate and port for first RGB matrix
 
  // create an FFT object that has a time-domain buffer 
  // the same size as line-in's sample buffer
  fft = new FFT(in.bufferSize(), in.sampleRate());
  // Tapered window important for log-domain display
  fft.window(FFT.HAMMING);
}


void draw()
{
for(int k=0; k<3; k++){
freq_array[k] = 0;
}

  // perform a forward FFT on the samples in input buffer
  fft.forward(in.mix);
  
  
  freq_height[0] = fft.calcAvg((float) 50, (float) 100);
  freq_height[1] = fft.calcAvg((float) 110, (float) 200);
  freq_height[2] = fft.calcAvg((float) 200, (float) 600);
  freq_height[3] = (fft.calcAvg((float) 4000, (float) (5000)))*3;
  
  // <-O-> Log scaling function. Feel free to adjust x and y
  
  float x = 8;
  float y = 3;
  for(int j=0; j<3; j++){    
    freq_height[j] = freq_height[j]*(log(x)/y);
    x = x + (x); 
  }
  
  if (slopeMode == 1){
    for(int m=0; m<2; m++){
      if (freq_height[m]<freq_height_prev[m]){
        freq_height[m]=freq_height_prev[m]-.25;
      }
      if (freq_height[m]<0){
        freq_height[m]=0; 
      }
    }
  }
  else if (slopeMode == 2){
    for(int m=0; m<2; m++){
      if (freq_height[m]<freq_height_prev[m]){
        freq_height[m]=freq_height_prev[m]-.09;
      }
      if (freq_height[m]<0){
        freq_height[m]=0; 
      }
    }
  }

// Amplitude Ranges  if else tree
    for(int j=0; j<3; j++){
      if (freq_height[j] > 180*scale){
      freq_height[j] = 180*scale;
      }
    }
      
    if ((freq_height[0] > freq_height_prev[0]) && (freq_height[0] > 20)){
      redValue = random(100, 255);
      greenValue = random(255);
      blueValue = random(255);
      slopeMode = 1;
      print("Mode 0");
    }
    else if ((freq_height[1] > freq_height_prev[1]) && (freq_height[1] > 20)){
      redValue = random(255);
      greenValue = random(255);
      blueValue = random(255);
      slopeMode = 1;
      print("Mode 1");
    }
    else if ((freq_height[2] > freq_height_prev[2]) && (freq_height[2] > 20) && (slopeMode == 0)){
      redValue = random(100, 255);
      greenValue = random(255);
      blueValue = random(255);
      slopeMode = 2;
      print("Mode 2");
    }
    else if ((freq_height[3] > freq_height_prev[3]) && (freq_height[3] > 20)){
      redValue = 255;
      greenValue = 255;
      blueValue = 255;
      slopeMode = 2;
      print("Mode 3");
    }

    redValue = redValue - 5;
    greenValue = greenValue - 5;
    blueValue = blueValue - 5;
      
    if (redValue < 0) {
        redValue = 0;
    }if (greenValue < 0) {
        greenValue = 0;
    }if (blueValue < 0) {
        blueValue = 0;
    }
    //System.out.print(freq_height[0]);
    //System.out.println("    " + freq_height[1]);
    String send = ("M" + str(redValue) + "," + str(greenValue) +  "," + str(blueValue) + "\n");
    //port1.write(send);
    
    for(int m=0; m<3; m++){
      freq_height_prev[m] = freq_height[m];
    }
  background(redValue, greenValue, blueValue);
  print(freq_height[3]);
}
 
 
void stop()
{
  // always close Minim audio classes when you finish with them
  in.close();
  minim.stop();
 
  super.stop();
}
