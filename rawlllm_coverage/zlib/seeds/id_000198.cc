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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare a reasonably long, repetitive string to ensure good compression.
    std::string original;
    for (int i = 0; i < 100; ++i) {
        original += "The quick brown fox jumps over the lazy dog. ";
    }

    // ---- 1) compress() / uncompress() API usage ----
    uLong sourceLen = static_cast<uLong>(original.size());
    uLongf compBound = compressBound(sourceLen);
    std::vector<Bytef> compBuf(compBound);
    uLongf compLen = compBound;

    int ret = compress(compBuf.data(), &compLen, reinterpret_cast<const Bytef*>(original.data()), sourceLen);
    if (ret != Z_OK) {
        return 1;
    }

    std::vector<Bytef> uncompressedBuf(sourceLen);
    uLongf uncompLen = sourceLen;
    ret = uncompress(uncompressedBuf.data(), &uncompLen, compBuf.data(), compLen);
    if (ret != Z_OK || uncompLen != sourceLen) {
        return 2;
    }
    if (std::memcmp(uncompressedBuf.data(), original.data(), sourceLen) != 0) {
        return 3;
    }

    // ---- 2) deflateInit / deflate / deflateEnd (z_stream) ----
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 4;

    uLongf deflateOutBound = compressBound(sourceLen); // safe upper bound
    std::vector<Bytef> deflateOut(deflateOutBound);
    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    dstrm.avail_in = sourceLen;
    dstrm.next_out = deflateOut.data();
    dstrm.avail_out = static_cast<uInt>(deflateOutBound);

    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&dstrm); return 5; }
    uLong deflatedLen = dstrm.total_out;
    deflateEnd(&dstrm);

    // ---- 3) inflateInit / inflate / inflateEnd (z_stream) ----
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    ret = inflateInit(&istrm);
    if (ret != Z_OK) return 6;

    std::vector<Bytef> inflateOut(sourceLen);
    istrm.next_in = deflateOut.data();
    istrm.avail_in = static_cast<uInt>(deflatedLen);
    istrm.next_out = inflateOut.data();
    istrm.avail_out = static_cast<uInt>(sourceLen);

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&istrm); return 7; }
    uLong inflatedLen = istrm.total_out;
    inflateEnd(&istrm);

    if (inflatedLen != sourceLen) return 8;
    if (std::memcmp(inflateOut.data(), original.data(), sourceLen) != 0) return 9;

    // ---- 4) Checksums: adler32 and crc32 ----
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(original.data()), sourceLen);

    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(original.data()), sourceLen);

    (void)adl; // used to exercise the API; not checked further here
    (void)crc;

    // ---- 5) gzopen / gzwrite / gzread / gzclose ----
    const char *filename = "test_zlib_api_sequence.gz";
    gzFile gz = gzopen(filename, "wb");
    if (!gz) return 10;

    int wrote = gzwrite(gz, original.data(), static_cast<unsigned int>(original.size()));
    if (wrote != static_cast<int>(original.size())) {
        gzclose(gz);
        std::remove(filename);
        return 11;
    }
    gzclose(gz);

    gz = gzopen(filename, "rb");
    if (!gz) {
        std::remove(filename);
        return 12;
    }
    std::vector<char> gzReadBuf(original.size());
    int read = gzread(gz, gzReadBuf.data(), static_cast<unsigned int>(gzReadBuf.size()));
    gzclose(gz);
    if (read != static_cast<int>(original.size())) {
        std::remove(filename);
        return 13;
    }
    if (std::memcmp(gzReadBuf.data(), original.data(), original.size()) != 0) {
        std::remove(filename);
        return 14;
    }
    std::remove(filename);

    // ---- 6) Misc: zlibVersion() and compressBound() demonstration ----
    const char *zv = zlibVersion();
    (void)zv;
    uLong cbound = compressBound(sourceLen);
    (void)cbound;

    // All zlib operations succeeded
    return 66;
}