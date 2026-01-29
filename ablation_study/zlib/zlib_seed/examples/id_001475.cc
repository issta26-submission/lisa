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
//<ID> 1475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflateTune/gzread/crc32/adler32_combine/inflateBackEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&backstrm, 0, sizeof(backstrm));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;

    // step 2: Setup
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 64, 128, 128, 32);
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);
    int rc_backinit = inflateBackInit_(&backstrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Core operations
    gzFile gzf_w = gzopen("test_zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_out.gz", "rb");
    Bytef *readbuf = new Bytef[(size_t)srcLen];
    memset(readbuf, 0, (size_t)srcLen);
    int bytesRead = gzread(gzf_r, readbuf, (unsigned int)srcLen);
    uLong crc_initial = crc32(0UL, src, (uInt)srcLen);
    uLong crc_after = crc32(crc_initial, readbuf, (uInt)bytesRead);
    uLong ad1 = crc32(0UL, src, (uInt)srcLen); // reuse crc32 as a checksum for demonstration
    uLong ad2 = crc32(0UL, readbuf, (uInt)bytesRead);
    uLong combined_adler = adler32_combine(ad1, ad2, (off_t)bytesRead);
    int rc_back_end = inflateBackEnd(&backstrm);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    int rc_gzclose_r = gzclose(gzf_r);
    delete [] src;
    delete [] readbuf;
    delete [] window;
    (void)version;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_backinit;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)bytesRead;
    (void)crc_initial;
    (void)crc_after;
    (void)ad1;
    (void)ad2;
    (void)combined_adler;
    (void)rc_back_end;
    (void)rc_defend;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}