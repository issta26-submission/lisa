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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so this function is self-contained when pasted into a file.
    // (Including inside a function is allowed by the preprocessor.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>

    // Original data to compress/decompress
    const std::string original =
        "The quick brown fox jumps over the lazy dog. "
        "This is a small test string repeated several times to make streaming meaningful. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    const Bytef* orig_data = reinterpret_cast<const Bytef*>(original.data());
    uLong orig_len = static_cast<uLong>(original.size());

    // 1) compress() / uncompress() simple usage
    uLongf bound = compressBound(orig_len);
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_size = bound;
    int ret = compress(comp_buf.data(), &comp_size, orig_data, orig_len);
    if (ret != Z_OK) return -1; // compression failed

    // Check uncompress
    std::vector<Bytef> decomp_buf(orig_len);
    uLongf decomp_size = orig_len;
    ret = uncompress(decomp_buf.data(), &decomp_size, comp_buf.data(), comp_size);
    if (ret != Z_OK) return -2; // uncompress failed
    if (decomp_size != orig_len) return -3;
    if (std::memcmp(decomp_buf.data(), orig_data, orig_len) != 0) return -4;

    // 2) compute checksums: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, orig_data, static_cast<uInt>(orig_len));
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, orig_data, static_cast<uInt>(orig_len));
    // (We don't assert particular values; just ensure API calls succeed. Values are stored.)

    // 3) compress2 (explicit compression level)
    std::vector<Bytef> comp2_buf(compressBound(orig_len));
    uLongf comp2_size = static_cast<uLongf>(comp2_buf.size());
    ret = compress2(comp2_buf.data(), &comp2_size, orig_data, orig_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -5;

    // 4) streaming deflate (z_stream) and streaming inflate to validate streaming APIs.
    // Deflate (streaming)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -6;

    // Provide all input at once (but use streaming output chunks)
    defstream.next_in = const_cast<Bytef*>(orig_data);
    defstream.avail_in = static_cast<uInt>(orig_len);

    std::vector<Bytef> stream_comp; // will hold streaming-compressed data
    const size_t OUT_CHUNK = 128;
    Bytef outchunk[OUT_CHUNK];

    // First compress with Z_NO_FLUSH while input remains
    while (defstream.avail_in > 0) {
        defstream.next_out = outchunk;
        defstream.avail_out = OUT_CHUNK;
        ret = deflate(&defstream, Z_NO_FLUSH);
        if (ret != Z_OK) {
            deflateEnd(&defstream);
            return -7;
        }
        size_t have = OUT_CHUNK - defstream.avail_out;
        stream_comp.insert(stream_comp.end(), outchunk, outchunk + have);
    }

    // Finish with Z_FINISH
    for (;;) {
        defstream.next_out = outchunk;
        defstream.avail_out = OUT_CHUNK;
        ret = deflate(&defstream, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -8;
        }
        size_t have = OUT_CHUNK - defstream.avail_out;
        stream_comp.insert(stream_comp.end(), outchunk, outchunk + have);
        if (ret == Z_STREAM_END) break;
    }

    deflateEnd(&defstream);

    // Inflate (streaming) the stream_comp back to original
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -9;

    infstream.next_in = stream_comp.empty() ? Z_NULL : stream_comp.data();
    infstream.avail_in = static_cast<uInt>(stream_comp.size());

    std::vector<Bytef> stream_decomp;
    const size_t IN_CHUNK = 64;
    Bytef inchunk[IN_CHUNK];

    // Inflate until stream end
    for (;;) {
        infstream.next_out = inchunk;
        infstream.avail_out = IN_CHUNK;
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -10;
        }
        size_t have = IN_CHUNK - infstream.avail_out;
        stream_decomp.insert(stream_decomp.end(), inchunk, inchunk + have);
        if (ret == Z_STREAM_END) break;
        if (infstream.avail_in == 0 && have == 0) {
            // No progress and no input left
            break;
        }
    }

    inflateEnd(&infstream);

    // Validate streaming roundtrip
    if (stream_decomp.size() != original.size()) return -11;
    if (std::memcmp(stream_decomp.data(), original.data(), original.size()) != 0) return -12;

    // 5) zlib version and compressBound usage (already used compressBound earlier)
    const char* ver = zlibVersion();
    if (!ver) return -13; // improbable

    // If all operations succeeded, return 66 as requested.
    (void)crc; // silence unused-variable warnings in some build configs
    (void)adl;
    (void)comp_size;
    (void)comp2_size;
    (void)ver;
    return 66;
}