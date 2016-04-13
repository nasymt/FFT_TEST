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
    band_bottom[0]=1,band_bottom[1]=3,band_bottom[2]=50;
    band_top[0]=2,band_top[1]=10,band_top[2]=60;
    map_min[0]=0,map_min[1]=0,map_min[2]=0;
    map_max[0]=5,map_max[1]=1,map_max[2]=0.5;
    map_newMin[0]=0,map_newMin[1]=0,map_newMin[2]=0;
    map_newMax[0]=1,map_newMax[1]=1,map_newMax[2]=2;
    mode=0;
    rate=0.05;
    smoothRate = 0.7;
    for(int i=0;i<3;i++){
        lmh_length[i] = band_top[i] - band_bottom[i];
        pre_val[i]=1;
    }
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
    
    /* do the FFT	*/
    myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
    
    /* start from 1 because mag[0] = DC component */
    /* and discard the upper half of the buffer */
    for(int j=1; j < BUFFER_SIZE/2; j++) {
        freq[index][j] = magnitude[j];
    }
    
    /* draw the FFT */
    for (int i = 1; i < (int)(BUFFER_SIZE/2); i++){
        if(band_bottom[0]<=i&&band_top[0]>i)ofSetColor(255, 0, 0);
        else if(band_bottom[1]<=i&&band_top[1]>i)ofSetColor(0, 255, 0);
        else if(band_bottom[2]<=i&&band_top[2]>i)ofSetColor(0, 0, 255);
        else ofSetColor(50,50,50);
        ofDrawLine(100+(i*8),400,100+(i*8),400-magnitude[i]*10.0f);
    }
    for(int i=0;i<3;i++){
        for(int j=band_bottom[i];j<band_top[i];j++){
            if(magnitude[j]!=INFINITY)val[i]+=magnitude[j];
        }
    }
    ofSetColor(255);
    for(int i=0;i<3;i++){
        float temp_val = val[i]/lmh_length[i];
        if(temp_val<map_min[i])bCut[i]=true;
        if(temp_val>vol_max[i]){
            vol_max[i]=temp_val;
            if(bAutoMaxGet)map_max[i]=vol_max[i];
        }
        float mapped = ofMap(temp_val,map_min[i],map_max[i],map_newMin[i],map_newMax[i]);
        if(bSmooth){//平滑化あり
            pre_ave[i] = mapped;
            val[i] = smoothRate*pre_ave[i] + (1-smoothRate)*pre_val[i];
            pre_val[i]=val[i];
        }else{//平滑化なし
            val[i] = mapped;
        }
        
        if(bCut[i]){//一定値以下切り捨て
            bCut[i]=false;
            val[i]=0;
        }
        ofDrawCircle(200+i*300, 100, val[i]*50);
        string string_index[] = {"low:","mid:","high:"};
        /*-------四捨五入----------*/
        float tmp = map_min[i]*100;
        tmp=round(tmp);
        map_min[i]=tmp/100;
        tmp = map_max[i]*100;
        tmp = round(tmp);
        map_max[i]=tmp/100;
        
        
/*        sBand_data[i] =ofToString(string_index[i])+ofToString(map_min[i])+":"+ofToString(map_max[i])+":"+ofToString(map_newMin[i])+":"+ofToString(map_newMax[i])+"  val:"+ofToString(temp_val);
        ofDrawBitmapString(sBand_data[i], 100, 480+i*30);*/
        ofDrawBitmapString(ofToString(string_index[i]), 50, 480+i*30);
        ofDrawBitmapString(ofToString(map_min[i]), 100, 480+i*30);
        ofDrawBitmapString(ofToString(map_max[i]), 150, 480+i*30);
        ofDrawBitmapString(ofToString(map_newMin[i]), 200, 480+i*30);
        ofDrawBitmapString(ofToString(map_newMax[i]), 250, 480+i*30);
        ofDrawBitmapString(ofToString(temp_val),300,480+i*30);
    }
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(mode),100,450);
    ofDrawBitmapString("bSmooth "+ofToString(bSmooth)+":"+ofToString(smoothRate), 100, 570);
    if(bSelectPreset)ofDrawBitmapString("===SELECT PRESET(Press key 1-2, 0 is reset)=== ", 100, 600);
    
    
    if(bSmooth){
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
    
    if(!bReset){
        ofSetColor(0, 0, 0);
        ofDrawRectangle(230, 650, 100, 50);
        ofSetColor(200, 0, 103);
        font.drawString("Reset", 245, 678);
    }else {
        ofSetColor(200, 0, 103);
        ofDrawRectangle(230,650,100,50);
        ofSetColor(0, 0, 0);
        font.drawString("Reset", 245, 678);
        bReset=false;
        for(int i=0;i<3;i++)vol_max[i]=0;
    }
    
    if(bAutoMaxGet){
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
        rect_color[i] = ofMap(val[i], 0, 2, 0, 255);
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
        if(bSmooth)bSmooth=false;
        else bSmooth=true;
    }else if(key==OF_KEY_RIGHT_COMMAND){
        if(bSelectPreset)bSelectPreset=false;
        else bSelectPreset=true;
    }
    if(bSmooth){
        if(key==OF_KEY_UP)smoothRate+=0.05;
        else if(key==OF_KEY_DOWN)smoothRate-=0.05;
    }
    if(bSelectPreset){
        if(key=='1')preset_index=1;
        else if(key=='2')preset_index=2;
        else if(key=='0')preset_index=0;
        if(key==OF_KEY_RETURN){
            setPreset(preset_index);
            bSelectPreset=false;
        }
    }
    if(mode==0){
        if(key == 'p')cout<<low_ave<<":"<<mid_ave<<":"<<high_ave<<endl;
        else {
            if(key == 'q')band_bottom[0]++;
            else if(key=='a')band_bottom[0]--;
            else if(key=='w')band_top[0]++;
            else if(key=='s')band_top[0]--;
            else if(key=='e')band_bottom[1]++;
            else if(key=='d')band_bottom[1]--;
            else if(key=='r')band_top[1]++;
            else if(key=='f')band_top[1]--;
            else if(key=='t')band_bottom[2]++;
            else if(key=='g')band_bottom[2]--;
            else if(key=='y')band_top[2]++;
            else if(key=='h')band_top[2]--;
            for(int i=0;i<3;i++){
                lmh_length[i] = band_top[i] - band_bottom[i];
            }
        }
    }else if(mode==1){
        if(key=='1')map_newMin[0]+=rate;
        else if(key=='q')map_newMin[0]-=rate;
        else if(key=='2')map_newMax[0]+=rate;
        else if(key=='w')map_newMax[0]-=rate;
        else if(key=='3')map_newMin[1]+=rate;
        else if(key=='e')map_newMin[1]-=rate;
        else if(key=='4')map_newMax[1]+=rate;
        else if(key=='r')map_newMax[1]-=rate;
        else if(key=='5')map_newMin[2]+=rate;
        else if(key=='t')map_newMin[2]-=rate;
        else if(key=='6')map_newMax[2]+=rate;
        else if(key=='y')map_newMax[2]-=rate;
        
        else if(key=='a')map_min[0]+=rate;
        else if(key=='z')map_min[0]-=rate;
        else if(key=='s')map_max[0]+=rate;
        else if(key=='x')map_max[0]-=rate;
        else if(key=='d')map_min[1]+=rate;
        else if(key=='c')map_min[1]-=rate;
        else if(key=='f')map_max[1]+=rate;
        else if(key=='v')map_max[1]-=rate;
        else if(key=='g')map_min[2]+=rate;
        else if(key=='b')map_min[2]-=rate;
        else if(key=='h')map_max[2]+=rate;
        else if(key=='n')map_max[2]-=rate;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x,int y,int button){
    if((x>=230&&x<330)&&(y>=650&&y<700)){
        if(!bReset)bReset=true;
    }
    if((x>=360&&x<460)&&(y>=650&&y<700)){
        if(!bAutoMaxGet)bAutoMaxGet=true;
        else bAutoMaxGet=false;
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
    if(index == 1){
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
    }
}

