Description of the repository
The repository contains the calculated yield functions of nucleon directed differential fluxes and the C++ code necessary for calculation of atmospheric particle fluxes.

Suggested data citation
Aleksandr Nesterenok, Numerical calculations of cosmic ray cascade in the Earth's atmosphere - results for nucleon spectra, accepted in Nuclear Instruments in Physics Research B.

Description of the data files
The yield functions of nucleon fluxes induced by prymary protons are located in the directory ./_p_ . The yield functions of nucleon fluxes induced by prymary alpha-particles are located in the directory ./_a_ . There is a file in each directory - "id_file_p_.txt" and "id_file_a_.txt", respectively. Each row in these files includes the identification suffix of data file, the energy interval of primary particle (the minimal and maximal energy), the number of primary particles, for which the simulations were performed.
There are two sets of data files - the files which contain the angle distributions of proton spectra and the files with the angle distributions of neutron spectra. The data on particle fluxes are normalized to the unit flux of primary particle of the given type and energy, impinging on the top of the atmosphere. 
The data files have the following structure:
The first row contains the identification suffix of data file, the energy interval of primary particle (the minimal and maximal energy), the number of primary particles.
In the second row, the number of columns and rows are provided (the first two rows are not taken into account).
The third row has the following structure
-1	-1	d1	c1	d1	c2	d1	c3	d1	c4	d1	c5	d1	c6	d1	c7	d2	c1	d2	c2 ...
where d1, d2 and etc. - are the depth values; c1, c2 and etc. - are the cosine values of the nadir angle. The other rows contain the values of directed differential particle fluxes, with the first and second columns being the center of energy interval and the interval half length, respectively. For example:
e	de	v1	dv1	v2	dv2	v3	dv3	v4	dv4	v5	dv5	v6	dv6	v7	dv7	v8	dv8	...
where v1 is the average directed differential particle flux in the energy interval (e-de, e+de) and in the nadir angle range (t1, t2), with cos(t1) = c1, cos(t2) = c2; dv1 is the flux error. For v7 the following is correct ? cos(t7) = c7, cos(t8) = -1. For the particle flux value v8 the nadir angle range is again equal to (t1, t2). The nadir angle intervals repeat cyclic from left to right, the depth value being changed with each new cycle.

Description of the C++ code.
PrimaryCosmicRaySpectrum.cpp is the main source file.
Header files: Utils.h, Constant.h, PrimaryParticle.h, PrimaryCosmicRaySpectrum.h, ParameterGrid.h
Source files: PrimaryCosmicRaySpectrum.cpp, ParameterGrid.cpp
Text file: input.txt
The file "input.txt" contains the input information: the directory name, where input data are located, the directory of output, the solar modulation parameter value (in MV), the geomagnetic cutoff rigidity value (in MV).
The code generates the files with the data on directed differential particle fluxes, corresponding to the given solar modulation parameter and geomagnetic cutoff rigidity. The files have the structure similar to the input data files, described above.
