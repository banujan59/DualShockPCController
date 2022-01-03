# DualShockPCController
## Dependencies
This project has the following dependencies:
- Qt
- Boost

The Boost library is included in the VS project using an environment variable. Here's how to set it up for your system:

1. Open Start menu, type "Environment Variables"
2. Click on "Edit the system environment variables"
3. In the advance tab, click on "Environment Variables" button
4. In the user variable add a new variable called "BOOST_INCLUDE"
5. In the variable value, type the path to the Boost includes. Should be something like "C:\Program Files\boost\boost_1_76_0"
6. In the user variable add a new variable called "BOOST_LIB"
7. In the variable value, type the path to the Boost libs. Should be something like "C:\Program Files\boost\boost_1_76_0\stage\lib"
8. This project is already configured to use these variables. You should be able to compile now.