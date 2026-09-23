# Keccak 5-Round Collision Attack Research

This repository contains the source code, experimental data, and research materials for finding 5-round collisions in the Keccak hash function. 

## Project Structure & Overview

This project is divided into four main phases, progressing from basic layer analysis to advanced collision search algorithms:

* **Part 1: Inverse of the Linear Layer**
  This section introduces the linear layer of Keccak and implements its inverse function.

* **Part 2: Inverse of the Non-Linear Layer (Differential Analysis)**
  This section explores the non-linear layer ($\chi$ step). Because direct inversion is highly complex due to non-linearity, we utilize **differential cryptanalysis** to linearize the problem, making the algebraic system easier to analyze and discuss.

* **Part 3: Target Difference Algorithm (TDA)**
  This is the core of the research. We apply the Target Difference Algorithm to search for valid collision paths. To resolve the complex algebraic constraints generated in this phase, we heavily rely on the SAT solver **CryptoMiniSat**.

* **Part 4: Target Internal Difference Algorithm (TIDA)**
  The final section contains programs and implementations related to the Target Internal Difference Algorithm, which serves as a comparative and alternative approach to finding collisions.

## Tools & Dependencies
* **C++**: Core logic and matrix operations.
* **CryptoMiniSat**: Used for solving CNF formulas to verify algebraic constraints.
