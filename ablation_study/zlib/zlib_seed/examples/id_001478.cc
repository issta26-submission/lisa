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
//<ID> 1478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzread/deflateTune/crc32/adler32_combine/inflateBackEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&backstrm, 0, sizeof(backstrm));

    // step 2: Setup
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 4, 8, 16, 32);

    const unsigned int windowBits = 15u;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);
    int rc_backinit = inflateBackInit_(&backstrm, (int)windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Core operations
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, src, (unsigned int)srcLen);
    int rc_gwclose = gzclose(gw);

    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = new Bytef[(size_t)srcLen];
    memset(readBuf, 0, (size_t)srcLen);
    int readLen = gzread(gr, readBuf, (unsigned int)srcLen);

    uLong crc = crc32(0L, readBuf, (uInt)readLen);
    uLong ad1 = adler32(0L, readBuf, (uInt)readLen);
    uLong ad2 = adler32(0L, src, (uInt)srcLen);
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)readLen);

    int rc_backend = inflateBackEnd(&backstrm);
    int rc_defend = deflateEnd(&defstrm);

    // step 4: Cleanup
    int rc_grclose = gzclose(gr);
    delete [] src;
    delete [] readBuf;
    delete [] window;
    (void)version;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_backinit;
    (void)rc_gzwrite;
    (void)rc_gwclose;
    (void)readLen;
    (void)crc;
    (void)combined_ad;
    (void)rc_backend;
    (void)rc_defend;
    (void)rc_grclose;
    // API sequence test completed successfully
    return 66;
}