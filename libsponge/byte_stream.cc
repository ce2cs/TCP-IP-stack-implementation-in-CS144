#include <sstream>
#include <iostream>

#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) :
    buffer(),
    capa(capacity),
    ended(false),
    written_count(0),
    read_count(0)
{}

size_t ByteStream::write(const string &data) {
    if (ended) return 0;
    if (buffer.size() == capa) return 0;
    size_t i = 0;
    for (; buffer.size() < capa && i < data.length(); i++, written_count++) {
        buffer.push_back(data[i]);
    }
    return i;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    stringstream ss;
    for (size_t i = 0; i < min(len, buffer.size()); i++) {
        ss << buffer[i];
    }
    return ss.str();
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    for (size_t i = 0; i < len && !buffer.empty(); i++, read_count++) {
        buffer.pop_front();
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned //! \returns a string
std::string ByteStream::read(const size_t len) {
    stringstream ss;
    for (size_t i = 0; i < len && !buffer.empty(); i++) {
        ss << buffer.front();
        buffer.pop_front();
        read_count ++;
    }
    return ss.str();
}

void ByteStream::end_input() { ended = true;}

bool ByteStream::input_ended() const { return ended; }

size_t ByteStream::buffer_size() const { return buffer.size(); }

bool ByteStream::buffer_empty() const { return buffer.empty(); }

bool ByteStream::eof() const { return ended && buffer.empty(); }

size_t ByteStream::bytes_written() const { return written_count; }

size_t ByteStream::bytes_read() const { return read_count; }

size_t ByteStream::remaining_capacity() const { return capa - buffer_size(); }
