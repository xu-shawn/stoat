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

#include "stats.h"

#include <atomic>
#include <limits>
#include <utility>

#include "util/multi_array.h"

namespace stoat::stats {
    namespace {
        constexpr usize kSlots = 32;

        struct Range {
            std::atomic<i64> min{std::numeric_limits<i64>::max()};
            std::atomic<i64> max{std::numeric_limits<i64>::min()};
        };

        util::MultiArray<std::atomic<u64>, kSlots, 2> s_conditionHits{};
        util::MultiArray<Range, kSlots> s_ranges{};
        util::MultiArray<std::pair<std::atomic<i64>, std::atomic<u64>>, kSlots> s_means{};

        std::atomic_bool s_anyUsed{false};

        template <typename T>
        inline void atomicMin(std::atomic<T>& v, T x) {
            auto curr = v.load();
            while (x < curr && v.compare_exchange_weak(curr, x)) {
                //
            }
        }

        template <typename T>
        inline void atomicMax(std::atomic<T>& v, T x) {
            auto curr = v.load();
            while (x > curr && v.compare_exchange_weak(curr, x)) {
                //
            }
        }
    } // namespace

    void conditionHit(bool condition, usize slot) {
        if (slot >= kSlots) {
            fmt::println(stderr, "tried to hit condition {} (max {})", slot, kSlots - 1);
            return;
        }

        ++s_conditionHits[slot][condition];

        s_anyUsed = true;
    }

    void range(i64 v, usize slot) {
        if (slot >= kSlots) {
            fmt::println(stderr, "tried to hit range {} (max {})", slot, kSlots - 1);
            return;
        }

        atomicMin(s_ranges[slot].min, v);
        atomicMax(s_ranges[slot].max, v);

        s_anyUsed = true;
    }

    void mean(i64 v, usize slot) {
        if (slot >= kSlots) {
            fmt::println(stderr, "tried to hit mean {} (max {})", slot, kSlots - 1);
            return;
        }

        s_means[slot].first += v;
        ++s_means[slot].second;

        s_anyUsed = true;
    }

    void print() {
        if (!s_anyUsed.load()) {
            return;
        }

        for (usize slot = 0; slot < kSlots; ++slot) {
            const auto hits = s_conditionHits[slot][1].load();
            const auto misses = s_conditionHits[slot][0].load();

            if (hits == 0 && misses == 0) {
                continue;
            }

            const auto hitrate = static_cast<f64>(hits) / static_cast<f64>(hits + misses);

            fmt::println("condition {}:", slot);
            fmt::println("    hits: {}", hits);
            fmt::println("    misses: {}", misses);
            fmt::println("    hitrate: {:.6g}%", hitrate * 100);
        }

        for (usize slot = 0; slot < kSlots; ++slot) {
            const auto min = s_ranges[slot].min.load();
            const auto max = s_ranges[slot].max.load();

            if (min == std::numeric_limits<i64>::max()) {
                continue;
            }

            fmt::println("range {}:", slot);
            fmt::println("    min: {}", min);
            fmt::println("    max: {}", max);
        }

        for (usize slot = 0; slot < kSlots; ++slot) {
            const auto total = s_means[slot].first.load();
            const auto count = s_means[slot].second.load();

            if (count == 0) {
                continue;
            }

            const auto mean = static_cast<f64>(total) / static_cast<f64>(count);

            fmt::println("mean {}:", slot);
            fmt::println("    mean: {:.6g}", mean);
            fmt::println("    total: {}", total);
            fmt::println("    count: {}", count);
        }
    }
} // namespace stoat::stats
