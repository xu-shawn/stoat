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

namespace stoat {
    Move MoveGenerator::next() {
        switch (m_stage) {
            case MovegenStage::kTtMove: {
                ++m_stage;

                if (m_ttMove && m_pos.isPseudolegal(m_ttMove)) {
                    return m_ttMove;
                }

                [[fallthrough]];
            }

            case MovegenStage::kGenerateCaptures: {
                movegen::generateCaptures(m_moves, m_pos);
                m_end = m_moves.size();

                scoreCaptures();

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::kCaptures: {
                if (const auto move = selectNext([this](Move move) { return move != m_ttMove; })) {
                    return move;
                }

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::kGenerateNonCaptures: {
                movegen::generateNonCaptures(m_moves, m_pos);
                m_end = m_moves.size();

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::kNonCaptures: {
                if (const auto move = selectNext([this](Move move) { return move != m_ttMove; })) {
                    return move;
                }

                m_stage = MovegenStage::kEnd;
                return kNullMove;
            }

            case MovegenStage::kQsearchGenerateCaptures: {
                movegen::generateCaptures(m_moves, m_pos);
                m_end = m_moves.size();

                scoreCaptures();

                ++m_stage;
                [[fallthrough]];
            }

            case MovegenStage::kQsearchCaptures: {
                if (const auto move = selectNext([](Move) { return true; })) {
                    return move;
                }

                m_stage = MovegenStage::kEnd;
                return kNullMove;
            }

            default:
                return kNullMove;
        }
    }

    MoveGenerator MoveGenerator::main(const Position& pos, Move ttMove) {
        return MoveGenerator{MovegenStage::kTtMove, pos, ttMove};
    }

    MoveGenerator MoveGenerator::qsearch(const Position& pos) {
        return MoveGenerator{MovegenStage::kQsearchGenerateCaptures, pos, kNullMove};
    }

    MoveGenerator::MoveGenerator(MovegenStage initialStage, const Position& pos, Move ttMove) :
            m_stage{initialStage}, m_pos{pos}, m_ttMove{ttMove} {}

    i32 MoveGenerator::scoreCapture(Move move) {
        const auto capturing = m_pos.pieceOn(move.from()).type();
        const auto captured = m_pos.pieceOn(move.to()).type();
        return static_cast<i32>(captured.idx()) - static_cast<i32>(capturing.idx());
    }

    void MoveGenerator::scoreCaptures() {
        for (usize idx = m_idx; idx < m_end; ++idx) {
            m_scores[idx] = scoreCapture(m_moves[idx]);
        }
    }

    usize MoveGenerator::findNext() {
        auto bestIdx = m_idx;
        auto bestScore = m_scores[m_idx];

        for (usize idx = m_idx + 1; idx < m_end; ++idx) {
            if (m_scores[idx] > bestScore) {
                bestIdx = idx;
                bestScore = m_scores[idx];
            }
        }

        if (bestIdx != m_idx) {
            std::swap(m_moves[m_idx], m_moves[bestIdx]);
            std::swap(m_scores[m_idx], m_scores[bestIdx]);
        }

        return m_idx++;
    }
} // namespace stoat
