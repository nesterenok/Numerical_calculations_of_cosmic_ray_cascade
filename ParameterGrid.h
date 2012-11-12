#include <vector>
#include <string>

#include "Constants.h"
#include "PrimaryParticle.h"

struct file_data 
{
public:

	int no_p;
	string str;
};

class parameter_interval
{
public:

	double	en_min, en_max, d_en;
	file_data fdata;

	parameter_interval(string str, double en_min, double en_max, int no_particles = 0);
};


class parameter_grid
{
public:
	cosmic_ray_particle			*particle;
	vector<parameter_interval>	data;
	
	parameter_grid(cosmic_ray_particle*);
	~parameter_grid();
};

