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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when pasted into a file.
    // (Including inside a function is valid because the preprocessor runs before compilation.)
    #include <zlib.h>
    #include <cstdio>
    #include <cstdlib>
    #include <cstring>
    #include <vector>
    #include <string>

    const char* input = "The quick brown fox jumps over the lazy dog";
    size_t input_len = std::strlen(input);

    // 1) Simple one-shot compress / uncompress
    uLong bound = compressBound((uLong)input_len);
    std::vector<Bytef> compBuf(bound);
    uLongf compSize = bound;
    if (compress(compBuf.data(), &compSize, reinterpret_cast<const Bytef*>(input), (uLong)input_len) != Z_OK) {
        return 1;
    }

    std::vector<Bytef> decompBuf(input_len);
    uLongf decompSize = (uLongf)input_len;
    if (uncompress(decompBuf.data(), &decompSize, compBuf.data(), compSize) != Z_OK) {
        return 2;
    }
    if (decompSize != input_len || std::memcmp(decompBuf.data(), input, input_len) != 0) {
        return 3;
    }

    // 2) Compute Adler-32 checksum
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input), (uInt)input_len);
    // Just compute it; not strictly checked here.

    // 3) Streaming deflate (z_stream -> deflateInit / deflate / deflateEnd)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 4;
    }
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = (uInt)input_len;

    std::vector<Bytef> streamedDeflated;
    const size_t CHUNK = 128;
    std::vector<Bytef> outChunk(CHUNK);
    int dret = Z_OK;
    while (dret != Z_STREAM_END) {
        defstream.next_out = outChunk.data();
        defstream.avail_out = (uInt)outChunk.size();
        int flush = defstream.avail_in ? Z_NO_FLUSH : Z_FINISH;
        dret = deflate(&defstream, flush);
        if (dret != Z_OK && dret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return 5;
        }
        size_t have = outChunk.size() - defstream.avail_out;
        streamedDeflated.insert(streamedDeflated.end(), outChunk.begin(), outChunk.begin() + have);
    }
    deflateEnd(&defstream);

    // 4) Streaming inflate (inflateInit / inflate / inflateEnd)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    if (inflateInit(&infstream) != Z_OK) {
        return 6;
    }
    infstream.next_in = streamedDeflated.empty() ? Z_NULL : streamedDeflated.data();
    infstream.avail_in = (uInt)streamedDeflated.size();

    std::vector<Bytef> streamedInflated;
    int iret = Z_OK;
    while (iret != Z_STREAM_END) {
        infstream.next_out = outChunk.data();
        infstream.avail_out = (uInt)outChunk.size();
        iret = inflate(&infstream, Z_NO_FLUSH);
        if (iret != Z_OK && iret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return 7;
        }
        size_t have = outChunk.size() - infstream.avail_out;
        streamedInflated.insert(streamedInflated.end(), outChunk.begin(), outChunk.begin() + have);
    }
    inflateEnd(&infstream);

    if (streamedInflated.size() != input_len || std::memcmp(streamedInflated.data(), input, input_len) != 0) {
        return 8;
    }

    // 5) gz* file API: write compressed data to a .gz file and read it back
    char tmpname[L_tmpnam];
    if (tmpnam(tmpname) == nullptr) {
        return 9;
    }
    std::string fname = std::string(tmpname) + ".gz";

    gzFile gz = gzopen(fname.c_str(), "wb");
    if (!gz) {
        return 10;
    }
    int written = gzwrite(gz, input, (unsigned)input_len);
    if (written != (int)input_len) {
        gzclose(gz);
        std::remove(fname.c_str());
        return 11;
    }
    gzclose(gz);

    gz = gzopen(fname.c_str(), "rb");
    if (!gz) {
        std::remove(fname.c_str());
        return 12;
    }
    std::vector<char> gzReadBuf(input_len);
    int readBytes = gzread(gz, gzReadBuf.data(), (unsigned)input_len);
    gzclose(gz);
    // remove file now
    std::remove(fname.c_str());
    if (readBytes != (int)input_len) {
        return 13;
    }
    if (std::memcmp(gzReadBuf.data(), input, input_len) != 0) {
        return 14;
    }

    // If all tests passed, return 66 as requested.
    return 66;
}