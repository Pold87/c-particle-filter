#define N 100 /* Total number of particles */
#define M 200 /* Number of fake measurements */
#define max_x 200
#define max_y 200

struct particle {
  
  double x;
  double y;
  double w; /* Weight (fitness) of the particle */
};

struct measurement {

  double x;
  double y;
};


void init_particles(struct particle particles[N]);
void particle_filter(struct particle particles[N], struct measurement *m);
