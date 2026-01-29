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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when inserted into a translation unit.
    // Preprocessor directives are valid inside a function.
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>
    #include <cstdio>
    #include <cstdlib>

    // Prepare sample input data (repeated to make compression meaningful)
    std::string input;
    const char* sample = "The quick brown fox jumps over the lazy dog. ";
    for (int i = 0; i < 200; ++i) input += sample;
    const size_t input_len_ssize = input.size();
    if (input_len_ssize == 0) return -1;
    uLong input_len = static_cast<uLong>(input_len_ssize);

    // 1) Compute checksums: adler32 and crc32
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), input_len);
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), input_len);

    // Simple sanity check: both functions must return non-zero for this non-empty input
    if (adl == 0 || crc == 0) return -2;

    // 2) Use compress2 / uncompress (simple convenience API)
    uLong bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(bound);
    uLong comp_size = bound;
    int zres = compress2(comp_buf.data(), &comp_size,
                         reinterpret_cast<const Bytef*>(input.data()), input_len,
                         Z_BEST_COMPRESSION);
    if (zres != Z_OK) return -3;
    comp_buf.resize(comp_size);

    // Uncompress and verify
    std::vector<Bytef> uncomp_buf(input_len);
    uLong uncomp_size = input_len;
    zres = uncompress(uncomp_buf.data(), &uncomp_size, comp_buf.data(), comp_size);
    if (zres != Z_OK) return -4;
    if (uncomp_size != input_len) return -5;
    if (std::memcmp(uncomp_buf.data(), input.data(), input_len) != 0) return -6;

    // 3) Use deflate/inflate streaming API
    // Deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    zres = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (zres != Z_OK) return -7;

    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    defstream.avail_in = input_len;

    uLong stream_bound = compressBound(input_len);
    std::vector<Bytef> stream_comp(stream_bound);
    defstream.next_out = stream_comp.data();
    defstream.avail_out = stream_bound;

    zres = deflate(&defstream, Z_FINISH);
    if (zres != Z_STREAM_END) { deflateEnd(&defstream); return -8; }
    uLong stream_comp_size = stream_bound - defstream.avail_out;
    zres = deflateEnd(&defstream);
    if (zres != Z_OK) return -9;
    stream_comp.resize(stream_comp_size);

    // Inflate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.next_in = stream_comp.data();
    infstream.avail_in = stream_comp_size;

    zres = inflateInit(&infstream);
    if (zres != Z_OK) return -10;

    std::vector<Bytef> stream_uncomp(input_len);
    infstream.next_out = stream_uncomp.data();
    infstream.avail_out = input_len;

    zres = inflate(&infstream, Z_FINISH);
    if (zres != Z_STREAM_END) { inflateEnd(&infstream); return -11; }
    uLong stream_uncomp_size = input_len - infstream.avail_out;
    zres = inflateEnd(&infstream);
    if (zres != Z_OK) return -12;
    if (stream_uncomp_size != input_len) return -13;
    if (std::memcmp(stream_uncomp.data(), input.data(), input_len) != 0) return -14;

    // 4) gzFile APIs: write to a gzip file and read it back
    const char* gzname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(gzname, "wb");
    if (!gz) return -15;
    int written = gzwrite(gz, input.data(), static_cast<unsigned int>(input_len));
    if (written != static_cast<int>(input_len)) { gzclose(gz); std::remove(gzname); return -16; }
    if (gzclose(gz) != Z_OK) { std::remove(gzname); return -17; }

    gz = gzopen(gzname, "rb");
    if (!gz) { std::remove(gzname); return -18; }
    std::vector<char> gz_read_buf(input_len);
    int read_bytes = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(input_len));
    if (read_bytes < 0) { gzclose(gz); std::remove(gzname); return -19; }
    if (gzclose(gz) != Z_OK) { std::remove(gzname); return -20; }
    if (static_cast<uLong>(read_bytes) != input_len) { std::remove(gzname); return -21; }
    if (std::memcmp(gz_read_buf.data(), input.data(), input_len) != 0) { std::remove(gzname); return -22; }

    // Clean up temp file
    std::remove(gzname);

    // 5) Query version and flags (no-op checks just to exercise API)
    const char* version = zlibVersion();
    (void)version;
    unsigned long flags = zlibCompileFlags();
    (void)flags;

    // If all steps succeeded, return 66 as requested.
    return 66;
}