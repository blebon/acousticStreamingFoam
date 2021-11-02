#include "createRDeltaT.H"

Info<< "Reading field p_Re\n" << endl;
volScalarField p_Re
(
    IOobject
    (
        "p_Re",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field p_Im\n" << endl;
volScalarField p_Im
(
    IOobject
    (
        "p_Im",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Initializing absolute acoustic pressure\n" << endl;
volScalarField ap
(
    IOobject
    (
        "ap",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    sqrt(pow(p_Re, 2) + pow(p_Im, 2))
);

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

dimensionedScalar omega
(
    "omega",
    inv(dimTime),
    transportProperties.lookup("omega")
);

dimensionedScalar c
(
   "c",
   dimVelocity,
   transportProperties.lookup("c")
);

dimensionedScalar rho
(
    "rho",
    dimDensity,
    transportProperties.lookup("rho")
);

dimensionedScalar A
(
    "A",
    dimPressure*inv(dimArea),
    transportProperties.lookup("A")
);

dimensionedScalar B
(
    "B",
    dimPressure*inv(dimArea),
    transportProperties.lookup("B")
);

volScalarField k_P
(
   IOobject
   (
      "k_P",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
   ),
   mesh,
   sqr(omega/c)
);

volScalarField Ar
(
   IOobject
   (
      "Ar",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
   ),
   mesh,
   A
);

volScalarField Bi
(
   IOobject
   (
      "Bi",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
   ),
   mesh,
   B
);

Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

#include "createPhi.H"

pressureReference pressureReference(p, pimple.dict());

mesh.setFluxRequired(p.name());


singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::momentumTransportModel> turbulence
(
    incompressible::momentumTransportModel::New(U, phi, laminarTransport)
);

#include "createMRF.H"
#include "createFvModels.H"
#include "createFvConstraints.H"
