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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Includes placed here so the function is self-contained when pasted into a file.
    // (Preprocessor includes are processed before compilation, so this is valid.)
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Prepare some sample input data (large enough to exercise streaming).
    const std::string phrase = "The quick brown fox jumps over the lazy dog. ";
    std::string original;
    for (int i = 0; i < 1024; ++i) original += phrase; // ~45KB

    const unsigned char* orig_ptr = reinterpret_cast<const unsigned char*>(original.data());
    uLong orig_len = static_cast<uLong>(original.size());

    // 1) Compute checksums (adler32 and crc32).
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, orig_ptr, orig_len);
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, orig_ptr, orig_len);

    // 2) Simple one-shot compress/uncompress using compress2/uncompress.
    uLongf comp_bound = compressBound(orig_len);
    std::vector<unsigned char> comp_one_shot(comp_bound);
    uLongf comp_one_shot_len = comp_bound;
    int zret = compress2(comp_one_shot.data(), &comp_one_shot_len, orig_ptr, orig_len, Z_BEST_COMPRESSION);
    if (zret != Z_OK) return 1; // failure

    std::vector<unsigned char> decomp_one_shot(orig_len);
    uLongf decomp_one_shot_len = orig_len;
    zret = uncompress(decomp_one_shot.data(), &decomp_one_shot_len, comp_one_shot.data(), comp_one_shot_len);
    if (zret != Z_OK) return 2; // failure
    if (decomp_one_shot_len != orig_len) return 3;
    if (std::memcmp(decomp_one_shot.data(), orig_ptr, orig_len) != 0) return 4;

    // 3) Streaming deflate (z_stream + deflateInit / deflate / deflateEnd)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    zret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (zret != Z_OK) return 5;

    // Provide entire input as next_in (demonstrates streaming API; deflate will be called repeatedly to drain output).
    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(orig_ptr));
    defstream.avail_in = orig_len;

    const size_t CHUNK = 16384;
    std::vector<unsigned char> comp_stream;
    comp_stream.reserve(static_cast<size_t>(comp_one_shot_len));

    unsigned char outbuf[CHUNK];
    int flush_flag = Z_FINISH; // we supply all input at once, indicate finished
    do {
        defstream.next_out = outbuf;
        defstream.avail_out = CHUNK;
        zret = deflate(&defstream, flush_flag);
        if (zret == Z_STREAM_ERROR) {
            deflateEnd(&defstream);
            return 6;
        }
        size_t have = CHUNK - defstream.avail_out;
        comp_stream.insert(comp_stream.end(), outbuf, outbuf + have);
    } while (zret != Z_STREAM_END);
    deflateEnd(&defstream);

    if (comp_stream.empty()) return 7;

    // 4) Streaming inflate (z_stream + inflateInit / inflate / inflateEnd)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    zret = inflateInit(&infstream);
    if (zret != Z_OK) return 8;

    infstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(comp_stream.data()));
    infstream.avail_in = static_cast<uInt>(comp_stream.size());

    std::vector<unsigned char> decomp_stream;
    decomp_stream.reserve(original.size());

    do {
        infstream.next_out = outbuf;
        infstream.avail_out = CHUNK;
        zret = inflate(&infstream, Z_NO_FLUSH);
        if (zret != Z_OK && zret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return 9;
        }
        size_t have = CHUNK - infstream.avail_out;
        decomp_stream.insert(decomp_stream.end(), outbuf, outbuf + have);
    } while (zret != Z_STREAM_END);

    inflateEnd(&infstream);

    if (decomp_stream.size() != original.size()) return 10;
    if (std::memcmp(decomp_stream.data(), orig_ptr, orig_len) != 0) return 11;

    // 5) Verify checksums still match computed values on decompressed data
    uLong adler_check = adler32(0L, Z_NULL, 0);
    adler_check = adler32(adler_check, decomp_stream.data(), static_cast<uInt>(decomp_stream.size()));
    if (adler_check != adler) return 12;

    uLong crc_check = crc32(0L, Z_NULL, 0);
    crc_check = crc32(crc_check, decomp_stream.data(), static_cast<uInt>(decomp_stream.size()));
    if (crc_check != crc) return 13;

    // If we reached here, all zlib API usages above succeeded and verified.
    return 66;
}