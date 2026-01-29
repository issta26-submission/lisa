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
//<ID> 1472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "test payload for gzread/deflateTune/crc32 sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream defstrm;
    z_stream inflstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&inflstrm, 0, sizeof(inflstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    const char *version = zlibVersion();

    // step 2: Setup
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 4, 8, 16, 32);
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);
    int rc_inflatebackinit = inflateBackInit_(&inflstrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Core operations
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned int readBufLen = (unsigned int)srcLen;
    Bytef *readBuf = new Bytef[(size_t)readBufLen];
    memset(readBuf, 0, (size_t)readBufLen);
    int rc_gzread = gzread(gzf_r, readBuf, readBufLen);
    uLong crc = crc32(0UL, readBuf, (uInt)readBufLen);
    off_t len_off = (off_t)readBufLen;
    uLong combined_adler = adler32_combine(crc, crc, len_off);
    int rc_inflatebackend = inflateBackEnd(&inflstrm);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    int rc_gzclose_r = gzclose(gzf_r);
    delete [] src;
    delete [] readBuf;
    delete [] window;
    (void)version;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_inflatebackinit;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)crc;
    (void)combined_adler;
    (void)rc_inflatebackend;
    (void)rc_defend;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}