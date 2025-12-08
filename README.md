Writing: Readme(.txt|.md|.pdf) with the following sections:
Title: Name of your Piece
Personal Information: Full Name, UtorID, student number, Assignment number you augmented (A1, A3, A8)
Instructions: instructions on how to run your code.
Description:
Write about what you produced with an explicit list of features you added to your rendered Piece and where in the code this is being applied.
Acknowledgements: Citations to all assets, libraries, or external materials you used in the Piece.
(Optional) Compilation Verification: If you are worried whether your code will compile on our machines, you may submit a screen recording video or output log of your compilation process working on your machine.

# Title: The Wind Dragon
***
## Personal Information:
#### Full Name:   **Morris Sun** <br>
#### UtorID:      **sunxia72**   <br>
#### Student id:  **1006701767** <br>
#### Assignments: **A3 and A4**  <br>
***
## Instructions:
### HEADSUP: The project has been tested only on Ubuntu 24.04 LTS WSL. It is not guaranteed to work on other linux distributions!
run the following commands in the terminal
Install essential tools
``` bash
    sudo apt update
    sudo apt install build-essential
```
Install Cmake build tool
``` bash
    sudo apt install cmake
```

Install Version Control tool
```bash
    sudo apt install git
```

Now change directory to where you want the project to be and run
```bash
    git clone --recurse-submodules 
```