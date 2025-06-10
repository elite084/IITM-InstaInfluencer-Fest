# IITM-InstaInfluencer-Fest
C++ mini project simulates the IITM InstaInfluencer Fest using a directed graph of event dependencies. It employs DFS for cycle detection, Strongly Connected Component (SCC) analysis, and topological sorting to manage event flow, and uses dynamic programming on the DAG to compute the maximum hype score showcasing advanced algorithms and OOPS.

---

## Background

March 2025. After Shaastra, IIT Madras hosts the very first **InstaInfluencer Fest**! A celebration of Gen Z culture, social media stardom, and digital creativity.

This fest includes:
-  Viral dance-offs  
-  Meme competitions  
-  Podcast sessions  
-  Live-streaming challenges  

But things get messy when influencers demand personalized event orders. Organizers now face complex **event dependencies**, which may lead to **cyclic conflicts** and **tightly-knit event groups**.

Your task? Help the organizers manage this chaos using powerful graph algorithms.

---

## Problem Overview

Events are modeled as a **directed graph**:
- **Nodes**: Events
- **Edges**: Dependency `u -> v` means event `u` must happen before `v`.
- **Hype Score**: Each event has a popularity value among students.

You must handle the following queries:

| Query Type | Description |
|------------|-------------|
| 1 | Detect if any **cycles** (drama!) exist in the event schedule. |
| 2 | Find number of **Strongly Connected Components (SCCs)** and the size of the largest group. |
| 3 | Print a valid **topological order** of events (sorted lexicographically among independent nodes). |
| 4 | Compute **maximum hype score** achievable through a valid path across the DAG after SCC condensation. |

---

## Core Concepts Used

- **Cycle Detection**: Using DFS or topological sort.
- **Kosaraju’s Algorithm**: For finding SCCs efficiently.
- **Topological Sorting**: Lexicographically stable using min-heaps.
- **Dynamic Programming on DAGs**: To calculate longest path in condensed graph.
- **SCC Condensation**: Each SCC is reduced to a single node with combined hype score.

---

## Implementation Overview

- Written in **C++**
- Utilizes **Object-Oriented Programming**
- Central abstract class: `GraphAlgorithm`
- Four derived classes:
  - `isCycle` → Detects cycles
  - `indepComponent` → Finds SCCs and cardinality
  - `validOrder` → Topological sorting
  - `maxHype` → DAG DP for hype score

### Design Features

-  **Graph Condensation**
-  **Dynamic DAG Path Scoring**
-  Fully query-driven
-  **Comparator functor** ready (optional)
-  Well-commented & clean class structure

---

