%%%%%%%%%%%%%%%%%%%%%%%%% Be a universe (not literally though!) %%%%%%%%%%%%%
    ///demangling
    int N           = atoi(argv[1]);
    char* filename  =      argv[2];
    int SN          = atoi(argv[3]);
    double dt       = atof(argv[4]);
    int graphics    = atoi(argv[5]);
    
    run like this ./a.out 1000 ellipse_N_01000.gal 200 1e-5 1
    easy, a.out is executable, 1000 is number of bodies in file you input, ellipse_N_01000.gal is file name i included, 
    200 is number of timesteps, 1e-5 is timestep size and the little 1 at the end is the graphic switch! set to 0 to switch
    it OFF But what fun is that? I dont even know why would you run this with no graphics...
    
    have fun!
