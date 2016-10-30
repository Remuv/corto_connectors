require 'rake/clean'

# Clobber generated header files

CLOBBER.include("include/Data.h")
CLOBBER.include("include/MySuite.h")
CLOBBER.include("include/Vec2.h")
CLOBBER.include(".corto/dep.rb")
