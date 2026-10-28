#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t low=(uint32_t)challenge;
uint32_t high = (uint32_t)(challenge >> 32);
uint32_t step2 = (low ^ UINT32_C(0xbcbb)) + UINT32_C(4) * high;
uint32_t step0 = (UINT32_C(5) * low) ^
                 ((uint32_t)(challenge >> 16) + UINT32_C(0x40a4));

values[0] = (step0 + UINT32_C(8) * step2) ^ UINT32_C(0x1b04);
count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    values[0] = (uint32_t)((challenge >> 30) & UINT64_C(0x7f));
values[0] = values[0] ^ UINT32_C(0x19);

for (uint32_t i = 1; i < 6; i++) {
    uint32_t shift = 7 + 5 * (i - 1);
    uint32_t part = (uint32_t)((challenge >> shift) & UINT64_C(0xf));
    uint32_t mixed = UINT32_C(3) * values[i - 1] - i;

    values[i] = (part + UINT32_C(6)) ^ mixed;
}
count = 6;
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    values[0] = (uint32_t)(challenge & UINT64_C(7));

uint32_t x = (uint32_t)(challenge >> 32) ^
             (uint32_t)(challenge >> 3);

switch (values[0]) {
case 0:
    values[1] = (x ^ UINT32_C(0xc604)) + (x >> 1);
    break;
case 1:
    values[1] = (x ^ UINT32_C(0x42e5)) + (x >> 1);
    break;
case 2:
    values[1] = UINT32_C(3) * x + UINT32_C(0xae0a);
    break;
case 3:
    values[1] = UINT32_C(5) * x + UINT32_C(0x997c);
    break;
case 4:
    values[1] = (UINT32_C(4) * x) ^ UINT32_C(0x87da);
    break;
case 5:
    values[1] = (UINT32_C(4) * x) ^ UINT32_C(0xf80f);
    break;
case 6:
    values[1] = UINT32_C(5) * x + UINT32_C(0x1b67f);
    break;
case 7:
    values[1] = UINT32_C(5) * x + UINT32_C(0x3bd5d);
    break;
}

    count = 2;
    return write_answer(out, cap, values, count);
}

static uint32_t phase4_walk(uint32_t target, uint32_t low,
                            uint32_t high, uint32_t x) {
    uint32_t mid = low + (high - low) / 2;
    uint32_t mixed = mid + x;

    if (target == mid) {
        return (mid + UINT32_C(0xd8)) ^ x;
    }

    if (target < mid) {
        uint32_t result = phase4_walk(target, low, mid - 1, x);
        return (result * UINT32_C(5) ^ mixed) ^ UINT32_C(0x13);
    }

    uint32_t result = phase4_walk(target, mid + 1, high, x);
    return (result * UINT32_C(5) ^ mixed) ^ UINT32_C(0x17);
}
int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t target = (uint32_t)(challenge & UINT64_C(0x7f));
uint32_t x = (uint32_t)(challenge >> 7) ^
             (uint32_t)(challenge >> 32);

values[0] = phase4_walk(target, 0, 127, x);
count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
 static const uint32_t node_a[12] = {
    0x6421, 0xc6c5, 0x4b78, 0x011b,
    0x033c, 0xc858, 0x5190, 0xfb06,
    0x2690, 0x2550, 0xeea4, 0x7bdf
};

static const uint32_t node_b[12] = {
    0x087d, 0x0105, 0x0a2b, 0x0737,
    0x056d, 0x042c, 0x091b, 0x0340,
    0x0b2b, 0x022a, 0x0055, 0x0622
};

static const int next_node[12] = {
    3, 8, 7, 11, 6, 0, -1, 5, 2, 1, 4, 10
};

static const int head[8] = {9, 1, 8, 2, 7, 5, 0, 3};

uint32_t start = (uint32_t)(challenge & UINT64_C(7));
uint32_t steps = (uint32_t)(challenge >> 3) & UINT32_C(3);
uint32_t x = (uint32_t)(challenge >> 6) ^
             (uint32_t)(challenge >> 32);
uint32_t result = x ^ UINT32_C(0x9f04);
int node = head[start];
uint32_t last_b = 0;

for (uint32_t i = 0; i <= steps; i++) {
    result = (result + node_a[node]) * UINT32_C(5);
    last_b = node_b[node];
    result ^= last_b;

    if (i < steps) {
        node = next_node[node];
    }
}

values[0] = result;
values[1] = last_b;
count = 2;
   return write_answer(out, cap, values, count);
}


static uint32_t phase6_apply(uint32_t op, uint32_t x) {
    switch (op) {
    case 0:
        return (x + UINT32_C(0xb429)) ^ (x >> 1);
    case 1:
        return (x >> 3) | (x << 29);
    case 2:
        return (x ^ UINT32_C(0x3c11)) + UINT32_C(8) * x;
    case 3:
        return (UINT32_C(0x84b8) + UINT32_C(4) * x) ^
               (UINT32_C(7) * x);
    case 4:
        return ((x >> 1) ^ x) + UINT32_C(0xb3c2);
    case 5:
        return ((x << 3) | (x >> 29)) ^ UINT32_C(0x84a3);
    default:
        return 0;
    }
}
int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    static const uint32_t op_for_index[8] = {4, 1, 5, 3, 2, 2, 0, 1};

uint32_t first_index = (uint32_t)(challenge & UINT64_C(7));
uint32_t second_index = (uint32_t)(challenge >> 3) & UINT32_C(7);

uint32_t x = (uint32_t)(challenge >> 6) ^
             (uint32_t)(challenge >> 32);

uint32_t first = phase6_apply(op_for_index[first_index], x);
uint32_t second = phase6_apply(
    op_for_index[second_index], first ^ UINT32_C(0x5f23));

values[0] = second + UINT32_C(0xd172);
count = 1;
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    uint32_t context_low = (uint32_t)context;
uint32_t context_high = (uint32_t)(context >> 32);
uint32_t challenge_low = (uint32_t)challenge;
uint32_t challenge_high = (uint32_t)(challenge >> 32);

uint32_t context_mix = context_low ^ context_high;
uint32_t challenge_mix = challenge_low ^ challenge_high;
uint32_t first = context_low & UINT32_C(3);

uint32_t mix_input = context_mix ^ challenge_mix;
uint32_t rotated = (mix_input >> 5) | (mix_input << 27);

uint32_t second =
    (rotated ^ UINT32_C(0xc285)) ^
    (uint32_t)(context >> 16) ^
    (first * UINT32_C(0x9080));

uint32_t third =
    second ^
    ((challenge_mix >> 5) + context_mix + first + UINT32_C(0x5205));

int n = snprintf(out, cap,
                 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN "
                 "%" PRIu32 " %" PRIu32 " %" PRIu32,
                 first, second, third);

return n < 0 || (size_t)n >= cap;

}
