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
//<ID> 1476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for gzread/deflateTune/crc32/adler32_combine/inflateBackEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;

    // step 2: Setup / Configure
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 4, 32, 16, 128);
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);
    int rc_inflateback_init = inflateBackInit_(&infstrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Operate / Validate
    uInt len1 = (uInt)(srcLen / 2u);
    uInt len2 = (uInt)(srcLen - (uLong)len1);
    uLong adler1 = adler32(1UL, src, len1);
    uLong adler2 = adler32(1UL, src + len1, len2);
    off_t offlen2 = (off_t)len2;
    uLong combined_adler = adler32_combine(adler1, adler2, offlen2);
    uLong crc_full = crc32(0UL, src, (uInt)srcLen);

    const char fname[] = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int rc_gzwrite = 0;
    if (gzf_w) rc_gzwrite = gzwrite(gzf_w, src, (unsigned int)srcLen);
    int rc_gzclose_w = 0;
    if (gzf_w) rc_gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *readBuf = new Bytef[(size_t)srcLen];
    memset(readBuf, 0, (size_t)srcLen);
    int rc_gzread = 0;
    if (gzf_r) rc_gzread = gzread(gzf_r, readBuf, (unsigned int)srcLen);
    int rc_gzclose_r = 0;
    if (gzf_r) rc_gzclose_r = gzclose(gzf_r);

    int rc_inflate_back_end = inflateBackEnd(&infstrm);

    // step 4: Cleanup
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_inflate_end = inflateEnd(&infstrm);
    delete [] src;
    delete [] readBuf;
    delete [] window;
    (void)version;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_inflateback_init;
    (void)adler1;
    (void)adler2;
    (void)combined_adler;
    (void)crc_full;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}