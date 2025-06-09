# Architecture Analysis: Pipeline Step System (`src/tg/testcase`)

This document enumerates the key architecture problems addressed by the pipeline step system, outlines the architectural decisions taken for each, and presents the rationale behind these decisions, including discussion of alternatives and trade-offs.

---

## 1. Architecture Problems

### 1.1. How to Represent and Organize Processing Steps
- How to model modular, reusable units of computation in a pipeline.

### 1.2. Type Safety and Data Passing
- How to allow steps to pass data between each other efficiently and safely, while accommodating heterogeneous data types.

### 1.3. Data Ownership and Usage Semantics
- How to manage ownership and access patterns (read, create, consume) for data flowing between steps to prevent race conditions and misuse.

### 1.4. Data Access and Synchronization
- How to synchronize access to data objects, especially in potential parallel or multi-threaded scenarios.

### 1.5. Step Orchestration and Execution Flow
- How to execute steps in a controlled manner, ensuring correct data preparation and output handling.

### 1.6. Extensibility and Maintainability
- How to make the framework easy to extend with new data types and processing steps, while minimizing coupling and boilerplate.

---

## 2. Architecture Decisions and Design Rationale

### 2.1. How to Represent and Organize Processing Steps

**Decision:**  
Use an abstract base class (`Step`) with virtual methods for execution (`execute()`) and data visitation (`visit_data()`).

**Design Rationale:**
- **Pro:** Enforces a common interface for all processing steps and enables polymorphic handling in executors and pipelines.
- **Pro:** The visitor pattern (`visit_data`) allows generic orchestration and introspection of step inputs/outputs.
- **Alternative Considered:**  
  - Template-based step interfaces: Would complicate polymorphic orchestration and increase code bloat.
  - Pure function-based steps: Would hinder extensibility and state encapsulation.

---

### 2.2. Type Safety and Data Passing

**Decision:**  
Use a combination of type erasure (`std::type_index`) and template wrappers (e.g., `Input<T>`, `Output<T>`) to enforce type safety in data connections.

**Design Rationale:**
- **Pro:** Allows steps to work with any data type while maintaining type checks at runtime.
- **Pro:** Template wrappers give compile-time type checking at data connection points.
- **Con:** Some runtime overhead and complexity in type management.
- **Alternative Considered:**  
  - Use only `void*` or shared pointers: Unsafe and error-prone.
  - Enforce a single data type: Not flexible enough for general pipelines.

---

### 2.3. Data Ownership and Usage Semantics

**Decision:**  
Explicitly model data usage (`Read`, `Create`, `Consume`) with enums, and enforce these semantics in the executor.

**Design Rationale:**
- **Pro:** Prevents common bugs (e.g., double consumption, accidental mutation, use-after-move).
- **Pro:** Supports future parallel execution by making data sharing and exclusivity explicit.
- **Alternative Considered:**  
  - Implicit usage conventions: Too error-prone and hard to audit.
  - Immutable data everywhere: Inefficient for large data (e.g., image matrices).

---

### 2.4. Data Access and Synchronization

**Decision:**  
Design data management (`DataImpl`) to support thread safety and explicit control over data copying/moving, though full multithreading is to be implemented later.

**Design Rationale:**
- **Pro:** Lays groundwork for safe parallelism.
- **Pro:** Design is future-proof, even if initial implementation runs sequentially.
- **Alternative Considered:**  
  - No synchronization: Unsafe in concurrent scenarios.
  - Global locks: Simple but would not scale in multi-step, multi-data workflows.

---

### 2.5. Step Orchestration and Execution Flow

**Decision:**  
Use a `StepExecutor` to orchestrate step execution, managing input preparation and output collection via the visitor pattern.

**Design Rationale:**
- **Pro:** Decouples execution policy from step logic.
- **Pro:** Enables future extension to dependency-aware scheduling.
- **Alternative Considered:**  
  - Steps manage their own data: Leads to duplication and tight coupling.
  - Global control flow only: Harder to localize execution logic for steps.

---

### 2.6. Extensibility and Maintainability

**Decision:**  
Encourage subclassing of `Step` and registration of new data types via template wrappers, with minimal intrusion into core framework.

**Design Rationale:**
- **Pro:** New steps/data types are easy to add without modifying existing code.
- **Pro:** Follows the Open/Closed Principle.
- **Alternative Considered:**  
  - Monolithic step registry or factory: Harder to maintain and extend.
  - Scripting-based extensibility: Would lose type safety and C++ performance.

---

## 3. Summary Table

| Problem                                    | Decision                                                      | Rationale (Short)                                             |
|---------------------------------------------|---------------------------------------------------------------|---------------------------------------------------------------|
| Represent Processing Steps                  | Abstract base class + visitor pattern                         | Enforces interface, supports generic orchestration            |
| Type Safety & Data Passing                  | Type erasure + template wrappers                              | Flexible, type safe, supports heterogeneous data              |
| Data Ownership & Usage                      | Explicit usage enums enforced by executor                     | Prevents race conditions, supports parallelism                |
| Data Access & Synchronization               | Thread-safe data management design                            | Prepares for safe parallel execution                          |
| Step Orchestration                         | Separate StepExecutor with visitor                            | Decouples policy, eases extension to scheduling               |
| Extensibility                              | Subclassing and template registration                         | Open/Closed, easy addition of new steps/data types            |

---

## 4. Conclusion

The architecture balances type safety, extensibility, and future-proofing for parallel execution. Each decision is made with explicit trade-offs in mind, and alternatives are rejected primarily due to safety, extensibility, or efficiency concerns.