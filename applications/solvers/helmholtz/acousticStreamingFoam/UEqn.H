// Solve the Momentum equation

MRF.correctBoundaryVelocity(U);

volVectorField Ua = fvc::grad(ap)/(rho*omega);
surfaceScalarField phiUa = linearInterpolate(Ua) & mesh.Sf();

tmp<fvVectorMatrix> tUEqn
(
    fvm::ddt(U) + fvm::div(phi, U)
  + MRF.DDt(U)
  + turbulence->divDevSigma(U)
 ==
    fvModels.source(U)
  - fvc::div(phiUa, Ua)
);
fvVectorMatrix& UEqn = tUEqn.ref();

UEqn.relax();

fvConstraints.constrain(UEqn);

if (pimple.momentumPredictor())
{
    solve(UEqn == -fvc::grad(p));

    fvConstraints.constrain(U);
}
