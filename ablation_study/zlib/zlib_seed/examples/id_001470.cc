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
//<ID> 1470
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
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    infstrm.next_in = src;
    infstrm.avail_in = (uInt)srcLen;
    uLong crc_src = crc32(0L, src, (uInt)srcLen);
    uLong adler_src = adler32(0L, src, (uInt)srcLen);

    // step 2: Setup
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 4, 32, 16, 128);
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);
    int rc_inflateback_init = inflateBackInit_(&infstrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Core operations
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzw = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzw, src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    Bytef *readbuf = new Bytef[(size_t)srcLen];
    memset(readbuf, 0, (size_t)srcLen);
    int readBytes = gzread(gzr, readbuf, (unsigned int)srcLen);
    uLong crc_read = crc32(0L, readbuf, (uInt)readBytes);
    uLong adler_read = adler32(0L, readbuf, (uInt)readBytes);
    uLong adler_combined = adler32_combine(adler_src, adler_read, (off_t)readBytes);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    int rc_inflateback_end = inflateBackEnd(&infstrm);
    int rc_gzclose_r = gzclose(gzr);
    delete [] src;
    delete [] readbuf;
    delete [] window;
    (void)version;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_inflateback_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)readBytes;
    (void)crc_src;
    (void)crc_read;
    (void)adler_src;
    (void)adler_read;
    (void)adler_combined;
    (void)rc_defend;
    (void)rc_inflateback_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}