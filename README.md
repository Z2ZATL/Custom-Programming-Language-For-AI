# Data Analysis and Processing Project

A comprehensive C++ project for data processing and analysis, with optional AI/ML capabilities.

## Features

- Read and write data from CSV, JSON and other file formats
- Process and transform data with configurable operations
- Optional ML/AI models using Eigen for linear algebra
- Comprehensive error handling and logging system
- Well-structured architecture with modular design
- Thoroughly tested with unit tests

## Getting Started

### Prerequisites

- C++ compiler with C++17 support (GCC 7+, Clang 5+, or MSVC 19.14+)
- CMake 3.10 or higher
- Eigen library (optional, for ML components)
- Google Test (automatically downloaded if not found)

### Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/data-analyzer.git
   cd data-analyzer
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Configure the project with CMake:
   ```
   cmake ..
   ```

   Optional flags:
   - `-DBUILD_TESTS=OFF` to disable tests
   - `-DUSE_EIGEN=OFF` to disable Eigen usage

4. Build the project:
   ```
   cmake --build .
   