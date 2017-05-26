#ifndef plot_h
#define plot_h

#include <Plotter.h>

class Plot
{
	private:
		Plotter plotter;
		const bool enable;

		double max1 = 1;
		double min1 = -1;
		double minZero = 0;

	public:
		double val = 0;
		double smoothed = 0;
		double smoothedRel = 0;
		double avg = 0;
		double std = 0;

		Plot(bool enable);
		void plot();
};

#endif
