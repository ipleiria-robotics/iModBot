# Files

Folder tree:
```
4_Files
├── 4-1_PCB_Files/        // 2 PCB versions
├── 4-2_Battery_Case_3D/  // Cases for the batteries
├── 4-3_Libraries/        // Dualshock libraries for remote controllers and iModBot library for various other functions
└── 4-4_ArduBlock/        // ArduBlock file for Arduino IDE and ArduBlock-Workspace for editing or creating blocks
```
## Programs

To be able to edit source files inside the folders, you will need the following programs:

`Eclipse IDE` This is used to edit the ArduBlock-Workspace file. In there, you can edit or create blocks, change default values or create new ones, change blocks image, etc. Tutorials for installing this program, along with JDK and maven, can be found in the ![3-Documents](https://github.com/DimitriOnLSD/iModBot/tree/master/3_Documents) folder.

`EAGLE` This is used to open and edit the first PCB version. 

`Proteus 8.10` This is used to open and edit the second PCB version. Keep in mind that this file can't be opened in previous versions of proteus, like 8.9.

`Fusion 360` `SolidWorks` `SketchUp Pro 2021` These can be used to open and edit the .F3D, .step and .skp files inside Battery_Case_3D folder.

`Visual Studio Code` This is used to edit the .h and .cpp files inside both libraries.

`Arduino IDE` This is used to upload the code to the ESP32.
