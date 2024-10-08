import numpy as np


def steadystateiceshelftemp(md, surfacetemp, basaltemp):
    """
    Compute the depth - averaged steady - state temperature of an ice shelf
    This routine computes the depth - averaged temperature accounting for vertical advection
    and diffusion of heat into the base of the ice shelf as a function of surface and basal
    temperature and the basal melting rate.  Horizontal advection is ignored.
   The solution is a depth - averaged version of Equation 25 in Holland and Jenkins (1999).

    In addition to supplying md, the surface and basal temperatures of the ice shelf must be supplied in degrees Kelvin.

    The model md must also contain the fields:
    md.geometry.thickness
    md.basalforcings.floatingice_melting_rate (positive for melting, negative for freezing)

   Usage:
      temperature = steadystateiceshelftemp(md, surfacetemp, basaltemp)
    """
    if len(md.geometry.thickness) != md.mesh.numberofvertices:
        raise ValueError('steadystateiceshelftemp error message: thickness should have a length of ' + md.mesh.numberofvertices)

    #surface and basal temperatures in degrees C
    if len(surfacetemp) != md.mesh.numberofvertices:
        raise ValueError('steadystateiceshelftemp error message: surfacetemp should have a length of ' + md.mesh.numberofvertices)

    if len(basaltemp) != md.mesh.numberofvertices:
        raise ValueError('steadystateiceshelftemp error message: basaltemp should have a length of ' + md.mesh.numberofvertices)

    # Convert temps to Celsius for Holland and Jenkins (1999) equation
    Ts = -273.15 + surfacetemp
    Tb = -273.15 + basaltemp

    Hi = md.geometry.thickness
    ki = 1.14e-6 * md.constants.yts  # ice shelf thermal diffusivity from Holland and Jenkins (1999) converted to m^2 / yr

    #vertical velocity of ice shelf, calculated from melting rate
    wi = md.materials.rho_water / md.materials.rho_ice * md.basalforcings.floatingice_melting_rate

    #temperature profile is linear if melting rate is zero, depth - averaged temp is simple average in this case
    temperature = (Ts + Tb) / 2  # where wi~=0

    pos = np.nonzero(abs(wi) >= 1e-4)  # to avoid division by zero

    np.seterr(over='raise', divide='raise')  # raise errors if floating point exceptions are encountered in following calculation
    #calculate depth - averaged temperature (in Celsius)
    try:
        temperature[pos] = -((Tb[pos] - Ts[pos]) * ki / wi[pos] + Hi[pos] * Tb[pos] - (Hi[pos] * Ts[pos] + (Tb[pos] - Ts[pos]) * ki / wi[pos]) * np.exp(Hi[pos] * wi[pos] / ki)) / (Hi[pos] * (np.exp(Hi[pos] * wi[pos] / ki) - 1))
    except FloatingPointError:
        print('Warning: steadystateiceshelf.py: overflow encountered in multipy/divide/exp, trying another formulation.')
        temperature[pos] = -(((Tb[pos] - Ts[pos]) * ki / wi[pos] + Hi[pos] * Tb[pos]) / np.exp(Hi[pos] * wi[pos] / ki) - Hi[pos] * Ts[pos] + (Tb[pos] - Ts[pos]) * ki / wi[pos]) / (Hi[pos] * (1 - np.exp(-Hi[pos] * wi[pos] / ki)))

    #temperature should not be less than surface temp
    pos = np.nonzero(temperature < Ts)
    temperature[pos] = Ts[pos]

    # NaN where melt rates are too high (infinity / infinity in exponential)
    pos = np.nonzero(np.isnan(temperature))
    temperature[pos] = Ts[pos]

    #convert to Kelvin
    temperature = temperature + 273.15

    return temperature
