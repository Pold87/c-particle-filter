/*
Particle filter without control input
 */

#include <stdio.h>
#include <stdlib.h>
#include <particle_filter.h>
#include <random_number_gen.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

void init_particles(struct particle particles[N]){

  srand(time(NULL)); /* Set random seed */
  int i;
  for (i = 0; i < N; i++) {
    /* Initialize with random x, y-positions */
    particles[i].x = randu(0, max_x);
    particles[i].y = randu(0, max_y);
  }
}


double normpdf(double x, double mu, double sigma) {

  double density, a;
  static const double inv_sqrt_2pi = 0.3989422804014327;

  a = (x - mu) / sigma;
  density = inv_sqrt_2pi / sigma * exp(-0.5 * a * a);

  return density * 100;

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

double max(double arr[], int size){

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
  double mw = max(weights, samples);
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


void particle_filter(struct particle xs[N], struct measurement *z) {

  //printf("x is: %f y is: %f\n", z->x, z->y);
  double w[N]; /* The weights of particles */

  double process_noise_x = 40;
  double process_noise_y = 40;

  double measurement_noise_x = 147;
  double measurement_noise_y = 245;

/* Obtaining new belief state (iterate over all particles) */
  int i = 0;
  for (i = 0; i < N; i++) {
    /* Process noise incorporates the movement of the UAV */
    /* According to p(x_t | x_(t-1)) */
    xs[i].x = randn(xs[i].x, process_noise_x);
    xs[i].y = randn(xs[i].y, process_noise_y);

    /* Calculate weight */
    double p1, p2;
    //printf("z->x is %f xs[i].x is %f\n", z->x, xs[i].x);
    p1 = normpdf(z->x, xs[i].x, measurement_noise_x);
    //printf("p1 is %f\n", w[i]);
    p2 = normpdf(z->y, xs[i].y, measurement_noise_y);
    //printf("p2 is %f\n", w[i]);
    w[i] = p1 * p2;
    xs[i].w = w[i]; /* Set weight of particle */
    /* printf("w is %f\n", w[i]); */
  }


/* Importance resampling: (iterate over all particles) */
  struct particle res[N];
  resampling_wheel(xs, res, w, N);

  for (i = 0; i < N; i++) {
    xs[i] = res[i];
  }

}


void create_fake_measurements(struct measurement zs[M]) {

  int i;

  int x, y;
  x = y = 0;

  double noise_x = 50;
  double noise_y = 50;


  for (i = 0; i < M; i++) {
     zs[i].x = (double) 5*i + randn(0, noise_x);
     zs[i].y = (double) 5*i + randn(0, noise_y);
}

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


int main(int argc, char *argv[])
{

  printf("Red: Measurement (output from treXton)\nGreen: Ground truth\nBlue: Particles(size represents their weight)\nYellow: Best estimate from particle filter (weighted average)");
  
  /* Create and initilize particles */
  struct particle particles[N];
  init_particles(particles);

  struct measurement measurements[M];
  create_fake_measurements(measurements);
  zs_to_csv(measurements, "fake_measurements.csv");

  /* Run particle filter for every measurement */
  int i = 0, j = 0;
  FILE *gnuplot;
  gnuplot = popen("gnuplot", "w");
  fprintf(gnuplot, "set palette model RGB defined (0 'blue', 1 'green', 2 'red', 3 'yellow')\n");
  fprintf(gnuplot, "unset colorbox\n");
  fprintf(gnuplot, "set xrange[0:400]\n");
  fprintf(gnuplot, "set yrange[0:400]\n");

  for (i = 0; i < M; i++) {
    particle_filter(particles, &measurements[i]);

    struct particle p;
    p = weighted_average(particles, N);

    fprintf(gnuplot, "plot '-' with points pt 7 ps variable palette\n");
    for (j = 0; j < N; j++) {
      fprintf(gnuplot, "%f %f %f 0\n", particles[j].x, particles[j].y, particles[j].w);
      /* printf("%f %f\n", particles[j].x, particles[j].y); */
    }
    printf("measurement (z) - x: %f , y: %f\n", measurements[i].x, measurements[i].y);
    fprintf(gnuplot, "%f %f 4 2\n", measurements[i].x, measurements[i].y);
    fprintf(gnuplot, "%f %f 4 1\n", (double) 5 * i, (double) 5 * i);
    fprintf(gnuplot, "%f %f 4 3\n", p.x, p.y); /* The best prediction (weighted average) */
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);
    fprintf(gnuplot, "refresh;\n");

    struct timespec ts;
    int milliseconds = 300;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
    /*fclose(gnuplot);*/
    /* Print all particles */
    /* for (j = 0; j < N; j++) { */
    /*   printf("x is: %f y is: %f\n", particles[j].x, particles[j].y); */
    /* } */
  }
  return 0;
}
