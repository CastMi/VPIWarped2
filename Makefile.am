bin_PROGRAMS = circuit_sim

circuit_sim_SOURCES = logic.hpp logic.cpp circuit.cpp

circuit_sim_CPPFLAGS = -isystem$(top_srcdir)/deps/rnd -I$(top_srcdir)/deps

circuit_sim_LDADD=$(top_builddir)/deps/rnd/libRnd.la
