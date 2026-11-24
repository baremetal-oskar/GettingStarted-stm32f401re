#include <stdint.h>

#define RCC_AHB1ENR	(*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER	(*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR	(*(volatile uint32_t *)0x40020014)

void Reset_Handler(void);

// Vectortabelle am Anfang des Flash-Speichers (Wert1 = Top of Stack, Wert2 = Adresse der Funktion Reset_Handler)
__attribute__((section(".isr_vector"))) const uint32_t vectorTable[2] = { 0x20018000, (uint32_t)(uintptr_t)&Reset_Handler };



void Reset_Handler(void)
{
	volatile uint32_t counter = 0;

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
		counter++;
	}
}

