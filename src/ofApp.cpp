#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    srand((unsigned int)time((time_t *)NULL));

    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // BUFFER_SIZE samples per buffer
    // 4 num buffers (latency)
    
    ofSoundStreamSetup(0,2,this, 44100,BUFFER_SIZE, 4);
    left = new float[BUFFER_SIZE];
    right = new float[BUFFER_SIZE];
    
    for (int i = 0; i < NUM_WINDOWS; i++){
        for (int j = 0; j < BUFFER_SIZE/2; j++){
            freq[i][j] = 0;
        }
    }
    font.load("Avenir.ttc", 14);
    myfft.setup();
    mode=0;
}


//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(150,150,150);
}

//--------------------------------------------------------------
void ofApp::draw(){
    static int index=0;
    float avg_power = 0.0f;
    if(index < 80)
        index += 1;
    else
        index = 0;
    
    myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
    
    for(int j=1; j < BUFFER_SIZE/2; j++) {
        freq[index][j] = magnitude[j];
    }
    
    /* draw the FFT */
    for (int i = 1; i < (int)(BUFFER_SIZE/2); i++){
        if(myfft.band_bottom[0]<=i&&myfft.band_top[0]>i)ofSetColor(255, 0, 0);
        else if(myfft.band_bottom[1]<=i&&myfft.band_top[1]>i)ofSetColor(0, 255, 0);
        else if(myfft.band_bottom[2]<=i&&myfft.band_top[2]>i)ofSetColor(0, 0, 255);
        else ofSetColor(50,50,50);
        ofDrawLine(100+(i*8),400,100+(i*8),400-magnitude[i]*10.0f);
    }
    ofSetColor(255);
    for(int i=0;i<3;i++){
        myfft.update(magnitude, i);
        ofDrawCircle(200+i*300, 100, myfft.val[i]*50);
        string string_index[] = {"low:","mid:","high:"};
        ofDrawBitmapString(ofToString(string_index[i]), 50, 480+i*30);
        ofDrawBitmapString(ofToString(myfft.map_min[i]), 100, 480+i*30);
        ofDrawBitmapString(ofToString(myfft.map_max[i]), 150, 480+i*30);
        ofDrawBitmapString(ofToString(myfft.map_newMin[i]), 200, 480+i*30);
        ofDrawBitmapString(ofToString(myfft.map_newMax[i]), 250, 480+i*30);
        ofDrawBitmapString(ofToString(myfft.temp_val),300,480+i*30);
    }
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(mode),100,450);
    ofDrawBitmapString("bSmooth "+ofToString(myfft.bSmooth)+":"+ofToString(myfft.smoothRate), 100, 570);
    if(myfft.bSelectPreset)ofDrawBitmapString("===SELECT PRESET(Press key 1-2, 0 is reset)=== ", 100, 600);
    
    
    if(myfft.bSmooth){
        ofSetColor(200, 0, 103);
        ofDrawRectangle(100,650,100,50);
        ofSetColor(0, 0, 0);
        font.drawString("smooth", 115, 678);
    }
    else{
        ofSetColor(0, 0, 0);
        ofDrawRectangle(100,650,100,50);
        ofSetColor(200, 0, 103);
        font.drawString("no smooth", 105, 678);
    }
    
    if(!myfft.bReset){
        ofSetColor(0, 0, 0);
        ofDrawRectangle(230, 650, 100, 50);
        ofSetColor(200, 0, 103);
        font.drawString("Reset", 245, 678);
    }else {
        ofSetColor(200, 0, 103);
        ofDrawRectangle(230,650,100,50);
        ofSetColor(0, 0, 0);
        font.drawString("Reset", 245, 678);
        myfft.bReset=false;
        for(int i=0;i<3;i++)myfft.vol_max[i]=0;
    }
    
    if(myfft.bAutoMaxGet){
        ofSetColor(200, 0, 103);
        ofDrawRectangle(360, 650, 100, 50);
        ofSetColor(0, 0, 0);
        font.drawString("AutoGetMax", 370, 678);
    }else{
        ofSetColor(0, 0, 0);
        ofDrawRectangle(360, 650, 100, 50);
        ofSetColor(200,0,103);
        font.drawString("ManualGet", 370, 678);
    }
    for(int i=0;i<3;i++){
        rect_color[i] = ofMap(myfft.val[i], 0, 2, 0, 255);
        ofSetColor(rect_color[i], rect_color[i], rect_color[i]);
        ofDrawRectangle(400+i*200,450,150,150);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    /*---------------------------------
     モード選択:
     ◆リターン：モード変更
     0：低中高音の帯域指定
     1：数値のマッピング用
     ◆シフト：平滑化オンオフ
     　キーの上下で平滑化係数の調整
     ◆右コマンド：プリセットの選択
     
     ---------------------------------*/
    if(key==OF_KEY_RETURN){
        if(mode==0)mode=1;
        else if(mode==1)mode=0;
        cout<<"mode:"<<mode<<endl;
    }
    else if(key==OF_KEY_SHIFT){
        if(myfft.bSmooth)myfft.bSmooth=false;
        else myfft.bSmooth=true;
    }else if(key==OF_KEY_RIGHT_COMMAND){
/*        if(bSelectPreset)bSelectPreset=false;
        else bSelectPreset=true;*/
    }
    if(myfft.bSmooth){
        if(key==OF_KEY_UP)myfft.smoothRate+=0.05;
        else if(key==OF_KEY_DOWN)myfft.smoothRate-=0.05;
    }
/*    if(bSelectPreset){
        if(key=='1')preset_index=1;
        else if(key=='2')preset_index=2;
        else if(key=='0')preset_index=0;
        if(key==OF_KEY_RETURN){
            setPreset(preset_index);
            bSelectPreset=false;
        }
    }*/
    if(mode==0)myfft.changeBandRange(key);
    else if(mode==1)myfft.changeParam(key);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x,int y,int button){
    if((x>=230&&x<330)&&(y>=650&&y<700)){
        if(!myfft.bReset)myfft.bReset=true;
    }
    if((x>=360&&x<460)&&(y>=650&&y<700)){
        if(!myfft.bAutoMaxGet)myfft.bAutoMaxGet=true;
        else myfft.bAutoMaxGet=false;
    }
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x,int y,int button){
    
}
//--------------------------------------------------------------
void ofApp::audioReceived 	(float * input, int bufferSize, int nChannels){
    // samples are "interleaved"
    for (int i = 0; i < bufferSize; i++){
        left[i] = input[i*2];
        right[i] = input[i*2+1];
    }
    bufferCounter++;
}

//--------------------------------------------------------------
void ofApp::setPreset (int index){
/*    if(index == 1){
        map_min[0]=2.95,map_min[1]=0.4,map_min[2]=0;
        map_max[0]=8.05,map_max[1]=0.85,map_max[2]=0.6;
        map_newMin[0]=0,map_newMin[1]=0,map_newMin[2]=0.05;
        map_newMax[0]=0.85,map_newMax[1]=1.3,map_newMax[2]=1.2;
        smoothRate=0.75;
    }
    else if(index == 0){
        map_min[0]=0,map_min[1]=0,map_min[2]=0;
        map_max[0]=5,map_max[1]=1,map_max[2]=0.5;
        map_newMin[0]=0,map_newMin[1]=0,map_newMin[2]=0;
        map_newMax[0]=1,map_newMax[1]=1,map_newMax[2]=2;
        smoothRate = 0.7;
    }*/
}

