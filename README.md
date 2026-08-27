ABC 2.0
A modern, concise, and accessible programming language built for the future of clear software development.

ABC 2.0 is a ground-up re-implementation and evolution of the legendary ABC programming language originally created at CWI (Centrum Wiskunde & Informatica) in the 1980s—the language that served as the primary inspiration for Python.
ABC 2.0 preserves the original language's emphasis on clean syntax, intuitive high-level data types, and readability, while introducing a modernized execution engine, modern tooling, and enterprise compliance readiness.
Key Features
Readable Syntax: Designed to minimize boilerplate and keep code structured and expressive.
Modern Toolchain: Built from the ground up with high-performance execution and seamless developer tooling.
Zero Telemetry: Total privacy out of the box. Operates entirely locally without data collection or tracking.
Historic Foundation: Built upon the design principles of Leo Geurts, Lambert Meertens, and Steven Pemberton, tailored for 21st-century software engineering.
Quick Start
Installation
Bash
# Clone the repository
git clone https://github.com/your-org/abc2.git

# Navigate to the directory
cd abc2

# Build the compiler
cargo build --release
Running Your First Program
Create a file named hello.abc:

WRITE "Hello, ABC 2.0!"
Execute it with the CLI:
Bash
abc run hello.abc


Repository Structure
Plaintext
├── .github/          # CI/CD workflows and issue templates
├── cli/              # Command-line interface and tool binaries
├── compiler/         # Core lexer, parser, AST, and code generator
├── docs/             # Language specifications and manuals
├── stdlib/           # Built-in standard library
├── tests/            # Test suite and syntax validations
├── CONTRIBUTING.md   # Guidelines for contributing
└── LICENSE           # ABC Software License (Version 2.0)
License & Governance
ABC 2.0 is distributed under the ABC Software License (Version 2.0).
This modern license honors the historical CWI distribution model while ensuring source integrity, brand protection, explicit zero-telemetry privacy guarantees, and enterprise compatibility.
Original Language Concepts: Copyright (c) Stichting Mathematisch Centrum (CWI), Amsterdam.
ABC 2.0 Implementation: Copyright (c) 2026 ABC Programming Language Project.
See the full terms in the LICENSE file.
Contributing
We welcome contributions to the compiler, standard library, and documentation! Please review CONTRIBUTING.md before submitting pull requests or opening issues. All contributors agree to distribute their work under the ABC Software License v2.0.
