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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when pasted into a file.
    // (Preprocessor includes may be placed anywhere; declarations will be in block scope but
    // that's acceptable for a standalone test function.)
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>

    // Prepare input data
    const std::string pattern = "The quick brown fox jumps over the lazy dog. ";
    std::string original;
    for (int i = 0; i < 50; ++i) original += pattern; // make a reasonably sized buffer

    // 1) Simple one-shot compress / uncompress using compress() and uncompress()
    {
        uLong srcLen = static_cast<uLong>(original.size());
        uLong compBound = compressBound(srcLen);
        std::vector<Bytef> compBuf(compBound);

        int rc = compress(compBuf.data(), &compBound,
                          reinterpret_cast<const Bytef*>(original.data()), srcLen);
        if (rc != Z_OK) return 1; // compress failed
        compBuf.resize(compBound);

        // Decompress
        std::vector<Bytef> decompBuf(srcLen);
        uLong decompLen = srcLen;
        rc = uncompress(decompBuf.data(), &decompLen, compBuf.data(), compBuf.size());
        if (rc != Z_OK) return 2; // uncompress failed
        if (decompLen != srcLen) return 3;
        if (std::memcmp(decompBuf.data(), original.data(), srcLen) != 0) return 4;
    }

    // 2) Stream compress using deflateInit / deflate / deflateEnd
    std::vector<Bytef> streamCompressed;
    {
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        int rc = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (rc != Z_OK) return 5;

        const size_t CHUNK = 128;
        std::vector<Bytef> out(CHUNK);
        // Input will be fed in two parts
        size_t half = original.size() / 2;

        defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
        defstream.avail_in = static_cast<uInt>(half);
        while (defstream.avail_in > 0) {
            defstream.next_out = out.data();
            defstream.avail_out = static_cast<uInt>(out.size());
            rc = deflate(&defstream, Z_NO_FLUSH);
            if (rc != Z_OK) { deflateEnd(&defstream); return 6; }
            size_t produced = out.size() - defstream.avail_out;
            streamCompressed.insert(streamCompressed.end(), out.data(), out.data() + produced);
        }

        // Feed the remainder
        defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data() + half));
        defstream.avail_in = static_cast<uInt>(original.size() - half);
        int flush = Z_FINISH;
        do {
            defstream.next_out = out.data();
            defstream.avail_out = static_cast<uInt>(out.size());
            rc = deflate(&defstream, flush);
            if (rc != Z_OK && rc != Z_STREAM_END) { deflateEnd(&defstream); return 7; }
            size_t produced = out.size() - defstream.avail_out;
            streamCompressed.insert(streamCompressed.end(), out.data(), out.data() + produced);
        } while (rc != Z_STREAM_END);

        deflateEnd(&defstream);
    }

    // 3) Stream decompress using inflateInit / inflate / inflateEnd
    {
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        int rc = inflateInit(&infstream);
        if (rc != Z_OK) return 8;

        const size_t CHUNK = 128;
        std::vector<Bytef> out(CHUNK);
        std::vector<Bytef> recovered;
        infstream.next_in = streamCompressed.data();
        infstream.avail_in = static_cast<uInt>(streamCompressed.size());

        do {
            infstream.next_out = out.data();
            infstream.avail_out = static_cast<uInt>(out.size());
            rc = inflate(&infstream, Z_NO_FLUSH);
            if (rc != Z_OK && rc != Z_STREAM_END) { inflateEnd(&infstream); return 9; }
            size_t produced = out.size() - infstream.avail_out;
            recovered.insert(recovered.end(), out.data(), out.data() + produced);
        } while (rc != Z_STREAM_END);

        inflateEnd(&infstream);

        if (recovered.size() != original.size()) return 10;
        if (std::memcmp(recovered.data(), original.data(), original.size()) != 0) return 11;
    }

    // 4) gz* API: write original data to a .gz file and read back
    {
        const char *tmpname = "test_zlib_api_sequence_tmp.gz";
        gzFile gz = gzopen(tmpname, "wb");
        if (!gz) return 12;
        int written = gzwrite(gz, original.data(), static_cast<unsigned int>(original.size()));
        if (written <= 0 || static_cast<size_t>(written) != original.size()) { gzclose(gz); remove(tmpname); return 13; }
        if (gzclose(gz) != Z_OK) { remove(tmpname); return 14; }

        // Read it back
        gz = gzopen(tmpname, "rb");
        if (!gz) { remove(tmpname); return 15; }
        std::vector<char> readbuf(original.size());
        int readBytes = gzread(gz, readbuf.data(), static_cast<int>(readbuf.size()));
        if (readBytes <= 0 || static_cast<size_t>(readBytes) != original.size()) { gzclose(gz); remove(tmpname); return 16; }
        gzclose(gz);

        if (std::memcmp(readbuf.data(), original.data(), original.size()) != 0) { remove(tmpname); return 17; }

        // Clean up temporary file
        if (std::remove(tmpname) != 0) {
            // Not fatal to fail removing temp file, but report a non-zero code if you prefer.
        }
    }

    // If all operations succeeded, return 66 as requested
    return 66;
}