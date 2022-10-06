Acoustic streaming solver
=========================

acousticStreamingFoam is based on `pimpleFoam <https://github.com/OpenFOAM/OpenFOAM-9/tree/master/applications/solvers/incompressible/pimpleFoam>`_, 
a transient solver for incompressible, turbulent flow of Newtonian fluids. acousticStreamingFoam includes an non-linear acoustic pressure solver and
an acoustic streaming source term to ``pimpleFoam``.

Installation
------------

Pre-requisites:  

* A working installation of `OpenFOAM 9 <https://openfoam.org/release/9/>`_.

In the acousticStreamingFoam/applications/solver/helmholtz/acousticStreamingFoam directory, run:

.. code-block:: console
  
  $ wmake

Running the application
-----------------------

In the case directory, run:

.. code-block:: console
  
  $ acousticStreamingFoam

Theory
------

Full details of the theory behind the solver implementation can be found in 
G.S. Bruno Lebon, Georges Salloum-Abou-Jaoude, Dmitry Eskin, Iakovos Tzanakis, 
Koulis Pericleous, Philippe Jarry, "Numerical modelling of acoustic streaming 
during the ultrasonic melt treatment of direct-chill (DC) casting", *Ultrasonics 
Sonochemistry* **54** (2019) 171-182 
`doi:10.1016/j.ultsonch.2019.02.002 <https://doi.org/10.1016/j.ultsonch.2019.02.002>`_.

Nomenclature
------------

.. table:: Variables used in acoustic streaming simulations.
  :widths: auto

  +----------+---------------------------------------------------------------------------------------------------+
  | Variable | Description                                                                                       |
  +==========+===================================================================================================+
  | nu       | Kinematic viscosity [m^2 s-1]                                                                     |
  +----------+---------------------------------------------------------------------------------------------------+
  | omega    | Frequency [rad/s^1]                                                                               |
  +----------+---------------------------------------------------------------------------------------------------+
  | c        | Speed of sound [m/s^1]                                                                            |
  +----------+---------------------------------------------------------------------------------------------------+
  | rho      | Density [kg/m^3]                                                                                  |
  +----------+---------------------------------------------------------------------------------------------------+
  | phorn    | Horn pressure [Pa]                                                                                |
  +----------+---------------------------------------------------------------------------------------------------+
  | pblake   | Blake pressure [Pa]                                                                               |
  +----------+---------------------------------------------------------------------------------------------------+
  | N        | Number of bubbles per unit volume [m^-3]                                                          |
  +----------+---------------------------------------------------------------------------------------------------+
  | AbyN     | Eqns (40-42) in `Trujillo (2018) <https://doi.org/10.1016/j.ultsonch.2018.04.014>`_  divided by N |
  +----------+---------------------------------------------------------------------------------------------------+
  | BbyN     | Eqns (43-45) in `Trujillo (2018) <https://doi.org/10.1016/j.ultsonch.2018.04.014>`_  divided by N |
  +----------+---------------------------------------------------------------------------------------------------+
  