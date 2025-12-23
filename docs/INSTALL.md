# Installation Guide

This document provides detailed instructions for installing and setting up the Student Body Organization Management System.

## Table of Contents
- [System Requirements](#system-requirements)
- [Prerequisites](#prerequisites)
- [Installation Steps](#installation-steps)
- [Configuration](#configuration)
- [Post-Installation](#post-installation)
- [Troubleshooting](#troubleshooting)

## System Requirements

### Operating Systems
- Windows 7 or later
- Linux (any recent distribution)
- macOS 10.12 or later

### Hardware Requirements
- CPU: Any modern processor (x86 or x64)
- RAM: 512 MB minimum, 1 GB recommended
- Storage: 50 MB available space
- Display: Terminal or console access

### Software Requirements
- C compiler (GCC recommended)
- Make build system
- Git (for cloning the repository)

## Prerequisites

Before installing the application, ensure you have the following software installed:

### On Windows
1. Install MinGW-w64 or TDM-GCC for C compilation
2. Install Make for Windows
3. Install Git for Windows

### On Linux/macOS
1. Ensure GCC is installed:
   ```bash
   gcc --version
   ```
2. Ensure Make is installed:
   ```bash
   make --version
   ```
3. Install Git if not already present:
   ```bash
   # On Ubuntu/Debian:
   sudo apt-get install git
   
   # On CentOS/RHEL:
   sudo yum install git
   
   # On macOS:
   xcode-select --install
   ```

## Installation Steps

### 1. Clone the Repository
```bash
git clone <repository-url>
cd <repository-directory>
```

### 2. Verify Directory Structure
After cloning, you should see the following directory structure:
```
payment_system/
├── src/                          # Source code files
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
├── README.md                     # Project overview
└── ...
```

### 3. Build the Application
```bash
make
```

This will:
- Create the necessary build directories
- Compile all source files
- Link the executable
- Place the binary in `build/bin/payment_system`

### 4. Verify the Build
Check that the executable was created:
```bash
ls -la build/bin/
```

You should see the `payment_system` executable.

### 5. Run the Application
```bash
make run
```

Or directly:
```bash
./build/bin/payment_system
```

## Configuration

### Initial Setup
On first run, the system will:
1. Create necessary directories (data, backup, logs)
2. Initialize the database files
3. Create a default admin user with credentials:
   - Username: `admin`
   - Password: `SecurePass123!`

### Configuration Files
The system uses the following configuration files:

#### Application Configuration
- File: `config/app_config.json`
- Contains: General application settings

#### Security Configuration
- File: `config/security_config.json`
- Contains: Security-related settings

#### Logging Configuration
- File: `config/logging_config.json`
- Contains: Logging settings

### Environment Variables
The system can be configured using environment variables:

| Variable | Description | Default |
|----------|-------------|---------|
| `DATA_DIR` | Directory for data files | `./data/` |
| `BACKUP_DIR` | Directory for backup files | `./backup/` |
| `LOG_DIR` | Directory for log files | `./logs/` |

## Post-Installation

### 1. Change Default Credentials
After the first login, immediately change the default admin password:
1. Log in with default credentials
2. Navigate to the user management section
3. Update the admin user's password

### 2. Configure Backup Schedule
Set up automated backups using the system's backup functionality:
1. Use the `make backup` command to create manual backups
2. Set up a cron job or scheduled task for automated backups

### 3. Test the System
Perform these basic tests:
1. Create a test student record
2. Process a test payment
3. Verify that logs are created
4. Test user authentication

### 4. Set Up Monitoring
Monitor the system for:
- Error logs in the `logs/` directory
- Data file integrity
- System performance

## Troubleshooting

### Common Issues

#### Build Errors
**Problem**: `make` command fails with compilation errors
**Solution**: 
1. Verify GCC installation: `gcc --version`
2. Check for missing dependencies
3. Ensure proper file permissions

#### Missing Dependencies
**Problem**: Error about missing libraries or tools
**Solution**: Install the required dependencies based on your OS:
- On Ubuntu/Debian: `sudo apt-get install build-essential`
- On CentOS/RHEL: `sudo yum groupinstall "Development Tools"`
- On macOS: `xcode-select --install`

#### Permission Issues
**Problem**: Cannot create files in data directory
**Solution**: Check directory permissions and ensure the application has write access

#### Runtime Errors
**Problem**: Application crashes or behaves unexpectedly
**Solution**: 
1. Check log files in the `logs/` directory
2. Run with debug flags: `make debug`
3. Ensure sufficient system resources

### Getting Help
If you encounter issues not covered in this guide:

1. Check the [README.md](README.md) for additional information
2. Review the [API documentation](docs/API.md)
3. Contact the development team
4. Check the issue tracker for known problems

### Support Channels
- Email: [support email]
- Documentation: [docs directory]
- Issue Tracker: [repository issues]

## Upgrading

To upgrade from a previous version:

1. Backup your data: `make backup`
2. Download the new version
3. Follow the installation steps again
4. Restore your data if needed
5. Test the upgraded system thoroughly

## Uninstallation

To completely remove the system:

1. Delete the application directory
2. Remove any created system links (if installed with `make install`)
3. Clean up any scheduled backup tasks you created manually