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

#include "types.h"

#include <array>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#include "bitboard.h"
#include "move.h"
#include "util/result.h"

namespace stoat {
    class Hand {
    public:
        [[nodiscard]] inline bool empty() const {
            return m_hand == 0;
        }

        [[nodiscard]] u32 count(PieceType pt) const;
        [[nodiscard]] u32 count() const;

        u32 increment(PieceType pt);
        u32 decrement(PieceType pt);

        void set(PieceType pt, u32 count);

        [[nodiscard]] std::string sfen(bool uppercase) const;

        [[nodiscard]] bool operator==(const Hand&) const = default;

    private:
        u32 m_hand{};

        friend std::ostream& operator<<(std::ostream& stream, const Hand& hand);
    };

    class SfenError {
    public:
        explicit SfenError(std::string message) :
                m_message{std::move(message)} {}

        [[nodiscard]] std::string_view message() const {
            return m_message;
        }

    private:
        std::string m_message{};
    };

    struct PositionKeys {
        u64 all{};

        void clear();

        void flipPiece(Piece piece, Square sq);
        void movePiece(Piece piece, Square from, Square to);

        void flipStm();

        void flipHandCount(Color c, PieceType pt, u32 count);
        void switchHandCount(Color c, PieceType pt, u32 before, u32 after);

        [[nodiscard]] bool operator==(const PositionKeys&) const = default;
    };

    enum class SennichiteStatus {
        kNone = 0,
        kDraw,
        kWin, // perpetual check by opponent
    };

    class Position {
    public:
        Position();

        Position(const Position&) = default;
        Position(Position&&) = default;

        [[nodiscard]] Position applyMove(Move move) const;
        [[nodiscard]] Position applyNullMove() const;

        [[nodiscard]] inline Bitboard occupancy() const {
            return m_colors[0] | m_colors[1];
        }

        [[nodiscard]] inline Bitboard colorBb(Color color) const {
            assert(color);
            return m_colors[color.idx()];
        }

        [[nodiscard]] inline Bitboard pieceTypeBb(PieceType pt) const {
            assert(pt);
            return m_pieces[pt.idx()];
        }

        [[nodiscard]] inline Bitboard pieceBb(Piece piece) const {
            assert(piece);
            return m_colors[piece.color().idx()] & m_pieces[piece.type().idx()];
        }

        [[nodiscard]] inline Bitboard pieceBb(PieceType pt, Color c) const {
            assert(pt);
            assert(c);
            return m_colors[c.idx()] & m_pieces[pt.idx()];
        }

        [[nodiscard]] inline Piece pieceOn(Square square) const {
            assert(square);
            return m_mailbox[square.idx()];
        }

        [[nodiscard]] inline const Hand& hand(Color color) const {
            assert(color);
            return m_hands[color.idx()];
        }

        [[nodiscard]] inline u64 key() const {
            return m_keys.all;
        }

        [[nodiscard]] inline bool isInCheck() const {
            return !m_checkers.empty();
        }

        [[nodiscard]] inline Bitboard checkers() const {
            return m_checkers;
        }

        [[nodiscard]] inline Bitboard pinned() const {
            return m_pinned;
        }

        [[nodiscard]] inline Color stm() const {
            return m_stm;
        }

        [[nodiscard]] inline u32 moveCount() const {
            return m_moveCount;
        }

        [[nodiscard]] inline Square king(Color c) const {
            assert(c);
            return pieceBb(PieceTypes::kKing, c).lsb();
        }

        [[nodiscard]] SennichiteStatus testSennichite(
            bool cuteChessWorkaround,
            std::span<const u64> keyHistory,
            i32 limit = 16
        ) const;

        [[nodiscard]] bool isPseudolegal(Move move) const;
        [[nodiscard]] bool isLegal(Move move) const;

        [[nodiscard]] bool isCapture(Move move) const;

        [[nodiscard]] bool isAttacked(Square sq, Color attacker, Bitboard occ) const;

        [[nodiscard]] bool isAttacked(Square sq, Color attacker) const {
            return isAttacked(sq, attacker, occupancy());
        }

        [[nodiscard]] Bitboard attackersTo(Square sq, Color attacker) const;

        [[nodiscard]] Bitboard allAttackersTo(Square sq, Bitboard occ) const;

        [[nodiscard]] std::string sfen() const;

        void regenKey();

        [[nodiscard]] bool operator==(const Position&) const = default;

        Position& operator=(const Position&) = default;
        Position& operator=(Position&&) = default;

        [[nodiscard]] static Position startpos();

        [[nodiscard]] static util::Result<Position, SfenError> fromSfenParts(std::span<std::string_view> sfen);
        [[nodiscard]] static util::Result<Position, SfenError> fromSfen(std::string_view sfen);

        friend std::ostream& operator<<(std::ostream& stream, const Position& pos);

    private:
        std::array<Bitboard, Colors::kCount> m_colors{};
        std::array<Bitboard, PieceTypes::kCount> m_pieces{};

        std::array<Piece, Squares::kCount> m_mailbox{};

        std::array<Hand, Colors::kCount> m_hands{};

        std::array<u16, Colors::kCount> m_consecutiveChecks{};

        PositionKeys m_keys{};

        Bitboard m_checkers{};
        Bitboard m_pinned{};

        Color m_stm{Colors::kBlack};

        u16 m_moveCount{1};

        void addPiece(Square sq, Piece piece);
        void movePiece(Square from, Square to, Piece piece);
        void promotePiece(Square from, Square to, Piece piece);
        void dropPiece(Square sq, Piece piece);

        void updateAttacks();

        void regen();
    };
} // namespace stoat
