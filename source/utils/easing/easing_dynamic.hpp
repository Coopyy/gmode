
#ifndef EASING_EASING_DYNAMIC_HPP_INCLUDED
#define EASING_EASING_DYNAMIC_HPP_INCLUDED
#pragma once 

namespace easing
{
	enum class EasingFunctions
	{
		inSine,
		outSine,
		inOutSine,
		inQuad,
		outQuad,
		inOutQuad,
		inCubic,
		outCubic,
		inOutCubic,
		inQuart,
		outQuart,
		inOutQuart,
		inQuint,
		outQuint,
		inOutQuint,
		inExpo,
		outExpo,
		inOutExpo,
		inCirc,
		outCirc,
		inOutCirc,
		inBack,
		outBack,
		inOutBack,
		inElastic,
		outElastic,
		inOutElastic,
		inBounce,
		outBounce,
		inOutBounce,
	};

	typedef double(*EasingFunction)(double);

	EasingFunction getEasingFunction(EasingFunctions function);
}
#endif // INCLUDE_GUARD
