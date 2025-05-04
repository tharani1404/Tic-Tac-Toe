# 🎮 Multiplayer Tic-Tac-Toe with Networking (TCP & UDP)

## 📋 Overview

This project implements a simple **multiplayer Tic-Tac-Toe game** using socket programming. The game supports two clients (players) connecting to a server, where the server manages the game state, enforces rules, and handles communication. The implementation is provided using both **TCP** and **UDP** protocols.

---

## 🧠 Features

- 3x3 classic Tic-Tac-Toe board
- Real-time turn-based multiplayer gameplay
- Server-enforced rules and turn control
- Win/draw detection
- Replay support after game end
- Implemented using both **TCP** and **UDP** sockets

---

## 🛠️ Components

### 🖥️ Server

- Maintains the game board and current state
- Assigns symbols: Player 1 = 'X', Player 2 = 'O'
- Validates moves and alternates turns
- Broadcasts updated board state after each move
- Detects win or draw conditions
- Handles replay logic

### 👤 Clients (Player 1 & Player 2)

- Connect to the server using TCP or UDP
- Send moves by entering grid coordinates (e.g., `1 2`)
- Receive and display board updates
- Informed of turns, invalid inputs, and game results

---

## 🎮 Game Rules

- **Board:** 3x3 grid initialized empty
- **Start:** Game begins when both players are connected
- **Turn:** Players alternate turns. Only the active player can move.
- **Move Format:** Enter row and column (e.g., `2 3`)
- **Invalid Moves:** Rejected by the server
- **Win Condition:** Three identical symbols in a row, column, or diagonal
- **Draw:** All cells filled with no winner
- **Replay Option:** After game end, both players can choose to replay

---

## 🚀 How to Run

### 🔧 Requirements

- Python 3.x
- Socket programming knowledge (no external libraries needed)

### 💻 TCP Version

1. Start the server:
   ```bash
   python tcp_server.py
