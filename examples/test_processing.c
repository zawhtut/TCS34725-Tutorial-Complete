/* Offline instructional checks: NOT a calibrated production RGB algorithm. */
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct { uint16_t clear, red, green, blue; } raw_t;
typedef struct { uint8_t r,g,b; } color_t;
typedef struct { const char *name; color_t rgb; } ref_t;
static uint8_t clamp8(float x)
{
    if (x <= 0) return 0;
    if (x >= 255) return 255;
    return (uint8_t)(x+0.5f);
}
static int normalize(raw_t in, float rs, float gs, float bs, color_t *out)
{
    if (!out || in.clear == 0) return -1;
    out->r=clamp8((float)in.red/in.clear * rs);
    out->g=clamp8((float)in.green/in.clear * gs);
    out->b=clamp8((float)in.blue/in.clear * bs);
    return 0;
}
static const ref_t refs[] = {
    {"Red",{255,0,0}}, {"Green",{0,255,0}}, {"Blue",{0,0,255}},
    {"White",{255,255,255}}, {"Black",{0,0,0}}
};
static const ref_t *closest(color_t color, float *score)
{
    const ref_t *best=NULL;
    int best_distance=3*255*255+1;
    for (size_t i=0; i<sizeof refs/sizeof refs[0]; ++i) {
        int dr=(int)color.r-(int)refs[i].rgb.r;
        int dg=(int)color.g-(int)refs[i].rgb.g;
        int db=(int)color.b-(int)refs[i].rgb.b;
        int d=dr*dr+dg*dg+db*db;
        if (d < best_distance) { best_distance=d; best=&refs[i]; }
    }
    *score=100.0f * (1.0f-(float)best_distance/(3.0f*255.0f*255.0f));
    return best;
}
static void must(int condition,const char *message)
{
    if (!condition) { fprintf(stderr,"FAIL: %s\n",message); exit(1); }
}
int main(void)
{
    color_t rgb;
    float score;
    must(normalize((raw_t){0,1,2,3},255,255,255,&rgb)<0,"reject Clear=0");
    must(normalize((raw_t){100,100,0,0},255,255,255,&rgb)==0,"normalization works");
    must(rgb.r==255 && rgb.g==0 && rgb.b==0,"pure red output");
    const ref_t *match=closest(rgb,&score);
    must(match && score==100.0f,"exact match similarity");
    must(match->name[0]=='R',"reference match Red");
    must(normalize((raw_t){10,999,888,777},255,255,255,&rgb)==0,"clamp output");
    must(rgb.r==255 && rgb.g==255 && rgb.b==255,"channel clamping to 255");
    match=closest((color_t){10,30,80},&score);
    must(match && score>=0 && score<=100,"confidence range");
    puts("PASS: offline processing demonstration tests");
    puts("Note: sample scaling is illustrative, not calibrated sRGB.");
    return 0;
}
