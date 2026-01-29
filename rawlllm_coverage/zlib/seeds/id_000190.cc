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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Ensure the zlib header and basic C++ headers are available to this translation unit.
    // (Preprocessor directives are valid here; they will be processed before compilation.)
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>

    // Sample data to compress/decompress
    const char* sample = "The quick brown fox jumps over the lazy dog. "
                         "This is a simple test string used to exercise zlib APIs.";
    const std::size_t sample_len = std::strlen(sample);

    // 1) Simple one-shot compress / uncompress using compress() and uncompress()
    {
        // Allocate destination buffer using compressBound
        uLongf comp_bound = compressBound(static_cast<uLong>(sample_len));
        std::vector<unsigned char> comp_buf(comp_bound);

        uLongf comp_size = comp_bound;
        int ret = compress(comp_buf.data(), &comp_size,
                           reinterpret_cast<const Bytef*>(sample),
                           static_cast<uLong>(sample_len));
        if (ret != Z_OK) return -1; // compression failed

        // Now uncompress to verify
        std::vector<unsigned char> uncomp_buf(sample_len);
        uLongf uncomp_size = static_cast<uLongf>(sample_len);
        ret = uncompress(uncomp_buf.data(), &uncomp_size,
                         comp_buf.data(), comp_size);
        if (ret != Z_OK) return -2; // uncompress failed

        if (uncomp_size != sample_len) return -3;
        if (std::memcmp(uncomp_buf.data(), sample, sample_len) != 0) return -4;
    }

    // 2) Streaming compression using deflateInit / deflate / deflateEnd
    std::vector<unsigned char> stream_comp; // will hold streamed compressed data
    {
        z_stream defstrm;
        std::memset(&defstrm, 0, sizeof(defstrm));
        int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return -5;

        // Prepare input
        defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
        defstrm.avail_in = static_cast<uInt>(sample_len);

        // Output buffer in chunks
        const std::size_t CHUNK = 128;
        std::vector<unsigned char> outbuf(CHUNK);

        // Compress until input consumed, then finish stream
        while (defstrm.avail_in > 0) {
            defstrm.next_out = outbuf.data();
            defstrm.avail_out = static_cast<uInt>(outbuf.size());
            ret = deflate(&defstrm, Z_NO_FLUSH);
            if (ret != Z_OK) {
                deflateEnd(&defstrm);
                return -6;
            }
            std::size_t have = outbuf.size() - defstrm.avail_out;
            stream_comp.insert(stream_comp.end(), outbuf.data(), outbuf.data() + have);
        }

        // Finish
        for (;;) {
            defstrm.next_out = outbuf.data();
            defstrm.avail_out = static_cast<uInt>(outbuf.size());
            ret = deflate(&defstrm, Z_FINISH);
            if (ret == Z_STREAM_END) {
                std::size_t have = outbuf.size() - defstrm.avail_out;
                stream_comp.insert(stream_comp.end(), outbuf.data(), outbuf.data() + have);
                break;
            } else if (ret == Z_OK) {
                std::size_t have = outbuf.size() - defstrm.avail_out;
                stream_comp.insert(stream_comp.end(), outbuf.data(), outbuf.data() + have);
                continue;
            } else {
                deflateEnd(&defstrm);
                return -7;
            }
        }

        deflateEnd(&defstrm);
        if (stream_comp.empty()) return -8;
    }

    // 3) Streaming decompression using inflateInit / inflate / inflateEnd
    {
        z_stream infstrm;
        std::memset(&infstrm, 0, sizeof(infstrm));
        int ret = inflateInit(&infstrm);
        if (ret != Z_OK) return -9;

        // Feed the compressed stream produced above
        infstrm.next_in = stream_comp.empty() ? Z_NULL : reinterpret_cast<Bytef*>(stream_comp.data());
        infstrm.avail_in = static_cast<uInt>(stream_comp.size());

        const std::size_t CHUNK = 128;
        std::vector<unsigned char> outbuf(CHUNK);
        std::vector<unsigned char> recovered;
        recovered.reserve(sample_len);

        for (;;) {
            infstrm.next_out = outbuf.data();
            infstrm.avail_out = static_cast<uInt>(outbuf.size());
            ret = inflate(&infstrm, Z_NO_FLUSH);
            if (ret == Z_STREAM_END || ret == Z_OK) {
                std::size_t have = outbuf.size() - infstrm.avail_out;
                recovered.insert(recovered.end(), outbuf.data(), outbuf.data() + have);
                if (ret == Z_STREAM_END) break;
                // else continue until avail_in==0 and inflate returns Z_STREAM_END after input exhausted
                if (infstrm.avail_in == 0 && ret == Z_OK) {
                    // If inflate needs more input we'd normally provide it; here we provided all input already.
                    // Continue loop to allow inflate to return Z_STREAM_END after processing all input.
                    continue;
                }
            } else {
                inflateEnd(&infstrm);
                return -10;
            }
        }

        inflateEnd(&infstrm);

        if (recovered.size() != sample_len) return -11;
        if (std::memcmp(recovered.data(), sample, sample_len) != 0) return -12;
    }

    // 4) Compute CRC and Adler checksums
    {
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));
        // Compute adler32 as well
        uLong adl = adler32(0L, Z_NULL, 0);
        adl = adler32(adl, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));
        // Basic sanity checks: non-zero for non-empty input
        if (crc == 0 || adl == 0) return -13;
    }

    // If we reached here, all zlib API sequences succeeded
    return 66;
}