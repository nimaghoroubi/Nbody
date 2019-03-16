//### version 1, pow function creating bubbles in pipeline
//########################################################

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PRINT 0

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
    double** dist;
    dist = malloc(N*(sizeof(double*)));
    for (int i = 0; i < N; i++) {
        dist[i] = malloc(N * sizeof(double));
    }
    double** fx;
    double** fy;
    fx = malloc(N*(sizeof(double*)));
    for (int i = 0; i < N; i++) {
        fx[i] = malloc(N * sizeof(double));
    }
    fy = malloc(N*(sizeof(double*)));
    for (int i = 0; i < N; i++) {
        fy[i] = malloc(N * sizeof(double));
    }


    
    for (int i = 0; i<N;++i){
        for (int j = 0; j < N; ++j) {

            fy[i][j] = 0.0;
            fx[i][j] = 0.0;   
        }
    }

    for (int iteration_number = 0; iteration_number < SN; ++iteration_number) {

        ///calculation of distance

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == j){
                    dist[i][j] = 0.0;
                }else if(i<j) {
                    dist[i][j] = eps + sqrt((pow(((*cluster_list[i]).pos_x - (*cluster_list[j]).pos_x),2))+ pow(((*cluster_list[i]).pos_y - (*cluster_list[j]).pos_y),2));
                }else if(j<i){
                    dist[i][j] = dist[j][i];
                }
            }
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if(i == j){
                    continue;
                }else if(i<j){
                    fx[i][j] = -((G*((*cluster_list[i]).mass)*((*cluster_list[j]).mass)/pow(dist[i][j],3))*(((*cluster_list[i]).pos_x)-((*cluster_list[j]).pos_x)));
                    fx[i][i] = fx[i][i] + fx[i][j];
                    fy[i][j] = -((G*((*cluster_list[i]).mass)*((*cluster_list[j]).mass)/pow(dist[i][j],3))*(((*cluster_list[i]).pos_y)-((*cluster_list[j]).pos_y)));
                    fy[i][i] = fy[i][i] + fy[i][j];
                }else if (j<i){
                    fx[i][j] = -fx[j][i];
                    fx[i][i] = fx[i][i] + fx[i][j];
                    fy[i][j] = -fy[j][i];
                    fy[i][i] = fy[i][i] + fy[i][j];

                }
            }
            
        }

        for (int i = 0; i < N; ++i) {


            (*cluster_list[i]).vel_x = (*cluster_list[i]).vel_x + (dt*fx[i][i]/(*cluster_list[i]).mass);
            (*cluster_list[i]).vel_y = (*cluster_list[i]).vel_y + (dt*fy[i][i]/(*cluster_list[i]).mass);

            (*cluster_list[i]).pos_x = (*cluster_list[i]).pos_x + dt*(*cluster_list[i]).vel_x;
            (*cluster_list[i]).pos_y = (*cluster_list[i]).pos_y + dt*(*cluster_list[i]).vel_y;

        }
        for (int i = 0; i<N;++i){
            for (int j = 0; j < N; ++j) {

                fy[i][j] = 0.0;
                fx[i][j] = 0.0;
            }
        }

        if(iteration_number%20 == 0){
            printf("%.0f%%\n",((float)iteration_number/SN*100));
        }
    }

    printf("100%%!\n");

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

    FILE* output;
    output = fopen("output.gal", "w+");

    for (int i = 0; i < N; ++i) {
        fwrite((cluster_list[i]), 6, sizeof(double),output);
    }

    for (int l = 0; l < N; ++l) {
        free(cluster_list[l]);
        free(dist[l]);
        free(fx[l]);
        free(fy[l]);
    }
    free(dist);
    free(fx);
    free(fy);

    fclose(output);
    fclose(fp);
    printf("script finished running successfully!");
    return 0;
}
