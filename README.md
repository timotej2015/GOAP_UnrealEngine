# GOAP_UnrealEngine: AI Planning Evolution in Unreal Engine

## 📺 Project Demo & Showcase
[![GOAP AI System Demo](https://raw.githubusercontent.com/timotej2015/GOAP_UnrealEngine/main/screenshots/Posnetek%20zaslona%202024-06-11%20211309.png)](https://www.youtube.com/watch?v=D5K0g2sD15g)

*Click the image above to watch the system in action (AI planning, PCG, and environment showcase).*

This repository showcases a high-performance **Goal-Oriented Action Planning (GOAP)** system. It tracks the journey from a research-focused implementation (2024) to a production-optimized bitmask-based system (2026).

## 🎮 Game Development Context (2024)
The original project was developed as part of a comprehensive game development study in **Unreal Engine 5**. 
- **Environment & Level Design:** The world was constructed within Unreal Engine using a mix of custom-built assets and procedural tools.
- **Modeling:** I used **Blender** to model custom assets for the project.
- **PCG (Procedural Content Generation):** The project utilizes PCG techniques to dynamically generate environment layouts, ensuring varied gameplay scenarios.

## 📈 System Evolution: 2024 vs. 2026

This repository contains two distinct versions of the GOAP planner:

### v1-UnrealEngine (2024 - Thesis Version)
- **Origin:** Developed as the core AI engine for my Bachelor’s Thesis.
- **Implementation:** Uses Unreal Engine's containers.
- **Focus:** Logic accuracy, readability, and integration with the Unreal Engine actor lifecycle.
- **Documentation:** This version is fully documented in the accompanying thesis.

### v2-FastAndOptimized (2026 - High-Performance Version)
- **Origin:** A complete rewrite focused on extreme performance.
- **Optimization:** Transitioned to **Bitmask State Representation**.
- **Performance:** State comparisons and heuristic calculations are now handled via bitwise operators, eliminating heap allocations during the planning phase.
- **Suitability:** Designed for modern games requiring hundreds of agents planning simultaneously without frame drops.

## 📄 Academic Research (Thesis)
My Bachelor's thesis, titled **"Razvoj računalniške igre z inteligentnimi akterji" (2024)**, is included in this repository. It provides a deep dive into:
- The entire game development process in 2024.
- Implementation of AI agents and the GOAP architecture.
- Detailed walkthrough of the **PCG (Procedural Content Generation)** setup.

You can read the full PDF here: [**Diplomska_Naloga_Timotej_Kuzma.pdf**](./docs/2024.08.07_diplomsko_delo-Timotej_Kuzma.pdf)

## 📁 Project Structure
- `v1-UnrealEngine/`: The original 2024 source code.
- `v2-FastAndOptimized/`: The 2026 rewrite with bitmask optimizations.
- `docs/`: Contains the full Bachelor's Thesis.
- `screenshots/`: Visuals of the Unreal Engine environment, Blender models, and AI behavior.

## 📜 License & Attribution
This project is licensed under the **MIT License**.

**Author: Timotej Kuzma** (2024 - 2026)
