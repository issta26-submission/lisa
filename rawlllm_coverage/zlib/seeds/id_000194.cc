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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {

    // Simple test payload
    const char* sample = "The quick brown fox jumps over the lazy dog.";
    const size_t sample_len = std::strlen(sample);

    // 1) Simple compress / uncompress using the convenience API
    uLong bound = compressBound(static_cast<uLong>(sample_len));
    std::vector<Bytef> comp_buf(bound);
    uLong comp_len = bound;
    int ret = compress(comp_buf.data(), &comp_len, reinterpret_cast<const Bytef*>(sample), static_cast<uLong>(sample_len));
    if (ret != Z_OK) return 1;

    std::vector<Bytef> uncomp_buf(sample_len);
    uLong uncomp_len = static_cast<uLong>(sample_len);
    ret = uncompress(uncomp_buf.data(), &uncomp_len, comp_buf.data(), comp_len);
    if (ret != Z_OK) return 2;
    if (uncomp_len != sample_len) return 3;
    if (std::memcmp(uncomp_buf.data(), sample, sample_len) != 0) return 4;

    // 2) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));
    (void)crc; // keep compilers from warning if not otherwise used
    (void)adl;

    // 3) Streaming deflate (z_stream, deflateInit / deflate / deflateEnd)
    // Build a larger input by repeating the sample so streaming is exercised.
    std::string big_input;
    big_input.reserve(sample_len * 100);
    for (int i = 0; i < 100; ++i) big_input.append(sample, sample_len);

    z_stream def_strm;
    std::memset(&def_strm, 0, sizeof(def_strm));
    ret = deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    def_strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(big_input.data()));
    def_strm.avail_in = static_cast<uInt>(big_input.size());

    std::vector<Bytef> stream_comp;
    const size_t CHUNK = 1024;
    // Keep calling deflate until we get Z_STREAM_END
    for (;;) {
        size_t cur_size = stream_comp.size();
        stream_comp.resize(cur_size + CHUNK);
        def_strm.next_out = stream_comp.data() + cur_size;
        def_strm.avail_out = static_cast<uInt>(CHUNK);

        ret = deflate(&def_strm, Z_FINISH);
        if (ret == Z_STREAM_END) {
            size_t used = cur_size + (CHUNK - def_strm.avail_out);
            stream_comp.resize(used);
            break;
        } else if (ret == Z_OK) {
            // consumed output chunk, continue to obtain more output
            size_t used = cur_size + (CHUNK - def_strm.avail_out);
            stream_comp.resize(used);
            continue;
        } else {
            deflateEnd(&def_strm);
            return 6;
        }
    }
    deflateEnd(&def_strm);

    // 4) Streaming inflate (inflateInit / inflate / inflateEnd)
    z_stream inf_strm;
    std::memset(&inf_strm, 0, sizeof(inf_strm));
    ret = inflateInit(&inf_strm);
    if (ret != Z_OK) return 7;

    inf_strm.next_in = stream_comp.data();
    inf_strm.avail_in = static_cast<uInt>(stream_comp.size());

    std::vector<Bytef> stream_uncomp;
    for (;;) {
        size_t cur_size = stream_uncomp.size();
        stream_uncomp.resize(cur_size + CHUNK);
        inf_strm.next_out = stream_uncomp.data() + cur_size;
        inf_strm.avail_out = static_cast<uInt>(CHUNK);

        ret = inflate(&inf_strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            size_t used = cur_size + (CHUNK - inf_strm.avail_out);
            stream_uncomp.resize(used);
            break;
        } else if (ret == Z_OK) {
            size_t used = cur_size + (CHUNK - inf_strm.avail_out);
            stream_uncomp.resize(used);
            continue;
        } else {
            inflateEnd(&inf_strm);
            return 8;
        }
    }
    inflateEnd(&inf_strm);

    // 5) Verify streaming round-trip equals original big_input
    if (stream_uncomp.size() != big_input.size()) return 9;
    if (std::memcmp(stream_uncomp.data(), big_input.data(), big_input.size()) != 0) return 10;

    // 6) Query zlib version string as a lightweight API check
    const char* vz = zlibVersion();
    if (!vz) return 11;

    // If we reach here, all zlib API usages in this sequence succeeded
    return 66;
}