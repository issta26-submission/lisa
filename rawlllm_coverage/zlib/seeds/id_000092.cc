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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress/decompress
    const char* original = "The quick brown fox jumps over the lazy dog. zlib API sequence test.";
    size_t original_len = std::strlen(original);

    // --- 1) Simple one-shot compress/uncompress using compress()/uncompress() ---
    // compress()
    uLong source_len = static_cast<uLong>(original_len);
    uLongf comp_bound = compressBound(source_len);
    std::vector<Bytef> comp_buf(comp_bound);
    uLongf comp_size = comp_bound;
    int zret = compress(comp_buf.data(), &comp_size, reinterpret_cast<const Bytef*>(original), source_len);
    if (zret != Z_OK) return -1;

    // uncompress()
    std::vector<Bytef> uncomp_buf(source_len + 1); // +1 for safety/terminator
    uLongf uncomplen = source_len;
    zret = uncompress(uncomp_buf.data(), &uncomplen, comp_buf.data(), comp_size);
    if (zret != Z_OK) return -2;
    // verify
    if (uncomplen != source_len) return -3;
    if (std::memcmp(uncomp_buf.data(), original, source_len) != 0) return -4;

    // --- 2) Streaming compress using deflateInit/deflate/deflateEnd ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) return -5;
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    defstream.avail_in = static_cast<uInt>(source_len);

    uLongf stream_comp_bound = compressBound(source_len);
    std::vector<Bytef> stream_comp_buf(stream_comp_bound);
    defstream.next_out = stream_comp_buf.data();
    defstream.avail_out = static_cast<uInt>(stream_comp_buf.size());

    int dret = deflate(&defstream, Z_FINISH);
    if (dret != Z_STREAM_END && dret != Z_OK) {
        deflateEnd(&defstream);
        return -6;
    }
    uLongf stream_comp_size = stream_comp_buf.size() - defstream.avail_out;
    if (deflateEnd(&defstream) != Z_OK) return -7;

    // --- 3) Streaming inflate using inflateInit/inflate/inflateEnd ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    if (inflateInit(&infstream) != Z_OK) return -8;
    infstream.next_in = stream_comp_buf.data();
    infstream.avail_in = static_cast<uInt>(stream_comp_size);

    std::vector<Bytef> stream_uncomp_buf(source_len + 1);
    infstream.next_out = stream_uncomp_buf.data();
    infstream.avail_out = static_cast<uInt>(stream_uncomp_buf.size());

    int iret = inflate(&infstream, Z_FINISH);
    if (iret != Z_STREAM_END && iret != Z_OK) {
        inflateEnd(&infstream);
        return -9;
    }
    uLongf stream_uncomp_size = stream_uncomp_buf.size() - infstream.avail_out;
    if (inflateEnd(&infstream) != Z_OK) return -10;

    if (stream_uncomp_size != source_len) return -11;
    if (std::memcmp(stream_uncomp_buf.data(), original, source_len) != 0) return -12;

    // --- 4) gz* file-based API: gzopen/gzwrite/gzread/gzclose ---
    const char* gz_filename = "test_zlib_api_sequence.gz";
    gzFile gz = gzopen(gz_filename, "wb");
    if (!gz) return -13;
    int written = gzwrite(gz, original, static_cast<unsigned int>(source_len));
    if (written <= 0 || static_cast<size_t>(written) != source_len) {
        gzclose(gz);
        return -14;
    }
    if (gzclose(gz) != Z_OK) return -15;

    gz = gzopen(gz_filename, "rb");
    if (!gz) return -16;
    std::vector<char> gz_read_buf(source_len + 1);
    int read_bytes = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(source_len));
    if (read_bytes <= 0 || static_cast<size_t>(read_bytes) != source_len) {
        gzclose(gz);
        return -17;
    }
    if (gzclose(gz) != Z_OK) return -18;
    if (std::memcmp(gz_read_buf.data(), original, source_len) != 0) return -19;

    // Optionally remove the temporary file (ignore result)
    std::remove(gz_filename);

    // --- 5) Check CRC and Adler checksum APIs ---
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(source_len));
    if (crc == 0) return -20; // unlikely but check that value computed

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(source_len));
    if (adl == 0) return -21;

    // --- 6) zlibVersion() usage (no runtime check necessary) ---
    const char* ver = zlibVersion();
    if (!ver || ver[0] == '\0') return -22;

    // All tests passed
    return 66;
}