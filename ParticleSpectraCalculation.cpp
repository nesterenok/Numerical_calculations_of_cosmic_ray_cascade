// ParticleSpectraCalculation.cpp: определяет точку входа для консольного приложения.
//

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "math.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

#include "Constants.h"
#include "Utils.h"
#include "PrimaryCosmicRaySpectrum.h"
#include "ParameterGrid.h"

using namespace std;

void read_input_file(std::string &input_data_path, std::string &output_data_path, double &sol_mod_par, double &cutoff_rigidity);

void read_id_file(const std::string &input_data_path, parameter_grid*);

void make_spectrum(const std::string &input_data_path, const std::string &output_data_path, const std::string &base_name, double cutoff_rigidity, double sol_mod_par, 
				   primary_particle_list *particle_list, cumulative_spectrum **cum_sp, parameter_grid	**p_grid);

void read_input_file(string &input_data_path, string &output_data_path, double &sol_mod_par, double &cutoff_rigidity)
{
	const int num = 80;
	char str[num];
	
	ifstream input("input.txt");
	
	if (input) 
	{
		input.getline(str, num-1);
		input >> input_data_path >> str;

		input.getline(str, num-1);
		input >> output_data_path >> str;

		input.getline(str, num-1);
		input >> sol_mod_par >> str;

		input.getline(str, num-1);
		input >> cutoff_rigidity >> str;

		input.close();
	}
}

int main()
{
	int no_of_prim;
	double sol_mod_par = 650*MeV;
	double cutoff_rigidity = 0.;

	string input_data_path, output_data_path, file_name;
	
	cosmic_proton				*proton;
	cosmic_helium				*helium;
	primary_particle_list		*particle_list;
	
	parameter_grid				*proton_grid = 0, *alpha_grid = 0, **p_grid = 0;
	loc_interst_spect			*p_lis = 0, *a_lis = 0; 
	modulated_proton_spectrum	*p_spectrum = 0;
	modulated_alpha_spectrum	*a_spectrum = 0;
	primary_spectrum			*atic_p = 0, *atic_a = 0;
	cumulative_spectrum			*cum_proton_spectrum = 0, *cum_alpha_spectrum = 0, **cum_spectrum = 0;
	
	proton = new cosmic_proton();
	helium = new cosmic_helium();

	particle_list = new primary_particle_list();
	particle_list->add_particle(proton);
	particle_list->add_particle(helium);
	
	read_input_file(input_data_path, output_data_path, sol_mod_par, cutoff_rigidity);
	
	p_lis = new us_proton_lis();
//	p_lis = new webber09_proton_lis();
//	p_lis = new webber03_proton_lis();

	p_spectrum = new modulated_proton_spectrum(100*MeV, 100*GeV);
	p_spectrum->lis = p_lis;
	atic_p = new atic_proton_spectrum(100*GeV, 10000*GeV);
	
	cum_proton_spectrum = new cumulative_spectrum(proton);
	cum_proton_spectrum->add_spectrum(p_spectrum);
	cum_proton_spectrum->add_spectrum(atic_p);

	a_lis = new us_alpha_lis();
//	a_lis = new webber09_alpha_lis();
//	a_lis = new webber03_alpha_lis();

	a_spectrum = new modulated_alpha_spectrum(100*MeV, 25*GeV);
	a_spectrum->lis = a_lis;
	atic_a = new atic_alpha_spectrum(25*GeV, 10000*GeV);

	cum_alpha_spectrum = new cumulative_spectrum(helium);
	cum_alpha_spectrum->add_spectrum(a_spectrum);
	cum_alpha_spectrum->add_spectrum(atic_a);
	
	proton_grid = new parameter_grid(proton);
	read_id_file(input_data_path, proton_grid);
	
	alpha_grid = new parameter_grid(helium);
	read_id_file(input_data_path, alpha_grid);
	
	no_of_prim = (int) particle_list->data.size();
	
	cum_spectrum = new cumulative_spectrum* [no_of_prim];
	cum_spectrum[0] = cum_proton_spectrum;
	cum_spectrum[1] = cum_alpha_spectrum;
	
	p_grid = new parameter_grid* [no_of_prim];
	p_grid[0] = proton_grid;
	p_grid[1] = alpha_grid;

	make_spectrum(input_data_path, output_data_path, NEUTRON_BASENAME, cutoff_rigidity, sol_mod_par, 
		particle_list, cum_spectrum, p_grid);
	
	make_spectrum(input_data_path, output_data_path, PROTON_BASENAME, cutoff_rigidity, sol_mod_par, 
		particle_list, cum_spectrum, p_grid);
	
	delete proton;
	delete helium;
	delete particle_list;

	delete p_lis;
	delete a_lis;
	delete p_spectrum;
	delete a_spectrum;
	delete atic_p;
	delete atic_a;

	delete cum_proton_spectrum;
	delete cum_alpha_spectrum;
	delete [] cum_spectrum;
	
	delete proton_grid;
	delete alpha_grid;
	delete [] p_grid;
	
	return 0;
}

void read_id_file(const std::string &input_data_path, parameter_grid *p_grid)
{
	int i, no, no_particles;
	double energy_min, energy_max;

	string file_name, str;
	ifstream infile;
	parameter_interval	*p_i_temp = 0;
	
	p_grid->data.clear();

	file_name = input_data_path + p_grid->particle->id;
	file_name += PATH_SLASH;
	file_name += "id_file";
	file_name += p_grid->particle->id;
	file_name += ".txt";
	
	infile.open(file_name.c_str(), std::ios_base::in);

	if (infile.is_open())
	{
		infile >> no;
		for (i = 0; i < no; i++)
		{
			infile >> str >> energy_min >> energy_max  >> no_particles;
			
			energy_min *= GeV;
			energy_max *= GeV;

			if (p_i_temp != 0) delete p_i_temp;
			p_i_temp = new parameter_interval(str, energy_min, energy_max, no_particles);
			
			p_grid->data.push_back(*p_i_temp);
		}
		infile.close();
		cout << " > id file has been read " << endl;
	}
	else {
		cout << "\n error: can't open id_file" << endl;
		exit(1);
	}

	delete p_i_temp;
}

void make_spectrum(const std::string &input_data_path, const std::string &output_data_path, const std::string &base_name, double cutoff_rigidity, double sol_mod_par, 
				   primary_particle_list *particle_list, cumulative_spectrum **cum_spectrum, parameter_grid **p_grid)
{
	int	i, p, row, col, no_cols, no_rows, no_particles;
	double val, d_val, factor, primary_flux;
	double energy_min, energy_max;
	double **temp_array = 0;

	char id[6];
	string file_name, str;
	ifstream infile;
	ofstream outfile;

	for (p = 0; p < (int) particle_list->data.size(); p++)
	{
		if (particle_list->data[p]->id == HELIUM_ID) factor = 1.4;
		else factor = 1.;

		for (i = 0; i < (int) p_grid[p]->data.size(); i++)
		{
			primary_flux = cum_spectrum[p]->get_area_flux(cutoff_rigidity, sol_mod_par, 0., 1., p_grid[p]->data[i].en_min, p_grid[p]->data[i].en_max);  
			str = p_grid[p]->data[i].fdata.str;

			file_name = input_data_path + particle_list->data[p]->id;
			file_name += PATH_SLASH;
			file_name += base_name;
			file_name += str;
			file_name += ".txt";
			
			infile.open(file_name.c_str(), std::ios_base::in);
			infile.seekg(0, std::ios::beg);
				
			infile >> str >> energy_min >> energy_max >> no_particles ;
			infile >> no_rows >> no_cols;
			
			if (temp_array == 0) 
			{
				temp_array = alloc_2d_array<double>(no_rows, no_cols);
				memset(*temp_array, 0, no_rows*no_cols*sizeof(double));
			}

			for (col = 0; col < no_cols; col++) {
				infile >> temp_array[0][col];
			}

			for (row = 1; row < no_rows; row++)
			{
				infile >> val >> d_val;
				if (i == 0) 
				{
					temp_array[row][0] = val;
					temp_array[row][1] = d_val;
				}

				for (col = 2; col < no_cols; col += 2)
				{
					infile >> val >> d_val;
					
					temp_array[row][col] += val *primary_flux *factor;
					d_val *= primary_flux *factor;
					temp_array[row][col+1] += d_val*d_val;
				}
			}
			infile.close();
		}
	}

	for (row = 1; row < no_rows; row++) 
	{
		for (col = 2; col < no_cols; col += 2)
		{
			temp_array[row][col+1] = sqrt(temp_array[row][col+1]);
		}
	}
	
	i = (int) (sol_mod_par);
	sprintf(id, "%d", i);

	file_name = output_data_path + "sm";
	file_name += id;
	file_name += "_";

	i = (int) (cutoff_rigidity);
	sprintf(id, "%d", i);

	file_name += "rg";
	file_name += id;
	file_name += "_";

	file_name += base_name;
	file_name += ".txt";
	
	outfile.open(file_name.c_str(), std::ios_base::out | std::ios_base::trunc);
	outfile.seekp(0, std::ios::beg);

	outfile.setf(std::ios::scientific);
	outfile.width(10);

	outfile << no_rows << "	" << no_cols << endl;
	for (row = 0; row < no_rows; row++)
	{
		for (col = 0; col < no_cols; col += 2)
		{
			outfile.precision(4);
			outfile << temp_array[row][col] << "	";
			
			if (row > 0 && col > 0) outfile.precision(1);
			outfile << temp_array[row][col+1] << "	";
		}
		outfile << endl;
	}
	outfile.close();

	cout << " > " << base_name << " - spectrum is made" << endl;
	free_2d_array<double>(temp_array);
}
