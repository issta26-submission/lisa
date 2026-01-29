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
//<ID> 1473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for crc/adler/deflateTune/inflateBackEnd/gzread demo";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_all = crc32(0L, src, (uInt)srcLen);
    uLong adler_first = adler32(1L, src, (uInt)(srcLen / 2));
    uLong adler_second = adler32(1L, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    off_t adler_split_len = (off_t)(srcLen - (srcLen / 2));
    uLong adler_combined = adler32_combine(adler_first, adler_second, adler_split_len);

    // step 2: Setup
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 10, 20, 30, 40);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)compBound;

    // step 3: Core operations
    gzFile gzf = gzopen("test_zlib_api_sequence_temp.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose(gzf);
    gzFile gzfr = gzopen("test_zlib_api_sequence_temp.gz", "rb");
    Bytef *readBuf = new Bytef[(size_t)srcLen];
    memset(readBuf, 0, (size_t)srcLen);
    int bytes_read = gzread(gzfr, readBuf, (unsigned int)srcLen);
    int rc_gzclose_r = gzclose(gzfr);
    unsigned char *window = new unsigned char[(size_t)(1u << 15)];
    memset(window, 0, (size_t)(1u << 15));
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    int windowBits = 15;
    int rc_back_init = inflateBackInit_(&backstrm, windowBits, window, version, (int)sizeof(z_stream));
    unsigned long codes_used = inflateCodesUsed(&backstrm);
    int rc_back_end = inflateBackEnd(&backstrm);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    delete [] src;
    delete [] compBuf;
    delete [] readBuf;
    delete [] window;
    (void)version;
    (void)crc_all;
    (void)adler_first;
    (void)adler_second;
    (void)adler_combined;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)bytes_read;
    (void)rc_gzclose_r;
    (void)rc_back_init;
    (void)codes_used;
    (void)rc_back_end;
    (void)rc_defend;
    // API sequence test completed successfully
    return 66;
}