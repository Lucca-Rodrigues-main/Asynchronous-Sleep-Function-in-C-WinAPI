#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <windows.h>

long SleepAssinc(long segs); // Aguarda algo do teclado por um intervalo de tempo
int PausaEnter(void); // Aguarda pela tecla Enter

int main()
{
    long tempo;

    tempo = SleepAssinc(10);

    if(tempo == -1) {
        printf("Tempo esgotado!\n");
    } else {
        printf("Interrompido! Tempo decorrido: %ld segundos\n", tempo);
    }

    printf("\nPressione Enter para finalizar...");
    PausaEnter();
    return 0;
}

long SleepAssinc(long segs) // Aguarda algo do teclado por um intervalo de tempo
{
    INPUT_RECORD record;
    HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
    int total = 0;
    unsigned short mili1, mili2;
    struct _timeb timebuffer;
    long totalms1, totalms2;
    long tempo_decorrido;
    long intervalo_ms = segs * 1000;

    // Obtendo o tempo inicial
    _ftime(&timebuffer);
    mili1 = timebuffer.millitm;
    totalms1 = (timebuffer.time % 86400) * 1000 + mili1;

    do {
        // Verifica se ha entrada disponivel sem bloquear
        if (WaitForSingleObject(console, 0) == WAIT_OBJECT_0) {
            if (!PeekConsoleInput(console, &record, 1, (LPDWORD)&total))
                return -1; // Erro ao verificar entrada

            if (total > 0) {
                if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
                    // Tecla pressionada - calcula tempo decorrido
                    _ftime(&timebuffer);
                    mili2 = timebuffer.millitm;
                    totalms2 = (timebuffer.time % 86400) * 1000 + mili2;
                    tempo_decorrido = totalms2 - totalms1;
                    FlushConsoleInputBuffer(console);
                    return tempo_decorrido / 1000;
                }
                // Remove o evento do buffer se nao for de tecla pressionada
                FlushConsoleInputBuffer(console);
            }
        }

        // Verifica se o tempo acabou
        _ftime(&timebuffer);
        mili2 = timebuffer.millitm;
        totalms2 = (timebuffer.time % 86400) * 1000 + mili2;
        tempo_decorrido = totalms2 - totalms1;

        if (tempo_decorrido >= intervalo_ms) {
            return -1; // Tempo esgotado
        }

    } while (1);
}

int PausaEnter(void) // Aguarda pela tecla Enter
{
	INPUT_RECORD record;
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	int total = 0;
	// Tecla
	WORD v = 0;

	do {
		FlushConsoleInputBuffer(console);
		WaitForSingleObject(console, INFINITE);

		/* Se PeekConsoleInput() for bem-sucedida,
	    o valor retornado será diferente de zero */
		if(!PeekConsoleInput(console, &record, 1, (LPDWORD)&total))
            return 1;
		if (total > 0)
            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
            {
                v = record.Event.KeyEvent.wVirtualKeyCode;
                FlushConsoleInputBuffer(console);
                /* Se o evento de entrada for um evento do
                teclado e o código de chave virtual tiver
                o valor da tecla Enter, retorna 0 */
                if (v == VK_RETURN) return 0;
            }
	} while(1);
}
