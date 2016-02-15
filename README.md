# Particle Filter in C
A particle filter for x, y coordinates without control input. The main file is particle_filter.c. The particle filter uses the following parameters:

Prior: P(X<sub>0</sub>) ~ N(0, mu=Inf) (uniformly distributed)
Transition Model: P(X<sub>t</sub> | x_<sub>t-1</sub>) ~ N(mu=x_(t-1), sigma^2)$, where sigma^2 is currently
  the process noise only (currently set to sigma<sup>2</sup> = 1600)
Sensor Model: p(z<sub>t</sub> | X<sub>t</sub>) ~ N(mu=X<sub>t</sub>, sigma<sup>2</sup>), \sigma<sup>2</sup><sub>x</sub>= 21566, \sigma<sup>2</sup><sub>y</sub>= 60369$
