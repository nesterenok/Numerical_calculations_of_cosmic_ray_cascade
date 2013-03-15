# --------------------------------------------------------------
# GNUmakefile.
# --------------------------------------------------------------

CXX = g++ -ggdb3
CFLAGS=-I$(IDIR)

target = ParticleSpectraCalculation

objects = ParticleSpectraCalculation.o PrimaryCosmicRaySpectrum.o ParameterGrid.o

$(target) : $(objects)
	$(CXX) -o $(target) $(objects)

ParticleSpectraCalculation.o : PrimaryCosmicRaySpectrum.h ParameterGrid.h Constants.h Utils.h

PrimaryCosmicRaySpectrum.o : PrimaryCosmicRaySpectrum.h PrimaryParticle.h

ParameterGrid.o : ParameterGrid.h PrimaryParticle.h

.PHONY : clean
clean :
	rm $(target) $(objects)