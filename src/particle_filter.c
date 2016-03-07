/*
Particle filter without control input
 */

#include <stdio.h>
#include <stdlib.h>
#include <particle_filter.h>
#include "random_number_gen.h"
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

#define max_x 1280
#define max_y 720

int informed_prior = 0;
int global_k = 0;

void init_particles(struct particle particles[N]){

  srand(time(NULL)); /* Set random seed */
  int i;
  for (i = 0; i < N; i++) {

    /* Initialize with informed prior */
    if (informed_prior) {
      particles[i].x = randu(1060, 1080);
      printf("%f", particles[i].x);
      particles[i].y = randu(275, 285);
    }

    /* Initialize with random x, y-positions */
    else {

      particles[i].prev_x = randu(0, max_x);
      particles[i].prev_y = randu(0, max_y);

      particles[i].vel_x = randu(0, 5);
      particles[i].vel_y = randu(0, 5);

      particles[i].x = randu(0, max_x);
      particles[i].y = randu(0, max_y);
      }
  }
}


double normpdf(double x, double mu, double sigma) {

  double density, a;
  static const double inv_sqrt_2pi = 0.3989422804014327;

  a = (x - mu) / sigma;
  density = inv_sqrt_2pi / sigma * exp(-0.5 * a * a);

  return density;

}

void weighted_sample(struct particle ps[], struct particle res[], double weights[], int samples){

  double sum = 0, x;
  int i;
  /* Calculate sum */
  for (i = 0; i < N; i++) {
    sum += weights[i];
  }
  printf("sum is %f", sum);

  i = 0;
  int m;
  double w = weights[0];
  struct particle v;
  for (m = 0; m < samples; m++) {
    x = sum * (1 - pow(randu(0, 1), (1.0 / samples)));
    //printf("%f is\n", x);
    sum -= x;
    while (x > w) {
            x -= w;
            i += 1;
            w = weights[i];
      v = ps[i];
    }
    w -= x;
    res[m] = v;
  }
}

double fmax(double a, double b) {

  if (a > b)
    return a;
  else
    return b;
}

double fmin(double a, double b) {

  if (a < b)
    return a;
  else
    return b;
}


double array_max(double arr[], int size){

  double m = -1;
  int i;
  for (i = 0; i < size; i++) {
    if (arr[i] > m)
      m = arr[i];
  }
  return m;
}


void resampling_wheel(struct particle ps[], struct particle res[], double weights[], int samples) {

    /* p: particles */
    /* w: weights */
    /* N: Desired number of particles */

  int idx = randu(0, 1) * samples;
  /* printf("index is %d\n", idx); */
  double beta = 0.0;
  double mw = array_max(weights, samples);
  /* printf("MAX IS %f\n", mw); */
  int j;
  /* for (j = 0; j < samples; j++) { */
  /*   printf("particle is: %f %f\n", ps[j].x, ps[j].y); */
  /* } */


  int i;
  for (i = 0; i < N; i++) {
    /* printf("[resampling wheel] weights: %f\n", weights[i]); */
    beta += randu(0, 1) * 2.0 * mw;
    while (beta > weights[idx]){
      /* printf(" beta is %f\n", beta); */
      /* printf(" weights[ifx] is %f\n", weights[idx]); */
      beta -= weights[idx];
      idx = (idx + 1) % N;
    }
    /* printf("idx outside is %d\n", idx); */
    /* printf("ps[ids] outside is %f\n", ps[idx].x); */
    res[i] = ps[idx];
  }
}


void particle_filter_multiple(struct particle xs[N], struct measurement *z, struct measurement *z2, int use_variance) {

  //printf("x is: %f y is: %f\n", z->x, z->y);
  double w[N]; /* The weights of particles */

  double process_noise_x = 35;
  double process_noise_y = 35;

  double measurement_noise_x;
  double measurement_noise_y;

  double measurement_noise_2_x = 100;
  double measurement_noise_2_y = 50;


  if (use_variance) {

    measurement_noise_x = sqrt(z->var_x);
    measurement_noise_y = sqrt(z->var_y);
    printf("Measurement noise x is %f", measurement_noise_x);

  } else {

    measurement_noise_x = 100;
    measurement_noise_y = 100;

  }

  /* For SIFT */
  /* double process_noise_x = 8; */
  /* double process_noise_y = 8; */

  /* double measurement_noise_x = 70; */
  /* double measurement_noise_y = 70; */


  /* Obtaining new belief state (iterate over all particles) */
  int i = 0;

  for (i = 0; i < N; i++) {
    /* Process noise incorporates the movement of the UAV */
    /* According to p(x_t | x_(t-1)) */
    xs[i].x = randn(xs[i].x, process_noise_x);
    xs[i].y = randn(xs[i].y, process_noise_y);

    /* Calculate weight */
    double p_x, p_y, p_x2, p_y2;
    //printf("z->x is %f xs[i].x is %f\n", z->x, xs[i].x);

    double weight_regressor_1 = 0.5;
    double weight_regressor_2 = 0.5;

    weight_regressor_1 = measurement_noise_2_x / sqrt(z->var_x);
    /* printf("weight %f", weight_regressor_1); */
    weight_regressor_2 = 1;

    if (z->x != -1) {
      p_x = normpdf(z->x, xs[i].x, measurement_noise_x);
      p_y = normpdf(z->y, xs[i].y, measurement_noise_y);

      p_x2 = normpdf(z2->x, xs[i].x, measurement_noise_2_x);
      p_y2 = normpdf(z2->y, xs[i].y, measurement_noise_2_y);

      /* printf("weight %f", weight_regressor_2 * p_x2 * p_y2); */
      //printf("p2 is %f\n", w[i]);
      w[i] = (weight_regressor_1 * p_x * p_y) + (weight_regressor_2 * p_x2 * p_y2);
      //w[i] = p_x * p_y;
      xs[i].w = w[i]; /* Set weight of particle */
    /* printf("w is %f\n", w[i]); */
    }
  }


  /* Importance resampling: (iterate over all particles) */
  struct particle res[N];
  resampling_wheel(xs, res, w, N);

  for (i = 0; i < N; i++) {
    xs[i] = res[i];
  }

}


void particle_filter(struct particle xs[N], struct measurement *z, struct measurement *flow, int use_variance, int use_flow) {

  //printf("x is: %f y is: %f\n", z->x, z->y);
  double w[N]; /* The weights of particles */

  double process_noise_x = 12;
  double process_noise_y = 12;

  double measurement_noise_x;
  double measurement_noise_y;

  if (use_variance) {

    measurement_noise_x = sqrt(z->var_x);
    measurement_noise_y = sqrt(z->var_y);
    printf("Measurement noise x is %f", measurement_noise_x);

  } else {

    measurement_noise_x = 120;
    measurement_noise_y = 120;

  }

  /* For SIFT */
  /* double process_noise_x = 8; */
  /* double process_noise_y = 8; */

  /* double measurement_noise_x = 70; */
  /* double measurement_noise_y = 70; */


  /* Obtaining new belief state (iterate over all particles) */
  int i = 0;

  for (i = 0; i < N; i++) {

    /* printf("particle x pos %f", xs[i].x); */

    /* Process noise incorporates the movement of the UAV */
    /* According to p(x_t | x_(t-1)) */

    /* Calculate current heading */
    /* double heading = atan2(z->y - xs[i].y, z->x - xs[i].x); */

    /* Update heading */
    /* xs[i].heading = 0.8 * xs[i].heading + 0.2 * heading; */

    /* Move according to velocity */
    /* double updated_x = xs[i].x + xs[i].vel_x * cos(xs[i].heading); */
    /* double updated_y = xs[i].y + xs[i].vel_y * sin(xs[i].heading); */

    /* Move according to velocity */
    /* double updated_x = xs[i].x + xs[i].vel_x; */
    /* double updated_y = xs[i].y + xs[i].vel_y; */

    double updated_x;
    double updated_y;
    if (use_flow) {


       /* Change heading */
       /* TODO */

       /* atan2(flow->y, flow->x) */

       updated_x = xs[i].x + flow->x;
       updated_y = xs[i].y + flow->y;


    } else {
       updated_x = xs[i].x;
       updated_y = xs[i].y;
    }

    if (z->x != -1) {
    /* Calculate current velocity */
    double vel_x = z->x - xs[i].x;
    double vel_y = z->y - xs[i].y;

   /* Update velocity */
    double speed_p_x = fmin(1, 0.01 + 100 * normpdf(xs[i].vel_x, vel_x, 50));
    double speed_p_y = fmin(1, 0.01 + 100 * normpdf(xs[i].vel_y, vel_y, 50));

    if (i == 0)
      printf("p: %f\n", normpdf(xs[i].vel_y, vel_y, 50));

    xs[i].vel_x = fmax(-25, fmin(25, (1 - speed_p_x) * xs[i].vel_x + speed_p_x * vel_x));
    xs[i].vel_y = fmax(-25, fmin(25, (1 - speed_p_y) * xs[i].vel_y + speed_p_y * vel_y));

    if (i == 0)
      printf("vel %f", xs[i].vel_x);

    }

    /* Add some random process noise */
    xs[i].x = randn(updated_x, process_noise_x);
    xs[i].y = randn(updated_y, process_noise_y);
    /* xs[i].x = updated_x; */
    /* xs[i].y = updated_y; */
    /* xs[i].vel_x = randn(xs[i].vel_x, process_noise_x); */
    /* xs[i].vel_y = randn(xs[i].vel_y, process_noise_y); */

    /* Calculate weight */
    double p_x, p_y;
    //printf("z->x is %f xs[i].x is %f\n", z->x, xs[i].x);

    if (z->x != -1) {

      p_x = 0.001 + normpdf(z->x, xs[i].x, measurement_noise_x);
      p_y = 0.001 + normpdf(z->y, xs[i].y, measurement_noise_y);

      //printf("p2 is %f\n", w[i]);
      w[i] = p_x * p_y;
      //w[i] = p_x * p_y;
      xs[i].w = w[i]; /* Set weight of particle */
    /* printf("w is %f\n", w[i]); */
    }
  }


  /* Importance resampling: (iterate over all particles) */
  struct particle res[N];
  resampling_wheel(xs, res, w, N);

  for (i = 0; i < N; i++) {
    xs[i] = res[i];
  }

}


void read_measurements_from_csv(struct measurement zs[], char filename[], int size) {

  int i;
  FILE *fp = fopen(filename, "r");

  /* Read header */
  char header[30];
  fscanf(fp, "%s\n", header);

  double dummy_num;
  for (i = 0; i < size; i++) {
fscanf(fp, "%lf,%lf,%lf,%lf\n", &dummy_num, &zs[i].x, &zs[i].y, &zs[i].uncertainty);
  }

  fclose(fp);

}


void read_predictions_from_csv(struct measurement zs[], char filename[], int size, int use_variance) {

  int i;
  FILE *fp = fopen(filename, "r");

  for (i = 0; i < size; i++) {

    if (use_variance) { /* Read variance from file */

      fscanf(fp, "%lf,%lf,%lf,%lf\n", &zs[i].x, &zs[i].y,
       &zs[i].var_x, &zs[i].var_y);
    }

    else
      fscanf(fp, "%lf,%lf\n", &zs[i].x, &zs[i].y);
  }

  fclose(fp);

}

void read_sift_from_csv(struct sift sifts[], char filename[]) {

  int i;
  FILE *fp = fopen(filename, "r");

  /* Read header */
  char header[30];
  fscanf(fp, "%s\n", header);

  double dummy_num;
  for (i = 0; i < M; i++) {
    fscanf(fp, "%lf,%lf,%lf,%d\n", &dummy_num, &sifts[i].x, &sifts[i].y, &sifts[i].matches);
  }

  fclose(fp);

}

void create_fake_measurements(struct measurement zs[]) {

  int i;

  int x, y;
  x = y = 0;

  double noise_x = 50;
  double noise_y = 50;


  /* Movement delta_x = delta_y = 5 */
  for (i = 0; i < M; i++) {
     zs[i].x = (double) 5*i + randn(0, noise_x);
     zs[i].y = (double) 5*i + randn(0, noise_y);
}


  /* Bimodal measurements */
  /* for (i = 0; i < M; i++) { */
  /*   zs[i].x = (double) (100 + ((i * 200) % 400)) + randn(0, noise_x); */
  /*   zs[i].y = (double) (100 + ((i * 200) % 400)) + randn(0, noise_y); */
/* } */


}


/* Write measurements to csv */
void zs_to_csv(struct measurement zs[M], char filename[]) {

  int i;
  FILE *fp = fopen(filename, "w");

  for (i = 0; i < M; i++) {
    fprintf(fp, "%f,%f\n" , zs[i].x, zs[i].y);
  }

  fclose(fp);
}


struct particle weighted_average(struct particle ps[], int size) {

  int i;
  double total_weight = 0;
  double x = 0;
  double y = 0;

  for (i = 0; i < size; i++) {
    total_weight += ps[i].w;
    x += ps[i].x * ps[i].w;
    y += ps[i].y * ps[i].w;
  }

  struct particle p;
  p.x = x / total_weight;
  p.y = y / total_weight;

  return p;
}


struct particle weight_forward_backward(struct particle p_forward, struct particle p_backward, int i, int k) {

  double forward_x = p_forward.x;
  double backward_x = p_backward.x;
  double forward_y = p_forward.y;
  double backward_y = p_backward.y;

  double combined_x = (i * forward_x + k * backward_x) / (i + k);
  double combined_y = (i * forward_y + k * backward_y) / (i + k);

  struct particle p;
  p.x = combined_x;
  p.y = combined_y;

  return p;

}

struct particle calc_uncertainty(struct particle ps[], struct particle weighted_mean, int size) {

  int i;
  double total_weight = 0;
  double x = 0;
  double y = 0;

    for (i = 0; i < size; i++) {
    total_weight += ps[i].w;
    x += ps[i].w * pow(ps[i].x - weighted_mean.x, 2);
    y += ps[i].w * pow(ps[i].y - weighted_mean.y, 2);
  }

  struct particle p;
  p.x = x / total_weight;
  p.y = y / total_weight;

  return p;

}
