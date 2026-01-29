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
//<ID> 1471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for crc32/adler32/deflateTune/gzread/inflateBackEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_val = crc32(0UL, src, (uInt)srcLen);
    uLong adler1 = adler32(1UL, src, (z_size_t)srcLen);
    uLong adler_combined = adler32_combine(adler1, adler1, (off_t)srcLen);

    // step 2: Setup
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 4, 32, 16, 128);

    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);
    int rc_inflate_back_init = inflateBackInit_(&infstrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Core operations
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gw, src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose_w(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = new Bytef[(size_t)srcLen];
    memset(readbuf, 0, (size_t)srcLen);
    int rc_gzread = gzread(gr, readbuf, (unsigned int)srcLen);
    int rc_inflate_back_end = inflateBackEnd(&infstrm);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    int rc_gzclose_r = gzclose(gr);
    delete [] src;
    delete [] readbuf;
    delete [] window;
    (void)version;
    (void)crc_val;
    (void)adler1;
    (void)adler_combined;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_inflate_back_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_inflate_back_end;
    (void)rc_defend;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}