🔐 SafePass - Modern & Secure Password Manager

SafePass is a lightweight and robust password manager developed in C++ using the Qt framework. It prioritizes maximum security through industry-standard key derivation and encryption algorithms.

✨ Features

    Secure Vault: Local storage for your credentials (Username, Password, Website, URL).

    Instant Search: Real-time filtering of entries by website name.

    Password Generator: Create strong passwords with adjustable length (8 to 32 characters).

    Visual Management: Toggle password visibility and quick-copy to clipboard.

    Performance: Optimized memory footprint (~50 MB during operation).

🛡️ Security Architecture

The security of SafePass relies on modern cryptographic standards to ensure your data remains unreadable, even if the database file is compromised.
1. Authentication & Key Derivation (PBKDF2)

Instead of storing your Master Password, SafePass utilizes PBKDF2 (Password-Based Key Derivation Function 2):

    Algorithm: HMAC-SHA256.

    Iterations: 100,000 cycles to mitigate brute-force attacks.

    Salt: A unique 128-bit (16 bytes) salt is randomly generated for every user using QRandomGenerator::securelySeeded().

2. Data Encryption (AES-256)

All sensitive information is encrypted before being written to the SQLite database:

    Standard: AES-256 (Advanced Encryption Standard).

    Mode: CBC (Cipher Block Chaining) with a random Initialization Vector (IV) for each entry.

    Encryption Key: The AES key is directly derived from the PBKDF2 process, ensuring maximum entropy.

🛠️ Built With

    Language: C++17

    UI Framework: Qt 6 (Widgets)

    Database: SQLite (via Qt SQL module)

    Cryptography:

        Qt Network (for QPasswordDigestor)

        tiny-AES-c: External C library for AES implementation.

📜 Credits & Dependencies

This project uses the tiny-AES-c library for AES encryption management.

    Original Repository: kokke/tiny-AES-c

    License: The aes.c and aes.h files are included in compliance with their original open-source license.

🚀 Getting Started

    Clone the repository:
    
    git clone https://github.com/your-username/SafePass.git

    Open the project in Qt Creator.

    Configure the project with a Qt Kit (Qt 6 recommended).

    Build and run the application.
