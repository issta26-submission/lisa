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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare sample input data (repeated to be compressible)
    std::string input;
    for (int i = 0; i < 16; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    const size_t input_size = input.size();

    // 1) Compute adler32 and crc32 checksums
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_size));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_size));
    (void)adl; // keep the variables used; no-op otherwise
    (void)crc;

    // 2) Single-call compress() and uncompress()
    uLong bound = compressBound(static_cast<uLong>(input_size));
    std::vector<unsigned char> compressed(bound);
    uLong compressed_len = bound;
    int ret = compress(compressed.data(), &compressed_len,
                       reinterpret_cast<const Bytef*>(input.data()),
                       static_cast<uLong>(input_size));
    if (ret != Z_OK) return -1;
    compressed.resize(compressed_len);

    std::vector<unsigned char> uncompressed(input_size);
    uLong uncompressed_len = static_cast<uLong>(input_size);
    ret = uncompress(uncompressed.data(), &uncompressed_len,
                     compressed.data(), compressed.size());
    if (ret != Z_OK) return -2;
    if (uncompressed_len != input_size) return -3;
    if (std::memcmp(uncompressed.data(), input.data(), input_size) != 0) return -4;

    // 3) Stream-style deflate (zlib wrapper) and inflate
    {
        z_stream strm{};
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        ret = deflateInit(&strm, Z_BEST_COMPRESSION);
        if (ret != Z_OK) return -5;

        // Provide input
        strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
        strm.avail_in = static_cast<uInt>(input_size);

        // Allocate output buffer using compressBound (safe)
        std::vector<unsigned char> deflated(compressBound(static_cast<uLong>(input_size)));
        strm.next_out = deflated.data();
        strm.avail_out = static_cast<uInt>(deflated.size());

        ret = deflate(&strm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            deflateEnd(&strm);
            return -6;
        }
        size_t deflated_size = deflated.size() - strm.avail_out;
        deflateEnd(&strm);
        deflated.resize(deflated_size);

        // Inflate it back
        z_stream istrm{};
        istrm.zalloc = Z_NULL;
        istrm.zfree = Z_NULL;
        istrm.opaque = Z_NULL;
        istrm.next_in = deflated.data();
        istrm.avail_in = static_cast<uInt>(deflated.size());

        ret = inflateInit(&istrm);
        if (ret != Z_OK) return -7;

        std::vector<unsigned char> inflated(input_size);
        istrm.next_out = inflated.data();
        istrm.avail_out = static_cast<uInt>(inflated.size());

        ret = inflate(&istrm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            inflateEnd(&istrm);
            return -8;
        }
        inflateEnd(&istrm);

        if (std::memcmp(inflated.data(), input.data(), input_size) != 0) return -9;
    }

    // 4) Create a gzip-format stream with deflateInit2 and decode with inflateInit2
    {
        z_stream gzstrm{};
        gzstrm.zalloc = Z_NULL;
        gzstrm.zfree = Z_NULL;
        gzstrm.opaque = Z_NULL;

        // 15 + 16 to produce gzip wrapper
        ret = deflateInit2(&gzstrm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
        if (ret != Z_OK) return -10;

        gzstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
        gzstrm.avail_in = static_cast<uInt>(input_size);

        // Provide an output buffer (compressBound + small header margin)
        std::vector<unsigned char> gz_compressed(compressBound(static_cast<uLong>(input_size)) + 18);
        gzstrm.next_out = gz_compressed.data();
        gzstrm.avail_out = static_cast<uInt>(gz_compressed.size());

        ret = deflate(&gzstrm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            deflateEnd(&gzstrm);
            return -11;
        }
        size_t gz_compressed_size = gz_compressed.size() - gzstrm.avail_out;
        deflateEnd(&gzstrm);
        gz_compressed.resize(gz_compressed_size);

        // Inflate gzip with inflateInit2 using 15 + 16
        z_stream igz{};
        igz.zalloc = Z_NULL;
        igz.zfree = Z_NULL;
        igz.opaque = Z_NULL;
        igz.next_in = gz_compressed.data();
        igz.avail_in = static_cast<uInt>(gz_compressed.size());

        ret = inflateInit2(&igz, MAX_WBITS + 16);
        if (ret != Z_OK) return -12;

        std::vector<unsigned char> gz_inflated(input_size);
        igz.next_out = gz_inflated.data();
        igz.avail_out = static_cast<uInt>(gz_inflated.size());

        ret = inflate(&igz, Z_FINISH);
        if (ret != Z_STREAM_END) {
            inflateEnd(&igz);
            return -13;
        }
        inflateEnd(&igz);

        if (std::memcmp(gz_inflated.data(), input.data(), input_size) != 0) return -14;
    }

    // 5) Use gzwrite / gzread to write/read a gzip file on disk
    const char* fname = "test_zlib_api_sequence.gz";
    {
        gzFile gz = gzopen(fname, "wb");
        if (!gz) return -15;
        int wrote = gzwrite(gz, input.data(), static_cast<unsigned int>(input_size));
        if (wrote == 0) {
            gzclose(gz);
            return -16;
        }
        gzclose(gz);
    }
    {
        gzFile gz = gzopen(fname, "rb");
        if (!gz) return -17;
        std::vector<char> read_back(input_size);
        int read = gzread(gz, read_back.data(), static_cast<unsigned int>(input_size));
        gzclose(gz);
        if (read < 0 || static_cast<size_t>(read) != input_size) return -18;
        if (std::memcmp(read_back.data(), input.data(), input_size) != 0) return -19;
    }

    // 6) Query zlib version and compile flags (non-fatal; just ensure API usage)
    const char* zver = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    (void)zver;
    (void)compile_flags;

    // All tests passed
    return 66;
}