#define  exit_error(IFB,IFC) { fprintf(stderr,IFB,IFC); exit(0); }
#define  FTOI(a) ( (a) < 0 ? ((int)(a-0.5)) : ((int)(a+0.5)) )
typedef unsigned char uchar;

void susan(uchar *input,int x_size, int y_size);
void get_image(char filename[200],unsigned char **in,int *x_size,int *y_size);
