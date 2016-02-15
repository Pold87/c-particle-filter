# Particle Filter in C
A particle filter for x, y coordinates without control input. The main file is src/particle_filter.c. The visualization is done on-the-fly with gnuplot. The particle filter uses the following parameters:


- Prior: P(X<sub>0</sub>) ~ N(0, mu=Inf) (uniformly distributed)
- Transition Model: P(X<sub>t</sub> | x_<sub>t-1</sub>) ~ N(mu=x<sub>t-1</sub>, sigma<sup>2</sup>), where sigma<sup>2</sup> is currently
  the process noise only (currently set to sigma<sup>2</sup> = 1600)
- Sensor Model: p(z<sub>t</sub> | X<sub>t</sub>) ~ N(mu=X<sub>t</sub>, sigma<sup>2</sup>), sigma<sup>2</sup><sub>x</sub>= 21566, sigma<sup>2</sup><sub>y</sub>= 60369

![Particle filter visualization](https://github.com/Pold87/c-particle-filter/tree/master/particle_filter.png)

- Red: Measurement (output from treXton)
- Green: Ground truth
- Blue: Particles (size represents their weight)
- Yellow: Best estimate from particle filter (weighted average)