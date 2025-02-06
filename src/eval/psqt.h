/*
 * Stoat, a USI shogi engine
 * Copyright (C) 2025 Ciekce
 *
 * Stoat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Stoat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Stoat. If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include "../types.h"

#include "../core.h"

namespace stoat::eval {
    namespace PSQT_values {
        using PSQT = std::array<Score, 81>;
        // clang-format off
        constexpr PSQT kPawn = {
              0,   0,   0,   0,   0,   0,   0,   0,   0,
             90,  90,  90,  90,  90,  90,  90,  90,  90,
             80,  80,  81,  80,  80,  80,  80,  81,  80,
             40,  40,  50,  40,  40,  40,  40,  50,  10,
             20,  20,  30,  20,  20,  20,  20,  30,  20,
             10,  10,  20,  15,  10,  10,  10,  20,  10,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
        };
        constexpr PSQT kPromotedPawn = {
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
            -20, -20, -20, -20, -20, -20, -20, -20, -20,
            -20, -20, -20, -20, -20, -20, -20, -20, -20,
            -20, -20, -20, -20, -20, -20, -20, -20, -20,
            -20, -20, -20, -20, -20, -20, -20, -20, -20,
            -20, -20, -20, -20, -20, -20, -20, -20, -20,
        };
        constexpr PSQT kPromotedLance = kPromotedPawn;
        constexpr PSQT kPromotedKnight = kPromotedPawn;
        constexpr PSQT kPromotedSilver = kPromotedPawn;
        constexpr PSQT kLance = {
              0,   0,   0,   0,   0,   0,   0,   0,   0,
             90,  90,  90,  90,  90,  90,  90,  90,  90,
             80,  80,  80,  80,  80,  80,  80,  80,  80,
             40,  40,  40,  40,  40,  40,  40,  40,  40,
             10,  10,  10,  10,  10,  10,  10,  10,  10,
             10,  10,  10,  10,  10,  10,  10,  10,  10,
             10,  10,  10,  10,  10,  10,  10,  10,  10,
             10,  10,  10,  10,  10,  10,  10,  10,  10,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
        };
        constexpr PSQT kKnight = {
              0,   0,   0,   0,   0,   0,   0,   0,   0,
             90,  90,  90,  90,  90,  90,  90,  90,  90,
             80,  80,  80,  80,  80,  80,  80,  80,  80,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
             10,  10,  10,  10,  10,  10,  10,  10,  10,
             10,  10,  10,  10,  10,  10,  10,  10,  10,
             10,  10,  10,  10,  10,  10,  10,  10,  10,
        };
        constexpr PSQT kSilver = {
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
        };
        constexpr PSQT kGold = {
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
        };
        constexpr PSQT kBishop = {
            100, 100, 100, 100, 100, 100, 100, 100, 100,
            100, 100, 100, 100, 100, 100, 100, 100, 100,
            100, 100, 100, 100, 100, 100, 100, 100, 100,
             70,  70,  70,  70,  70,  70,  70,  70,  70,
             70,  70,  70,  50,  50,  50,  70,  70,  70,
             70,  70,  50,  50,  30,  50,  50,  70,  70,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,  30,   0,   0,   0,   0,   0,   0,   0,
            -10, -10, -10, -10, -10, -10, -10, -10, -10,
        };
        constexpr PSQT kPromotedBishop = kBishop;
        constexpr PSQT kRook = {
            100, 100, 100, 100, 100, 100, 100, 100, 100,
            100, 100, 100, 100, 100, 100, 100, 100, 100,
            100, 100, 100, 100, 100, 100, 100, 100, 100,
             70,  70,  70,  70,  70,  70,  70,  70,  70,
             50,  50,  50,  50,  50,  50,  50,  50,  70,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
              0,   0,   0,   0,   0,   0,   0,   0,   0,
            -10, -10, -10, -10, -10, -10, -10, -10, -10,
        };
        constexpr PSQT kPromotedRook = kRook;
        // clang-format on
    } // namespace PSQT_values
    [[nodiscard]] constexpr Score psqtValue(PieceType pt, Color stm, Square sq) {
        constexpr std::array kValues = {
            PSQT_values::kPawn,
            PSQT_values::kPromotedPawn,
            PSQT_values::kLance,
            PSQT_values::kKnight,
            PSQT_values::kPromotedLance,
            PSQT_values::kPromotedKnight,
            PSQT_values::kSilver,
            PSQT_values::kPromotedSilver,
            PSQT_values::kGold,
            PSQT_values::kBishop,
            PSQT_values::kRook,
            PSQT_values::kPromotedBishop,
            PSQT_values::kPromotedRook,
        };

        assert(pt);
        return kValues[pt.idx()][stm == Colors::kWhite ? sq.rotate().idx() : sq.idx()];
    }
} // namespace stoat::eval
