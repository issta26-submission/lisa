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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates use of multiple zlib APIs:
    // - compress2 / uncompress
    // - deflateInit / deflate / deflateEnd
    // - inflateInit / inflate / inflateEnd
    // - gzopen / gzwrite / gzread / gzclose
    // - crc32 and adler32
    //
    // It returns 66 on success; on any failure it will try to clean up and return a non-66 code.

    // Note: This code assumes the compilation unit includes <zlib.h> and the usual C++ headers
    // (e.g., <vector>, <string>, <cstring>, <cstdio>, <cstdlib>). If not, add them to your file.

    const char *tmp_gzip_name = "test_zlib_api_sequence.gz";

    // Prepare sample data (repetitive to compress well)
    std::string original;
    original.reserve(4096);
    for (int i = 0; i < 256; ++i) {
        original += "The quick brown fox jumps over the lazy dog. ";
    }
    // Also add some unique tail
    original += "End of test data.";

    // 1) compress2 / uncompress
    {
        uLong src_len = static_cast<uLong>(original.size());
        uLong bound = compressBound(src_len);
        std::vector<Bytef> comp(bound);
        uLong comp_len = bound;

        int ret = compress2(comp.data(), &comp_len,
                            reinterpret_cast<const Bytef*>(original.data()), src_len,
                            Z_BEST_COMPRESSION);
        if (ret != Z_OK) {
            // compress failed
            return 1;
        }

        // uncompress
        std::vector<Bytef> decomp(src_len);
        uLong decomp_len = src_len;
        ret = uncompress(decomp.data(), &decomp_len, comp.data(), comp_len);
        if (ret != Z_OK || decomp_len != src_len) {
            return 2;
        }

        if (memcmp(decomp.data(), original.data(), src_len) != 0) {
            return 3; // data mismatch after uncompress
        }
    }

    // 2) deflate (streaming) / inflate (streaming)
    std::vector<Bytef> deflated;
    {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return 10;

        // Provide input
        strm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(original.data()));
        strm.avail_in = static_cast<uInt>(original.size());

        // Deflate in chunks
        const size_t chunk = 128;
        std::vector<Bytef> outbuf(chunk);
        do {
            strm.next_out = outbuf.data();
            strm.avail_out = static_cast<uInt>(outbuf.size());
            ret = deflate(&strm, Z_FINISH); // we pass all input at once, finish immediately
            if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
                deflateEnd(&strm);
                return 11;
            }
            size_t have = outbuf.size() - strm.avail_out;
            deflated.insert(deflated.end(), outbuf.data(), outbuf.data() + have);
        } while (ret != Z_STREAM_END);

        deflateEnd(&strm);
    }

    // inflate the deflated data
    std::vector<Bytef> inflated;
    {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        int ret = inflateInit(&strm);
        if (ret != Z_OK) return 20;

        strm.next_in = deflated.empty() ? nullptr : deflated.data();
        strm.avail_in = static_cast<uInt>(deflated.size());

        const size_t chunk = 128;
        std::vector<Bytef> outbuf(chunk);
        do {
            strm.next_out = outbuf.data();
            strm.avail_out = static_cast<uInt>(outbuf.size());
            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                inflateEnd(&strm);
                return 21;
            }
            size_t have = outbuf.size() - strm.avail_out;
            inflated.insert(inflated.end(), outbuf.data(), outbuf.data() + have);
        } while (ret != Z_STREAM_END && strm.avail_out == 0);

        inflateEnd(&strm);
        // Verify sizes and content
        if (inflated.size() != original.size()) return 22;
        if (memcmp(inflated.data(), original.data(), original.size()) != 0) return 23;
    }

    // 3) gzopen / gzwrite / gzread / gzclose
    {
        // Write gzip file
        gzFile gz = gzopen(tmp_gzip_name, "wb");
        if (!gz) return 30;
        int wrote = gzwrite(gz, original.data(), static_cast<unsigned int>(original.size()));
        if (wrote == 0) {
            gzclose(gz);
            return 31;
        }
        if (gzclose(gz) != Z_OK) {
            return 32;
        }

        // Read back gzip file
        gz = gzopen(tmp_gzip_name, "rb");
        if (!gz) {
            // try to remove file if exists
            std::remove(tmp_gzip_name);
            return 33;
        }
        std::vector<char> readbuf(original.size());
        int readBytes = gzread(gz, readbuf.data(), static_cast<unsigned int>(readbuf.size()));
        if (readBytes < 0) {
            gzclose(gz);
            std::remove(tmp_gzip_name);
            return 34;
        }
        if (static_cast<size_t>(readBytes) != original.size() ||
            memcmp(readbuf.data(), original.data(), original.size()) != 0) {
            gzclose(gz);
            std::remove(tmp_gzip_name);
            return 35;
        }
        if (gzclose(gz) != Z_OK) {
            std::remove(tmp_gzip_name);
            return 36;
        }

        // Clean up the temporary gzip file
        std::remove(tmp_gzip_name);
    }

    // 4) crc32 and adler32 checks
    {
        uLong crc_before = crc32(0L, Z_NULL, 0);
        crc_before = crc32(crc_before,
                           reinterpret_cast<const Bytef*>(original.data()),
                           static_cast<uInt>(original.size()));

        uLong adler_before = adler32(0L, Z_NULL, 0);
        adler_before = adler32(adler_before,
                               reinterpret_cast<const Bytef*>(original.data()),
                               static_cast<uInt>(original.size()));

        // Compute again on inflated buffer
        uLong crc_after = crc32(0L, Z_NULL, 0);
        crc_after = crc32(crc_after,
                          reinterpret_cast<const Bytef*>(inflated.data()),
                          static_cast<uInt>(inflated.size()));

        uLong adler_after = adler32(0L, Z_NULL, 0);
        adler_after = adler32(adler_after,
                              reinterpret_cast<const Bytef*>(inflated.data()),
                              static_cast<uInt>(inflated.size()));

        if (crc_before != crc_after) return 40;
        if (adler_before != adler_after) return 41;
    }

    // If we reached here, all tests passed
    return 66;
}