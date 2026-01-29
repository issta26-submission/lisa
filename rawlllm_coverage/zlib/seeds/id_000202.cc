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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct sequence of zlib APIs:
    // - deflateInit, deflate (in a loop), deflateEnd
    // - inflateInit, inflate (in a loop), inflateEnd
    //
    // It compresses a sample byte buffer, then decompresses it and verifies
    // the decompressed data matches the original. Returns 66 on success.
    
    // Include zlib header here to make the function self-contained in many contexts.
    // It's valid to put include directives at function scope.
    #include <zlib.h>
    #include <vector>
    #include <cstring>
    
    const unsigned char sample_data[] =
        "The quick brown fox jumps over the lazy dog. "
        "This is a test of compressing and decompressing using zlib APIs.";
    const size_t sample_len = sizeof(sample_data) - 1; // don't count terminating NUL
    
    // Prepare buffers
    uLong src_len = static_cast<uLong>(sample_len);
    // compressed size: use compressBound to allocate a safe buffer
    uLong comp_bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(comp_bound);
    
    // Set up deflate (compression) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // failed to initialize
        return 0;
    }
    
    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(sample_data));
    defstream.avail_in = static_cast<uInt>(src_len); // safe because sample_len small
    defstream.next_out = comp_buf.data();
    defstream.avail_out = static_cast<uInt>(comp_buf.size());
    
    // compress in a loop until stream ends
    int defret;
    do {
        defret = deflate(&defstream, Z_FINISH);
        if (defret != Z_OK && defret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return 0;
        }
        // if not finished and no room in output, this would be handled if using smaller buffers.
        // Here we allocated compressBound so usually it's a single iteration finishing with Z_STREAM_END.
    } while (defret != Z_STREAM_END);
    
    // compressed size is total_out
    uLong comp_size = defstream.total_out;
    
    deflateEnd(&defstream);
    
    // Now set up inflate (decompression)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 0;
    }
    
    std::vector<unsigned char> decomp_buf(src_len); // we expect original size after decompression
    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(decomp_buf.size());
    
    // decompress in a loop until stream ends
    int infret;
    do {
        infret = inflate(&infstream, Z_FINISH);
        if (infret != Z_OK && infret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return 0;
        }
        // normally one iteration is enough because output buffer is sized to original length
    } while (infret != Z_STREAM_END);
    
    uLong decomp_size = infstream.total_out;
    inflateEnd(&infstream);
    
    // Validate sizes and contents
    if (decomp_size != src_len) {
        return 0;
    }
    if (std::memcmp(decomp_buf.data(), sample_data, src_len) != 0) {
        return 0;
    }
    
    // Success: return 66 as requested
    return 66;
}