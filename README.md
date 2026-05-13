# GOAP AI System: Unreal Engine → Standalone C++

A **Goal-Oriented Action Planning (GOAP)** implementation that evolved from a 2024 Bachelor's thesis game built in Unreal Engine 5 into a standalone, engine-agnostic C++ planner in 2026.

> **tl;dr for engineers:** A world-state graph over boolean flags, searched with Dijkstra's algorithm to find the optimal action sequence toward a goal. The 2024 version is tightly integrated with UE5's Blueprint/C++ bridge; the 2026 rewrite strips the engine dependency entirely and represents world states as 64-bit bitmasks, making planning ~10× leaner and suitable for hundreds of simultaneous agents.

---

## 📺 Demo

[![GOAP AI System Demo](https://raw.githubusercontent.com/timotej2015/GOAP_UnrealEngine/main/screenshots/Posnetek%20zaslona%202024-06-11%20211309.png)](https://www.youtube.com/watch?v=D5K0g2sD15g)

*Click to watch the system in action — AI planning, PCG terrain, and medieval siege combat.*

---

## 🔬 Research Context

The original system was developed as the AI backbone of a third-person medieval siege game, built as part of a Bachelor's thesis at the University of Maribor (FERI). The game features two factions of intelligent actors — attackers trying to breach a fortified settlement, defenders trying to stop them — each planning independently using GOAP.

The thesis (*"Razvoj računalniške igre z inteligentnimi akterji"*, 2024) covers the full development process including PCG terrain generation, Dijkstra's algorithm implementation, Blueprint/C++ integration, and performance analysis. The full PDF is included in this repository.

---

## 🛠 Key Technical Decisions

**v1 (Unreal Engine, 2024)**
- World state represented as an 11-field struct (`WorldState`) with ternary values: `0` / `1` / `2` (N/A), allowing wildcard matching during graph construction.
- The full state graph (2¹¹ = 2048 nodes, 2336 edges) is prebuilt at actor spawn time. Dijkstra then runs over this fixed graph on every planning tick.
- Action costs are dynamic — evaluated at query time based on live distances between actors and points of interest (doors, high ground, enemies).
- C++ planning logic exposed to Unreal's Blueprint system via `UFUNCTION(BlueprintCallable)` and `UFUNCTION(BlueprintImplementableEvent)`, keeping AI logic in C++ while action *execution* (animations, movement) stays in Blueprint.
- Priority queue implemented as a custom min-binary heap with O(1) index lookup — search updates skip re-scanning by tracking each node's heap position directly.

**v2 (Standalone C++, 2026)**
- World state collapsed to a `uint64` bitmask. Preconditions split into `goalSet` / `goalClear` pairs; effects split into `effectsSet` / `effectsClear`.
- `CanExecute` and `ApplyAction` become single bitwise operations, eliminating heap allocations during planning entirely.
- BST-backed closed set replaces the adjacency list, with a shared min-binary heap priority queue (`PriorityQueue_MBH`) operating over BST node indices.
- Multi-goal support with per-goal weights: the planner finds all reachable goals in a single pass and returns the best adjusted cost.
- Zero engine dependencies — drop `GOAP_Core.h` / `GOAP_Core.cpp` into any C++ project.

---

## 📁 Project Structure

```
v1-UnrealEngine/     Original 2024 source (UE5, CPP_GoapPlanner + Blueprint actors)
v2-FastAndOptimized/ 2026 standalone rewrite (GOAP_Core, GOAP_Soldier demo)
docs/                Bachelor's thesis PDF
screenshots/         In-engine screenshots
```

---

## 📊 How the Planner Works (v1)

```
Actor spawns
    └─(constructor: build 2048-node state graph)
        └─(Blueprint tick: GOAPExecuteDijkstra)
            └─(Dijkstra over prebuilt graph)
                └─(evaluateGoal: find best reachable goal node)
                    └─(outputBestPath: walk back to start)
                        └─(GOAPTriggerNextActionBlueprintEvent)
                            └─(Blueprint event fires → animation / movement)
```

Each action defines preconditions and promised results as `WorldState` structs (unset fields treated as wildcards). Graph edges are built once at init by exhaustively matching all world states against each action's preconditions. Edge weights are re-evaluated live at planning time.

---

## 📄 Academic Reference

The thesis is included as [`2024_08_07_diplomsko_delo-Timotej_Kuzma.pdf`](./docs/2024_08_07_diplomsko_delo-Timotej_Kuzma.pdf). Relevant sections for this codebase: §2.2.2 (GOAP theory), §3.5.1 (full implementation walkthrough), §4 (performance measurements — Dijkstra averages 0.041ms per call at 60 agents, init takes 0.876ms).

---

## 📜 License & Attribution

Licensed under the **MIT License**.

**Author: Timotej Kuzma** (2024–2026). If you use this in your own research or projects, attribution is appreciated.
