#pragma once
/*#ifndef _TEST_APP
#define _TEST_APP*/
#include "ofMain.h"
#include "fft.h"

#define BUFFER_SIZE 256
#define NUM_WINDOWS 80



/*class ofApp : public ofSimpleApp{*/
class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased();
    
    void audioReceived 	(float * input, int bufferSize, int nChannels);
    
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
    
    float lmh_ave[3],lmh_length[3],lmh_min[3],lmh_max[3],lmh_bottom[3],lmh_top[3],lmh_val[3],lmh_newMin[3],lmh_newMax[3],pre_ave[3],pre_val[3];
    /*
    int low_bottom,low_top,mid_bottom,mid_top,high_bottom,high_top;
    float low_min,low_max,mid_min,mid_max,high_min,high_max;
    float low_newMin,low_newMax,mid_newMin,mid_newMax,high_newMin,high_newMax;
    int low_length,mid_length,high_length;*/
    float low_ave,mid_ave,high_ave;
    int mode;
    float rate,smoothRate;

    float low_color,mid_color,high_color;
    float pre_low_ave,pre_mid_ave,pre_high_ave;
    float pre_low,pre_mid,pre_high;
    //float low_val,mid_val,high_val;
    bool bCut[3];
};

//#endif

