#!/usr/bin/env python
# -*- coding: utf-8 -*-

import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

import argparse
import sys

import Artus.Utility.tools as tools
import Excalibur.Plotting.harryZJet as harryZJet
import Excalibur.Plotting.utility.toolsZJet as toolsZJet

def main():
	"""This script calls the harry core or python plot functions."""
	if len(sys.argv) == 1:
		sys.argv.append("-h")

	# the try-except is needed so -h doesnt exit at the parsing here
	try:
		parser = argparse.ArgumentParser(description="Merlin, a plotting tool derived\
			from HarryPlotter. Please have a look at the documentation for Excalibur\
			(https://github.com/KIT-CMS/Excalibur/blob/master/README.md)\
			and for HarryPlotter (https://github.com/KIT-CMS/Artus/blob/master/HarryPlotter/README.md).")
		parser.add_argument('--python', nargs='+', default=[None],
			help="execute python function(s). Available functions can be listed with --list-functions")
		known_args, unknown_args = parser.parse_known_args()

		# call python config function
		if known_args.python != [None]:
			logger.initLogger()
			for function in known_args.python:
				toolsZJet.call_python_function(function, tools.get_environment_variable("PLOTCONFIGS"), unknown_args)
		else:
			harryZJet.HarryPlotterZJet(list_of_args_strings=" ".join(unknown_args))

	except SystemExit, e:
		if '-h' in sys.argv or '--help' in sys.argv:
			harryZJet.HarryPlotterZJet()


if __name__ == "__main__":
	main()
