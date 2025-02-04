#pragma once
#include <array>

constexpr const std::array<short, 26> _make_letter_points() {
    std::array<short, 26> _letter_points{};
    _letter_points[ 0] = 1;  // A
    _letter_points[ 1] = 3;  // B
    _letter_points[ 2] = 3;  // C
    _letter_points[ 3] = 2;  // D
    _letter_points[ 4] = 1;  // E
    _letter_points[ 5] = 4;  // F
    _letter_points[ 6] = 2;  // G
    _letter_points[ 7] = 4;  // H
    _letter_points[ 8] = 1;  // I
    _letter_points[ 9] = 8;  // J
    _letter_points[10] = 10; // K
    _letter_points[11] = 1;  // L
    _letter_points[12] = 2;  // M
    _letter_points[13] = 1;  // N
    _letter_points[14] = 1;  // O
    _letter_points[15] = 3;  // P
    _letter_points[16] = 8;  // Q
    _letter_points[17] = 1;  // R
    _letter_points[18] = 1;  // S
    _letter_points[19] = 1;  // T
    _letter_points[20] = 1;  // U
    _letter_points[21] = 4;  // V
    _letter_points[22] = 10; // W
    _letter_points[23] = 10; // X
    _letter_points[24] = 10; // Y
    _letter_points[25] = 10; // Z
    return _letter_points;
}

constexpr static const std::array<short, 26> letter_points = _make_letter_points();

/**
 * @brief Returns the amount of points a given letter is worth.
 * @param letter A letter (from A-Z).
 * @return The amount of points the letter is worth, or -1 if the letter is invalid.
 */
constexpr short get_points_from_letter(char letter) {
    char index = 0;
    if (letter >= 'a' && letter <= 'z')
        index = letter - 'a';
    else if (letter >= 'A' && letter <= 'Z')
        index = letter - 'A';
    else
        return -1;
    
    return letter_points[index];
}
