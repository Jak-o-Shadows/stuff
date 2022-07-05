// Define Functions
#define IsHigh(BIT, PORT) ((PORT & (1 << BIT)) != 0)
#define IsLow(BIT, PORT) ((PORT & (1 << BIT)) == 0)
#define SetBit(BIT, PORT) PORT |= (1 << BIT)
#define ClearBit(BIT, PORT) PORT &= ~(1 << BIT)
#define InvertBit(BIT, PORT) PORT ^= (1 << BIT)

#define JoinInt8(BYTEH, BYTEL) ( ((int16_t) BYTEH << 8) | BYTEL )

// Specific to the comms protocol
#define PackBits(V, P) (((V << 5) & 0xFF00) | (V & 0x07) | 0x8000 | (P << 3))