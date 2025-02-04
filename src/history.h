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

#include "bitboard.h"
#include "types.h"

#include <algorithm>
#include <cstring>
#include <valarray>

#include "move.h"
#include "util/multi_array.h"

namespace stoat {
    using HistoryScore = i16;

    inline constexpr auto historyBonusDepthScale = 300;
    inline constexpr auto historyBonusOffset = 128;
    inline constexpr auto maxHistoryBonus = 2048;

    inline constexpr auto historyPenaltyDepthScale = 300;
    inline constexpr auto historyPenaltyOffset = 128;
    inline constexpr auto maxHistoryPenalty = 2048;

    inline auto historyBonus(i32 depth) -> HistoryScore {
        return static_cast<HistoryScore>(
            std::clamp(depth * historyBonusDepthScale - historyBonusOffset, 0, maxHistoryBonus)
        );
    }

    inline auto historyPenalty(i32 depth) -> HistoryScore {
        return static_cast<HistoryScore>(
            -std::clamp(depth * historyPenaltyDepthScale - historyPenaltyOffset, 0, maxHistoryPenalty)
        );
    }

    inline constexpr auto maxHistory = 16384;

    struct HistoryEntry {
        i16 value{};

        HistoryEntry() = default;
        HistoryEntry(HistoryScore v) :
                value{v} {};

        [[nodiscard]] inline operator HistoryScore() const {
            return value;
        }

        [[nodiscard]] inline auto operator=(HistoryScore v) -> auto& {
            value = v;
            return *this;
        }

        inline auto update(HistoryScore bonus) {
            value += bonus - value * std::abs(bonus) / maxHistory;
        }
    };

    class HistoryTables {
    public:
        HistoryTables() = default;
        ~HistoryTables() = default;

        inline auto clear() {
            std::memset(&m_main, 0, sizeof(m_main));
        }

        inline auto updateQuietScore(Color color, Move move, HistoryScore bonus) {
            mainEntry(color, move).update(bonus);
        }

        [[nodiscard]] inline auto quietScore(Color color, Move move) const -> i32 {
            if (move.isDrop())
                return 0;

            return mainEntry(color, move);
        }

    private:
        // [color][from][to]
        util::MultiArray<HistoryEntry, 2, 81, 81> m_main{};

        [[nodiscard]] inline auto mainEntry(Color color, Move move) const -> const HistoryEntry& {
            return m_main[color.idx()][move.from().idx()][move.to().idx()];
        }

        [[nodiscard]] inline auto mainEntry(Color color, Move move) -> HistoryEntry& {
            return m_main[color.idx()][move.from().idx()][move.to().idx()];
        }
    };
} // namespace stoat
