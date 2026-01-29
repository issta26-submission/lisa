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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {

    // Prepare some sample data
    const std::string input = std::string("The quick brown fox jumps over the lazy dog. ")
                              + "Repeat to enlarge. "
                              + "The quick brown fox jumps over the lazy dog. "
                              + "The quick brown fox jumps over the lazy dog.";
    // 1) Use compress()/uncompress() convenience APIs.
    uLong src_len = static_cast<uLong>(input.size());
    uLong comp_bound = compressBound(src_len);
    std::vector<Bytef> comp_buf(comp_bound);

    int zret = compress(comp_buf.data(), &comp_bound, reinterpret_cast<const Bytef*>(input.data()), src_len);
    if (zret != Z_OK) return 1; // compress failed

    // Decompress back
    std::vector<Bytef> decomp_buf(src_len);
    uLong decomp_len = src_len;
    zret = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_bound);
    if (zret != Z_OK) return 2; // uncompress failed

    if (decomp_len != src_len) return 3;
    if (std::memcmp(decomp_buf.data(), input.data(), src_len) != 0) return 4;

    // 2) Use deflate/inflate stream APIs (z_stream)
    {
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        zret = deflateInit(&defstream, Z_BEST_COMPRESSION);
        if (zret != Z_OK) return 5;

        defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
        defstream.avail_in = static_cast<uInt>(src_len);

        std::vector<Bytef> def_out_buf(compressBound(src_len));
        defstream.next_out = def_out_buf.data();
        defstream.avail_out = static_cast<uInt>(def_out_buf.size());

        // Call deflate with Z_FINISH; buffer is large enough (compressBound)
        zret = deflate(&defstream, Z_FINISH);
        if (zret != Z_STREAM_END) { deflateEnd(&defstream); return 6; }

        uLong def_size = def_out_buf.size() - defstream.avail_out;
        deflateEnd(&defstream);

        // Now inflate the deflated data
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        infstream.next_in = def_out_buf.data();
        infstream.avail_in = static_cast<uInt>(def_size);

        zret = inflateInit(&infstream);
        if (zret != Z_OK) return 7;

        std::vector<Bytef> inflate_out_buf(src_len);
        infstream.next_out = inflate_out_buf.data();
        infstream.avail_out = static_cast<uInt>(inflate_out_buf.size());

        zret = inflate(&infstream, Z_FINISH);
        if (zret != Z_STREAM_END) { inflateEnd(&infstream); return 8; }

        inflateEnd(&infstream);

        if (std::memcmp(inflate_out_buf.data(), input.data(), src_len) != 0) return 9;
    }

    // 3) Use gzopen/gzwrite/gzread/gzclose for .gz file operations
    const char* gz_filename = "test_zlib_api_sequence_tmp.gz";
    {
        gzFile gz = gzopen(gz_filename, "wb");
        if (!gz) return 10;
        int written = gzwrite(gz, input.data(), static_cast<unsigned int>(input.size()));
        if (written != static_cast<int>(input.size())) { gzclose(gz); return 11; }
        if (gzclose(gz) != Z_OK) return 12;
    }

    // Read back
    {
        gzFile gz = gzopen(gz_filename, "rb");
        if (!gz) return 13;
        std::vector<char> read_buf(input.size());
        int nread = gzread(gz, read_buf.data(), static_cast<unsigned int>(read_buf.size()));
        if (nread < 0) { gzclose(gz); return 14; }
        if (gzclose(gz) != Z_OK) return 15;
        if (static_cast<size_t>(nread) != input.size()) return 16;
        if (std::memcmp(read_buf.data(), input.data(), input.size()) != 0) return 17;
    }

    // Clean up temp file
    std::remove(gz_filename);

    // If we reached here, all zlib sequences succeeded
    return 66;
}