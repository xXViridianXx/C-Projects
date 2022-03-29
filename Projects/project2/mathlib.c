#include "mathlib.h"

#include <stdio.h>
double range(double x) { /*used to shift range of sin and cos from 0-2pi*/
    int sum;
    double pi = 3.141592653589793;
    sum = (x / (2 * pi));
    x = x - (2 * pi * sum);
    return x;
}

double Exp(double x) {
    double trm = 1;
    double sum = trm;
    int k = 1;
    while (trm > EPSILON) { /*1e-14 so that value doesn't equal 0*/
        trm *= Abs(x) / k;
        sum += trm;
        k += 1;
    }
    return x > 0 ? sum : 1 / sum;
}

double Sin(double x) {
    double s, v, t, k;
    x = range(x); /*used my range function to make big values smaller*/
    s = 1.0;
    v = x;
    t = x;
    k = 3.0;
    while (Abs(t) > EPSILON) {
        t = t * (x * x) / ((k - 1) * k); /*factorial equation so don't have to compute every time*/
        s = -s;
        v += s * t;
        k += 2.0;
    }
    return v;
}

double Cos(double x) {
    double s, v, t, k;
    x = range(x);
    s = -1.0;
    v = 1;
    k = 2.0;
    t = x;
    t = (t * x) / ((k - 1) * k);
    v += s * t;
    k += 2.0;
    while (Abs(t) > EPSILON) {
        t = t * (x * x) / ((k - 1) * k);
        s = -s;
        v += s * t;
        k += 2.0;
    }
    return v;
}

double Sqrt(double x) {
    double z = 0.0;
    double y = 1.0;
    double f = 1.0;
    while (Abs(y - z) > EPSILON) {
        while (x > 1) {
            x /= 4; /*factored out 4 so large numbers could bd calculated*/
            f *= 2;
        }
        z = y;
        y = 0.5 * (z + x / z);
    }
    y = f * y; /*multiplied the factored out 4's which became 2's*/
    return y;
}

double Log(double x) {
    double y = 1.0;
    double p = Exp(y);
    double f = 0.0;
    double e = Exp(1.0);

    while (Abs(p - x) > EPSILON) {
        while (x > e) {
            x /= e;
            f += 1;
        }
        y = y + x / p - 1;
        p = Exp(y);
    }
    y = f + y;
    return y;
}

double integrate(double (*f)(double x), double a, double b, uint32_t n) {
    double zero = 1e-60; /*if function took in zero, replace with 1e-60 so that no zero error*/
    if (a == 0) {
        a = zero; /*1e-16 taken from lecture*/
    }
    if (b == 0) {
        b = zero;
    }
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    for (uint32_t i = 1; i < n; i++) {
        if ((a + i * h) == 0) { /*checks if 0 when incremented bound*/
            if (i % 2 == 1) {
                sum += 4 * f(zero);
            } else {
                sum += 2 * f(zero);
            }
        } else {
            if (i % 2 == 1) {
                sum += 4 * f(a + i * h);
            } else {
                sum += 2 * f(a + i * h);
            }
        }
    }
    sum *= (h / 3);
    return sum;
}
