/*
** EPITECH PROJECT, 2024
** raytracer
** File description:
** Maybe.hpp
*/

#ifndef MAYBE_HPP
#define MAYBE_HPP

template<typename T>
class Maybe {
    bool _hasValue;
    T _value;
public:
    Maybe() : _hasValue(false) {}
    explicit Maybe(T value) : _hasValue(true), _value(value) {}
    Maybe(const Maybe& other) : _hasValue(other._hasValue), _value(other._value) {}
    Maybe(Maybe&& other) noexcept : _hasValue(other._hasValue), _value(other._value) { other._hasValue = false; }
    ~Maybe() = default;

    Maybe& operator=(const Maybe& other) {_hasValue = other._hasValue; _value = other._value; return *this;}
    Maybe& operator=(Maybe&& other) noexcept
    {
        _hasValue = other._hasValue;
        _value = other._value;
        other._hasValue = false;
        return *this;
    }

    [[nodiscard]] bool has_value() const { return _hasValue; }
    [[nodiscard]] T value() const { return _value; }
};

#endif
