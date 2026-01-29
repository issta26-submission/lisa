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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a sequence of common zlib APIs:
    // - compress2 / uncompress
    // - adler32 and crc32
    // - z_stream deflate / inflate (stream API)
    // - gzopen / gzwrite / gzread
    //
    // On success the function returns 66. On failure it returns a negative error code.

    // Include headers here so the function is self-contained when pasted into a file.
    // (Including headers inside a function is valid; the preprocessor will inline them.)
    #include <zlib.h>
    #include <vector>
    #include <cstring>
    #include <cstdio>
    #include <string>

    // Sample data to compress and decompress
    const char sample_text[] =
        "The quick brown fox jumps over the lazy dog. "
        "zlib API sequence test: compress, uncompress, deflate, inflate, gz* I/O.";
    const uLong sample_len = static_cast<uLong>(strlen(sample_text));

    // 1) compress2 / uncompress
    {
        uLong bound = compressBound(sample_len);
        std::vector<Bytef> comp_buf(bound);

        int ret = compress2(comp_buf.data(), &bound,
                            reinterpret_cast<const Bytef*>(sample_text), sample_len,
                            Z_BEST_COMPRESSION);
        if (ret != Z_OK) return -1; // compress failed
        comp_buf.resize(bound);

        // Uncompress into buffer of expected size
        std::vector<Bytef> decomp_buf(sample_len);
        uLong decomp_len = sample_len;
        ret = uncompress(decomp_buf.data(), &decomp_len,
                         comp_buf.data(), static_cast<uLong>(comp_buf.size()));
        if (ret != Z_OK) return -2; // uncompress failed
        if (decomp_len != sample_len) return -3;
        if (std::memcmp(decomp_buf.data(), sample_text, sample_len) != 0) return -4;
    }

    // 2) checksums: adler32 and crc32
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(sample_text), sample_len);
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(sample_text), sample_len);
    // Just compute to exercise the APIs; not otherwise used here.

    // 3) stream-based deflate (z_stream) and inflate
    std::vector<unsigned char> stream_compressed;
    std::vector<unsigned char> stream_decompressed;
    {
        // Deflate (compress) using z_stream
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return -5;

        defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample_text));
        defstream.avail_in = static_cast<uInt>(sample_len);

        const size_t CHUNK = 128;
        std::vector<Bytef> outbuf(CHUNK);

        do {
            defstream.next_out = outbuf.data();
            defstream.avail_out = static_cast<uInt>(CHUNK);
            ret = deflate(&defstream, defstream.avail_in ? Z_NO_FLUSH : Z_FINISH);
            if (ret == Z_STREAM_ERROR) { deflateEnd(&defstream); return -6; }
            size_t have = CHUNK - defstream.avail_out;
            stream_compressed.insert(stream_compressed.end(), outbuf.data(), outbuf.data() + have);
        } while (defstream.avail_out == 0);

        // finish with Z_FINISH to ensure all output is produced
        while (ret != Z_STREAM_END) {
            defstream.next_out = outbuf.data();
            defstream.avail_out = static_cast<uInt>(CHUNK);
            ret = deflate(&defstream, Z_FINISH);
            if (ret == Z_STREAM_ERROR) { deflateEnd(&defstream); return -7; }
            size_t have = CHUNK - defstream.avail_out;
            stream_compressed.insert(stream_compressed.end(), outbuf.data(), outbuf.data() + have);
            if (defstream.avail_out != 0 && ret != Z_STREAM_END) break; // safety
        }

        deflateEnd(&defstream);
        if (stream_compressed.empty()) return -8;

        // Inflate (decompress) using z_stream
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        ret = inflateInit(&infstream);
        if (ret != Z_OK) return -9;

        infstream.next_in = stream_compressed.empty() ? Z_NULL : reinterpret_cast<Bytef*>(stream_compressed.data());
        infstream.avail_in = static_cast<uInt>(stream_compressed.size());

        std::vector<Bytef> in_outbuf(CHUNK);
        bool finished = false;
        while (!finished) {
            infstream.next_out = in_outbuf.data();
            infstream.avail_out = static_cast<uInt>(CHUNK);
            ret = inflate(&infstream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                inflateEnd(&infstream);
                return -10;
            }
            size_t have = CHUNK - infstream.avail_out;
            stream_decompressed.insert(stream_decompressed.end(), in_outbuf.data(), in_outbuf.data() + have);
            if (ret == Z_STREAM_END) finished = true;
            if (infstream.avail_in == 0 && have == 0) break; // safety guard
        }

        inflateEnd(&infstream);

        if (stream_decompressed.size() != sample_len) return -11;
        if (std::memcmp(stream_decompressed.data(), sample_text, sample_len) != 0) return -12;
    }

    // 4) gz* file API: write compressed data to a .gz file then read it back
    {
        const char *fname = "test_zlib_api_sequence.gz";
        gzFile gz = gzopen(fname, "wb");
        if (!gz) return -13;
        int written = gzwrite(gz, sample_text, static_cast<unsigned int>(sample_len));
        if (written == 0) { gzclose(gz); return -14; }
        gzclose(gz);

        // Read back
        gz = gzopen(fname, "rb");
        if (!gz) return -15;
        std::vector<char> read_back(sample_len + 1);
        int r = gzread(gz, read_back.data(), static_cast<unsigned int>(sample_len));
        gzclose(gz);
        if (r < 0) return -16;
        if (static_cast<uLong>(r) != sample_len) return -17;
        read_back[sample_len] = '\0';
        if (std::memcmp(read_back.data(), sample_text, sample_len) != 0) return -18;

        // remove the temporary file; ignore result
        std::remove(fname);
    }

    // 5) zlibVersion just to exercise it (no effect)
    const char *ver = zlibVersion();
    (void)ver; // silence unused variable warning

    // If we get here, all steps succeeded
    return 66;
}