{   
    fvScalarMatrix ReEqn
    (
        fvm::laplacian(p_Re)
        + fvm::SuSp(k_P, p_Re)
        == Ar
    );

    fvScalarMatrix ImEqn
    (
        fvm::laplacian(p_Im)
        + fvm::SuSp(k_P, p_Im)
        == Bi
    );

    p_Re.relax();
    p_Im.relax();

    ReEqn.solve();
    ImEqn.solve();

    p_Re.correctBoundaryConditions();
    p_Im.correctBoundaryConditions();

    ap = sqrt(pow(p_Re, 2) + pow(p_Im, 2));
}
