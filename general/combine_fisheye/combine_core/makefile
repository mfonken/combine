# Make with - make
# If path is missing, run - export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
# Run dtrace with (work in progress) - sudo dtrace -qs tau.d -c ./tau

CXX = g++
D_GEN = -MMD -MP
CFLAGS = -g -Wall -x c
CXXFLAGS = -g -Wall -x c++

KIN_CSOURCES = kinetic/kinetic_types.h kinetic/kinetic.c kinetic/kinetic.h kinetic/matrix.c kinetic/matrix.h
SEN_CSOURCES = sensors/LSM9DS1_regs.c sensors/LSM9DS1_regs.h sensors/LSM9DS1_types.h sensors/LSM9DS1.c sensors/LSM9DS1.h
UTI_CSOURCES = utilities/i2c_sp.c utilities/i2c_sp.h
TES_CSOURCES = stubs/nonpi_stubs.h

TAU_CSOURCES = tau+/gaussian.c tau+/gaussian.h tau+/global_types.h tau+/kalman.c tau+/kalman.h tau+/kmat.c tau+/kmat.h tau+/kmat_print.h tau+/rho.c tau+/rho.h tau+/sigma.c tau+/sigma.h tau+/statem.c tau+/statem.h tau+/tau.c tau+/tau.h tau+/test_setup.h
TAU_CXXSOURCES = main.cpp tau+/tau_test.cpp tau+/tau_test.hpp tau+/utility_functions.cpp tau+/utility_functions.hpp tau+/image_utility.cpp tau+/image_utility.hpp

CSOURCES = $(KIN_CSOURCES) $(SEN_CSOURCES) $(UTI_CSOURCES) $(TAU_CSOURCES) $(TES_CSOURCES)
CXXSOURCES = $(TAU_CXXSOURCES)

DEPS := $(CSOURCES:.o=.d)
DEPS += $(CXXSOURCES:.o=.d)

NAME = combine

OPENCV = `pkg-config opencv --cflags --libs`

LIBS = $(OPENCV)

$(NAME):$(CXXSOURCES) $(CSOURCES)
	$(CXX) -w $(D_GEN) $(CFLAGS) $(CSOURCES) $(CXXFLAGS) $(CXXSOURCES) -o $(NAME) $(LIBS)

.PHONY: clean
clean: ; $(RM) $(NAME) $(CXXSOURCES) $(CSOURCES) $(DEPS)