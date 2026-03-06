# 🛣️ Highway Toll Barrier Simulation with ETC Lanes

[![OMNeT++](https://img.shields.io/badge/OMNeT%2B%2B-6.0.3-blue?logo=cplusplus&logoColor=white)](https://omnetpp.org/)
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-green.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![University](https://img.shields.io/badge/University-Computer%20Engineering-orange)](/)
[![Academic Year](https://img.shields.io/badge/Academic%20Year-2024%2F25-purple)](/)

> **Performance Evaluation of a Highway Toll Barrier with Electronic Toll Collection (ETC) lanes**  
> Built with [OMNeT++ 6.0](https://omnetpp.org/)

A discrete-event simulation model that evaluates the performance of a highway toll barrier system with both **Standard** and **ETC (Electronic Toll Collection)** lanes. The project analyzes how varying the number of ETC lanes (**K**), the percentage of ETC-equipped vehicles (**p**), and the traffic workload affect key metrics such as **mean response time** and **mean waiting time**.

![Demo](demo.gif)

---

## 📋 Table of Contents

- [Introduction](#introduction)
- [Project Structure](#project-structure)
- [Modules](#modules)
- [Message Format](#message-format)
- [Parameters](#parameters)
- [Getting Started](#getting-started)
- [Simulation Scenarios](#simulation-scenarios)
- [Verification Tests](#verification-tests)
- [Authors](#authors)
- [License](#license)

---

## Introduction

A highway toll barrier has **N** lanes, **K** of which are dedicated exclusively to vehicles equipped with an **Electronic Toll Collection (ETC)** device. A percentage **p** of vehicles is equipped with an ETC device, and vehicles arriving at the barrier are either **cars** or **trucks**, each with their own exponentially distributed arrival rate.

- **Standard lanes**: service time for trucks is **1.5×** that of cars (exponential or lognormal distribution).
- **ETC lanes**: constant service time for all vehicle types.
- **Lane selection**: vehicles choose the lane with the **smallest queue** among those they are permitted to use. Ties are broken by minimum lane index.

### Objectives

Study the behavior of the highway toll barrier under different combinations of **K** (ETC lanes) and **p** (ETC-equipped percentage), under varying workload, by evaluating:

| Performance Index | Definition |
|---|---|
| **E[R]** — Mean Response Time | Time between vehicle arrival at the barrier and end of service |
| **E[W]** — Mean Waiting Time | Time between vehicle arrival at the barrier and start of service |

---

## Project Structure

```
highway/
├── README.md
├── Documentation.txt
├── Makefile
├── demo.gif
├── simulations/
│   ├── HighwayBarrier.ned      # Network topology (top-level)
│   ├── omnetpp.ini             # Simulation configurations
│   ├── package.ned
│   └── results/                # Output vectors/scalars
└── src/
    ├── Highway.ned / .h / .cc  # Vehicle generator module
    ├── Barrier.ned              # Compound module (switch + lanes)
    ├── Switch.ned / .h / .cc   # Vehicle router/dispatcher
    ├── Lane.ned / .h / .cc     # Abstract base lane (FIFO queue)
    ├── StandardLane.ned/.h/.cc  # Standard toll lane (exp/lognormal)
    ├── ETCLane.ned / .h / .cc  # Electronic toll collection lane
    ├── Vehicle.msg              # Message definition
    └── package.ned
```

---

## Modules

### Architecture Overview

```
┌──────────┐         ┌─────────────────────────────────────────┐
│          │         │              Barrier                    │
│          │         │  ┌────────┐    ┌─────────────────────┐  │
│          │         │  │        ├───►│ ETCLane[0]     🟢   │  │
│          │         │  │        ├───►│ ETCLane[1]     🟢   │  │
│ Highway  ├────────►│  │ Switch │    └─────────────────────┘  │
│  (src)   │         │  │   🔵   │    ┌─────────────────────┐  │
│          │         │  │        ├───►│ StdLane[0]     🟠   │  │
│          │         │  │        ├───►│ StdLane[1]     🟠   │  │
│          │         │  │        ├───►│ ...            🟠   │  │
└──────────┘         │  └────────┘    └─────────────────────┘  │
                     └─────────────────────────────────────────┘
```

| Module | Type | Description |
|---|---|---|
| **Highway** | Simple | Generates cars and trucks with exponential interarrival times. Determines ETC equipment based on probability **p**. |
| **Barrier** | Compound | Contains the Switch and all Lane submodules. |
| **Switch** | Simple | Routes vehicles to the appropriate lane type (ETC or Standard) with shortest-queue policy. |
| **Lane** | Simple (abstract) | Base class implementing FIFO queue, signal emission, and service lifecycle. |
| **StandardLane** | Simple (extends Lane) | Exponential or lognormal service time. Trucks take 1.5× longer. |
| **ETCLane** | Simple (extends Lane) | Constant service time for all vehicles. |

### Vehicle Colors in Qtenv

| Vehicle Type | Animation Color |
|---|---|
| 🔴 Car (Standard) | Red |
| 🟢 Car (ETC) | Green |
| 🟡 Truck (Standard) | Yellow |
| 🔵 Truck (ETC) | Cyan |

---

## Message Format

Defined in `Vehicle.msg`:

```
message Vehicle {
    simtime_t arrivalTime;  // Arrival time at the barrier
    bool isCar;             // true = car, false = truck
    bool hasEtc;            // true = equipped with ETC device
}
```

---

## Parameters

### Configurable Factors

| Parameter | Description | Default | Unit |
|---|---|---|---|
| `N` | Total number of lanes | 8 | — |
| `K` | Number of ETC-dedicated lanes | 2 | — |
| `p` | Percentage of ETC-equipped vehicles | 0.3 | [0, 1] |
| `Tc` | Mean interarrival time for cars | 9 | seconds |
| `Tt` | Mean interarrival time for trucks | 90 | seconds |
| `S` | Mean service time (standard lanes) | 62 | seconds |
| `S_etc` | Service time (ETC lanes) | 5 | seconds |
| `stddev` | Std. deviation for lognormal distribution | 1 | seconds |

### Calibrated Ranges (from real-world data)

| Factor | Range | Source |
|---|---|---|
| S | [54s, 62s] | Analysis of system stability |
| Tc | [9s, 12s] | ~300–400 cars/hour (Anas S.p.A.) |
| Tt | [90s, 120s] | ~30–40 trucks/hour (~10% of traffic) |
| p | [0.1, 0.6] | ETC devices sold vs. registered cars in Italy |
| K | [0, 3] | Experimental range |

### Simulation Settings

| Setting | Value |
|---|---|
| Warm-up time | 200,000 s |
| Simulation time | 1,000,000 s |
| Repetitions | 35 |
| Confidence interval | 95% |
| RNGs | 3 (cars, trucks, service) |

---

## Getting Started

### Prerequisites

- [OMNeT++ 6.0.x](https://omnetpp.org/download/) installed and configured

### Build

```bash
cd highway
make
```

Or from the OMNeT++ IDE: **Right-click project → Build Project**.

### Run

```bash
cd simulations
../src/highway -u Qtenv -f omnetpp.ini -c General
```

Available configurations can be listed with:

```bash
../src/highway -f omnetpp.ini -a
```

---

## Simulation Scenarios

### Exponential Distribution — Workload Levels

| Workload | Tc | Tt | S | Cars/hour | Trucks/hour |
|---|---|---|---|---|---|
| **Low** | 12s | 120s | 54s | ~300 | ~30 |
| **Medium** | 10.5s | 105s | 58s | ~350 | ~35 |
| **High** | 9s | 90s | 62s | ~400 | ~40 |

### Lognormal Distribution

Uses medium workload (Tc=10.5s, Tt=105s, S=58s) with three variance levels:

| Variance Level | σ (stddev) |
|---|---|
| Low | 30s |
| Medium | 60s |
| High | 90s |

---

## Verification Tests

| Test | Purpose | Method |
|---|---|---|
| **Degeneracy** | Extreme parameter behavior | K=0 (no ETC lanes), p=0 (no ETC vehicles) |
| **Consistency A** | Cars vs. trucks service ratio | Only cars vs. only trucks → expect 1.5× ratio |
| **Consistency B** | Scaling invariance | Double arrival + service times → same queue lengths |
| **Continuity** | Sensitivity to small changes | S ∈ {53.5, 54, 54.5, 55}s → linear response |
| **Monotonicity** | Expected trends | Increasing N → decreasing response time |

---

## Authors

| Name |
|---|
| **Klaudio Çaça** |
| **Gabriele Caioli** |
| **Alessio Simoncini** |

📅 Academic Year 2024/25 — Computer Engineering  
📚 *Performance Evaluation of Computer Systems and Networks*

---

## License

This project is licensed under the **GNU Lesser General Public License v3.0** — see the [LGPL-3.0](https://www.gnu.org/licenses/lgpl-3.0.html) for details.
