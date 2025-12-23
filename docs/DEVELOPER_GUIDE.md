# Developer Guide

This document provides information for developers who want to contribute to or extend the Student Body Organization Management System.

## Table of Contents
- [Project Structure](#project-structure)
- [Architecture Overview](#architecture-overview)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Module Details](#module-details)
- [Testing](#testing)
- [Building and Deployment](#building-and-deployment)
- [Extending the System](#extending-the-system)
- [Common Development Tasks](#common-development-tasks)

## Project Structure

```
payment_system/
├── src/                          # Source code files
│   ├── main.c                    # Main program entry point
│   ├── auth.c                    # Authentication module
│   ├── auth.h                    # Authentication header
│   ├── student.c                 # Student management module
│   ├── student.h                 # Student header
│   ├── officer.c                 # Officer management module
│   ├── officer.h                 # Officer header
│   ├── payment.c                 # Payment processing module
│   ├── payment.h                 # Payment header
│   ├── logging.c                 # Logging and audit module
│   ├── logging.h                 # Logging header
│   ├── ui.c                      # User interface module
│   ├── ui.h                      # UI header
│   ├── file_io.c                 # File operations module
│   ├── file_io.h                 # File operations header
│   ├── utils.c                   # Utility functions
│   ├── utils.h                   # Utility header
│   ├── data_structures.h         # Centralized data structures
│   └── config.h                  # Configuration definitions
├── data/                         # Application data files
├── backup/                       # Backup files and recovery
├── docs/                         # Documentation files
├── tests/                        # Unit and integration tests
├── config/                       # Configuration files
├── scripts/                      # Utility and automation scripts
├── logs/                         # Application runtime logs
├── build/                        # Build artifacts and intermediates
├── assets/                       # Static assets
├── tools/                        # Development and maintenance tools
├── Makefile                      # Build configuration
└── ...
```

## Architecture Overview

The system follows a modular architecture with clear separation of concerns:

### Layered Architecture
- **Presentation Layer**: UI module handles user interaction
- **Business Logic Layer**: Auth, Student, Officer, Payment, Logging modules
- **Data Access Layer**: File I/O module handles data persistence

### Design Patterns Used
- **Module Pattern**: Each feature is encapsulated in its own module
- **Factory Pattern**: For creating structured data objects
- **Observer Pattern**: For logging and audit trail system
- **Strategy Pattern**: For different validation strategies

### Data Flow
```
UI → Business Logic → Data Access → Storage
 ↑        ↓              ↓          ↑
 ←-------- ←------------ ←----------
```

## Development Setup

### Prerequisites
- GCC or compatible C compiler
- Make build system
- Git for version control
- Text editor or IDE (VS Code, Vim, etc.)

### Setting Up the Development Environment

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. Verify the build system works:
   ```bash
   make
   ```

3. Run the application to ensure everything works:
   ```bash
   make run
   ```

4. Run the tests:
   ```bash
   make test
   ```

### Development Workflow
1. Create a feature branch: `git checkout -b feature/my-feature`
2. Make your changes
3. Add tests for new functionality
4. Run existing tests: `make test`
5. Build the application: `make`
6. Commit your changes: `git commit -am 'Add new feature'`
7. Push to the branch: `git push origin feature/my-feature`
8. Create a pull request

## Coding Standards

### C Code Standards
- Use C11 standard
- Follow consistent indentation (4 spaces)
- Use meaningful variable and function names
- Include comments for complex logic
- Use const for parameters that don't change
- Initialize all variables

### Naming Conventions
- Functions: `snake_case` (e.g., `add_student`)
- Variables: `snake_case` (e.g., `student_count`)
- Constants: `UPPER_CASE` (e.g., `MAX_STUDENTS`)
- Types: `PascalCase` (e.g., `Student`)

### File Organization
- Each module should have a `.c` and `.h` file pair
- Include guards in all header files
- Minimal includes in header files
- Implementation details in `.c` files

### Error Handling
- Use the `ErrorCode` enum for consistent error reporting
- Always check return values from functions
- Log errors appropriately using the logging module
- Clean up resources in error conditions

### Security Practices
- Validate all inputs
- Use safe string functions (`strncpy`, `snprintf`, etc.)
- Avoid buffer overflows
- Sanitize user inputs
- Protect sensitive data

## Module Details

### Authentication Module (`auth.c/h`)
**Purpose**: Handles user authentication and session management
- User creation, authentication, and password management
- Session creation, validation, and termination
- Access level enforcement
- Account lockout mechanisms

**Key Functions**:
- `authenticate_user()` - Verify user credentials
- `create_session()` - Create authenticated session
- `check_access_level()` - Verify user permissions

### Student Module (`student.c/h`)
**Purpose**: Manage student records and operations
- CRUD operations for student data
- Validation of student information
- Search and retrieval functions

**Key Functions**:
- `add_student()` - Add new student
- `find_student()` - Retrieve student by ID
- `update_student()` - Update student information

### Officer Module (`officer.c/h`)
**Purpose**: Manage officer records and roles
- Officer data management
- Role-based access control
- Position and access level tracking

**Key Functions**:
- `add_officer()` - Add new officer
- `officer_has_access_level()` - Check officer permissions

### Payment Module (`payment.c/h`)
**Purpose**: Handle payment processing and tracking
- Payment creation and validation
- Transaction history management
- Amount calculations

**Key Functions**:
- `process_payment()` - Process a new payment
- `get_student_total_payments()` - Calculate total payments for a student

### Logging Module (`logging.c/h`)
**Purpose**: Maintain system audit trail
- Event logging with user identification
- Search and retrieval of log entries
- Export functionality

**Key Functions**:
- `log_event()` - Log a system event
- `search_logs_by_user()` - Find logs for a specific user

### File I/O Module (`file_io.c/h`)
**Purpose**: Handle data persistence
- Binary file operations
- Data integrity validation
- Backup and recovery mechanisms

**Key Functions**:
- `write_student_record()` - Save student data
- `validate_file_integrity()` - Check file validity

### UI Module (`ui.c/h`)
**Purpose**: Handle user interface and input/output
- Menu systems and navigation
- Input validation and display
- User interaction management

**Key Functions**:
- `display_menu()` - Show interactive menus
- `get_user_input()` - Safely get user input

## Testing

### Test Structure
Tests are located in the `tests/` directory and follow this structure:
- `test_runner.c` - Main test runner
- Additional test files for specific modules

### Running Tests
```bash
make test
```

### Writing Tests
1. Add test functions to the test runner
2. Use the `TEST_ASSERT` macro for assertions
3. Follow the naming convention: `test_module_functions()`
4. Test both success and failure cases
5. Test edge cases and error conditions

### Test Coverage
Aim for at least 80% code coverage for critical modules:
- Authentication module: 90%+
- Data validation: 95%+
- Core business logic: 85%+

## Building and Deployment

### Building the Application
```bash
# Standard build
make

# Debug build
make debug

# Clean build artifacts
make clean

# Build and run
make run
```

### Build Targets
- `all` - Build the application (default)
- `debug` - Build with debug information
- `clean` - Remove build artifacts
- `run` - Build and run the application
- `test` - Build and run unit tests
- `install` - Install the application to system
- `backup` - Create a backup of data files

### Deployment Considerations
- Ensure proper file permissions
- Set up automated backups
- Configure logging appropriately
- Plan for data migration if needed
- Consider security hardening

## Extending the System

### Adding New Features
1. Identify the appropriate module for the new feature
2. Add function declarations to the header file
3. Implement functions in the source file
4. Add tests for the new functionality
5. Update documentation if needed

### Adding New Modules
1. Create new `.c` and `.h` files
2. Define data structures if needed
3. Implement required functions
4. Add to the build system
5. Integrate with existing modules if necessary

### Database Migration
The system uses binary file storage. For future enhancements:
- Consider adding database migration tools
- Implement versioning for data files
- Plan for backward compatibility

### API Extensions
- Maintain backward compatibility
- Follow existing patterns
- Document new functions
- Add appropriate error handling

## Common Development Tasks

### Adding a New Data Type
1. Define the structure in `data_structures.h`
2. Create CRUD operations in the appropriate module
3. Add file I/O functions in `file_io.c/h`
4. Implement UI functions in `ui.c/h`
5. Add tests for the new data type

### Adding Validation Rules
1. Add validation function to `utils.c/h`
2. Update the validation logic in the appropriate module
3. Add tests for the new validation
4. Update error handling as needed

### Adding New Menu Options
1. Define the menu option in the appropriate display function
2. Implement the menu action handler
3. Add access level checks if needed
4. Update the user guide documentation

### Adding New Error Codes
1. Add the error code to the `ErrorCode` enum in `data_structures.h`
2. Implement error handling in the appropriate functions
3. Add tests for the new error condition
4. Update documentation if needed

### Updating Build System
The Makefile is located in the root directory:
- Modify `SOURCES` to add new source files
- Update include paths if needed
- Add new build targets if necessary

### Adding Configuration Options
1. Define configuration in `config.h`
2. Create configuration file parsing functions
3. Add validation for configuration values
4. Update documentation for new options