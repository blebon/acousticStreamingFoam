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

dimensionedScalar phorn
(
    "phorn",
    dimPressure,
    transportProperties.lookup("phorn")
);

dimensionedScalar pblake
(
    "pblake",
    dimPressure,
    transportProperties.lookup("pblake")
);

dimensionedScalar N
(
    "N",
    inv(dimVolume),
    transportProperties.lookup("N")
);

autoPtr<Function1<scalar>> A_curve(Function1<scalar>::New("AbyN", transportProperties));
autoPtr<Function1<scalar>> B_curve(Function1<scalar>::New("BbyN", transportProperties));

volScalarField A
(
    IOobject
    (
        "A",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar(
        "_A",
        dimPressure*inv(dimArea),
        N.value()*A_curve->value(phorn.value())
    )
);

volScalarField B
(
    IOobject
    (
        "B",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar(
        "_B",
        dimPressure*inv(dimArea),
        N.value()*B_curve->value(phorn.value())
    )
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
