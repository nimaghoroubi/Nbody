#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

#define PRINT 0



///graphics
const float W = 1;
const float H = 1;
const int   redColor    = 0;
const int   greenColor  = 1;
const int   yellowColor = 2;

/// high numbers crash, fix the xorg, fix optimization

typedef struct cluster_attribute{
    double pos_x;
    double pos_y;
    double mass;
    double vel_x;
    double vel_y;
    double brightness;
}cluster;



int main(int argc, char** argv) {
    ///intialize graphics


    ///prel check
    if(argc != 6){
        printf("wrong arguments\n");
        exit(1);
    }

    ///demangling
    int N           = atoi(argv[1]);
    char* filename  =      argv[2];
    int SN          = atoi(argv[3]);
    double dt       = atof(argv[4]);
    int graphics    = atoi(argv[5]);

    if (graphics){
        InitializeGraphics(argv[0],700,700);
        SetCAxes(0,1);
        ClearScreen();
        Refresh();
        ClearScreen();
        Refresh();
    }


    printf("\n*****************************"
           "\n*****************************"
           "\nN is %23d\n"
           "filename %19s\n"
           "step number %16d\n"
           "delta t %20.6f\n"
           "graphics %19s\n"
           "*****************************\n"
           "*****************************\n\n\n", N, filename, SN, dt, graphics?"on":"off");


    FILE *fp = fopen(filename,"r");
    if (fp == NULL){
        printf("error in file opening\n");
        exit(1);
    }

    ///all clusters
    cluster* cluster_list[N];

    ///filling it in
    int counter = 1;
    for(int i = 0; i<N;i++){
        cluster_list[i] = (cluster*) malloc(sizeof(cluster));
        for (int j = 0; j < 6; ++j) {
            double d = 0.0;
            fread(&d,1,sizeof(double),fp);
            if (counter == 1){
                (*cluster_list[i]).pos_x = d;
            }else if(counter == 2){
                (*cluster_list[i]).pos_y = d;
            }else if(counter == 3){
                (*cluster_list[i]).mass = d;
            }else if(counter == 4){
                (*cluster_list[i]).vel_x = d;
            }else if(counter == 5){
                (*cluster_list[i]).vel_y = d;
            }else if(counter == 6){
                (*cluster_list[i]).brightness = d;
            }
            counter++;
        }
        counter = 1;
    }

    ///cluster check set to 0 if there are big number of clusters

    if (PRINT){
        for (int k = 0; k < N; ++k) {
            printf("\n*****************************"
                   "\nfor cluster %d\n"
                   "px %f\n"
                   "py %f\n"
                   "mass %f\n"
                   "vx %f\n"
                   "vy %f\n"
                   "brightness %f"
                   "\n*****************************\n\n\n",k+1, (*cluster_list[k]).pos_x,(*cluster_list[k]).pos_y,(*cluster_list[k]).mass,
                   (*cluster_list[k]).vel_x,(*cluster_list[k]).vel_y,(*cluster_list[k]).brightness);
        }
    }

    printf("starting calculations\n");





    ///calculation of force
    double G  = 100.0/N;
    double eps = 0.001;
    //int distance_num = N;
    //double dist[10][10];
    int memory_size = (N*(N-1))/2;


    double* dist  = calloc(1, memory_size * sizeof(double));
    double* fx    = calloc(1, memory_size * sizeof(double));
    double* fy    = calloc(1, memory_size * sizeof(double));
    double* fxs   = calloc(1, N * sizeof(double));
    double* fys   = calloc(1, N * sizeof(double));


    for (int iteration_number = 0; iteration_number < SN; ++iteration_number) {
        /*if (graphics) {
            for (int i = 0; i < N; ++i) {
                DrawCircle(cluster_list[i]->pos_x, cluster_list[i]->pos_y, W, H, 0.002, 0, redColor);
            }
        }
        if (graphics){
            Refresh();
            usleep(40000);
            ClearScreen();
        }*/

        ///calculation of distance
        int id = 0;


        id = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if(i<j){
                    double dx = (*cluster_list[i]).pos_x - (*cluster_list[j]).pos_x;
                    double dy = (*cluster_list[i]).pos_y - (*cluster_list[j]).pos_y;
                    dist[id] = eps + sqrt((dx*dx) + (dy*dy));

                    fx[id] = -((G*((*cluster_list[i]).mass)*((*cluster_list[j]).mass)/(dist[id]*dist[id]*dist[id]))*(((*cluster_list[i]).pos_x)-((*cluster_list[j]).pos_x)));
                    fy[id] = -((G*((*cluster_list[i]).mass)*((*cluster_list[j]).mass)/(dist[id]*dist[id]*dist[id]))*(((*cluster_list[i]).pos_y)-((*cluster_list[j]).pos_y)));
                    id++;
                }
            }

        }

        id = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i<j){
                    fxs[i] = fxs[i] + fx[id];
                    fys[i] = fys[i] + fy[id];
                    id++;
                }
            }
        }

        id = 0;
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < N; ++i) {
                if (i>j){
                    fxs[i] = fxs[i] - fx[id];
                    fys[i] = fys[i] - fy[id];
                    id++;
                }
            }
        }



        for (int i = 0; i < N; ++i) {


            (*cluster_list[i]).vel_x = (*cluster_list[i]).vel_x + (dt*fxs[i]/(*cluster_list[i]).mass);
            (*cluster_list[i]).vel_y = (*cluster_list[i]).vel_y + (dt*fys[i]/(*cluster_list[i]).mass);

            (*cluster_list[i]).pos_x = (*cluster_list[i]).pos_x + dt*(*cluster_list[i]).vel_x;
            (*cluster_list[i]).pos_y = (*cluster_list[i]).pos_y + dt*(*cluster_list[i]).vel_y;

        }
        for (int i = 0; i<N;++i){

            fxs[i]   = 0;
            fys[i]   = 0;
        }
        for (int k = 0; k < memory_size; ++k) {
            dist[k]  = 0;
            fx[k]    = 0;
            fy[k]    = 0;
        }

        /*if(iteration_number%20 == 0){
            printf("%.0f%%\n",((float)iteration_number/SN*100));
        }
	*/
    }

    printf("100%%!\n");

    ///cluster check set to 0 if there are big number of clusters


    FILE* output;
    output = fopen("output.gal", "w+");

    for (int i = 0; i < N; ++i) {
        fwrite((cluster_list[i]), 6, sizeof(double),output);
    }


    free(dist);
    free(fx);
    free(fy);
    free(fxs);
    free(fys);
    for (int l = 0; l < N; ++l) {
        free(cluster_list[l]);
    }

    fclose(output);
    fclose(fp);
    printf("script finished running successfully!");
    return 0;
}

