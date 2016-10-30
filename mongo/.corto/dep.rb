require 'rake/clean'

# Clobber generated header files

CLOBBER.include("include/Connector.h")
CLOBBER.include(".corto/dep.rb")
