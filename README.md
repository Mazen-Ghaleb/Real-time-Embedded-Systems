# ON-OFF Temperature controller

The project is a Temperature controller made using Tivaware for Tiva C (tm4c123gh6pm) and using FreeRTOS. The temperature sensor value is read through the Tiva’s built-in ADC. The value is then sent to the connected LCD after being encoded the correct way. Multiple synchronization and data-safety patterns are being used from the FreeRTOS layer (e.g. semaphores and queues) to have maximum reliability.

This repo contains earlier commits for the labs and finally a project.
