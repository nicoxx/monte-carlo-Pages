#include "Cdo.h"
#include <assert.h>
#include <algorithm>
#include <functional>

Cdo::Cdo(double k1, double k2, const std::vector<double>& spreadPaimentDates)
: _k1(k1), _k2(k2), _spreadPaimentDates(spreadPaimentDates)
{
	assert(k1 >=0 && k1 <= 1);
	assert(k2 >= 0 && k2 <= 1);
	std::is_sorted(spreadPaimentDates.begin(), spreadPaimentDates.end());

}


Cdo::~Cdo()
{
}

double Cdo::computeSpread(const std::vector<double>& expectedLosses, double rate) const
{
	assert(expectedLosses.size() == _spreadPaimentDates.size());

	auto first = expectedLosses.begin();
	auto end = expectedLosses.end();

	double numerator = 0;
	double denominator = 0;

	int dateIndex = 0;
	for (auto it = first; it < end; ++it, ++dateIndex)
	{
		double previousLost = it == first ? 0 : *(it - 1);
		double previousDate = it == first ? 0 : _spreadPaimentDates[dateIndex - 1];
		double discountFactor = exp(-rate*_spreadPaimentDates[dateIndex]);

		numerator += (*it - previousLost)*discountFactor;
		denominator += (_spreadPaimentDates[dateIndex] - previousDate)*(1 - *it)*discountFactor;
	}
	return numerator/denominator;
}


double Cdo::computeLoss(double loss) const
{
	return std::max(std::min(loss, _k2)-_k1,0.)/(_k2-_k1);
}