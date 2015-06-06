// WinSPPcl.cpp : Main section of WinSPP command-line.
//
#include <tools/CExtensions.h>
#include <tools/ContainerExtensions.h>
#include <plotting/SPPlot.h>
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>

using std::string;
using std::cout;
using std::cerr;

void GenerateSPPlot(std::istream &plotinfo, string destEmfPath, bool compensateSObug = false) {
	SParamPlots::Plot plot(compensateSObug);
	plot.DrawFromStream(plotinfo);
	plot.Save(destEmfPath);
} //GenerateSPPlot

void ShowHelp(string &progName) {
	cout << progName << ": Generates S-Parameter/Polar plot .emf files.\n";
	cout << "\n";
	cout << "Example:\n";
	cout << "   " << progName << " [OPTIONS] [OUTPUT_FILE.emf] < [INPUT_FILE.plot]\n";
	cout << "\n";
	cout << "OPTIONS:\n";
	cout << "   -h: Show this help.\n";
	cout << "   -fixso: Compensates for StarOffice EMF text angle bug.\n";
} //ShowHelp

int _tmain(int argc, _TCHAR* argv[])
{
	using StdExt::IsElementOf;
	typedef std::set<string> StringSet;
	typedef std::vector<string> StringV;
	StringSet switches = StdExt::GetSwitches(argc, argv);
	StringV args = StdExt::GetArguments(argc, argv, false);
	int returnCode = 0;

	if (IsElementOf(switches, string("h")))
		ShowHelp(args.at(0));
	else if (args.size() != 2) {
		cerr << "ERROR: Wrong number of arguments\n";
		cerr << "\n";
		ShowHelp(args.at(0));
	} else {
		bool compensateSObug = false;
		string destEmfPath = args.at(1);

		if (IsElementOf(switches, string("fixso")))
			compensateSObug = true;

		try {
			GenerateSPPlot(std::cin, destEmfPath, compensateSObug);
		}
		catch (std::exception &e) {
			cerr << e.what();
			cerr << "\n\n";
			cerr << "ERROR: Could not save file: " << destEmfPath << "\n";
			returnCode = 1;
		}
	}

	return returnCode;
} //_tmain