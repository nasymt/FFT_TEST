
#ifndef _FFT
#define _FFT

#ifndef M_PI
#define	M_PI		3.14159265358979323846  /* pi */
#endif

#define BAND_NUM 4


class fft {
	
	public:
		
	fft();
	~fft();	
	
	/* Calculate the power spectrum */
	void powerSpectrum(int start, int half, float *data, int windowSize,float *magnitude,float *phase, float *power, float *avg_power);
	/* ... the inverse */
	void inversePowerSpectrum(int start, int half, int windowSize, float *finalOut,float *magnitude,float *phase);	
	
    /*-----add myCode-----*/
    float lmh_length[BAND_NUM],map_min[BAND_NUM],map_max[BAND_NUM],band_bottom[BAND_NUM],band_top[BAND_NUM],val[BAND_NUM],map_newMin[BAND_NUM],map_newMax[BAND_NUM],pre_ave[BAND_NUM],pre_val[BAND_NUM];
    float vol_max[BAND_NUM],temp_val;
    bool bCut[BAND_NUM],bSmooth,bSelectPreset,bReset,bAutoMaxGet;
    int preset_index;
    float smoothRate,rate;
    
    void update(float *magni,int i);
    void setup();
    void changeBandRange(int key);
    void changeParam(int key);
    
};


#endif	
