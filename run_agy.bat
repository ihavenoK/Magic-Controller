@echo off
powershell -NoExit -Command "$env:HTTP_PROXY='http://127.0.0.1:2334'; $env:HTTPS_PROXY='http://127.0.0.1:2334'; agy"
