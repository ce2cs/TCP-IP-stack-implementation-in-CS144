#include <iostream>
#include <sstream>
#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) :
    _output(capacity),
    _capacity(capacity),
    _eof(false),
    _window(capacity, '\0'),
    _received(capacity, false),
    _first_unread(0),
    _first_unassembled(0),
    _last_index(0)
{}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof) {
        _eof = true;
        _last_index = index + data.length();
    }
    if (!_if_overlap(data, index)) return;
    _fill_window(data, index);
    _update_assembled();
    _push_unread();

    if (_eof && _first_unread == _last_index) _output.end_input();
}

bool StreamReassembler::_if_overlap(const string &data, const size_t index) const {
    if (index + data.length() < _first_unread || _first_unread + _capacity <= index) return false;
    return true;
}

void StreamReassembler::_fill_window(const string &data, const size_t index) {
    for (size_t i = 0; i < data.length(); i++) {
        int window_idx = i + index - _first_unread;
        if (window_idx < 0 || window_idx >= static_cast<int>(_capacity)) continue;
        if (_received[window_idx]) continue;
        _received[window_idx] = true;
        _window[window_idx] = data[i];
    }
}

void StreamReassembler::_update_assembled() {
    for (size_t i = _first_unassembled - _first_unread; i < _capacity; i++) {
        if (_received[i]) _first_unassembled ++;
        else break;
    }
}

void StreamReassembler::_push_unread() {
    stringstream ss;
    size_t can_push = min(_first_unassembled - _first_unread, _output.remaining_capacity());
    for (size_t i = 0; i < can_push; i++) {
        ss << _window.front();
        _window.pop_front();
        _window.push_back('\0');
        _received.pop_front();
        _received.push_back(false);
        _first_unread ++;
    }
    _output.write(ss.str());
}

size_t StreamReassembler::unassembled_bytes() const {
    size_t ans = 0;
    for (size_t i = _first_unassembled - _first_unread; i < _capacity; i++) {
        if (_received[i]) ans ++;
    }
    return ans;
}

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
