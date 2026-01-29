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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Required headers (placed here so the function is self-contained for copy/paste).
    // Note: these preprocessor lines are processed before compilation; placing them
    // inside the function body is valid though uncommon.
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>
    #include <cstdio>
    #include <cstdlib>

    // Original data to compress/decompress
    const char* original = "The quick brown fox jumps over the lazy dog";
    uLong orig_len = static_cast<uLong>(std::strlen(original));

    // 1) Simple one-shot compress2/uncompress roundtrip
    uLongf max_comp = compressBound(orig_len);
    std::vector<unsigned char> comp_buf(max_comp);
    uLongf comp_size = max_comp;
    int ret = compress2(reinterpret_cast<Bytef*>(comp_buf.data()), &comp_size,
                        reinterpret_cast<const Bytef*>(original), orig_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1;

    std::vector<unsigned char> decomp_buf(orig_len + 1);
    uLongf decomp_size = static_cast<uLongf>(decomp_buf.size());
    ret = uncompress(reinterpret_cast<Bytef*>(decomp_buf.data()), &decomp_size,
                     reinterpret_cast<const Bytef*>(comp_buf.data()), comp_size);
    if (ret != Z_OK) return -2;

    // verify content
    if (decomp_size != orig_len || std::memcmp(decomp_buf.data(), original, orig_len) != 0) return -3;

    // 2) Checksum APIs: adler32 and crc32
    uLong adl = adler32(0L, Z_NULL, 0);
    uLong adl_data = adler32(adl, reinterpret_cast<const Bytef*>(original), orig_len);
    uLong crc = crc32(0L, Z_NULL, 0);
    uLong crc_data = crc32(crc, reinterpret_cast<const Bytef*>(original), orig_len);
    (void)adl_data; (void)crc_data; // values are computed; not strictly needed further

    // 3) Streaming deflate/inflate (z_stream) - default zlib (zlib-wrapped)
    {
        z_stream defstream;
        std::memset(&defstream, 0, sizeof(defstream));
        ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return -4;

        std::vector<unsigned char> def_out(comp_size + 64); // a bit extra
        defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
        defstream.avail_in = orig_len;
        defstream.next_out = def_out.data();
        defstream.avail_out = static_cast<uInt>(def_out.size());

        ret = deflate(&defstream, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK) {
            deflateEnd(&defstream);
            return -5;
        }
        uLong deflated_bytes = static_cast<uLong>(def_out.size()) - defstream.avail_out;
        deflateEnd(&defstream);

        // inflate the stream
        z_stream infstream;
        std::memset(&infstream, 0, sizeof(infstream));
        ret = inflateInit(&infstream);
        if (ret != Z_OK) return -6;

        std::vector<unsigned char> inf_out(orig_len + 1);
        infstream.next_in = def_out.data();
        infstream.avail_in = static_cast<uInt>(deflated_bytes);
        infstream.next_out = inf_out.data();
        infstream.avail_out = static_cast<uInt>(inf_out.size());

        ret = inflate(&infstream, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK) {
            inflateEnd(&infstream);
            return -7;
        }
        uLong inflated_bytes = static_cast<uLong>(inf_out.size()) - infstream.avail_out;
        inflateEnd(&infstream);

        if (inflated_bytes != orig_len || std::memcmp(inf_out.data(), original, orig_len) != 0) return -8;
    }

    // 4) deflateInit2/inflateInit2 to demonstrate gzip wrapper usage (windowBits = MAX_WBITS + 16)
    {
        z_stream s;
        std::memset(&s, 0, sizeof(s));
        // Create gzip-compressed data (windowBits = MAX_WBITS + 16 to add gzip header)
        ret = deflateInit2(&s, Z_BEST_SPEED, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
        if (ret != Z_OK) return -9;

        std::vector<unsigned char> gz_out(compressBound(orig_len) + 32);
        s.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
        s.avail_in = orig_len;
        s.next_out = gz_out.data();
        s.avail_out = static_cast<uInt>(gz_out.size());

        ret = deflate(&s, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK) {
            deflateEnd(&s);
            return -10;
        }
        uLong gz_size = static_cast<uLong>(gz_out.size()) - s.avail_out;
        deflateEnd(&s);

        // Inflate the gzip data
        z_stream si;
        std::memset(&si, 0, sizeof(si));
        ret = inflateInit2(&si, MAX_WBITS + 16); // accept gzip header
        if (ret != Z_OK) return -11;

        std::vector<unsigned char> gz_infl(orig_len + 1);
        si.next_in = gz_out.data();
        si.avail_in = static_cast<uInt>(gz_size);
        si.next_out = gz_infl.data();
        si.avail_out = static_cast<uInt>(gz_infl.size());

        ret = inflate(&si, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK) {
            inflateEnd(&si);
            return -12;
        }
        uLong gz_infl_size = static_cast<uLong>(gz_infl.size()) - si.avail_out;
        inflateEnd(&si);

        if (gz_infl_size != orig_len || std::memcmp(gz_infl.data(), original, orig_len) != 0) return -13;
    }

    // 5) Use gzFile API: gzopen, gzwrite, gzread, gzclose, gzerror
    const char* tmp_filename = "test_zlib_api_sequence_tmp.gz";
    gzFile gf = gzopen(tmp_filename, "wb");
    if (!gf) return -14;
    int written = gzwrite(gf, original, static_cast<unsigned int>(orig_len));
    int gzerrnum = 0;
    const char* gzerrstr = gzerror(gf, &gzerrnum);
    if (written <= 0 || gzerrnum != Z_OK) {
        gzclose(gf);
        std::remove(tmp_filename);
        return -15;
    }
    gzclose(gf);

    gf = gzopen(tmp_filename, "rb");
    if (!gf) {
        std::remove(tmp_filename);
        return -16;
    }
    std::vector<char> gz_readbuf(orig_len + 1, 0);
    int readbytes = gzread(gf, gz_readbuf.data(), static_cast<unsigned int>(orig_len));
    gzerrstr = gzerror(gf, &gzerrnum);
    if (readbytes != static_cast<int>(orig_len) || gzerrnum != Z_OK) {
        gzclose(gf);
        std::remove(tmp_filename);
        return -17;
    }
    gzclose(gf);
    if (std::memcmp(gz_readbuf.data(), original, orig_len) != 0) {
        std::remove(tmp_filename);
        return -18;
    }
    // remove temporary file
    std::remove(tmp_filename);

    // 6) Query zlib version string
    const char* version = zlibVersion();
    if (!version) return -19;
    (void)version; // not used further, just demonstrating API call

    // All zlib API usage checks succeeded; return the required success value
    return 66;
}