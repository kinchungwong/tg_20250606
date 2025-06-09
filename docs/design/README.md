# Design Overview

This folder collects historic planning notes, design monologues, and code samples for the task graph prototype. The documents capture the evolution of the project from early discussions to incremental code iterations.

## Historic Monologues

- **overview.md** – Summarises the core concepts such as tasks, subgraphs and data handling. Describes the typical programmer workflow and explains how the system validates dependencies and executes tasks in parallel.
- **project_guidelines.md** – States that the project targets C++17 and forbids features requiring newer standards.
- **ai_planning_20250529_2244pm.md** – Presents a phased implementation plan, starting with type-safe data ports and culminating in a full execution engine with validation and testing.
- **ai_review_20250524.md** – Reviews early factory design choices. Highlights benefits of explicit naming, encapsulated task creation and factory-managed subgraph export.
- **ai_review_20250525_0959am.md** – Recommends best practices for iteration 3 of the factory pattern, including helper utilities, early validation and improved error reporting.
- **ai_review_20250530_2012pm.md** – Provides feedback on the class diagram, suggesting the documentation should better reflect implementation complexity.
- **class_diagram_20250530_1940pm.md** – Mermaid diagram showing relationships between Subgraph, Task, TaskInput and TaskOutput classes.
- **design_task_graph.md** – Explores the task graph metaphor, comparing it to electronic breadboards and emphasizing validation before execution.
- **design_dag_check.md** – Notes on implementing reachability checks (using Kahn's algorithm) and summarizing unreachable nodes and pruning strategies.
- **instruction_20250605_1958pm.md** – Requirements for redesigning a `GlobalDataSet` class using a PImpl layer and a type-erased variant value.

## Historic Code Samples

The `historic_code_samples` folder contains six C++ excerpts demonstrating progressive refinements of the factory-based API for creating subgraphs. Later iterations introduce context-aware tasks and automatic data management.

## Usage

These documents serve as reference material for contributors. See the root `README.md` for build instructions and visit each markdown file for full details.

## An update on naming

The latest architecture document shifts terminology from **Task** to **Step**, as
implemented in the conceptual code (`src/tg/testcase/blur_step.hpp`). The design
rationale also notes that **Subgraph** will be renamed **Proc**. See
[architectural_rationale/architecture_2025-06-06.md](architectural_rationale/architecture_2025-06-06.md)
for background on these changes.
