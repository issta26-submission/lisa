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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include necessary headers (allowed anywhere; these bring in zlib and C/C++ utilities)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>
    #include <cstdio>

    // Helper lambda for convenience error handling
    auto fail = [](const char* msg) -> int {
        // In a full program you might log msg; here just return non-66 value
        (void)msg;
        return -1;
    };

    const char* sample = "The quick brown fox jumps over the lazy dog";
    const size_t sample_len = std::strlen(sample); // do not include terminating NUL in content checks

    // 1) Compute checksums: adler32 and crc32
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(sample_len));

    // Basic sanity: checksums must be non-zero for this input
    if (adl == 0 || crc == 0) return fail("checksum failed");

    // 2) compress() / uncompress() simple usage
    uLong bound = compressBound(static_cast<uLong>(sample_len));
    std::vector<Bytef> comp_buf(bound);
    uLong comp_len = bound;
    int ret = compress(comp_buf.data(), &comp_len,
                       reinterpret_cast<const Bytef*>(sample),
                       static_cast<uLong>(sample_len));
    if (ret != Z_OK) return fail("compress failed");

    // uncompress back
    std::vector<Bytef> decomp_buf(sample_len + 1); // +1 for safety if we wanted to place a NUL
    uLong decomp_len = static_cast<uLong>(decomp_buf.size());
    ret = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len);
    if (ret != Z_OK) return fail("uncompress failed");
    if (decomp_len != sample_len) return fail("uncompressed length mismatch");
    if (std::memcmp(decomp_buf.data(), sample, sample_len) != 0) return fail("uncompressed data mismatch");

    // 3) compress2() with different compression level
    std::vector<Bytef> comp2_buf(compressBound(static_cast<uLong>(sample_len)));
    uLong comp2_len = static_cast<uLong>(comp2_buf.size());
    ret = compress2(comp2_buf.data(), &comp2_len,
                    reinterpret_cast<const Bytef*>(sample),
                    static_cast<uLong>(sample_len),
                    Z_BEST_COMPRESSION);
    if (ret != Z_OK) return fail("compress2 failed");
    // Basic check: comp2_len should be >= 0 and decompression should produce original
    std::vector<Bytef> decomp2_buf(sample_len);
    uLong decomp2_len = static_cast<uLong>(decomp2_buf.size());
    ret = uncompress(decomp2_buf.data(), &decomp2_len, comp2_buf.data(), comp2_len);
    if (ret != Z_OK || decomp2_len != sample_len) return fail("uncompress(comp2) failed");
    if (std::memcmp(decomp2_buf.data(), sample, sample_len) != 0) return fail("decompressed(comp2) mismatch");

    // 4) Use z_stream with deflate/inflate (zlib wrapper)
    {
        z_stream strm;
        std::memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return fail("deflateInit failed");

        // feed input
        strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
        strm.avail_in = static_cast<uInt>(sample_len);

        std::vector<Bytef> stream_comp;
        stream_comp.resize(compressBound(static_cast<uLong>(sample_len)));
        strm.next_out = stream_comp.data();
        strm.avail_out = static_cast<uInt>(stream_comp.size());

        ret = deflate(&strm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            deflateEnd(&strm);
            return fail("deflate did not finish");
        }
        uLong stream_comp_len = static_cast<uLong>(stream_comp.size() - strm.avail_out);
        ret = deflateEnd(&strm);
        if (ret != Z_OK) return fail("deflateEnd failed");

        // Inflate it back using inflateInit
        z_stream istrm;
        std::memset(&istrm, 0, sizeof(istrm));
        istrm.zalloc = Z_NULL;
        istrm.zfree = Z_NULL;
        istrm.opaque = Z_NULL;

        ret = inflateInit(&istrm);
        if (ret != Z_OK) return fail("inflateInit failed");

        istrm.next_in = stream_comp.data();
        istrm.avail_in = static_cast<uInt>(stream_comp_len);
        std::vector<Bytef> stream_decomp(sample_len);
        istrm.next_out = stream_decomp.data();
        istrm.avail_out = static_cast<uInt>(stream_decomp.size());

        ret = inflate(&istrm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            inflateEnd(&istrm);
            return fail("inflate did not finish");
        }
        uLong stream_decomp_len = static_cast<uLong>(stream_decomp.size() - istrm.avail_out);
        ret = inflateEnd(&istrm);
        if (ret != Z_OK) return fail("inflateEnd failed");

        if (stream_decomp_len != sample_len) return fail("stream decompressed length mismatch");
        if (std::memcmp(stream_decomp.data(), sample, sample_len) != 0) return fail("stream decompressed data mismatch");
    }

    // 5) Use raw deflate/inflate via deflateInit2/inflateInit2 with negative windowBits
    {
        z_stream raw_strm;
        std::memset(&raw_strm, 0, sizeof(raw_strm));
        raw_strm.zalloc = Z_NULL;
        raw_strm.zfree = Z_NULL;
        raw_strm.opaque = Z_NULL;

        // -MAX_WBITS means raw deflate (no zlib header or trailer)
        ret = deflateInit2(&raw_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
        if (ret != Z_OK) return fail("deflateInit2 (raw) failed");

        raw_strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
        raw_strm.avail_in = static_cast<uInt>(sample_len);
        std::vector<Bytef> raw_comp(compressBound(static_cast<uLong>(sample_len)));
        raw_strm.next_out = raw_comp.data();
        raw_strm.avail_out = static_cast<uInt>(raw_comp.size());

        ret = deflate(&raw_strm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            deflateEnd(&raw_strm);
            return fail("raw deflate did not finish");
        }
        uLong raw_comp_len = static_cast<uLong>(raw_comp.size() - raw_strm.avail_out);
        deflateEnd(&raw_strm);

        // inflate raw
        z_stream raw_istrm;
        std::memset(&raw_istrm, 0, sizeof(raw_istrm));
        raw_istrm.zalloc = Z_NULL;
        raw_istrm.zfree = Z_NULL;
        raw_istrm.opaque = Z_NULL;

        ret = inflateInit2(&raw_istrm, -MAX_WBITS);
        if (ret != Z_OK) return fail("inflateInit2 (raw) failed");

        raw_istrm.next_in = raw_comp.data();
        raw_istrm.avail_in = static_cast<uInt>(raw_comp_len);
        std::vector<Bytef> raw_decomp(sample_len);
        raw_istrm.next_out = raw_decomp.data();
        raw_istrm.avail_out = static_cast<uInt>(raw_decomp.size());

        ret = inflate(&raw_istrm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            inflateEnd(&raw_istrm);
            return fail("raw inflate did not finish");
        }
        uLong raw_decomp_len = static_cast<uLong>(raw_decomp.size() - raw_istrm.avail_out);
        inflateEnd(&raw_istrm);

        if (raw_decomp_len != sample_len) return fail("raw decompressed length mismatch");
        if (std::memcmp(raw_decomp.data(), sample, sample_len) != 0) return fail("raw decompressed data mismatch");
    }

    // 6) gz* interface: write to a .gz file and read back
    const char* gz_filename = "test_zlib_api_sequence.gz";
    {
        gzFile gz = gzopen(gz_filename, "wb");
        if (!gz) return fail("gzopen (write) failed");
        int wrote = gzwrite(gz, reinterpret_cast<const void*>(sample), static_cast<unsigned int>(sample_len));
        if (wrote != static_cast<int>(sample_len)) {
            gzclose(gz);
            return fail("gzwrite failed");
        }
        // flush and close
        if (gzclose(gz) != Z_OK) return fail("gzclose (write) failed");
    }
    {
        gzFile gz = gzopen(gz_filename, "rb");
        if (!gz) return fail("gzopen (read) failed");
        std::vector<char> gz_read_buf(sample_len + 1);
        int read = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(sample_len));
        if (read < 0) { gzclose(gz); return fail("gzread error"); }
        if (static_cast<size_t>(read) != sample_len) { gzclose(gz); return fail("gzread length mismatch"); }
        if (std::memcmp(gz_read_buf.data(), sample, sample_len) != 0) { gzclose(gz); return fail("gzread data mismatch"); }
        if (gzclose(gz) != Z_OK) return fail("gzclose (read) failed");
    }

    // Remove the temporary gz file; ignore remove result (not critical)
    std::remove(gz_filename);

    // If we reached this point, all zlib API usage checks succeeded
    return 66;
}