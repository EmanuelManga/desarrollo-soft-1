# Desarrollo de Software

Entorno configurado para **ANSI C (C89/C90)** únicamente — sin C++ ni extensiones del estándar.

## Cursor / VS Code

Extensiones recomendadas (instalar desde el marketplace):

1. **clangd** (`llvm-vs-code-extensions.vscode-clangd`) — autocompletado y diagnósticos en C89
2. **C/C++** (`ms-vscode.cpptools`) — solo para depurar con GDB (IntelliSense desactivado en este proyecto)

El proyecto ya trae la configuración en `.vscode/` y `.clangd`. Al abrir un `.c`, el editor usa C89 estricto.

Atajos útiles:

| Acción | Atajo (Linux) |
|--------|----------------|
| Compilar archivo actual | `Ctrl+Shift+B` |
| Depurar | `F5` |

## Flags de compilación

- `-ansi` — modo ANSI C (equivalente a `-std=c89`)
- `-pedantic` — avisa si se usan extensiones fuera del estándar
- `-Wall -Wextra` — avisos adicionales

## Make (Linux)

Desde la raíz del proyecto:

```bash
make clase_01_09              # compila clase_01_09.c
make clase_condicionasl/sumar # compila clase_condicionasl/sumar.c
./clase_01_09
```

## Reglas ANSI C (C89) — qué evitar

Para que no aparezcan errores ni advertencias:

| Permitido (C89) | Evitar (C99/C++ o extensiones) |
|-----------------|----------------------------------|
| Comentarios `/* ... */` | Comentarios `//` |
| Declarar variables al inicio del bloque | Declarar variables en medio del código |
| `printf`, `scanf`, `fgets`, `fputs` | `cout`, `cin`, clases, namespaces |
| `.c` compilado con `gcc` | Compilar con `g++` |
| `for (int i = 0; ...)` en C99+ | En C89: declarar `int i;` antes del `for` |

## Linux

Desde la raíz del proyecto:

```bash
# clase_condicionasl/sumar.c
gcc -ansi -pedantic -Wall clase_condicionasl/sumar.c -o clase_condicionasl/sumar

# clase_ciclo/sumar.c
gcc -ansi -pedantic -Wall clase_ciclo/sumar.c -o clase_ciclo/sumar
```

O entrando a cada carpeta:

```bash
cd clase_condicionasl
gcc -ansi -pedantic -Wall sumar.c -o sumar
./sumar
```

```bash
cd clase_ciclo
gcc -ansi -pedantic -Wall sumar.c -o sumar
./sumar
```

## Windows

Requiere tener **GCC** instalado (por ejemplo [MinGW-w64](https://www.mingw-w64.org/) o MSYS2).

Desde la raíz del proyecto (CMD o PowerShell):

```cmd
gcc -ansi -pedantic -Wall clase_condicionasl\sumar.c -o clase_condicionasl\sumar.exe
gcc -ansi -pedantic -Wall clase_ciclo\sumar.c -o clase_ciclo\sumar.exe
```

O entrando a cada carpeta:

```cmd
cd clase_condicionasl
gcc -ansi -pedantic -Wall sumar.c -o sumar.exe
sumar.exe
```

```cmd
cd clase_ciclo
gcc -ansi -pedantic -Wall sumar.c -o sumar.exe
sumar.exe
```
