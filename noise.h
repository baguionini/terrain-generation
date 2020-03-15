#include <cmath>
#include <time.h>
#include <cstdlib>

inline double findnoise2(double x,double y)
{
 int n=(int)x+(int)y*57;
 n=(n<<13)^n;
 int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
 return 1.0-((double)nn/1073741824.0);
}

inline double interpolate(double a,double b,double x)
{
 double ft=x * 3.1415927;
 double f=(1.0-cos(ft))* 0.5;
 return a*(1.0-f)+b*f;
}

double noise(double x,double y)
{
 double floorx=(double)((int)x);
 double floory=(double)((int)y);
 double s,t,u,v;
 s=findnoise2(floorx,floory); 
 t=findnoise2(floorx+1,floory);
 u=findnoise2(floorx,floory+1);
 v=findnoise2(floorx+1,floory+1);
 double int1=interpolate(s,t,x-floorx);
 double int2=interpolate(u,v,x-floorx);
 return interpolate(int1,int2,y-floory);
}

double noise2d(int x, int y, int octaves, double zoom, double p, double scale){
    double getnoise =0;
    for(int a=0; a<octaves-1; a++){
    double frequency = pow(2,a);
    double amplitude = pow(p,a);
    getnoise += noise(((double)x/scale)*frequency/zoom,((double)y/scale)/zoom*frequency)*amplitude;
    }
    return ((getnoise*128.0)+128.0);
}

void perlinNoise1D(int size, float *seed, int octave, float *out, float bias){
    for(int x = 0; x < size;x++){
        float noise = 0.0f, scale = 1.0f;
        for(int o = 0; o < octave; o++){
            int pitch = size >> o;
            int sample1 = (x/pitch)*pitch;
            int sample2 = (sample1+pitch)%size;
            float blend = (float)(x-sample1)/(float)pitch;
            float sample = (1.0f-blend)* seed[sample1] + blend * seed[sample2];
            noise += sample * scale;
            scale = scale / bias;
        }
        out[x] = noise;
    }
}





