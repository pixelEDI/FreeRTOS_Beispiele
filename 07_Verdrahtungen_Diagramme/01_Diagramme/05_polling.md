```mermaid
graph TD
    C[Loop]
    C --> D{Ein Ereignis aufgetreten?}
    D -->|Ja| E[Aktualisieren/Verarbeiten]
    D -->|Nein| C
    E --> F[evt. nächste Abfrage]
    F --> C
```
