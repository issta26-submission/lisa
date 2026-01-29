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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // includes placed here to make the function self-contained if pasted alone.
    // It's legal for the preprocessor to include headers at this point.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Sample input data
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "zlib API sequence test. Repeat to enlarge: "
                              "The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog.";

    // Compute an Adler-32 checksum of the original data (demonstrate another zlib API)
    uLong orig_adler = adler32(0L, Z_NULL, 0);
    orig_adler = adler32(orig_adler,
                         reinterpret_cast<const Bytef*>(input.data()),
                         static_cast<uInt>(input.size()));

    // Prepare z_stream for deflate (compression)
    z_stream deflateStream{};
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    int ret = deflateInit(&deflateStream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // failed to initialize deflate
        return -1;
    }

    // Allocate an output buffer using compressBound to be safe
    uLong sourceLen = static_cast<uLong>(input.size());
    uLong bound = compressBound(sourceLen);
    std::vector<unsigned char> comp(bound);

    deflateStream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    deflateStream.avail_in = static_cast<uInt>(input.size());
    deflateStream.next_out = reinterpret_cast<Bytef*>(comp.data());
    deflateStream.avail_out = static_cast<uInt>(comp.size());

    // Run deflate until finished
    while (true) {
        ret = deflate(&deflateStream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break; // all input consumed and output finished
        } else if (ret == Z_OK) {
            // Should not normally happen here because we provided enough output buffer,
            // but if it does, expand output buffer and continue.
            size_t used = deflateStream.total_out;
            size_t oldSize = comp.size();
            comp.resize(oldSize * 2);
            deflateStream.next_out = reinterpret_cast<Bytef*>(comp.data() + used);
            deflateStream.avail_out = static_cast<uInt>(comp.size() - used);
            // continue the loop
        } else {
            // error
            deflateEnd(&deflateStream);
            return -2;
        }
    }

    // total compressed size
    uLong comp_size = deflateStream.total_out;

    // Clean up deflate state
    deflateEnd(&deflateStream);

    // Now decompress with inflate
    z_stream inflateStream{};
    inflateStream.zalloc = Z_NULL;
    inflateStream.zfree = Z_NULL;
    inflateStream.opaque = Z_NULL;

    ret = inflateInit(&inflateStream);
    if (ret != Z_OK) {
        return -3;
    }

    std::vector<unsigned char> decomp(input.size()); // we expect decompressed size equals original
    inflateStream.next_in = reinterpret_cast<Bytef*>(comp.data());
    inflateStream.avail_in = static_cast<uInt>(comp_size);
    inflateStream.next_out = reinterpret_cast<Bytef*>(decomp.data());
    inflateStream.avail_out = static_cast<uInt>(decomp.size());

    // Run inflate until finished
    while (true) {
        ret = inflate(&inflateStream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        } else if (ret == Z_OK) {
            // If output buffer was too small, enlarge and continue.
            size_t used = inflateStream.total_out;
            size_t oldSize = decomp.size();
            decomp.resize(oldSize * 2);
            inflateStream.next_out = reinterpret_cast<Bytef*>(decomp.data() + used);
            inflateStream.avail_out = static_cast<uInt>(decomp.size() - used);
            // continue
        } else {
            inflateEnd(&inflateStream);
            return -4;
        }
    }

    // actual decompressed size
    uLong decomp_size = inflateStream.total_out;

    // cleanup inflate
    inflateEnd(&inflateStream);

    // Validate decompressed size matches original
    if (decomp_size != input.size()) {
        return -5;
    }

    // Validate decompressed contents match original
    if (std::memcmp(decomp.data(), input.data(), input.size()) != 0) {
        return -6;
    }

    // Compute adler of decompressed and compare with original
    uLong decomp_adler = adler32(0L, Z_NULL, 0);
    decomp_adler = adler32(decomp_adler,
                           reinterpret_cast<const Bytef*>(decomp.data()),
                           static_cast<uInt>(decomp_size));
    if (decomp_adler != orig_adler) {
        return -7;
    }

    // Optionally demonstrate compress/uncompress convenience functions too
    {
        // Use compress2 to produce a second compressed buffer and then uncompress it
        uLong destBound = compressBound(sourceLen);
        std::vector<unsigned char> comp2(destBound);
        uLong comp2_size = destBound;
        int cret = compress2(comp2.data(), &comp2_size,
                             reinterpret_cast<const Bytef*>(input.data()),
                             sourceLen, Z_BEST_COMPRESSION);
        if (cret != Z_OK) {
            return -8;
        }

        std::vector<unsigned char> decomp2(input.size());
        uLong decomp2_size = static_cast<uLong>(decomp2.size());
        int uret = uncompress(decomp2.data(), &decomp2_size,
                              comp2.data(), comp2_size);
        if (uret != Z_OK || decomp2_size != sourceLen ||
            std::memcmp(decomp2.data(), input.data(), sourceLen) != 0) {
            return -9;
        }
    }

    // If we reached here, all zlib API usage succeeded and data integrity is preserved.
    return 66;
}