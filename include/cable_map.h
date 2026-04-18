#ifndef CABLE_MAP_H
#define CABLE_MAP_H

/*
  Pin mapping and timing settings
  Author: Amir Mobasheraghdam
*/

#define CHANNEL_COUNT 8
#define STABILIZE_DELAY_MS 15
#define LOOP_DELAY_MS 1200

// TX = sender side
const uint8_t TX_PINS[CHANNEL_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9};

// RX = receiver side
const uint8_t RX_PINS[CHANNEL_COUNT] = {10, 11, 12, 13, A0, A1, A2, A3};

// Straight-through cable map
const uint8_t EXPECTED_MAP[CHANNEL_COUNT] = {0, 1, 2, 3, 4, 5, 6, 7};

#endif
