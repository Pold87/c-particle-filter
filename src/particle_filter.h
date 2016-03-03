#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#define N 1000 /* Total number of particles */
#define M 200 /* Number of fake measurements */

struct particle {

  double prev_x;
  double prev_y;
  
  double x;
  double y;
  double heading;
  double vel_x;
  double vel_y;
  double w; /* Weight (fitness) of the particle */
};

struct measurement {

  double x;
  double y;
  double uncertainty;
  double dist;
  double var_x; /* Variance of predictions in x direction */
  double var_y; /* Variance of predictions in y direction */
};

struct sift {

  double x;
  double y;
  int matches;
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
void particle_filter(struct particle particles[N], struct measurement *z, struct measurement *flow, int use_variance, int use_flow);
void particle_filter_multiple(struct particle particles[N], struct measurement *z, struct measurement *z2, int use_variance);
void read_measurements_from_csv(struct measurement zs[], char filename[], int size);
struct particle weighted_average(struct particle ps[], int size);
struct particle weight_forward_backward(struct particle p_forward, struct particle p_backward, int i, int k);
struct particle calc_uncertainty(struct particle particles[], struct particle weighted_mean, int size);

#endif
