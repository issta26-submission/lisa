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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary includes (allowed inside a function for a self-contained snippet)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <cstdio>

    // Sample data to compress / checksum / write to gzip
    const char* sample = "The quick brown fox jumps over the lazy dog. "
                         "Zlib API sequence test data. Repeat to enlarge: ";
    // Make the input somewhat larger by repeating
    std::string input;
    for (int i = 0; i < 16; ++i) input += sample;
    size_t input_len = input.size();

    // 1) compress / uncompress (simple API)
    uLongf comp_bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> comp_buf(comp_bound);
    uLongf comp_size = comp_bound;
    int zret = compress(comp_buf.data(), &comp_size,
                        reinterpret_cast<const Bytef*>(input.data()),
                        static_cast<uLong>(input_len));
    if (zret != Z_OK) return 1;

    // decompress using uncompress()
    std::vector<unsigned char> decomp_buf(input_len);
    uLongf decomp_size = static_cast<uLong>(input_len);
    zret = uncompress(decomp_buf.data(), &decomp_size,
                      comp_buf.data(), comp_size);
    if (zret != Z_OK) return 2;
    if (decomp_size != input_len) return 3;
    if (std::memcmp(decomp_buf.data(), input.data(), input_len) != 0) return 4;

    // 2) deflate / inflate using z_stream (streaming API)
    // Deflate
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    zret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (zret != Z_OK) return 5;

    uLongf stream_bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> stream_comp(stream_bound);
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(input_len > UINT_MAX ? UINT_MAX : input_len);
    defstrm.next_out = stream_comp.data();
    defstrm.avail_out = static_cast<uInt>(stream_comp.size() > UINT_MAX ? UINT_MAX : stream_comp.size());

    // Since input is not huge, one call with Z_FINISH should work
    zret = deflate(&defstrm, Z_FINISH);
    if (zret != Z_STREAM_END) { deflateEnd(&defstrm); return 6; }
    size_t stream_comp_size = stream_comp.size() - defstrm.avail_out;
    deflateEnd(&defstrm);

    // Inflate
    z_stream instrm;
    std::memset(&instrm, 0, sizeof(instrm));
    zret = inflateInit(&instrm);
    if (zret != Z_OK) return 7;

    std::vector<unsigned char> stream_decomp(input_len);
    instrm.next_in = stream_comp.data();
    instrm.avail_in = static_cast<uInt>(stream_comp_size > UINT_MAX ? UINT_MAX : stream_comp_size);
    instrm.next_out = stream_decomp.data();
    instrm.avail_out = static_cast<uInt>(stream_decomp.size() > UINT_MAX ? UINT_MAX : stream_decomp.size());

    zret = inflate(&instrm, Z_FINISH);
    if (zret != Z_STREAM_END) { inflateEnd(&instrm); return 8; }
    size_t stream_decomp_size = stream_decomp.size() - instrm.avail_out;
    inflateEnd(&instrm);

    if (stream_decomp_size != input_len) return 9;
    if (std::memcmp(stream_decomp.data(), input.data(), input_len) != 0) return 10;

    // 3) Checksums: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_len > UINT_MAX ? UINT_MAX : input_len));
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input_len > UINT_MAX ? UINT_MAX : input_len));
    // Basic sanity: checksums should be non-zero for non-empty input
    if (crc == 0 || adler == 0) return 11;

    // 4) gz* file API: write and read a gzip file
    const char* fname = "test_zlib_api_sequence_temp.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return 12;
    int wrote = gzwrite(gz, input.data(), static_cast<unsigned int>(input_len > UINT_MAX ? UINT_MAX : input_len));
    if (wrote == 0 || wrote != static_cast<int>(input_len)) { gzclose(gz); return 13; }
    if (gzclose(gz) != Z_OK) return 14;

    // reopen and read
    gz = gzopen(fname, "rb");
    if (!gz) { std::remove(fname); return 15; }
    std::vector<char> gz_read_buf(input_len);
    int read = gzread(gz, gz_read_buf.data(), static_cast<unsigned int>(gz_read_buf.size() > UINT_MAX ? UINT_MAX : gz_read_buf.size()));
    if (read <= 0 || static_cast<size_t>(read) != input_len) { gzclose(gz); std::remove(fname); return 16; }

    // test gzseek (seek back to beginning and read again)
    if (gzseek(gz, 0, SEEK_SET) == -1) { gzclose(gz); std::remove(fname); return 17; }
    std::vector<char> gz_read_buf2(input_len);
    read = gzread(gz, gz_read_buf2.data(), static_cast<unsigned int>(gz_read_buf2.size() > UINT_MAX ? UINT_MAX : gz_read_buf2.size()));
    if (read <= 0 || static_cast<size_t>(read) != input_len) { gzclose(gz); std::remove(fname); return 18; }

    if (std::memcmp(gz_read_buf.data(), input.data(), input_len) != 0) { gzclose(gz); std::remove(fname); return 19; }
    if (std::memcmp(gz_read_buf2.data(), input.data(), input_len) != 0) { gzclose(gz); std::remove(fname); return 20; }

    if (gzclose(gz) != Z_OK) { std::remove(fname); return 21; }
    // remove temporary file
    std::remove(fname);

    // If we reach here, all zlib API sequences succeeded
    return 66;
}