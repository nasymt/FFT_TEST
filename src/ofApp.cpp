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
    
    for (int i = 0; i < NUM_WINDOWS; i++)
    {
        for (int j = 0; j < BUFFER_SIZE/2; j++)
        {
            freq[i][j] = 0;
        }
    }
    
    ofSetColor(0x666666);
    lmh_bottom[0]=1;
    lmh_top[0]=2;
    lmh_min[0]=0;
    lmh_max[0]=5;
    lmh_newMin[0]=0;
    lmh_newMax[0]=1;
    lmh_bottom[1]=3;
    lmh_top[1]=10;
    lmh_min[1]=0;
    lmh_max[1]=1;
    lmh_newMin[1]=0;
    lmh_newMax[1]=1;
    lmh_bottom[2]=50;
    lmh_top[2]=60;
    lmh_min[2]=0;
    lmh_max[2]=0.5;
    lmh_newMin[2]=0;
    lmh_newMax[2]=2;
    lmh_length[0] = lmh_top[0] - lmh_bottom[0];
    lmh_length[1] = lmh_top[1] - lmh_bottom[1];
    lmh_length[2] = lmh_top[2] - lmh_bottom[2];
    mode=0;
    rate=0.05;
    smoothRate = 0.7;
    for(int i=0;i<3;i++){
        pre_val[i]=1;
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(80,80,20);
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
        if(lmh_bottom[0]<=i&&lmh_top[0]>i)ofSetColor(255, 0, 0);
        else if(lmh_bottom[1]<=i&&lmh_top[1]>i)ofSetColor(0, 255, 0);
        else if(lmh_bottom[2]<=i&&lmh_top[2]>i)ofSetColor(0, 0, 255);
        else ofSetColor(50,50,50);
        ofLine(100+(i*8),400,100+(i*8),400-magnitude[i]*10.0f);
    }
//    float lmh_val[0],lmh_val[1],lmh_val[2];
    /*for(int i=lmh_bottom[0];i<lmh_top[0];i++){
        if(magnitude[i]!=INFINITY)lmh_val[0]+=magnitude[i];
        cout<<"magnitude:"<<magnitude[i]<<":"<<i<<endl;
    }
    for(int i=lmh_bottom[1];i<lmh_top[1];i++){
        lmh_val[1]+=magnitude[i];
    }
    for(int i=lmh_bottom[2];i<lmh_top[2];i++){
        lmh_val[2]+=magnitude[i];
    }*/
    for(int i=0;i<3;i++){
        for(int j=lmh_bottom[i];j<lmh_top[i];j++){
            if(magnitude[j]!=INFINITY)lmh_val[i]+=magnitude[j];
        }
    }
    
    
    //--------小数点第3位で切り捨て
    lmh_val[0]*=100;
    lmh_val[0]=floor(lmh_val[0]);
    lmh_val[0]/=100;
    
    cout<<"low_val:"<<lmh_val[0]<<endl;
    
    for(int i=0;i<3;i++){
        lmh_ave[i] = lmh_val[i]/lmh_length[i];
        
        if(lmh_ave[i]<lmh_min[i])bCut[i]=true;
        
        float mapped = ofMap(lmh_ave[i],lmh_min[i],lmh_max[i],lmh_newMin[i],lmh_newMax[i]);
        pre_ave[i] = mapped;
        
        lmh_val[i] = smoothRate*pre_ave[i] + (1-smoothRate)*pre_val[i];
    }
    
    
/*    low_ave = lmh_val[0] / lmh_length[0];
    mid_ave = lmh_val[1] / lmh_length[1];
    high_ave = lmh_val[2] / lmh_length[2];
    if(low_ave<lmh_min[0])bLowcut=true;
    if(mid_ave<lmh_min[1])bMidcut=true;
    if(high_ave<lmh_min[2])bHighcut=true;*/
    
/*    float low_mapped = ofMap(low_ave, lmh_min[0], lmh_max[0], lmh_newMin[0] , lmh_newMax[0]);
    float mid_mapped = ofMap(mid_ave, lmh_min[1], lmh_max[1], lmh_newMin[1] , lmh_newMax[1]);
    float high_mapped = ofMap(high_ave, lmh_min[2], lmh_max[2], lmh_newMin[2] , lmh_newMax[2]);*/
    
    //--------平滑化---------------
/*    pre_low_ave = low_mapped;
    pre_mid_ave = mid_mapped;
    pre_high_ave = high_mapped;*/
//    cout<<"pre_low:"<<pre_low<<" low_ave:"<<low_ave<<" :"<<low_mapped<<" : "<<lmh_val[0]<<endl;
/*    lmh_val[0] = smoothRate*pre_low_ave + (1-smoothRate)*pre_low;
    lmh_val[1] = smoothRate*pre_mid_ave + (1-smoothRate)*pre_mid;
    lmh_val[2] = smoothRate*pre_high_ave + (1-smoothRate)*pre_high;*/
    cout<<"lmh_val[0]"<<lmh_val[0]<<":"<<smoothRate*pre_ave[0]<<":"<<(1-smoothRate)*pre_val[0]<<endl;
    ofDrawBitmapString(ofToString(smoothRate*pre_ave[0])+":"+ofToString((1-smoothRate)*pre_val[0]), 100, 400);
    pre_val[0] = lmh_val[0];
    pre_val[0] = lmh_val[1];
    pre_val[0] = lmh_val[2];
    cout<<"pre_low2:"<<pre_val[0]<<endl;
    cout<<"-------------------"<<endl;
    
   /* if(bLowcut){
        bLowcut=false;
        lmh_val[0]=0;
    }
    if(bMidcut){
        bMidcut=false;
        lmh_val[1]=0;
    }
    if(bHighcut){
        bHighcut=false;
        lmh_val[2]=0;
    }*/
    
    for(int i=0;i<3;i++){
        if(bCut[i]){
            bCut[i]=false;
            lmh_val[i]=0;
        }
    }
    
    
/*    ofCircle(200, 100, low_ave*50);
    ofCircle(500, 100, mid_ave*50);
    ofCircle(800, 100, high_ave*50);*/
    ofCircle(200, 100, lmh_val[0]*50);
    ofCircle(500, 100, lmh_val[1]*50);
    ofCircle(800, 100, lmh_val[2]*50);
    
    ofSetColor(255);
    string low_string = "low"+ofToString(lmh_min[0])+":"+ofToString(lmh_max[0])+":"+ofToString(lmh_newMin[0])+":"+ofToString(lmh_newMax[0])+"  val:"+ofToString(low_ave);
    string mid_string = "mid"+ofToString(lmh_min[1])+":"+ofToString(lmh_max[1])+":"+ofToString(lmh_newMin[1])+":"+ofToString(lmh_newMax[1])+"  val:"+ofToString(mid_ave);
    string high_string = "high"+ofToString(lmh_min[2])+":"+ofToString(lmh_max[2])+":"+ofToString(lmh_newMin[2])+":"+ofToString(lmh_newMax[2])+"  val:"+ofToString(high_ave);
    
    ofDrawBitmapString(ofToString(mode),100,450);
    ofDrawBitmapString(low_string,100,500);
    ofDrawBitmapString(mid_string,100,550);
    ofDrawBitmapString(high_string,100,600);
    
    low_color = ofMap(lmh_val[0],0,2,0,255);
    mid_color = ofMap(lmh_val[1],0,2,0,255);
    high_color = ofMap(lmh_val[2], 0, 2, 0, 255);
    ofSetHexColor(low_color);
    ofRect(300,500,150,150);
    ofSetHexColor(mid_color);
    ofRect(500,500,150,150);
    ofSetHexColor(high_color);
    ofRect(700,500,150,150);
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    /*---------------------------------
     モード選択:0-低中高音の帯域指定 1-数値のマッピング用
     
     ---------------------------------*/
    if(key==OF_KEY_RETURN){
        if(mode==0)mode=1;
        else if(mode==1)mode=0;
        cout<<"mode:"<<mode<<endl;
    }
    if(mode==0){
        if(key == 'p')cout<<low_ave<<":"<<mid_ave<<":"<<high_ave<<endl;
        else {
            if(key == 'q')lmh_bottom[0]++;
            else if(key=='a')lmh_bottom[0]--;
            else if(key=='w')lmh_top[0]++;
            else if(key=='s')lmh_top[0]--;
            else if(key=='e')lmh_bottom[1]++;
            else if(key=='d')lmh_bottom[1]--;
            else if(key=='r')lmh_top[1]++;
            else if(key=='f')lmh_top[1]--;
            else if(key=='t')lmh_bottom[2]++;
            else if(key=='g')lmh_bottom[2]--;
            else if(key=='y')lmh_top[2]++;
            else if(key=='h')lmh_top[2]--;
            lmh_length[0] = lmh_top[0] - lmh_bottom[0];
            lmh_length[1] = lmh_top[1] - lmh_bottom[1];
            lmh_length[2] = lmh_top[2] - lmh_bottom[2];
            
            cout<<"low -> "<<lmh_bottom[0]<<" : "<<lmh_top[0]<<endl;
            cout<<"bottom -> "<<lmh_bottom[1]<<" : "<<lmh_top[1]<<endl;
            cout<<"high -> "<<lmh_bottom[2]<<" : "<<lmh_top[2]<<endl;
        }
    }else if(mode==1){
        if(key=='1')lmh_newMin[0]+=rate;
        else if(key=='q')lmh_newMin[0]-=rate;
        else if(key=='2')lmh_newMax[0]+=rate;
        else if(key=='w')lmh_newMax[0]-=rate;
        else if(key=='3')lmh_newMin[1]+=rate;
        else if(key=='e')lmh_newMin[1]-=rate;
        else if(key=='4')lmh_newMax[1]+=rate;
        else if(key=='r')lmh_newMax[1]-=rate;
        else if(key=='5')lmh_newMin[2]+=rate;
        else if(key=='t')lmh_newMin[2]-=rate;
        else if(key=='6')lmh_newMax[2]+=rate;
        else if(key=='y')lmh_newMax[2]-=rate;
        
        else if(key=='a')lmh_min[0]+=rate;
        else if(key=='z')lmh_min[0]-=rate;
        else if(key=='s')lmh_max[0]+=rate;
        else if(key=='x')lmh_max[0]-=rate;
        else if(key=='d')lmh_min[1]+=rate;
        else if(key=='c')lmh_min[1]-=rate;
        else if(key=='f')lmh_max[1]+=rate;
        else if(key=='v')lmh_max[1]-=rate;
        else if(key=='g')lmh_min[2]+=rate;
        else if(key=='b')lmh_min[2]-=rate;
        else if(key=='h')lmh_max[2]+=rate;
        else if(key=='n')lmh_max[2]-=rate;
//        cout<<"low"<<lmh_min[0]<<":"<<lmh_max[0]<<":"<<lmh_newMin[0]<<":"<<lmh_newMax[0]<<endl;
//        cout<<"mid"<<lmh_min[0]<<":"<<lmh_max[0]<<":"<<lmh_newMin[0]<<":"<<lmh_newMax[0]<<endl;
//        cout<<"hig"<<lmh_min[0]<<":"<<lmh_max[0]<<":"<<lmh_newMin[0]<<":"<<lmh_newMax[0]<<endl;
//        cout<<"--------------------------------------"<<endl;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(){
    
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

