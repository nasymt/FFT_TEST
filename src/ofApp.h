#pragma once

#include "ofMain.h"
#include "fft.h"
#include "math.h"

#define BUFFER_SIZE 256
#define NUM_WINDOWS 80



/*class ofApp : public ofSimpleApp{*/
class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mousePressed(int x,int y,int button);
    void mouseReleased(int x,int y,int button);
    void audioReceived 	(float * input, int bufferSize, int nChannels);
    void setPreset(int index);
    ofTrueTypeFont font;
    
private:
    float * left;
    float * right;
    int 	bufferCounter;
    fft		myfft;
    
    float magnitude[BUFFER_SIZE];
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    
    float freq[NUM_WINDOWS][BUFFER_SIZE/2];
    float freq_phase[NUM_WINDOWS][BUFFER_SIZE/2];
    
    float lmh_length[3],map_min[3],map_max[3],band_bottom[3],band_top[3],val[3],map_newMin[3],map_newMax[3],pre_ave[3],pre_val[3];
    float vol_max[3];
    
    
    int rect_color[3];
    float low_ave,mid_ave,high_ave;
    int mode;
    float rate,smoothRate;
//    string sBand_data[3];

    bool bCut[3],bSmooth,bSelectPreset,bReset,bAutoMaxGet;
    int preset_index;
};

//#endif

