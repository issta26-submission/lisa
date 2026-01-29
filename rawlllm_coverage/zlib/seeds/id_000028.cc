#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include zlib header (can be also placed at top of the file)
    #include <zlib.h>

    // Prepare input data (a repeated phrase to give compressible content)
    const char *text =
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. ";
    // compute length without relying on <cstring>
    uLong in_len = 0;
    while (text[in_len] != '\0') ++in_len;

    // Allocate compressed buffer (use compressBound to be safe)
    uLong comp_bound = compressBound(in_len);
    unsigned char *comp_buf = new unsigned char[comp_bound];

    // Set up deflate (compression) stream
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.next_in = Z_NULL;
    defstream.avail_in = 0;
    defstream.next_out = Z_NULL;
    defstream.avail_out = 0;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        delete[] comp_buf;
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(text));
    defstream.avail_in = in_len;
    defstream.next_out = comp_buf;
    defstream.avail_out = comp_bound;

    // Perform compression (expect Z_STREAM_END when finished)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        delete[] comp_buf;
        return -2;
    }
    uLong comp_size = defstream.total_out;

    // Clean up deflate
    deflateEnd(&defstream);

    // Allocate buffer for decompression (we know original size)
    unsigned char *decomp_buf = new unsigned char[in_len];

    // Set up inflate (decompression) stream
    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.next_in = Z_NULL;
    infstream.avail_in = 0;
    infstream.next_out = Z_NULL;
    infstream.avail_out = 0;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        delete[] comp_buf;
        delete[] decomp_buf;
        return -3;
    }

    infstream.next_in = comp_buf;
    infstream.avail_in = comp_size;
    infstream.next_out = decomp_buf;
    infstream.avail_out = in_len;

    // Perform decompression (expect Z_STREAM_END)
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        delete[] comp_buf;
        delete[] decomp_buf;
        return -4;
    }

    // Finalize inflate
    inflateEnd(&infstream);

    // Verify decompressed length matches original
    if (infstream.total_out != in_len) {
        delete[] comp_buf;
        delete[] decomp_buf;
        return -5;
    }

    // Verify decompressed content matches original (byte-by-byte)
    for (uLong i = 0; i < in_len; ++i) {
        if (decomp_buf[i] != static_cast<unsigned char>(text[i])) {
            delete[] comp_buf;
            delete[] decomp_buf;
            return -6;
        }
    }

    // Clean up
    delete[] comp_buf;
    delete[] decomp_buf;

    // Success: return the required value
    return 66;
}