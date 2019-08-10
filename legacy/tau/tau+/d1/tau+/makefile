# Make with - make
# If path is missing, run - export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
# Run dtrace with (work in progress) - sudo dtrace -qs tau.d -c ./tau

CXX = g++ -MMD -MP
CFLAGS = -g -Wall -x c
CXXFLAGS = -g -Wall -x c++


CSOURCES = gaussian.c gaussian.h global_types.h kalman.c kalman.h kmat.c kmat.h kmat_print.h rho.c rho.h sigma.c sigma.h statem.c statem.h tau.c tau.h test_setup.h
CXXSOURCES = main.cpp tau_test.cpp tau_test.hpp utility_functions.cpp utility_functions.hpp image_utility.cpp image_utility.hpp

DEPS := $(CSOURCES:.o=.d)
DEPS += $(CXXSOURCES:.o=.d)

NAME = tau

OPENCV = `pkg-config opencv --cflags --libs`

LIBS = $(OPENCV)

$(NAME):$(CXXSOURCES) $(CSOURCES)
	$(CXX) $(CFLAGS) $(CSOURCES) $(CXXFLAGS) $(CXXSOURCES) -o $(NAME) $(LIBS)

.PHONY: clean
clean: ; $(RM) $(NAME) $(CXXSOURCES) $(CSOURCES) $(DEPS)