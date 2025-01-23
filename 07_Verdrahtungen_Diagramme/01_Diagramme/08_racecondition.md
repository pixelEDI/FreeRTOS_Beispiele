```mermaid
graph TD
    T1["Task 1"]
    T2["Task 2"]
    M1["Mutex 1"]
    M2["Mutex 2"]

    T1 -->|nimmt| M1
    T2 -->|nimmt| M2
    T1 -.->|wartet auf| M2
    T2 -.->|wartet auf| M1
```
