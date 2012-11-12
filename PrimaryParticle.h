#pragma once

#include <string>
#include "Constants.h"

using namespace std;

class cosmic_ray_particle
{
public:
	double mass, mass_no, charge;
	string id;
};

class cosmic_proton : public cosmic_ray_particle
{
public:
	cosmic_proton() {mass_no = 1; mass = proton_mass_energy; charge = 1.; id = PROTON_ID;}
};

class cosmic_helium : public cosmic_ray_particle
{
public:
	cosmic_helium() {mass_no = 4; mass = mass_no*proton_mass_energy; charge = 2.; id = HELIUM_ID;}
};

class primary_particle_list
{
public:
	vector<cosmic_ray_particle*> data;
	void add_particle(cosmic_ray_particle *crp) {data.push_back(crp);}
};