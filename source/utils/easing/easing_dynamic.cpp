
#include "easing_dynamic.hpp"

#include <cmath>
#include <map>

namespace cur = easing;

#ifndef PI
#define PI 3.1415926545
#endif

double easeInSine(double t) {
	return sin(1.5707963 * t);
}

double easeOutSine(double t) {
	return 1.0 + sin(1.5707963 * (--t));
}

double easeInOutSine(double t) {
	return 0.5 * (1.0 + sin(3.1415926 * (t - 0.5)));
}

double easeInQuad(double t) {
    return t * t;
}

double easeOutQuad(double t) { 
    return t * (2.0 - t);
}

double easeInOutQuad(double t) {
    return t < 0.5 ? 2.0 * t * t : t * (4.0 - 2.0 * t) - 1.0;
}

double easeInCubic(double t) {
    return t * t * t;
}

double easeOutCubic(double t) {
    return 1.0 + (--t) * t * t;
}

double easeInOutCubic(double t) {
    return t < 0.5 ? 4.0 * t * t * t : 1.0 + (--t) * (2.0 * (--t)) * (2.0 * t);
}

double easeInQuart(double t) {
    t *= t;
    return t * t;
}

double easeOutQuart(double t) {
    t = (--t) * t;
    return 1.0 - t * t;
}

double easeInOutQuart(double t) {
    if(t < 0.5) {
        t *= t;
        return 8.0 * t * t;
    } else {
        t = (--t) * t;
        return 1.0 - 8.0 * t * t;
    }
}

double easeInQuint(double t) {
    auto t2 = t * t;
    return t * t2 * t2;
}

double easeOutQuint(double t) {
    auto t2 = (--t) * t;
    return 1.0 + t * t2 * t2;
}

double easeInOutQuint(double t) {
    auto t2 = double{ 0.0 };
    if(t < 0.5) {
        t2 = t * t;
        return 16.0 * t * t2 * t2;
    } else {
        t2 = (--t) * t;
        return 1.0 + 16.0 * t * t2 * t2;
    }
}

double easeInExpo(double t) {
    return (pow(2.0, 8.0 * t) - 1.0) / 255.0;
}

double easeOutExpo(double t) {
    return 1.0 - pow(2.0, -8.0 * t);
}

double easeInOutExpo(double t) {
    if(t < 0.5) {
        return (pow(2.0, 16.0 * t) - 1.0) / 510.0;
    } else {
        return 1.0 - 0.5 * pow(2.0, -16.0 * (t - 0.5));
    }
}

double easeInCirc(double t) {
    return 1.0 - sqrt(1.0 - t);
}

double easeOutCirc(double t) {
    return sqrt(t);
}

double easeInOutCirc(double t) {
    if(t < 0.5) {
        return (1.0 - sqrt(1.0 - 2.0 * t)) * 0.5;
    } else {
        return (1.0 +sqrt(2.0 * t - 1.0)) * 0.5;
    }
}

double easeInBack(double t) {
    return t * t * (2.70158 * t - 1.70158);
}

double easeOutBack(double t) {
    return 1.0 + (--t) * t * (2.70158 * t + 1.70158);
}

double easeInOutBack(double t) {
    if(t < 0.5) {
        return t * t * (7.0 * t - 2.5) * 2.0;
    } else {
        return 1.0 + (--t) * t * 2.0 * (7.0 * t + 2.5);
    }
}

double easeInElastic(double t) {
    auto t2 = t * t;
    return t2 * t2 * sin(t * PI * 4.5);
}

double easeOutElastic(double t) {
    auto t2 = (t - 1.0) * (t - 1.0);
    return 1.0 - t2 * t2 * cos(t * PI * 4.5);
}

double easeInOutElastic(double t) {
    auto t2 = double{ 0.0 };
    if(t < 0.45) {
        t2 = t * t;
        return 8.0 * t2 * t2 * sin(t * PI * 9.0);
    } else if(t < 0.55) {
        return 0.5 + 0.75 * sin(t * PI * 4.0);
    } else {
        t2 = (t - 1.0) * (t - 1.0);
        return 1.0 - 8.0 * t2 * t2 * sin(t * PI * 9.0);
    }
}

double easeInBounce(double t) {
    return pow(2.0, 6.0 * (t - 1.0)) * abs(sin(t * PI * 3.5));
}

double easeOutBounce(double t) {
    return 1.0 - pow(2.0, -6.0 * t) * abs(cos(t * PI * 3.5));
}

double easeInOutBounce(double t) {
    if(t < 0.5) {
        return 8.0 * pow(2.0, 8.0 * (t - 1.0)) * abs(sin(t * PI * 7.0));
    } else {
        return 1.0 - 8.0 * pow(2.0, -8.0 * t) * abs(sin(t * PI * 7.0));
    }
}

cur::EasingFunction cur::getEasingFunction(cur::EasingFunctions function)
{
	static auto easingFunctions = std::map<cur::EasingFunctions, cur::EasingFunction>();
	if(easingFunctions.empty())
	{
		easingFunctions.insert(std::make_pair(EasingFunctions::inSine, 	        easeInSine));
		easingFunctions.insert(std::make_pair(EasingFunctions::outSine, 	    easeOutSine));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutSine, 	    easeInOutSine));
		easingFunctions.insert(std::make_pair(EasingFunctions::inQuad, 	        easeInQuad));
		easingFunctions.insert(std::make_pair(EasingFunctions::outQuad, 	    easeOutQuad));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutQuad,       easeInOutQuad));
		easingFunctions.insert(std::make_pair(EasingFunctions::inCubic, 	    easeInCubic));
		easingFunctions.insert(std::make_pair(EasingFunctions::outCubic, 	    easeOutCubic));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutCubic,      easeInOutCubic));
		easingFunctions.insert(std::make_pair(EasingFunctions::inQuart, 	    easeInQuart));
		easingFunctions.insert(std::make_pair(EasingFunctions::outQuart, 	    easeOutQuart));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutQuart,      easeInOutQuart));
		easingFunctions.insert(std::make_pair(EasingFunctions::inQuint, 	    easeInQuint));
		easingFunctions.insert(std::make_pair(EasingFunctions::outQuint, 	    easeOutQuint));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutQuint,      easeInOutQuint));
		easingFunctions.insert(std::make_pair(EasingFunctions::inExpo,          easeInExpo));
		easingFunctions.insert(std::make_pair(EasingFunctions::outExpo, 	    easeOutExpo));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutExpo,	    easeInOutExpo));
		easingFunctions.insert(std::make_pair(EasingFunctions::inCirc, 	        easeInCirc));
		easingFunctions.insert(std::make_pair(EasingFunctions::outCirc, 	    easeOutCirc));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutCirc,	    easeInOutCirc));
		easingFunctions.insert(std::make_pair(EasingFunctions::inBack, 	        easeInBack));
		easingFunctions.insert(std::make_pair(EasingFunctions::outBack, 	    easeOutBack));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutBack,	    easeInOutBack));
		easingFunctions.insert(std::make_pair(EasingFunctions::inElastic, 	    easeInElastic));
		easingFunctions.insert(std::make_pair(EasingFunctions::outElastic,      easeOutElastic));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutElastic,    easeInOutElastic));
		easingFunctions.insert(std::make_pair(EasingFunctions::inBounce, 	    easeInBounce));
		easingFunctions.insert(std::make_pair(EasingFunctions::outBounce, 	    easeOutBounce));
		easingFunctions.insert(std::make_pair(EasingFunctions::inOutBounce,     easeInOutBounce));

	}

	auto it = easingFunctions.find(function);
	return it == easingFunctions.end() ? nullptr : it->second;
}
