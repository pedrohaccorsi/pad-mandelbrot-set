/*
compile: gcc -o mandelbrot-set mandelbrot-set.c  
run: ./maldelbrot-set
*/

#include <stdio.h>
#include <math.h> 
#include <pthread.h>

#define ITERATION_MAX 200

struct Range {
    double min;
    double max;
};

struct Pixel {
    double height;
    double width;
};

struct QuadrantMetadata {
    int x_max;
    int y_max;
    struct Range cx;
    struct Range cy;
    struct Pixel pixel;
    double escapeRadiusSquared;
    FILE * filePointer;
};

void *computeQuadrant(void *quadrantMetadata){

    static unsigned char color[3];
    double cx, cy;
    double Zx, Zy;
    double Zx2, Zy2;
    int iteration;
 
	struct QuadrantMetadata *metadata = (struct QuadrantMetadata *)quadrantMetadata;
 
    for (int y=0; y < metadata->y_max; y++) {

        cy = metadata->cy.min + y * metadata->pixel.height;

        if (fabs(cy) < metadata->pixel.height / 2)
            cy = 0.0;  

        for (int x=0; x < metadata->x_max; x++) {

            cx = metadata->cx.min + x * metadata->pixel.width;
            Zx = 0.0;
            Zy = 0.0;
            Zx2 = Zx * Zx;
            Zy2 = Zy * Zy;

            for (iteration=0; iteration < ITERATION_MAX && ((Zx2 + Zy2) < metadata->escapeRadiusSquared); iteration++) {
                Zy  = 2 * Zx * Zy + cy;
                Zx  = Zx2 - Zy2 + cx;
                Zx2 = Zx * Zx;
                Zy2 = Zy * Zy;
            };

            if (iteration == ITERATION_MAX) {
                /*  interior of Mandelbrot set = black */
                color[0] = 0;
                color[1] = 0;
                color[2] = 0;
            } else {
                /* exterior of Mandelbrot set = white */
                color[0] = 255; 
                color[1] = 255; 
                color[2] = 255; 
            };

            fwrite(color, 1, 3, metadata->filePointer);

        }

    }

}

FILE* getFilePointer(){

    const int MaxColorComponentValue = 255;
    char * filename = "result.ppm";
    char * comment  = "# "; 

    FILE * fp = fopen(filename, "wb");   
    fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, 800, 800, MaxColorComponentValue);
    
    return fp;

}

int main() {
   
    struct QuadrantMetadata quadrantMetada; 
	pthread_t worker1;

    quadrantMetada.escapeRadiusSquared = 4.0;
    quadrantMetada.filePointer         = getFilePointer();
    quadrantMetada.x_max               = 800;
    quadrantMetada.y_max               = 800;
    quadrantMetada.cx.max              = 1.5;
    quadrantMetada.cx.min              = -2.5;
    quadrantMetada.cy.max              = 2.0;
    quadrantMetada.cy.min              = -2.0;
    quadrantMetada.pixel.width         = ( quadrantMetada.cx.max - quadrantMetada.cx.min ) / quadrantMetada.x_max;
    quadrantMetada.pixel.height        = ( quadrantMetada.cy.max - quadrantMetada.cy.min ) / quadrantMetada.y_max;

 
    if (pthread_create(&worker1, NULL, computeQuadrant, &quadrantMetada)){
		printf("Error creating thread\n");
		return -1;
	}

    if (pthread_join(worker1, NULL)){
		printf("Error joining thread\n");
		return -2;
	}

    fclose(quadrantMetada.filePointer);
    return 0;

}