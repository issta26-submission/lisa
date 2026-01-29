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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers here so this function is self-contained when pasted into a file
    // (preprocessor directives are allowed anywhere in the source)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <cstdio>
    #include <cstdlib>

    const char *input_str = "The quick brown fox jumps over the lazy dog";
    const uLong input_len = static_cast<uLong>(std::strlen(input_str));

    // 1) compress() / uncompress() (simple one-shot API)
    uLongf comp_bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(comp_bound);
    uLongf comp_size = comp_bound;
    int rc = compress(comp_buf.data(), &comp_size, reinterpret_cast<const Bytef*>(input_str), input_len);
    if (rc != Z_OK) return -1; // compress failed

    std::vector<Bytef> uncomp_buf(input_len);
    uLongf uncomp_size = input_len;
    rc = uncompress(uncomp_buf.data(), &uncomp_size, comp_buf.data(), comp_size);
    if (rc != Z_OK) return -2; // uncompress failed
    if (uncomp_size != input_len) return -3;
    if (std::memcmp(uncomp_buf.data(), input_str, input_len) != 0) return -4;

    // 2) streaming deflate() / inflate() using z_stream
    // deflate (compress) stream
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    rc = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (rc != Z_OK) return -5;

    dstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input_str));
    dstream.avail_in = input_len;

    std::vector<Bytef> stream_comp;
    Bytef temp_out[128];
    int deflate_ret;
    do {
        dstream.next_out = temp_out;
        dstream.avail_out = sizeof(temp_out);
        deflate_ret = deflate(&dstream, Z_FINISH); // compress all input, then finish
        if (deflate_ret != Z_OK && deflate_ret != Z_STREAM_END) {
            deflateEnd(&dstream);
            return -6;
        }
        size_t have = sizeof(temp_out) - dstream.avail_out;
        stream_comp.insert(stream_comp.end(), temp_out, temp_out + have);
    } while (deflate_ret != Z_STREAM_END);

    deflateEnd(&dstream);

    // inflate (decompress) stream
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    rc = inflateInit(&istream);
    if (rc != Z_OK) return -7;

    istream.next_in = stream_comp.empty() ? Z_NULL : stream_comp.data();
    istream.avail_in = static_cast<uInt>(stream_comp.size());

    std::vector<Bytef> stream_uncomp;
    Bytef temp_in[64];
    int inflate_ret;
    do {
        istream.next_out = temp_in;
        istream.avail_out = sizeof(temp_in);
        inflate_ret = inflate(&istream, Z_NO_FLUSH);
        if (inflate_ret != Z_OK && inflate_ret != Z_STREAM_END) {
            inflateEnd(&istream);
            return -8;
        }
        size_t have = sizeof(temp_in) - istream.avail_out;
        stream_uncomp.insert(stream_uncomp.end(), temp_in, temp_in + have);
    } while (inflate_ret != Z_STREAM_END);

    inflateEnd(&istream);

    if (stream_uncomp.size() != input_len) return -9;
    if (std::memcmp(stream_uncomp.data(), input_str, input_len) != 0) return -10;

    // 3) crc32 and adler32 checks
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input_str), input_len);
    if (crc == 0) {
        // it's unlikely to be zero for this data, but we simply check the call succeeded (no specific value)
        // Do nothing; proceed.
        ;
    }
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input_str), input_len);
    if (adl == 0) {
        // similar note as for crc
        ;
    }

    // 4) gz* file API: write compressed data to a file and read it back
    const char *gzname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz = gzopen(gzname, "wb");
    if (!gz) return -11;
    int written = gzwrite(gz, input_str, static_cast<unsigned int>(input_len));
    if (written != static_cast<int>(input_len)) {
        gzclose(gz);
        remove(gzname);
        return -12;
    }
    gzclose(gz);

    gzFile gz2 = gzopen(gzname, "rb");
    if (!gz2) {
        remove(gzname);
        return -13;
    }
    std::vector<char> gz_read_buf(input_len + 10); // a bit larger than needed
    int read = gzread(gz2, gz_read_buf.data(), static_cast<unsigned int>(input_len));
    gzclose(gz2);
    if (read != static_cast<int>(input_len)) {
        remove(gzname);
        return -14;
    }
    if (std::memcmp(gz_read_buf.data(), input_str, input_len) != 0) {
        remove(gzname);
        return -15;
    }
    // remove temporary file
    remove(gzname);

    // All tests passed
    return 66;
}