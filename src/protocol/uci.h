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

#include "uci_like.h"

namespace stoat::protocol {
    class UciHandler : public UciLikeHandler {
    public:
        explicit UciHandler(EngineState& state);
        ~UciHandler() override = default;

        void finishInitialInfo() const final;

        util::Result<Position, std::optional<std::string>> parsePosition(std::span<std::string_view> args) final;
        util::Result<Move, InvalidMoveError> parseMove(std::string_view str) final;

        void printBoard(std::ostream& stream, const Position& pos) const final;
        void printFen(std::ostream& stream, const Position& pos) const final;
        void printMove(std::ostream& stream, Move move) const final;

        void printFenLine(std::ostream& stream, const Position& pos) const final;
    };
} // namespace stoat::protocol
