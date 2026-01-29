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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // The following #includes are placed here so this function is self-contained
    // when embedded into a single-file test. It's legal to put #include inside a
    // function in C/C++ (preprocessor runs before compilation).
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>
    #include <cstdio>

    // Original data to compress/decompress
    std::string original;
    for (int i = 0; i < 1000; ++i)
        original += "The quick brown fox jumps over the lazy dog. ";

    const size_t orig_len = original.size();

    // --- Test 1: deflate (z_stream) -> inflate (z_stream) roundtrip ---

    // Helper chunk size
    const size_t CHUNK = 16384;

    // Compress using z_stream + deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(orig_len);

    std::vector<unsigned char> compressed;
    compressed.reserve(orig_len / 2 + 64);

    std::vector<unsigned char> outbuf(CHUNK);

    int deflate_ret;
    do {
        defstream.next_out = outbuf.data();
        defstream.avail_out = static_cast<uInt>(outbuf.size());
        deflate_ret = deflate(&defstream, defstream.avail_in ? Z_NO_FLUSH : Z_FINISH);
        if (deflate_ret != Z_OK && deflate_ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -1;
        }
        size_t have = outbuf.size() - defstream.avail_out;
        compressed.insert(compressed.end(), outbuf.data(), outbuf.data() + have);
    } while (deflate_ret != Z_STREAM_END);

    if (deflateEnd(&defstream) != Z_OK) {
        return -1;
    }

    // Decompress using z_stream + inflate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    if (inflateInit(&infstream) != Z_OK) {
        return -1;
    }

    infstream.next_in = compressed.empty() ? Z_NULL : reinterpret_cast<Bytef*>(compressed.data());
    infstream.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> recovered;
    recovered.reserve(orig_len);

    std::vector<unsigned char> inbuf(CHUNK);

    int inflate_ret;
    do {
        infstream.next_out = inbuf.data();
        infstream.avail_out = static_cast<uInt>(inbuf.size());
        inflate_ret = inflate(&infstream, Z_NO_FLUSH);
        if (inflate_ret != Z_OK && inflate_ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -1;
        }
        size_t have = inbuf.size() - infstream.avail_out;
        recovered.insert(recovered.end(), inbuf.data(), inbuf.data() + have);
    } while (inflate_ret != Z_STREAM_END);

    if (inflateEnd(&infstream) != Z_OK) {
        return -1;
    }

    if (recovered.size() != orig_len || std::memcmp(recovered.data(), original.data(), orig_len) != 0) {
        return -1;
    }

    // --- Test 2: compress() / uncompress() convenience functions ---

    uLong bound = compressBound(static_cast<uLong>(orig_len));
    std::vector<unsigned char> cbuf(bound);
    uLong comp_size = bound;
    if (compress2(cbuf.data(), &comp_size, reinterpret_cast<const Bytef*>(original.data()), static_cast<uLong>(orig_len), Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -1;
    }
    cbuf.resize(comp_size);

    std::vector<unsigned char> ubuf(orig_len);
    uLong uncompressed_size = static_cast<uLong>(orig_len);
    if (uncompress(ubuf.data(), &uncompressed_size, cbuf.data(), comp_size) != Z_OK) {
        return -1;
    }
    if (uncompressed_size != orig_len || std::memcmp(ubuf.data(), original.data(), orig_len) != 0) {
        return -1;
    }

    // --- Test 3: gzwrite / gzread roundtrip using a temporary file ---

    const char *temp_name = "test_zlib_api_sequence_tmp.gz";
    {
        gzFile gz = gzopen(temp_name, "wb");
        if (!gz) return -1;
        int written = gzwrite(gz, original.data(), static_cast<unsigned int>(orig_len));
        if (written == 0) { gzclose(gz); return -1; }
        if (gzclose(gz) != Z_OK) return -1;
    }
    {
        gzFile gz = gzopen(temp_name, "rb");
        if (!gz) return -1;
        std::vector<char> rbuf(orig_len + 16);
        int read_total = 0;
        int r;
        while ((r = gzread(gz, rbuf.data() + read_total, static_cast<unsigned int>(rbuf.size() - read_total))) > 0) {
            read_total += r;
            if (read_total >= static_cast<int>(rbuf.size())) rbuf.resize(rbuf.size() + CHUNK);
        }
        if (r < 0) { gzclose(gz); return -1; }
        if (gzclose(gz) != Z_OK) return -1;
        if (static_cast<size_t>(read_total) != orig_len || std::memcmp(rbuf.data(), original.data(), orig_len) != 0) {
            return -1;
        }
        // remove temp file (best-effort)
        std::remove(temp_name);
    }

    // --- Test 4: checksum utilities (crc32, adler32) ---

    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(orig_len));

    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(orig_len));

    // Recompute on recovered data and compare
    uLong crc_recovered = crc32(0L, Z_NULL, 0);
    crc_recovered = crc32(crc_recovered, recovered.data(), static_cast<uInt>(recovered.size()));

    uLong adler_recovered = adler32(0L, Z_NULL, 0);
    adler_recovered = adler32(adler_recovered, recovered.data(), static_cast<uInt>(recovered.size()));

    if (crc_orig != crc_recovered || adler_orig != adler_recovered) {
        return -1;
    }

    // --- Optionally check zlib version strings (no effect on logic) ---
    const char *ver = zlibVersion();
    (void)ver; // suppress unused variable warning; it's fine to call it.

    // All checks passed
    return 66;
}