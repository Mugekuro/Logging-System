# User Guide

This document provides instructions for using the Student Body Organization Management System.

## Table of Contents
- [Getting Started](#getting-started)
- [Login Process](#login-process)
- [Main Menu Options](#main-menu-options)
- [Student Management](#student-management)
- [Officer Management](#officer-management)
- [Payment Processing](#payment-processing)
- [Viewing Logs](#viewing-logs)
- [Security Features](#security-features)
- [Troubleshooting](#troubleshooting)
- [FAQ](#faq)

## Getting Started

### Launching the Application
1. Open your terminal or command prompt
2. Navigate to the application directory
3. Run the application using: `make run` or `./build/bin/payment_system`
4. You'll see the welcome screen with the application title

### Initial Login
- On first run, the system creates a default admin account
- Username: `admin`
- Password: `SecurePass123!`
- After first login, change these credentials immediately

## Login Process

1. When prompted, enter your username
2. Enter your password (passwords are hidden as you type)
3. If successful, you'll see a welcome message and be taken to the main menu
4. If login fails, you have 3 attempts before the account is temporarily locked

### Forgotten Password
Contact your system administrator to reset your password.

## Main Menu Options

Once logged in, you'll see the main menu with the following options:

| Option | Description |
|--------|-------------|
| 1 | Student Management |
| 2 | Officer Management |
| 3 | Payment Processing |
| 4 | View Logs |
| 5 | Help |
| 6 | Logout |

## Student Management

Accessed via option '1' from the main menu.

### Adding a Student
1. Select "Add Student" from the student management menu
2. Enter the Student ID (must be unique)
3. Enter the first name and last name
4. Optionally enter email and phone number
5. The system will automatically set status to "Active" and registration date to current date

### Viewing a Student
1. Select "View Student"
2. Enter the Student ID
3. The system will display all available information about the student

### Updating a Student
1. Select "Update Student"
2. Enter the Student ID
3. The system will prompt for new information (currently only status can be updated)
4. Enter the new status (Active, Inactive, Graduated)

### Deleting a Student
1. Select "Delete Student"
2. Enter the Student ID
3. Confirm deletion when prompted
4. The student record will be permanently removed

### Listing All Students
1. Select "List All Students"
2. The system will display all students in a formatted table

### Searching Students
1. Select "Search Students"
2. Enter a name to search for
3. The system will display all students whose names contain the search term

## Officer Management

Accessed via option '2' from the main menu.
> Note: Only users with access level 2 or higher can access officer management.

### Adding an Officer
1. Select "Add Officer" 
2. Enter the Officer ID (must be unique)
3. Enter the officer's name
4. Enter the officer's position
5. Enter the access level (1-5)
6. The system will automatically set status to "Active"

### Viewing an Officer
1. Select "View Officer"
2. Enter the Officer ID
3. The system will display all available information about the officer

### Updating an Officer
1. Select "Update Officer"
2. Enter the Officer ID
3. The system will prompt for new information (currently only position can be updated)
4. Enter the new position

### Deleting an Officer
1. Select "Delete Officer"
2. Enter the Officer ID
3. Confirm deletion when prompted
4. The officer record will be permanently removed
> Note: Only users with access level 5 can delete officers.

### Listing All Officers
1. Select "List All Officers"
2. The system will display all officers in a formatted table

### Searching Officers
1. Select "Search Officers"
2. Enter a name to search for
3. The system will display all officers whose names contain the search term

## Payment Processing

Accessed via option '3' from the main menu.

### Processing a Payment
1. Select "Process Payment"
2. Enter the Student ID for the payment
3. Enter the payment amount
4. Enter the purpose of the payment (e.g., "College Shirt", "Event Fee")
5. The system will automatically set status to "Completed" and payment method to "Cash"

### Viewing a Payment
1. Select "View Payment"
2. Enter the Payment ID
3. The system will display all details about the payment

### Updating a Payment
1. Select "Update Payment"
2. Enter the Payment ID
3. The system will prompt for new information (currently only status can be updated)
4. Enter the new status (Pending, Completed, Failed, Refunded)

### Listing All Payments
1. Select "List All Payments"
2. The system will display all payments in a formatted table

### Searching Payments
1. Select "Search Payments"
2. Enter a purpose to search for
3. The system will display all payments whose purpose contains the search term

### Viewing Student Payments
1. Select "View Student Payments"
2. Enter the Student ID
3. The system will display all payments for that student
4. The system will also show the total amount paid by the student

## Viewing Logs

Accessed via option '4' from the main menu.
> Note: Only users with access level 3 or higher can access log viewing.

### Viewing All Logs
1. Select "View All Logs"
2. The system will display all log entries in a formatted table

### Searching Logs by User
1. Select "Search Logs by User"
2. Enter the User ID
3. The system will display all log entries for that user

### Searching Logs by Module
1. Select "Search Logs by Module"
2. Enter the module name (e.g., "STUDENT", "PAYMENT", "AUTH")
3. The system will display all log entries for that module

### Searching Logs by Action
1. Select "Search Logs by Action"
2. Enter an action description (e.g., "Login", "Add Student")
3. The system will display all log entries containing that action

### Exporting Logs
1. Select "Export Logs"
2. Enter a filename for the export
3. Enter the format (csv, json, txt)
4. The system will export the logs in the specified format

## Security Features

### Session Management
- Sessions automatically timeout after 24 hours of inactivity
- All actions are logged with user identification
- Role-based access control restricts access to sensitive functions

### Password Security
- Passwords are securely hashed
- Password complexity requirements are enforced
- Failed login attempts are tracked

### Audit Trail
- All system operations are logged
- Logs include user ID, action, module, timestamp, and details
- Logs cannot be modified or deleted by regular users

## Troubleshooting

### Login Issues
**Problem**: Cannot login with correct credentials
**Solution**: 
1. Check for typos in username or password
2. Ensure caps lock is not on
3. If using the default admin account, verify you've changed the password after first login

**Problem**: Account locked after failed attempts
**Solution**: Wait 5 minutes for automatic unlock or contact administrator for immediate unlock

### Student/Officer/Other Data Issues
**Problem**: Cannot find a student/officer that should exist
**Solution**: 
1. Verify the ID is correct
2. Check for typos in the ID
3. Ensure you have the necessary access level to view the record

**Problem**: Cannot add a new record
**Solution**: 
1. Verify all required fields are filled
2. Check that the ID is unique
3. Ensure the data format is correct (e.g., valid email format)

### Payment Issues
**Problem**: Payment not showing in student's payment history
**Solution**: 
1. Verify the Student ID is correct
2. Check that the payment was successfully processed
3. Ensure you're viewing the correct student

### System Issues
**Problem**: Application crashes or freezes
**Solution**: 
1. Restart the application
2. Check the logs in the `logs/` directory
3. Contact system administrator if issue persists

## FAQ

**Q: How do I change my password?**
A: Password changes must be done by an administrator or through the user management interface if you have sufficient access level.

**Q: Can I recover a deleted student or officer record?**
A: No, deletion is permanent. However, the system maintains backup files that may contain the deleted data.

**Q: How are backups created?**
A: The system supports manual backups using the backup functionality. Automatic backups should be set up separately using system tools like cron.

**Q: Who can access the officer management features?**
A: Only users with access level 2 or higher can access officer management, and only level 5 users can delete officers.

**Q: How are payments tracked?**
A: Each payment is assigned a unique ID and linked to a specific student. The system tracks amount, purpose, date, status, and payment method.

**Q: What happens to my session if I step away?**
A: Sessions automatically timeout after 24 hours of inactivity for security purposes.

**Q: Can I export data from the system?**
A: Yes, logs can be exported in CSV, JSON, or text formats. Additional export functionality may be added in future versions.

**Q: How do I get help if I'm stuck?**
A: Use the Help option in the main menu, contact your system administrator, or refer to the documentation in the `docs/` directory.