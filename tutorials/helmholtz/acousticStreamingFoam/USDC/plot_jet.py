#!/usr/bin/env python3
"""Plots the pressure and jet profiles.

This script plots the acoustic pressure and jet profiles
using the output in the postProcessing directory.

Example:
    Run python script in case root directory:

        $ python3 plot_jet.py

Todo:
    * None

.. _Google Python Style Guide:
   http://google.github.io/styleguide/pyguide.html

"""

from __future__ import division, print_function

from os import listdir, path, sep
from natsort import natsorted, ns

import vtk
from vtk.numpy_interface import dataset_adapter as dsa
from vtk.util.numpy_support import vtk_to_numpy

import matplotlib as mpl
import matplotlib.tri as tr
import matplotlib.pyplot as plt
import matplotlib.patches as patches

from natsort import natsorted, ns
from numpy import (
    linspace,
    sqrt,
    around,
    where,
)

__author__ = "Bruno Lebon"
__copyright__ = "Copyright, 2022, Brunel University London"
__credits__ = ["Bruno Lebon"]
__email__ = "Bruno.Lebon@brunel.ac.uk"
__status__ = "Production"


def load_slice(filename):
    """Read field from .vtp file.

    :param filename: Filename of .vtp file, including path.
    :type filename: str
    :return: A tuple containing points, triangulation and scalar field.
    :rtype: tuple[numpy.array]

    """
    if not path.exists(filename):
        return None
    reader = vtk.vtkXMLPolyDataReader()
    reader.SetFileName(filename)
    reader.Update()

    data = reader.GetOutput()
    arrays = dsa.WrapDataObject(data)

    # Extracting triangular information
    cells = arrays.GetPolys()
    triangles = vtk_to_numpy(cells.GetData())

    ntri = cells.GetNumberOfCells()
    tris = [None] * ntri
    tri = [None] * ntri

    j = 0
    offset = 0

    for _ in range(ntri):
        points = triangles[j]
        tris[_] = list(triangles[j : j + points + 1])

        tri[_ + offset] = list([triangles[j + 1], triangles[j + 2], triangles[j + 3]])

        if points == 4:
            offset += 1
            tri.insert(
                _ + offset, list([triangles[j + 3], triangles[j + 4], triangles[j + 1]])
            )

        j += points + 1

    # Extracting points
    points = vtk_to_numpy(arrays.GetPoints())
    x, y, z = points.T

    # Extract data
    sc = vtk_to_numpy(arrays.GetPointData().GetArray(0))

    if any([i in filename for i in ["U", "grad(T)"]]):
        return (x, y, z, tri, sc.T)
    else:
        return (x, y, z, tri, sc)


Ucast = 0.00233  # Casting speed in m/s
offset = 0.02  # Set to horn position
p_blake = 320386.12807758985  # Blake pressure
p0 = 101325.0


def plot_jet(
    image_name="jet.png",
    pcmap="viridis",
    Ucmap="jet",
):
    """Plots acoustic pressure and velocity contours.

    :param image_name: Filename of image, including path.
    :type image_name: str
    :param pcmap: Color map for plotting P contour.
    :type pcmap: str or matplotlib.colormaps
    :param Ucmap: Color map for plotting U contour.
    :type Ucmap: str or matplotlib.colormaps
    :return: 0 if successful.
    :rtype: int

    """
    plt.clf()
    # plt.rc('text', usetex=True)
    fig, ax = plt.subplots(nrows=1, ncols=1)

    casedir = f"postProcessing{sep}yNormal"
    times = natsorted(listdir(casedir), alg=ns.FLOAT)
    time = times[-1]
    suffix = "cutPlane"

    # ap/p0 - Left hand side DC casting mould
    vtkFile = f"{casedir}/{time}/ap_{suffix}.vtp"
    x, y, z, tri, ap = load_slice(vtkFile)
    z -= offset
    P = ap / p0
    lhs_triang = tr.Triangulation(-x, z, triangles=tri)
    levels = linspace(0, 5.5, 12)
    levels = around(levels, decimals=1)
    cs_ap = ax.tricontourf(lhs_triang, P, cmap=pcmap, levels=levels)
    ax.tricontour(
        lhs_triang,
        P,
        colors=["k"],
        linewidths=0.7,
        linestyles="-.",
        levels=[p_blake / p0],
    )
    handles, labels = cs_ap.legend_elements(variable_name="P/p_0")
    labels[0] = "$P/p_0 \\leq {0:s}$".format(labels[1].split(" ")[0].replace("$", ""))
    labels[-1] = "$P/p_0 > {0:s}$".format(labels[-2].split(" ")[-1].replace("$", ""))
    ap_lgd = ax.legend(
        handles,
        labels,
        title=r"$P/p_0$",
        loc="center",
        bbox_to_anchor=(-0.30, 0.5),
    )

    # U - Right hand side DC casting mould
    vtkFile = f"{casedir}/{time}/U_{suffix}.vtp"
    x, y, z, tri, (Ux, Uy, Uz) = load_slice(vtkFile)
    z -= offset
    U = sqrt(Ux**2 + Uy**2 + Uz**2)
    Uz += Ucast
    rhs_triang = tr.Triangulation(x, z, triangles=tri)
    levels = linspace(0, 0.2, 11)
    levels = around(levels, decimals=2)
    cs_U = ax.tricontourf(rhs_triang, U, cmap=Ucmap, levels=levels)
    handles, labels = cs_U.legend_elements(variable_name=r"$U$")
    labels[0] = "$U \\leq {0:s}$".format(labels[1].split(" ")[0].replace("$", ""))
    labels[-1] = "$U > {0:s}$".format(labels[-2].split(" ")[-1].replace("$", ""))
    U_lgd = ax.legend(
        handles,
        labels,
        title=r"$U$ m s$^{-1}$",
        loc="center",
        bbox_to_anchor=(1.25, 0.5),
    )

    umask = where(U < 0.2, True, False)
    SKIP = 25
    quiveropts = dict(pivot="middle", scale=10.0, units="xy", angles="xy")
    q = ax.quiver(
        x[umask][::SKIP],
        z[umask][::SKIP],
        Ux[umask][::SKIP],
        Uz[umask][::SKIP],
        **quiveropts,
    )
    p = ax.quiverkey(
        q,
        0.12,
        -0.09,
        1e-1,
        "0.1 m s$^{-1}$",
        coordinates="data",
        labelpos="N",
        labelsep=0.1,
        labelcolor="k",
        color="k",
    )

    ax.text(-0.077 / 2, -0.075, "Acoustic pressure", color="k", ha="center")
    ax.text(0.077 / 2, -0.075, "Velocity", color="w", ha="center")

    rect = patches.Rectangle(
        (-0.011, 0.0),
        0.022,
        0.02,
        linewidth=5.0,
        linestyle="-",
        edgecolor="gray",
        facecolor="gray",
    )
    ax.add_patch(rect)

    ratio = 0.9
    ax.set_ylim([-0.085, 0.02])
    ax.set_aspect(1.0 * ratio)

    ax.tick_params(direction="out", which="both")
    ax.minorticks_on()

    ax.add_artist(ap_lgd)

    fig.set_size_inches(6, 4)
    plt.savefig(
        image_name,
        transparent=False,
        dpi=1000,
        bbox_extra_artists=(
            ap_lgd,
            U_lgd,
            p,
        ),
        bbox_inches="tight",
    )


if __name__ == "__main__":
    plot_jet(image_name="Jet.png", pcmap="magma_r", Ucmap="jet")
