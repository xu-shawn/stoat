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

#include "movepick.h"
#include "history.h"
#include <algorithm>

namespace stoat {
    void MoveGenerator::sortQuiets() {
        std::stable_sort(
            m_moves.begin() + m_idx,
            m_moves.begin() + m_end,
            [&pos = m_pos, &history = m_history](const Move& lhs, const Move& rhs) {
                return history.quietScore(pos.stm(), lhs) > history.quietScore(pos.stm(), rhs);
            }
        );
    }

    Move MoveGenerator::next() {
        switch (m_stage) {
            case MovegenStage::TtMove: {
                ++m_stage;

                if (m_ttMove && m_pos.isPseudolegal(m_ttMove)) {
                    return m_ttMove;
                }

                [[fallthrough]];
            }

            case MovegenStage::GenerateCaptures: {
                movegen::generateCaptures(m_moves, m_pos);
                m_end = m_moves.size();

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::Captures: {
                if (const auto move = selectNext([this](Move move) { return move != m_ttMove; })) {
                    return move;
                }

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::GenerateNonCaptures: {
                movegen::generateNonCaptures(m_moves, m_pos);
                m_end = m_moves.size();
                sortQuiets();

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::NonCaptures: {
                if (const auto move = selectNext([this](Move move) { return move != m_ttMove; })) {
                    return move;
                }

                m_stage = MovegenStage::End;
                return kNullMove;
            }

            case MovegenStage::QsearchGenerateCaptures: {
                movegen::generateCaptures(m_moves, m_pos);
                m_end = m_moves.size();

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::QsearchCaptures: {
                if (const auto move = selectNext([](Move) { return true; })) {
                    return move;
                }

                m_stage = MovegenStage::End;
                return kNullMove;
            }

            default:
                return kNullMove;
        }
    }

    MoveGenerator MoveGenerator::main(const Position& pos, const HistoryTables& history, Move ttMove) {
        return MoveGenerator{MovegenStage::TtMove, pos, history, ttMove};
    }

    MoveGenerator MoveGenerator::qsearch(const Position& pos, const HistoryTables& history) {
        return MoveGenerator{MovegenStage::QsearchGenerateCaptures, pos, history, kNullMove};
    }

    MoveGenerator::MoveGenerator(
        MovegenStage initialStage,
        const Position& pos,
        const HistoryTables& history,
        Move ttMove
    ) :
            m_stage{initialStage}, m_pos{pos}, m_history(history), m_ttMove{ttMove} {}
} // namespace stoat
