@echo off
REM Script para procesar en paralelo con diferente numero de hilos y diferente cantidad de datos
cls
echo Compilando...
gcc .\src\c\paralelo.c -o .\src\c\paralelo -fopenmp
gcc .\src\c\ejemplo_6_8.c -o .\src\c\ejemplo_6_8

if %errorlevel% neq 0 (
    echo Error en la compilacion
    pause
    exit /b %errorlevel%
)

echo Compilacion exitosa...
echo Ejecutando procesamiento_multihilo...

REM Eliminamos el archivo de datos si existe
del .\src\datos\paralelo.csv
del .\src\datos\secuencial.csv

REM Variables para manejar el rango en el numero de hilos
set NH1=1
set NH2=12

REM Variables para manejar el rango en el numero de datos
set ND1=10000
set ND2=50000
set INC=10000

REM Procesamos en secuencial
for /L %%i IN (%ND1%,%INC%,%ND2%) DO (
  echo Procesando %%i datos
  .\src\c\ejemplo_6_8.exe %%i
)

REM Procesamos en paralelo
for /L %%i IN (%ND1%,%INC%,%ND2%) DO (
  echo Procesando %%i datos
  for /L %%k IN (%NH1%,1,%NH2%) DO (
    .\src\c\paralelo.exe %%k %%i
  )
)

exit