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

#include "usi.h"

#include <algorithm>
#include <cassert>

namespace stoat::protocol {
    UsiHandler::UsiHandler(EngineState& state) :
            UciLikeHandler{state} {
        registerCommandHandler("usinewgame", [](std::span<std::string_view>, util::Instant) {});
        registerCommandHandler("isready", [this](std::span<std::string_view>, util::Instant) {
            handleNewGame();
            m_state.searcher->ensureReady();
            fmt::println("readyok");
        });
        registerCommandHandler("gameover", [](std::span<std::string_view>, util::Instant) {});
        registerCommandHandler("ping", [](std::span<std::string_view>, util::Instant) { fmt::println("pong"); });
    }

    void UsiHandler::handleNoLegalMoves() const {
        printInfoString("no legal moves");
        fmt::println("bestmove resign");
    }

    bool UsiHandler::handleEnteringKingsWin() const {
        fmt::println("bestmove win");
        return true;
    }

    void UsiHandler::printOptionName(std::string_view name) const {
        static constexpr std::array kFixedSemanticsOptions = {
            "Hash",
        };

        if (std::ranges::find(kFixedSemanticsOptions, name) != kFixedSemanticsOptions.end()) {
            fmt::print("USI_{}", name);
        } else {
            fmt::print("{}", name);
        }
    }

    std::string UsiHandler::transformOptionName(std::string_view name) const {
        if (name.starts_with("usi_")) {
            return std::string{name.substr(4)};
        }

        return std::string{name};
    }

    void UsiHandler::finishInitialInfo() const {
        fmt::println("usiok");
    }

    util::Result<Position, std::optional<std::string>> UsiHandler::parsePosition(std::span<std::string_view> args
    ) const {
        assert(!args.empty());

        if (args[0] != "sfen") {
            return util::err<std::optional<std::string>>();
        }

        if (args.size() == 1) {
            return util::err<std::optional<std::string>>("Missing sfen");
        }

        return Position::fromSfenParts(args.subspan<1>()).mapErr<std::optional<std::string>>([](const SfenError& err) {
            return std::optional{fmt::format("Failed to parse sfen: {}", err.message())};
        });
    }

    util::Result<Move, InvalidMoveError> UsiHandler::parseMove(std::string_view str) const {
        return Move::fromStr(str);
    }

    void UsiHandler::printBoard(const Position& pos) const {
        fmt::print("{}", pos);
    }

    void UsiHandler::printFen(const Position& pos) const {
        fmt::print("{}", pos.sfen());
    }

    void UsiHandler::printMove(Move move) const {
        fmt::print("{}", move);
    }

    void UsiHandler::printMateScore(i32 plies) const {
        fmt::print("{}", plies);
    }

    void UsiHandler::printFenLine(const Position& pos) const {
        fmt::println("Sfen: {}", pos.sfen());
    }

    std::string_view UsiHandler::btimeToken() const {
        return "btime";
    }

    std::string_view UsiHandler::wtimeToken() const {
        return "wtime";
    }

    std::string_view UsiHandler::bincToken() const {
        return "binc";
    }

    std::string_view UsiHandler::wincToken() const {
        return "winc";
    }

    void UsiHandler::printGoMateResponse() const {
        fmt::println("checkmate notimplemented");
    }
} // namespace stoat::protocol
