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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers needed for this function. Including inside the function is valid,
    // and keeps this snippet self-contained.
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>

    // Sample data to compress / decompress / write / read
    const std::string original = "The quick brown fox jumps over the lazy dog. "
                                 "zlib API sequence test data. Repeat: "
                                 "The quick brown fox jumps over the lazy dog.";

    // 1) Simple compress() / uncompress() usage
    {
        uLong src_len = static_cast<uLong>(original.size());
        uLong bound = compressBound(src_len);
        std::vector<unsigned char> comp(bound);
        uLong comp_len = bound;

        int cret = compress(comp.data(), &comp_len,
                            reinterpret_cast<const Bytef*>(original.data()), src_len);
        if (cret != Z_OK) return -1;

        // Decompress with uncompress()
        std::vector<unsigned char> decomp(src_len); // we know expected size
        uLong decomp_len = src_len;
        int uret = uncompress(decomp.data(), &decomp_len, comp.data(), comp_len);
        if (uret != Z_OK) return -1;
        if (decomp_len != src_len) return -1;
        if (std::memcmp(decomp.data(), original.data(), src_len) != 0) return -1;
    }

    // 2) Streaming deflate / inflate using z_stream
    std::vector<unsigned char> stream_compressed;
    {
        z_stream strm;
        std::memset(&strm, 0, sizeof(strm));
        int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return -1;

        const Bytef* input = reinterpret_cast<const Bytef*>(original.data());
        uInt input_len = static_cast<uInt>(original.size());
        strm.next_in = const_cast<Bytef*>(input); // zlib API uses non-const pointer
        strm.avail_in = input_len;

        const size_t OUT_CHUNK = 128;
        std::vector<unsigned char> outbuf(OUT_CHUNK);

        // Call deflate with Z_FINISH to compress entire buffer
        do {
            strm.next_out = outbuf.data();
            strm.avail_out = static_cast<uInt>(outbuf.size());
            ret = deflate(&strm, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&strm);
                return -1;
            }
            size_t have = outbuf.size() - strm.avail_out;
            stream_compressed.insert(stream_compressed.end(), outbuf.data(), outbuf.data() + have);
        } while (ret != Z_STREAM_END);

        deflateEnd(&strm);
    }

    // Inflate the previously produced stream_compressed
    {
        z_stream istrm;
        std::memset(&istrm, 0, sizeof(istrm));
        int ret = inflateInit(&istrm);
        if (ret != Z_OK) return -1;

        istrm.next_in = stream_compressed.empty() ? Z_NULL : &stream_compressed[0];
        istrm.avail_in = static_cast<uInt>(stream_compressed.size());

        const size_t OUT_CHUNK = 128;
        std::vector<unsigned char> outbuf(OUT_CHUNK);
        std::vector<unsigned char> result;

        do {
            istrm.next_out = outbuf.data();
            istrm.avail_out = static_cast<uInt>(outbuf.size());
            ret = inflate(&istrm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&istrm);
                return -1;
            }
            size_t have = outbuf.size() - istrm.avail_out;
            result.insert(result.end(), outbuf.data(), outbuf.data() + have);
        } while (ret != Z_STREAM_END);

        inflateEnd(&istrm);

        if (result.size() != original.size()) return -1;
        if (std::memcmp(result.data(), original.data(), original.size()) != 0) return -1;
    }

    // 3) gz* convenience API: write and read a .gz file
    const char* fname = "test_zlib_api_sequence_tmp.gz";
    {
        gzFile gz = gzopen(fname, "wb");
        if (!gz) return -1;
        int written = gzwrite(gz, original.data(), static_cast<unsigned int>(original.size()));
        if (written == 0) { gzclose(gz); return -1; }
        if (gzclose(gz) != Z_OK) return -1;

        // Reopen and read back
        gz = gzopen(fname, "rb");
        if (!gz) { remove(fname); return -1; }
        std::vector<char> readbuf(original.size());
        int readbytes = gzread(gz, readbuf.data(), static_cast<unsigned int>(readbuf.size()));
        if (readbytes < 0) { gzclose(gz); remove(fname); return -1; }
        gzclose(gz);

        if (static_cast<size_t>(readbytes) != original.size()) { remove(fname); return -1; }
        if (std::memcmp(readbuf.data(), original.data(), original.size()) != 0) { remove(fname); return -1; }

        // cleanup file
        remove(fname);
    }

    // 4) checksum helpers: adler32 and crc32
    {
        uLong adl = adler32(0L, Z_NULL, 0);
        adl = adler32(adl, reinterpret_cast<const Bytef*>(original.data()),
                      static_cast<uInt>(original.size()));
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, reinterpret_cast<const Bytef*>(original.data()),
                    static_cast<uInt>(original.size()));
        // Use them in trivial sanity checks (non-zero for non-empty input)
        if (original.size() > 0) {
            if (adl == 0 || crc == 0) return -1;
        }
    }

    // If we reached here, all sequences succeeded
    return 66;
}