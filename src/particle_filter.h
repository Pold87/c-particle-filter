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


/**
 * @brief Calculates the value of x of a normal distribution with mean mu and
 * standard deviation sigma.
 *
 * @param x
 * @param mu Mean of the normal distribution
 * @param sigma Standard deviation of the normal distribution
 *
 * @return Value of the normal distribution at position x
 */
double normpdf(double x, double mu, double sigma);

void init_particles(struct particle particles[N]);
void particle_filter(struct particle particles[N], struct measurement *m);
