#pragma once

enum unit_measure {B, KB, MB, GB, TB, PB, EB, ZB, YB}; // #werecool 💩

long to_bytes(int amount, enum unit_measure unit);