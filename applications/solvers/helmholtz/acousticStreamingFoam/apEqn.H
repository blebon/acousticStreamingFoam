for (int aPcorr=0; aPcorr<nacousticCorrectors; aPcorr++)
{   
    forAll(ap, celli)
    {
        if (ap[celli] > pblake.value())
        {
            A[celli] = N.value()*A_curve->value(ap[celli]);
            B[celli] = N.value()*B_curve->value(ap[celli]);
        }
        else
        {
            A[celli] = 0.;
            B[celli] = 0.;
        }
    }

    volScalarField k_Re(sqr(omega/c) - A/ap);
    
    fvScalarMatrix ReEqn
    (
        fvm::laplacian(p_Re)
        + fvm::SuSp(k_Re, p_Re)
        // ==
        // fvModels.source(p_Re)
    );

    volScalarField k_Im(-B/ap);

    fvScalarMatrix ImEqn
    (
        fvm::laplacian(p_Im)
        + fvm::SuSp(k_Im, p_Im)
        // ==
        // fvModels.source(p_Im)
    );

    p_Re.relax();
    p_Im.relax();

    ReEqn.solve();
    ImEqn.solve();

    p_Re.correctBoundaryConditions();
    p_Im.correctBoundaryConditions();

    ap = sqrt(sqr(p_Re) + sqr(p_Im));
}
