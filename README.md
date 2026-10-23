# Server Log Parser and Analyzer

This project is a C++ utility designed to parse and analyze structured server log files. It sequentially processes log entries, categorizes them by severity level, and provides a detailed statistical breakdown of failure points across API endpoints.

## Project Overview

The core of the project is the `Parse` class, which handles the extraction and aggregation of log data. It is currently built to ingest and process files directly formatted like server_logs.txt[cite: 2]. By scanning the raw text for specific key-value pairs, it generates a console-based report outlining server health and failure bottlenecks.

The analyzed logs contain JSON-formatted attributes, including fields such as timestamp, level, service, trace_id, and endpoint[cite: 2]. 

## Core Features

*   **Log Level Tracking:** The program identifies and tallies the total occurrences of specific operational states, tracking ERROR, WARN, INFO, and DEBUG statements throughout the file[cite: 2].
*   **Error Extraction:** When an ERROR level is detected, the program isolates and extracts the exact API endpoint where the failure occurred[cite: 2].
*   **Endpoint Normalization:** To prevent dynamic URLs from skewing the error statistics, the tool normalizes endpoints before counting them:
    *   Query parameters are actively stripped from the endpoint string, such as removing search terms from catalog queries[cite: 2].
    *   User-specific identifiers, specifically those beginning with the "usr_" prefix, are isolated and replaced with a generic "{id}" placeholder[cite: 2].
*   **Frequency Analysis:** The tool aggregates the normalized data into a frequency map and calculates the most frequently failing endpoints to help developers prioritize fixes.

## Known Limitations & Future Improvements

While functional for its current scope, the codebase relies on several rigid assumptions that are slated for future refactoring:

*   **Fragile String Matching:** The parsing mechanism utilizes standard `std::string::find` logic rather than a dedicated JSON parser. If the log output format shifts slightly (e.g., altered spacing or swapped key orders), the string matching will break. A transition to a robust library like `nlohmann/json` is planned.
*   **Incomplete ID Normalization:** The `normalize` function is currently hardcoded to only target "usr_" identifiers. It fails to normalize other dynamic tracking IDs present in the logs, such as the "crt_" prefix used for cart identification[cite: 2].
*   **Hardcoded File Paths:** The `server_logs.txt` filename is hardcoded directly into the `main()` function execution. The next iteration will implement `argc` and `argv` parsing to accept variable file paths via command-line arguments.
*   **Single-Threaded Processing:** Processing massive log dumps line-by-line on a single thread presents a performance bottleneck. Future optimization will include multithreaded chunking or memory-mapped file reading to handle gigabyte-sized logs efficiently.
