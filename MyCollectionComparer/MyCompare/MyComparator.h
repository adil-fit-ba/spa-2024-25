#pragma once

auto isLess = [](auto a, auto b) {
    if constexpr (std::is_pointer_v<decltype(a)>) {
        return *a < *b; // Za pokazivaèe
    }
    else {
        return a < b;   // Za osnovne tipove
    }
    };

auto isEqual = [](auto a, auto b) {
    if constexpr (std::is_pointer_v<decltype(a)>) {
        return *a == *b; // Za pokazivaèe
    }
    else {
        return a == b;   // Za osnovne tipove
    }
    };
