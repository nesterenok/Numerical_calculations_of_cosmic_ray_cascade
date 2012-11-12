#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>

#include "PrimaryCosmicRaySpectrum.h"
#include "PrimaryParticle.h"

// The local interstellar spectrum for protons and helium according to 
// Usoskyn et al. (2005) and Burger et al. (2000)

double us_proton_lis::get_flux(double energy)
{
	//cm-2 MeV/nucl-1 s-1 sr-1
	double p = energy*(energy + 2*proton_mass_energy)/GeV/GeV;
	return 1.9e-3*pow(p, -2.78/2)/(1 + 0.4866*pow(p, -2.51/2));
}

double us_alpha_lis::get_flux(double energy)
{
	//cm-2 MeV/nucl-1 s-1 sr-1
	double p = energy*(energy +2*proton_mass_energy)/GeV/GeV;
	return 0.95e-4 *pow(p, -2.78/2)/(1 + 0.4866*pow(p, -2.51/2));
}

// The local interstellar spectrum for protons and helium according to Webber and Higbie (2009, 2010)
//

double webber09_proton_lis::get_flux(double energy)
{
	//cm-2 MeV/nucl-1 s-1 sr-1
	return 18.9 *1e-4 *pow(energy/GeV,-2.79)/(1 + 6.75*pow(energy/GeV, -1.22) 
		+ 1.30 *pow(energy/GeV, -2.80) + 0.0087 *pow(energy/GeV, -4.32));
}

double webber09_alpha_lis::get_flux(double energy)
{
	//cm-2 MeV/nucl-1 s-1 sr-1
	return 0.99 *1e-4 *pow(energy/GeV, -2.77)/(1 + 4.14*pow(energy/GeV, -1.09) 
		+ 0.65 *pow(energy/GeV, -2.79) + 0.0094*pow(energy/GeV, -4.2));
}

// The local interstellar spectrum for protons and helium according to Webber and Higbie (2003)
//

double webber03_proton_lis::get_flux(double energy)
{
	//cm-2 MeV/nucl-1 s-1 sr-1
	return 21.1 *1e-4 *pow(energy/GeV,-2.8)/(1 + 5.85*pow(energy/GeV, -1.22) 
		+ 1.18*pow(energy/GeV, -2.54));
}

double webber03_alpha_lis::get_flux(double energy)
{
	//cm-2 MeV/nucl-1 s-1 sr-1
	return 1.075 *1.e-4 *pow(energy/GeV, -2.8)/(1 + 3.91*pow(energy/GeV, -1.09) 
		+ 0.9 *pow(energy/GeV, -2.54));
}

primary_spectrum::primary_spectrum(double e_min, double e_max)
: en_min(e_min), en_max(e_max)
{;}

modulated_proton_spectrum::modulated_proton_spectrum(double e_min, double e_max) 
: primary_spectrum(e_min, e_max), lis(0)
{;}

double modulated_proton_spectrum::get_flux(double sol_mod_par, double energy)
{
	double flux = lis->get_flux(energy + sol_mod_par) *energy*(energy + 2*proton_mass_energy)
		/(energy + sol_mod_par)/(energy + 2*proton_mass_energy + sol_mod_par);

	return flux;
}

modulated_alpha_spectrum::modulated_alpha_spectrum(double e_min, double e_max) 
: primary_spectrum(e_min, e_max), lis(0)
{;}

double modulated_alpha_spectrum::get_flux(double sol_mod_par, double energy)
{
	double flux = lis->get_flux(energy+sol_mod_par/2.) *energy*(energy + 2*proton_mass_energy)
		/(energy + sol_mod_par/2.)/(energy + 2*proton_mass_energy + sol_mod_par/2.);

	return flux;
}

atic_proton_spectrum::atic_proton_spectrum(double en_min, double en_max) : primary_spectrum(en_min, en_max)
{;}

// The particle fluxes according to ATIC2 experiment Panov et al. (2009)
//

double atic_proton_spectrum::get_flux(double sol_mod_par, double energy)
{
// cm-2 (MeV/nucl)-1 s-1 sr-1
	double flux;
	if (energy < 3.1e+5) flux = 1.66e-3 *pow(energy/GeV, -2.79); // ATIC2
	else flux = 7.84e-4*pow(energy/GeV, -2.65);
	
	return flux;
}

atic_alpha_spectrum::atic_alpha_spectrum(double en_min, double en_max) : primary_spectrum(en_min, en_max)
{;}

double atic_alpha_spectrum::get_flux(double sol_mod_par, double energy)
{
// cm-2 (MeV/nucl)-1 s-1 sr-1
	double flux;
	if (energy < 3.e+5) flux = 5.0e-5 *pow(energy/GeV, -2.63); //ATIC2
	else flux = 3.1e-5*pow(energy/GeV, -2.53);
	return flux;
}

cumulative_spectrum::cumulative_spectrum(cosmic_ray_particle* p)
: particle(p)
{;}

void cumulative_spectrum::add_spectrum(primary_spectrum *prim_sp)
{
	if (spectra.size() == 0)
	{
		en_min = prim_sp->en_min;
		en_max = prim_sp->en_max;
	}
	else
	{
		if (en_min > prim_sp->en_min) en_min = prim_sp->en_min;
		if (en_max < prim_sp->en_max) en_max = prim_sp->en_max;
	}
	spectra.push_back(prim_sp);
}

double cumulative_spectrum::get_flux(double cutoff_rig, double sol_mod_par, double energy)
{
	bool is_found = false;
	int i;
	double rigidity, flux;

	rigidity = sqrt((energy + proton_mass_energy)*(energy + proton_mass_energy) - proton_mass_energy *proton_mass_energy) 
		*particle->mass_no/particle->charge;
	
	if (rigidity < cutoff_rig) return 0.;
	
	for (i = 0; (i < (int) spectra.size()) && (!is_found); i++)
	{
		if (spectra[i]->en_min < energy && spectra[i]->en_max > energy)
		{
			flux = spectra[i]->get_flux(sol_mod_par, energy);
			is_found = true;
		}
	}
	return flux;
}

double cumulative_spectrum::get_integral_flux(double cutoff_rig, double sol_mod_par, double cth_min, double cth_max, double en_min, double en_max)
{
	double en, incr = 1.01, flux = 0.;
	
	for (en = en_min; en < en_max; en *= incr) 
	{
		flux += get_flux(cutoff_rig, sol_mod_par, en*(incr/2+0.5)) *en*(incr-1);
	}
	return 2*M_PI*(cth_max - cth_min) *flux;
}

double cumulative_spectrum::get_area_flux(double cutoff_rig, double sol_mod_par, double cth_min, double cth_max, double en_min, double en_max)
{
	double en, incr = 1.01, flux = 0.;
	
	for (en = en_min; en < en_max; en *= incr) 
	{
		flux += get_flux(cutoff_rig, sol_mod_par, en*(incr/2+0.5)) *en*(incr-1);
	}
	return M_PI*(cth_max*cth_max - cth_min*cth_min) *flux;
}