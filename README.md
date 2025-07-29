# Asynchronous-Sleep-Function-in-C-WinAPI
Asynchronous Sleep function in C that can be interrupted by keyboard/mouse events. Provides timed waiting with user interaction capability and returns elapsed time or timeout status. Alternative to Windows' Sleep() function.

## Descrição
A função `SleepAssinc()` oferece uma alternativa assíncrona à função tradicional `Sleep()` do Windows, permitindo que o tempo de espera seja interrompido por eventos de teclado.

## Funcionamento
- **Tempo de espera configurável** em segundos.
- **Pode ser interrompida** por eventos de teclado.
- **Retorna informações** sobre tempo decorrido (se interrompida) ou status de tempo esgotado.

## Implementação
- Utiliza `WaitForSingleObject()` e `PeekConsoleInput` para monitorar eventos de entrada. `WaitForSingleObject()` aguarda por um evento no buffer de entrada, enquanto `PeekConsoleInput` apenas "espia" o que há na stdin para poder validar um evento do teclado, sem de fato tentar ler e consumir o que estiver lá.
- Calcula tempo decorrido com precisão usando `_ftime()`.

## Vantagens
- Mais flexível que `Sleep()`
- Não bloqueia completamente a aplicação
- Permite interação do usuário durante a espera
