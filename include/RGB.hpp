/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** RGB.hpp
*/

#ifndef RGB_HPP
#define RGB_HPP

#include <sys/types.h>
#include "Maybe.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>

class RGB {
public:
    unsigned int R = 0;
    unsigned int G = 0;
    unsigned int B = 0;

    RGB() = default;
    RGB(unsigned int R, unsigned int G, unsigned int B) : R(R), G(G), B(B) {}
    RGB(const RGB& other) = default;
    RGB(RGB&& other) = default;
    ~RGB() = default;

    RGB& operator=(const RGB& other) = default;
    RGB& operator=(RGB&& other) = default;

    bool operator==(const RGB& other) const
    {
        return R == other.R && G == other.G && B == other.B;
    }
    bool operator!=(const RGB& other) const
    {
        return R != other.R || G != other.G || B != other.B;
    }

    RGB operator+(const RGB& other) const
    {
        return RGB{static_cast<unsigned int>(R + other.R),
            static_cast<unsigned int>(G + other.G),
            static_cast<unsigned int>(B + other.B)};
    }
    RGB operator+=(const RGB& other)
    {
        R += other.R;
        G += other.G;
        B += other.B;
        return *this;
    }
    RGB operator+(double val) const
    {
        return RGB{static_cast<unsigned int>(R + val),
            static_cast<unsigned int>(G + val),
            static_cast<unsigned int>(B + val)};
    }
    RGB operator+=(double val)
    {
        R += val;
        G += val;
        B += val;
        return *this;
    }
    RGB operator-(double val) const
    {
        return RGB{static_cast<unsigned int>(R - val),
            static_cast<unsigned int>(G - val),
            static_cast<unsigned int>(B - val)};
    }
    RGB operator-=(double val)
    {
        R -= val;
        G -= val;
        B -= val;
        return *this;
    }
    RGB operator-(const RGB& other) const
    {
        return RGB{static_cast<unsigned int>(R - other.R),
            static_cast<unsigned int>(G - other.G),
            static_cast<unsigned int>(B - other.B)};
    }
    RGB operator-=(const RGB& other)
    {
        R -= other.R;
        G -= other.G;
        B -= other.B;
        return *this;
    }
    RGB operator*(const RGB& other) const
    {
        return RGB{(R * other.R),
            (G * other.G),
            (B * other.B)};
    }
    RGB operator*=(const RGB& other)
    {
        R *= other.R;
        G *= other.G;
        B *= other.B;
        return *this;
    }
    RGB operator*(double val) const
    {
        return RGB{static_cast<unsigned int>(R * val),
            static_cast<unsigned int>(G * val),
            static_cast<unsigned int>(B * val)};
    }
    RGB operator*=(double val)
    {
        R *= val;
        G *= val;
        B *= val;
        return *this;
    }
    RGB operator/(const RGB& other) const
    {
        return RGB{static_cast<unsigned int>(R / other.R),
            static_cast<unsigned int>(G / other.G),
            static_cast<unsigned int>(B / other.B)};
    }
    RGB& operator/=(const RGB& other)
    {
        R /= other.R;
        G /= other.G;
        B /= other.B;
        return *this;
    }
    RGB operator/(double val) const
    {
        return RGB{static_cast<unsigned int>(R / val),
            static_cast<unsigned int>(G / val),
            static_cast<unsigned int>(B / val)};
    }
    RGB& operator/=(double val)
    {
        R /= val;
        G /= val;
        B /= val;
        return *this;
    }


    static Maybe<unsigned int> parseSingleAttr(char attrName, std::string str)
    {
        unsigned int res;

        if (str[0] != attrName)
            return Maybe<unsigned int>{};
        if (str.find(':') == std::string::npos)
            return Maybe<unsigned int>{};
        str.erase(0, str.find(':') + 1);

        try {
            res = static_cast<unsigned int>(std::stod(str));
        } catch (std::invalid_argument&) {
            return Maybe<unsigned int>{};
        }
        return Maybe{res};
    }

    [[nodiscard]] static Maybe<RGB> fromStr(std::string str)
    {

        if (str[0] != '{' || str[str.size() - 1] != '}')
            return Maybe<RGB>{};
        str.erase(0, 1);
        str.erase(str.size() - 1, 1);
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

        Maybe<unsigned int> maybeNum1 = parseSingleAttr('r', str);
        if (!maybeNum1.has_value())
            return Maybe<RGB>{};

        str.erase(0, str.find(';') + 1);
        Maybe<unsigned int> maybeNum2 = parseSingleAttr('g', str);
        if (!maybeNum2.has_value())
            return Maybe<RGB>{};

        str.erase(0, str.find(';') + 1);
        Maybe<unsigned int> maybeNum3 = parseSingleAttr('b', str);
        if (!maybeNum3.has_value())
            return Maybe<RGB>{};

        return Maybe{RGB{maybeNum1.value(), maybeNum2.value(), maybeNum3.value()}};
    }
};

#endif
