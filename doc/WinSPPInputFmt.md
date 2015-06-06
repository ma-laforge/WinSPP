# WinSPP Input File Format
	["Smith" or "Transmission"]
	DataSets [# data sets]

	[Dataset #1 - see next subsection]

	[Dataset #2]

	[...]

	[Dataset #N]

## Dataset Input Format [Dataset #i]
<pre><code>
	Description [Description of data set]
	TraceLine ["Yes" or "No"]
	LineWidth [Desired line width (in mm)<sup>1</sup>]
	LineColor RGB [Level of red &isin; [0,255]] [Level of green &isin; [0,255]] [Level of blue &isin; [0,255]]
	Symbol ["Circle" or "Square" or "Triangle" or "None"<sup>2</sup>]
	SymbolSize [Desired symbol<sup>3</sup>]
	SymbolFill ["Yes" or "No"<sup>4</sup>]
	FillColor RGB [Level of red &isin; [0,255]] [Level of green &isin; [0,255]] [Level of blue &isin; [0,255]]
	SymbolSkip [# of symbols to drop]

	DataPoints [# of data points in current data set]
	[real(Sij) @ freq/bias point 1] [imag(Sij) @ freq/bias point 1]
	[real(Sij) @ freq/bias point 2] [imag(Sij) @ freq/bias point 2]
	[...]
	[real(Sij) @ freq/bias point N] [imag(Sij) @ freq/bias point N]
</code></pre>

NOTE:

- <sup>1</sup>Entire image is 100mm&times;100mm.
- <sup>2</sup>If "Symbol" is set to "None", lines "SymbolSize", "SymbolFill", "FillColor", and "SymbolSkip" must *not* be included.
- <sup>3</sup>"SymbolSize" is defined in 1/100th of the reference impedance (real(z)=1) circle.
- <sup>4</sup>If "SymbolFill" is set to "No", line "SymbolSkip" must *not* be included.
- "LineWidth" & "SymbolSize" are not based on same units... sorry about that.

