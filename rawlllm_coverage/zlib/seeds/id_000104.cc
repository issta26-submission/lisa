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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary includes placed here so this function is self-contained.
    // Including headers inside a function is valid (preprocessing occurs before compilation).
    #include <zlib.h>
    #include <cstdio>
    #include <cstring>
    #include <string>
    #include <vector>
    #include <cstdlib>

    // Prepare sample input data
    const char *base = "The quick brown fox jumps over the lazy dog. ";
    std::string input;
    for (int i = 0; i < 100; ++i) input += base; // make input reasonably large
    uLong sourceLen = static_cast<uLong>(input.size());

    // 1) compress2 / uncompress (simple one-shot compression APIs)
    uLongf bound = compressBound(sourceLen);
    std::vector<Bytef> compBuf(bound);
    uLongf compLen = bound;
    int ret = compress2(compBuf.data(), &compLen, reinterpret_cast<const Bytef*>(input.data()), sourceLen, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1;

    std::vector<Bytef> decompBuf(sourceLen); // expected size = original
    uLongf decompLen = static_cast<uLongf>(decompBuf.size());
    ret = uncompress(decompBuf.data(), &decompLen, compBuf.data(), compLen);
    if (ret != Z_OK) return -2;
    if (decompLen != sourceLen) return -3;
    if (std::memcmp(decompBuf.data(), input.data(), sourceLen) != 0) return -4;

    // 2) Streaming deflate / inflate (z_stream)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = sourceLen;

    std::vector<Bytef> streamComp;
    const size_t CHUNK = 128;
    std::vector<Bytef> outChunk(CHUNK);

    // Deflate until input consumed and finish stream
    while (defstream.avail_in > 0) {
        defstream.next_out = outChunk.data();
        defstream.avail_out = static_cast<uInt>(outChunk.size());
        ret = deflate(&defstream, Z_NO_FLUSH);
        if (ret != Z_OK) {
            deflateEnd(&defstream);
            return -6;
        }
        size_t have = outChunk.size() - defstream.avail_out;
        streamComp.insert(streamComp.end(), outChunk.data(), outChunk.data() + have);
    }
    // Finish
    int flush_ret;
    do {
        defstream.next_out = outChunk.data();
        defstream.avail_out = static_cast<uInt>(outChunk.size());
        flush_ret = deflate(&defstream, Z_FINISH);
        if (flush_ret != Z_OK && flush_ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -7;
        }
        size_t have = outChunk.size() - defstream.avail_out;
        streamComp.insert(streamComp.end(), outChunk.data(), outChunk.data() + have);
    } while (flush_ret != Z_STREAM_END);

    ret = deflateEnd(&defstream);
    if (ret != Z_OK) return -8;

    // Inflate the streaming data
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -9;

    infstream.next_in = streamComp.empty() ? Z_NULL : streamComp.data();
    infstream.avail_in = static_cast<uInt>(streamComp.size());

    std::vector<Bytef> streamDecomp;
    do {
        infstream.next_out = outChunk.data();
        infstream.avail_out = static_cast<uInt>(outChunk.size());
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -10;
        }
        size_t have = outChunk.size() - infstream.avail_out;
        streamDecomp.insert(streamDecomp.end(), outChunk.data(), outChunk.data() + have);
    } while (ret != Z_STREAM_END);

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) return -11;

    if (streamDecomp.size() != sourceLen) return -12;
    if (std::memcmp(streamDecomp.data(), input.data(), sourceLen) != 0) return -13;

    // 3) CRC32 and Adler32 checks
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), sourceLen);
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), sourceLen);
    // simple sanity checks: both should be non-zero for this data
    if (crc == 0 || adl == 0) return -14;

    // 4) gz* APIs: write compressed data to a temporary file and read it back
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return -15;
    int wrote = gzwrite(gz, input.data(), static_cast<unsigned int>(input.size()));
    if (wrote != static_cast<int>(input.size())) {
        gzclose(gz);
        return -16;
    }
    if (gzclose(gz) != Z_OK) return -17;

    gz = gzopen(fname, "rb");
    if (!gz) return -18;
    std::vector<char> gzReadBuf(input.size());
    int read = gzread(gz, gzReadBuf.data(), static_cast<unsigned int>(gzReadBuf.size()));
    if (read < 0) {
        gzclose(gz);
        return -19;
    }
    // gzread returns number of uncompressed bytes read
    if (static_cast<size_t>(read) != input.size()) {
        gzclose(gz);
        return -20;
    }
    if (std::memcmp(gzReadBuf.data(), input.data(), input.size()) != 0) {
        gzclose(gz);
        return -21;
    }
    if (gzclose(gz) != Z_OK) return -22;

    // remove temporary file (best-effort; ignore failure)
    std::remove(fname);

    // 5) query version/flags (no-op but exercises API)
    const char* ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)ver; (void)flags; // silence unused-variable warnings

    // If we reached here, all zlib operations succeeded
    return 66;
}