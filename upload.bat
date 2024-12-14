@echo off

:: Define the webhook URL
set WEBHOOK_URL=https://discord.com/api/webhooks/1302674995280871545/fsmwXtFfChCn7ktcF3Gy8Pu0mv8YeOv9Izht3yC7Kstm5gHsa8ovmSvepksTpKXc7ICe

:: Define the input file
set INPUT_FILE=.\input.txt

:: Check if input.txt exists
if not exist "%INPUT_FILE%" (
    echo Error: %INPUT_FILE% not found!
    exit /b 1
)

:: Read the contents of the file into a variable
setlocal enabledelayedexpansion
set CONTENT=
for /f "delims=" %%i in (%INPUT_FILE%) do (
    set "CONTENT=!CONTENT! %%i"
)
endlocal & set CONTENT=%CONTENT:~1%

:: Escape double quotes in the content
set "CONTENT=%CONTENT:"=\\"%"

:: Prepare JSON payload
set PAYLOAD={"content":"%CONTENT%"}

:: Write the payload to a temporary JSON file
echo %PAYLOAD% > payload.json

:: Send the content to the Discord webhook using curl
curl -H "Content-Type: application/json" -d @payload.json %WEBHOOK_URL%

:: Check if curl succeeded
if %ERRORLEVEL% NEQ 0 (
    echo Failed to send the content to the webhook.
    del payload.json
    exit /b 1
)

echo Content successfully sent to the webhook.
del payload.json
exit /b 0
