@echo off
REM Check if the user provided an intention as a parameter
IF "%~1"=="" (
    echo Please provide an intention as a parameter.
    echo Usage: %0 "Your Intention Here"
    exit /b 1
)

REM Assign the first parameter to a variable, preserving quotes
set "INTENTION=%~1"

REM Run the Python script with the provided intention
python intention_repeater.py --intent "%INTENTION%" --file none --file2 none --freq 3 --hash y --runtime 1800 --log none

exit /b 0
