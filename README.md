acousticStreamingFoam
======

## About OpenFOAM  
  OpenFOAM is a free, open source computational fluid dynamics (CFD) software
  package released by the OpenFOAM Foundation. It has a large user base across
  most areas of engineering and science, from both commercial and academic
  organisations. OpenFOAM has an extensive range of features to solve anything
  from complex fluid flows involving chemical reactions, turbulence and heat
  transfer, to solid dynamics and electromagnetics.

## Copyright  
  OpenFOAM is free software: you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.  See the file [LICENSE](LICENSE) in this directory or
  [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/), for a description of the GNU General Public
  License terms under which you can copy the files.

## Acoustic Streaming Application  
  This repository contains developments for modeling direct-chill (DC) casting
  by Bruno Lebon at BCAST, Brunel University London using OpenFOAM.

  This work includes a solver based on:

  F.J. Trujillo, "A strict formulation of a nonlinear Helmholtz equation 
  for the propagation of sound in bubbly liquids. Part I: theory and validation 
  at low acoustic pressure amplitudes",  Ultrason. Sonochem. **47** (2018) 75-98.

  as well as a tutorial case whose geometry corresponds to the case in:
  
  T. Subroto, G.S.B. Lebon _et al._, "Numerical modelling and experimental validation 
  of the effect of ultrasonic melt treatment in a direct-chill cast AA6008 alloy billet",
  Journal of Materials Research and Technology **12** (2021) 1582-1596.

  * [Solver](applications/solvers/helmholtz/acousticStreamingFoam) - Updated version of the acoustic streaming solver
  * [Tutorial](tutorials/helmholtz/acousticStreamingFoam/USDC) - Tutorial case with geometry corresponding to the above reference 
