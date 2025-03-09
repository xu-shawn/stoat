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

#include "perft.h"

#include "movegen.h"
#include "util/timer.h"

namespace stoat {
    namespace {
        usize doPerft(const Position& pos, i32 depth) {
            if (depth <= 0) {
                return 1;
            }

            movegen::MoveList moves{};
            movegen::generateAll(moves, pos);

            usize total{};

            for (const auto move : moves) {
                if (!pos.isLegal(move)) {
                    continue;
                }

                if (depth == 1) {
                    ++total;
                } else {
                    const auto newPos = pos.applyMove(move);
                    total += doPerft(newPos, depth - 1);
                }
            }

            return total;
        }
    } // namespace

    void splitPerft(const Position& pos, i32 depth) {
        if (depth < 1) {
            depth = 1;
        }

        const auto start = util::Instant::now();

        movegen::MoveList moves{};
        movegen::generateAll(moves, pos);

        usize total{};

        for (const auto move : moves) {
            if (!pos.isLegal(move)) {
                continue;
            }

            const auto newPos = pos.applyMove(move);
            const auto value = doPerft(newPos, depth - 1);

            total += value;
            fmt::println("{}\t{}", move, value);
        }

        const auto nps = static_cast<usize>(static_cast<f64>(total) / start.elapsed());

        fmt::println("");
        fmt::println("total: {}", total);
        fmt::println("{} nps", nps);
    }
} // namespace stoat
