#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    return isn + n % (1UL << 32);
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    auto raw_v = n - isn;
    uint64_t start = 0;
    uint64_t end = (1UL << 32) - 1;
    while (start + 1 < end) {
        auto mid = start + (end - start) / 2;
        auto mid_raw = mid * (1UL << 32) + raw_v;
        if (checkpoint > mid_raw) {
            start = mid;
        } else {
            end = mid;
        }
    }
    auto start_raw = start * (1UL << 32) + raw_v;
    auto end_raw = end * (1UL << 32) + raw_v;
    auto start_diff = max(start_raw, checkpoint) - min(start_raw, checkpoint);
    auto end_diff = max(end_raw, checkpoint) - min(end_raw, checkpoint);
    if (start_diff < end_diff) {
        return start_raw;
    } else {
        return end_raw;
    }
}

