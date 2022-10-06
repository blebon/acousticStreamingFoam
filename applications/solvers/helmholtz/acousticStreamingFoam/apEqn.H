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
    volScalarField k_Im(-B/ap);
    
    fvScalarMatrix ReEqn
    (
        fvm::laplacian(p_Re)
        + fvm::SuSp(k_Re, p_Re)
        ==
        k_Im * p_Im
    );  

    fvScalarMatrix ImEqn
    (
        fvm::laplacian(p_Im)
        + fvm::SuSp(k_Re, p_Im)
        ==
        - k_Im * p_Re
    );

    if ( aPcorr % 2 == 0 )
    {
        p_Re.relax();
        p_Im.relax();

        ReEqn.solve();
        ImEqn.solve();

        p_Re.correctBoundaryConditions();
        p_Im.correctBoundaryConditions();
    }
    else
    {
        p_Im.relax();
        p_Re.relax();

        ImEqn.solve();
        ReEqn.solve();

        p_Im.correctBoundaryConditions();
        p_Re.correctBoundaryConditions();
    }

    ap = sqrt(sqr(p_Re) + sqr(p_Im));
}
