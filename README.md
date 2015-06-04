# WinSPP
Generates publication-quality S-Parameter plots & polar plots.  Plot are saved as an enanced Windows metafile (.emf file).  They can therefore be resized with no loss of quality.

## Sample Output (Click image to access .emf file)
### Smith Plot
[![Smith Plot](doc/SampleSmith.plot.png)](results/SampleSmith.plot.emf)

### Smith Plot
[![Polar Plot](doc/SamplePolar.plot.png)](results/SamplePolar.plot.emf)

## Features
### Two tools
- WinSPP: Graphical interface to generate/preview plots.
- WinSPPbatch: Simple command-line tool.

### Compensate for StarOffice Bug
Certain versions of StarOffice/OpenOffice (/LibreOffice?) incorrectly interpret the enhanced metafile text angle by a factor of -1.  This generates very unpleasant plots.  WinSPP can compensate for this issue:

- Check "Compensate for StarOffice bug" in WinSPP (gui).
- Use -fixso switch in WinSPPbatch (command-line tool).

## Known Limitations
- Does not support plot titles
- Does not support legends

## Disclaimer
This tool was built quickly to satisfy a need for S-parameter plots.  It is very bare bones... but it seems to work.  Also, I have not yet seen a good alternative since it was written around 2005.

The implementation is built on the older 32-bit windows GDI interface.  As a result, the plot preview window is not displayed using anti-aliasing.
