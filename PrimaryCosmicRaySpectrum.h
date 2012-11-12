#pragma once

#include <string>
#include <vector>

#include "PrimaryParticle.h"

class loc_interst_spect
{
public:
	virtual double get_flux(double) = 0;
};

class us_proton_lis : public loc_interst_spect
{
public:
	double get_flux(double);
};

class us_alpha_lis : public loc_interst_spect
{
public:
	double get_flux(double);
};

class webber09_proton_lis : public loc_interst_spect
{
public:
	double get_flux(double);
};

class webber09_alpha_lis : public loc_interst_spect
{
public:
	double get_flux(double);
};

class webber03_proton_lis : public loc_interst_spect
{
public:
	double get_flux(double);
};

class webber03_alpha_lis : public loc_interst_spect
{
public:
	double get_flux(double);
};

class primary_spectrum
{
public:

	double en_min, en_max;
	virtual double get_flux(double, double) = 0;
	primary_spectrum(double, double);
};

class modulated_proton_spectrum : public primary_spectrum
{
public:

	loc_interst_spect *lis;
	double get_flux(double, double);
	modulated_proton_spectrum(double, double);
};

class modulated_alpha_spectrum : public primary_spectrum
{
public:

	loc_interst_spect *lis;
	double get_flux(double, double);
	modulated_alpha_spectrum(double, double);
};

class atic_proton_spectrum : public primary_spectrum
{
public:
	
	double get_flux(double, double);
	atic_proton_spectrum(double, double);
};

class atic_alpha_spectrum : public primary_spectrum
{
public:
	
	double get_flux(double, double);
	atic_alpha_spectrum(double, double);
};

class cumulative_spectrum 
{
public:
	
	double						en_min, en_max;
	cosmic_ray_particle			*particle;
	vector<primary_spectrum*>	spectra;
	
	double get_flux(double, double, double);
	double get_integral_flux(double, double, double, double, double, double);
	double get_area_flux(double, double, double, double, double, double);
	
	void add_spectrum(primary_spectrum*);
	cumulative_spectrum(cosmic_ray_particle*);
};
