#include "plot.h"

Plot::Plot(bool enable) : enable(enable)
{
	if (enable)
	{
		plotter.Begin();
		/*plotter.AddTimeGraph("Sensor", 500,
			"x", x,
			"w", w
		);*/

		plotter.AddTimeGraph("Knobs", 500,
			//"A", knobA,
			//"B", knobB,
			"C", knobC
		);

		plotter.AddTimeGraph("Audio Sensor", 500,
			"val", val,
			"thresh", smoothed,
			//"smooth rel", smoothedRel,
			"avg", avg,
			"std", std
			//"min", minZero
		);
	}
}

void Plot::plot()
{
	if (enable)
		plotter.Plot();
}
