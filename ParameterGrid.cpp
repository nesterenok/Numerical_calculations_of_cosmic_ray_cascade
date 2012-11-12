
#include "ParameterGrid.h"
#include "PrimaryCosmicRaySpectrum.h"

parameter_interval::parameter_interval(string str, double e_min, double e_max, int no_p)
: en_min(e_min), en_max(e_max)
{
	file_data f_d;

	f_d.str = str;
	f_d.no_p = no_p;
	
	fdata = f_d;
	d_en = en_max - en_min;
}

parameter_grid::parameter_grid(cosmic_ray_particle *p)
: particle(p)
{;}

parameter_grid::~parameter_grid()
{ data.clear(); }
