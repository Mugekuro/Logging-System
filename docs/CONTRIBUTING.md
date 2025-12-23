# Contributing

Thank you for your interest in contributing to the Student Body Organization Management System! This document provides guidelines and information to help you contribute effectively.

## Table of Contents
- [Code of Conduct](#code-of-conduct)
- [How to Contribute](#how-to-contribute)
- [Development Setup](#development-setup)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Documentation](#documentation)
- [Community](#community)

## Code of Conduct

This project and everyone participating in it is governed by our Code of Conduct. By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

### Our Pledge

In the interest of fostering an open and welcoming environment, we as contributors and maintainers pledge to making participation in our project and our community a harassment-free experience for everyone.

### Our Standards

Examples of behavior that contributes to creating a positive environment include:
- Using welcoming and inclusive language
- Being respectful of differing viewpoints and experiences
- Gracefully accepting constructive criticism
- Focusing on what is best for the community
- Showing empathy towards other community members

Examples of unacceptable behavior by participants include:
- The use of sexualized language or imagery and unwelcome sexual attention or advances
- Trolling, insulting/derogatory comments, and personal or political attacks
- Public or private harassment
- Publishing others' private information, such as a physical or electronic address, without explicit permission
- Other conduct which could reasonably be considered inappropriate in a professional setting

## How to Contribute

### Reporting Bugs
1. Check the issue tracker to see if the bug has already been reported
2. If not, create a new issue with:
   - Clear title and description
   - Steps to reproduce the problem
   - Expected vs. actual behavior
   - System information (OS, compiler, version)

### Suggesting Features
1. Check existing feature requests in the issue tracker
2. Create a new issue with:
   - Clear description of the feature
   - Use case and benefit to users
   - Any proposed implementation details

### Submitting Code Changes
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Update documentation if needed
6. Submit a pull request

### Improving Documentation
- Fix typos or unclear language
- Add examples to existing documentation
- Write new documentation for features
- Update outdated information

## Development Setup

### Prerequisites
- GCC or compatible C compiler
- Make build system
- Git version control system
- Text editor or IDE

### Getting Started
1. Fork and clone the repository:
   ```bash
   git clone https://github.com/your-username/payment-system.git
   cd payment-system
   ```

2. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. Set up the development environment:
   ```bash
   make
   ```

4. Run the tests to ensure everything works:
   ```bash
   make test
   ```

### Development Workflow
1. Make your changes in small, logical commits
2. Write clear commit messages
3. Run tests frequently to ensure your changes don't break existing functionality
4. Update or add tests as needed
5. Update documentation if applicable
6. Push your changes to your fork
7. Submit a pull request

## Pull Request Process

1. Ensure your code follows the project's coding standards
2. Add tests for new functionality
3. Update documentation as needed
4. Ensure all tests pass
5. Describe your changes clearly in the pull request
6. Link to any related issues
7. Wait for review and address feedback

### Pull Request Checklist
- [ ] Code follows the project's style guidelines
- [ ] Changes are well-tested
- [ ] Documentation has been updated if applicable
- [ ] All tests pass
- [ ] Code is properly commented where necessary
- [ ] Changes don't break existing functionality

### Review Process
- Pull requests require at least one approval
- Maintainers will review code quality, functionality, and adherence to standards
- Feedback should be constructive and specific
- Changes may be requested before approval

## Coding Standards

### C Code Standards
- Use C11 standard
- Follow consistent indentation (4 spaces, no tabs)
- Use meaningful variable and function names
- Include comments for complex logic
- Use const for parameters that don't change
- Initialize all variables
- Check return values from functions
- Handle errors appropriately

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
- Use consistent file structure across modules

### Error Handling
- Use the `ErrorCode` enum for consistent error reporting
- Always check return values from functions
- Log errors appropriately using the logging module
- Clean up resources in error conditions
- Provide meaningful error messages

### Security Practices
- Validate all inputs
- Use safe string functions (`strncpy`, `snprintf`, etc.)
- Avoid buffer overflows
- Sanitize user inputs
- Protect sensitive data
- Follow secure coding practices

## Testing Guidelines

### Test Structure
- Place tests in the `tests/` directory
- Follow the naming convention: `test_module_functions()`
- Test both success and failure cases
- Test edge cases and error conditions
- Aim for high code coverage (80%+ for critical modules)

### Writing Tests
- Use the provided test framework
- Write clear, descriptive test names
- Test one behavior per test function
- Use appropriate assertions
- Mock external dependencies when needed
- Ensure tests are independent and repeatable

### Running Tests
- Run tests frequently during development
- Run the full test suite before submitting changes
- Use `make test` to run all tests
- Address any test failures immediately

## Documentation

### Code Documentation
- Document public functions with comments
- Include parameter descriptions
- Document return values and error conditions
- Update documentation when changing functionality

### Project Documentation
- Update user guides when adding new features
- Add API documentation for new functions
- Update installation and configuration guides as needed
- Keep examples up to date

### Commit Messages
- Use clear, descriptive commit messages
- Follow the format: "Brief summary (imperative mood)"
- Include additional details in the commit body if needed
- Reference related issues when applicable

## Community

### Getting Help
- Check the documentation first
- Search existing issues
- Ask questions in new issues if needed
- Be specific about your problem and what you've tried

### Providing Feedback
- Be constructive and specific
- Provide examples when possible
- Focus on the issue, not the person
- Suggest improvements rather than just pointing out problems

### Recognition
- Contributors will be acknowledged in the project
- Significant contributions may lead to maintainer status
- All contributions are valuable, regardless of size

## Questions?

If you have questions about contributing, feel free to open an issue or contact the maintainers. We're here to help and appreciate your interest in improving the project!