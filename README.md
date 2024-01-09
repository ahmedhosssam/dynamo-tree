##Installing Instructions:

1. First, you need to install CPPRESTSDK (the library I chose to make the server and the RESTAPI):
    With vcpkg on Windows
        PS> vcpkg install cpprestsdk cpprestsdk:x64-windows

    With apt-get on Debian/Ubuntu
        $ sudo apt-get install libcpprest-dev

    With dnf on Fedora
        $ sudo dnf install cpprest-devel

    With brew on OSX
        $ brew install cpprestsdk

    -------------------------------
    And then, you can type the command "make" to compile and run ./a.out to run the server, and it should work.
    -------------------------------

2. Second, to run the client (the web browser):
    1. Installing npm (node package manager):
        For Windows or macOS:
            - Visit https://nodejs.org/en and download the installer for your operating system.
            - Run the installer and follow the instructions.
        
        For Linux (ubuntu):
            $ sudo apt update
            $ sudo apt install nodejs npm
        
        And then run:
            $ node -v
        to make sure that node is successfully installed.
    
    2. Get into /web directory:
        $ cd web
    
    3. Run:
        $ npm install
        $ npm run dev
    
    --> And now, it should work on: http://localhost:5173/
