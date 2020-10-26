#pragma once

enum unit_measure {B, KB, MB, GB}; // #werecool 💩

long to_bytes(int amount, enum unit_measure unit);

unsigned char amount_octets(long total_bytes);

void size_in_octets(long file_size, char* size_in_octets, unsigned arr_size);

long octets_to_size(char* octets, int octets_length);