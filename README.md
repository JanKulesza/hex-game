# Hex Game

A desktop implementation of the abstract strategy board game **Hex**, featuring a graphical interface and an AI opponent powered by Monte Carlo Tree Search.

---

## What is Hex?

Hex is a two-player connection game played on an N×N rhombus-shaped board of hexagonal cells. One player connects left-to-right, the other top-to-bottom. The first player to form an unbroken chain of their color across the board wins. Unlike chess, Hex has no draws.

<img width="257" height="425" alt="image" src="https://github.com/user-attachments/assets/352a7b34-bb6f-4c38-bd4a-83a4a3a87aa1" />

## Features

- 🎮 &nbsp;Interactive hexagonal board rendered in QML
- 🤖 &nbsp;AI opponent using Monte Carlo Tree Search (MCTS) with UCT selection
- ⚡ &nbsp;Non-blocking AI — runs on a background thread so the UI stays responsive
- 🏆 &nbsp;Win detection using depth-first search on the hex graph
- 📐 &nbsp;Configurable game settings

---

## How the AI works

The AI uses **Monte Carlo Tree Search** with UCB1 applied to trees (UCT) to balance exploration and exploitation:

```
UCT(s, a) = Q(s, a) + c · √( ln(N_parent) / N(s, a) )
```

Each turn the AI builds a search tree from the current board state and runs thousands of random game simulations (rollouts) to estimate the win probability of each possible move. The move with the most visits is chosen.

The AI runs on a dedicated `QThread`, keeping the GUI fully responsive during its thinking time.

---

## Optimizations

### Memory

MCTS builds a tree that can grow to hundreds of thousands of nodes per move, so keeping each node small matters. Hexagon IDs and board positions are stored as `uint8_t` instead of `int`, reducing per-node memory footprint by 4×. Board state is stored in `QHash` rather than `QMap`, giving O(1) average lookups instead of O(log n) during simulation. Internal references are passed by reference throughout the tree to avoid unnecessary copies of board state.

### Rendering

The board does not repaint on every move — only the single hexagon that changed is re-rendered. The C++ backend never works directly with QML hexagon objects. Instead it tracks the board purely as a `uint8_t` ID → color mapping (a single source of truth), and notifies QML to update only the affected cell. This keeps the rendering cost constant regardless of board size.

---

## Tech stack

![C++](https://img.shields.io/badge/C%2B%2B_17-3D6B8F?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Qt](https://img.shields.io/badge/Qt_6-3D7A4F?style=for-the-badge&logo=qt&logoColor=white)
![QML](https://img.shields.io/badge/QML-6B5B8F?style=for-the-badge&logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake_3.16+-B85C2A?style=for-the-badge&logo=cmake&logoColor=white)

| Technology | Role |
|---|---|
| **C++17** | Game logic, MCTS algorithm, graph-based win detection |
| **Qt 6** | Application framework, threading model (`QThread`, `QObject`) |
| **QML** | Declarative UI — hexagonal board, animations, player interaction |
| **CMake** | Build system |

---

## Roadmap
 
- **BU-UCT** — a parallelisation strategy for MCTS introduced by Wang et al. (2020). Instead of virtual loss, BU-UCT tracks the number of ongoing simulations per node (`O`) and hard-blocks any node that exceeds a configurable threshold (`O ≥ m_max · M`), preventing wasted exploration while keeping the UCT value estimates unbiased. This is machine learning.
- **DSU (Disjoint Set Union)** — replace the current DFS-based win detection with a Union-Find structure. Each move would union the played cell with its same-color neighbours in O(α(n)) amortised time, making win checks nearly instantaneous instead of traversing the full graph every simulation.

---
 
## References
 
- Wikipedia, *Monte Carlo Tree Search* — [pl.wikipedia.org](https://en.wikipedia.org/wiki/Monte-Carlo_Tree_Search)
- Anji Liu, Yitao Liang, Ji Liu, Guy Van den Broeck & Jianshu Chen, *On Effective Parallelization of Monte Carlo Tree Search* — [arXiv:2006.08785](https://arxiv.org/abs/2006.08785)
