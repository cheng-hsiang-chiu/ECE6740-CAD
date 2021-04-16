#
# History
# -------------------------------------------------------------------------
# The programs listed herein are dependent on the BDD package CUDD-2.5.0
# provided herein, and developed by Fabio Zomenzi.
# 
# The packages bdd2add blif2bdd bdd2dot blif2dot were developed/modified by 
# Zheng from CUDD-2.3.?/nanotrav
#
#
# Update
# ------------------------------------------------------------------------
# simple installer for BLIF fascilities
# by Daniel F Gomez-Prado
# http//dgomezpr.com
# 2013
#
#
# This version of these packages simply port Zheng's code and its makefiles 
# from CUDD-2.3.X to CUDD-2.5.0
# 
#---------------------------------------------------------------------------

SUBDIRS = cudd-2.5.0 blif2bdd blif2dot

SUBDIRS_CLEAN = $(SUBDIRS:%=clean-%)

SUBDIRS_DISTCLEAN = $(SUBDIRS:%=distclean-%)

.PHONY : all $(SUBDIRS)
.PHONY : clean
.PHONY : distclean


# 
# release version
#
all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) --directory $@

# 
# clean 
#
clean: $(SUBDIRS_CLEAN)


$(SUBDIRS_CLEAN): 
	$(MAKE) --directory $(@:clean-%=%) clean

# 
# clean distribution
#
distclean: $(SUBDIRS_DISTCLEAN)

$(SUBDIRS_DISTCLEAN): 
	$(MAKE) --directory $(@:distclean-%=%) distclean


