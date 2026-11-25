#include <stdint.h>

#define RCC_AHB1ENR	(*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER	(*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR	(*(volatile uint32_t *)0x40020014)

void delay(uint32_t loops);
void Reset_Handler(void);

// Vectortabelle am Anfang des Flash-Speichers (Wert1 = Top of Stack, Wert2 = Adresse der Funktion Reset_Handler)
__attribute__((section(".isr_vector"))) const uint32_t vectorTable[2] = { 0x20018000, (uint32_t)(uintptr_t)&Reset_Handler };

void delay(uint32_t cycles)
{
	/*
	 * Berechnung für ein delay von 0,5 s
	 *
	 * 16 MHz --> T = 62,5 ns
	 * 10 Takte pro Schleifendurchlauf
	 *
	 * n = t / T / 10
	 * n = 0,5 s / 62,5 ns / 10
	 * n = 8.000.000 / 10
	 * n = 1142857
	 */
	
	volatile uint32_t counter = 0;
	uint32_t loops = cycles / 10;

	while (counter < loops)
	{
		counter++;
	}
}

void Reset_Handler(void)
{
	// Takt für GPIOA einschalten
	RCC_AHB1ENR |= (1U << 0);

	//  RCC_AHB1ENR einlesen um der Peripherie Zeit zu geben bereits zu sein (Erratum ES0299, 2.2.7)
	(void)RCC_AHB1ENR;

	// PA5 als Aufgang konfigurieren (ohne Takt würde das nicht klappen)
	uint32_t pin = 5;
	GPIOA_MODER &= ~(3U << (pin * 2));
	GPIOA_MODER |= (1U << (pin * 2));

	// PA5 auf HIGH setzen
	GPIOA_ODR |= (1U << pin);

	while(1)
	{

		// PA5 umschalten
		GPIOA_ODR ^= (1U << pin);
		
		delay(8000000); // delay von 0,5 s
	}
}

