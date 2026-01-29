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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so this function is self-contained when pasted into a file.
    // (Preprocessor directives are allowed anywhere; they are processed before compilation.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <cstdio>
    #include <cstdlib>

    const char* input = "The quick brown fox jumps over the lazy dog";
    size_t input_len = std::strlen(input);

    // 1) compress2 / uncompress (single-call convenience APIs)
    uLongf comp_bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> comp_buf(comp_bound);

    uLongf comp_size = comp_bound;
    int ret = compress2(comp_buf.data(), &comp_size,
                        reinterpret_cast<const Bytef*>(input),
                        static_cast<uLong>(input_len),
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 1;

    // 1b) uncompress and verify
    std::vector<unsigned char> decomp_buf(input_len);
    uLongf decomp_size = static_cast<uLongf>(input_len);
    ret = uncompress(decomp_buf.data(), &decomp_size,
                     comp_buf.data(), comp_size);
    if (ret != Z_OK) return 2;
    if (decomp_size != input_len) return 3;
    if (std::memcmp(decomp_buf.data(), input, input_len) != 0) return 4;

    // 2) CRC32 check
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input),
                     static_cast<uInt>(input_len));
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decomp_buf.data(),
                       static_cast<uInt>(decomp_size));
    if (crc_orig != crc_decomp) return 5;

    // 3) Streaming deflate (z_stream, deflateInit, deflate, deflateEnd)
    {
        z_stream dstrm;
        std::memset(&dstrm, 0, sizeof(dstrm));
        ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return 6;

        dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
        dstrm.avail_in = static_cast<uInt>(input_len);

        const size_t CHUNK = 64;
        std::vector<unsigned char> stream_comp;
        unsigned char outbuf[CHUNK];

        int flush = Z_FINISH;
        do {
            dstrm.next_out = outbuf;
            dstrm.avail_out = CHUNK;
            ret = deflate(&dstrm, flush);
            if (ret == Z_STREAM_ERROR) { deflateEnd(&dstrm); return 7; }
            size_t have = CHUNK - dstrm.avail_out;
            stream_comp.insert(stream_comp.end(), outbuf, outbuf + have);
        } while (ret != Z_STREAM_END);
        deflateEnd(&dstrm);

        // 4) Streaming inflate (inflateInit, inflate, inflateEnd) to verify
        z_stream istrm;
        std::memset(&istrm, 0, sizeof(istrm));
        ret = inflateInit(&istrm);
        if (ret != Z_OK) return 8;

        istrm.next_in = stream_comp.empty() ? Z_NULL : stream_comp.data();
        istrm.avail_in = static_cast<uInt>(stream_comp.size());

        std::vector<unsigned char> stream_decomp;
        unsigned char inout[CHUNK];
        do {
            istrm.next_out = inout;
            istrm.avail_out = CHUNK;
            ret = inflate(&istrm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                inflateEnd(&istrm);
                return 9;
            }
            size_t have = CHUNK - istrm.avail_out;
            stream_decomp.insert(stream_decomp.end(), inout, inout + have);
        } while (ret != Z_STREAM_END);
        inflateEnd(&istrm);

        if (stream_decomp.size() != input_len) return 10;
        if (std::memcmp(stream_decomp.data(), input, input_len) != 0) return 11;
    }

    // 5) gzopen / gzwrite / gzread / gzclose (file-based convenience API)
    {
        // Generate a temporary filename
        char tmpname[L_tmpnam];
        if (std::tmpnam(tmpname) == nullptr) return 12;

        // Write compressed data using gzopen/gzwrite
        gzFile gz = gzopen(tmpname, "wb");
        if (!gz) return 13;
        int wrote = gzwrite(gz, input, static_cast<unsigned int>(input_len));
        if (wrote != static_cast<int>(input_len)) {
            gzclose(gz);
            std::remove(tmpname);
            return 14;
        }
        gzclose(gz);

        // Read it back
        gz = gzopen(tmpname, "rb");
        if (!gz) {
            std::remove(tmpname);
            return 15;
        }
        std::vector<unsigned char> gz_read_buf(input_len);
        int read = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(input_len));
        gzclose(gz);
        // remove temporary file
        std::remove(tmpname);

        if (read < 0) return 16;
        if (static_cast<size_t>(read) != input_len) return 17;
        if (std::memcmp(gz_read_buf.data(), input, input_len) != 0) return 18;
    }

    // If all checks passed, return the required success value
    return 66;
}