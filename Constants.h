#pragma once

#define PROTON_ID "_p_"
#define HELIUM_ID "_a_"

#define NEUTRON_BASENAME	"neutron"
#define PROTON_BASENAME		"proton"

#ifdef __linux__
#define PATH_SLASH "/"
#elif defined _WIN32 || defined _WIN64 
#define PATH_SLASH "\\"
#endif

const double MeV = 1.;
const double GeV = 1000*MeV;
const double TeV = 1000*GeV;
const double proton_mass_energy = 938.3*MeV;
