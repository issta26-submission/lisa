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
//<ID> 1474
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
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&backstrm, 0, sizeof(backstrm));

    // step 2: Setup (create gz file, initialize streams and window)
    gzFile gzf_w = gzopen("test_zlib_api_sequence_temp.gz", "wb");
    gzwrite(gzf_w, src, (unsigned int)srcLen);
    gzclose(gzf_w);
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_backinit = inflateBackInit_(&backstrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Core operations (tune deflate, compute checksums, read gz, finalize inflate back)
    int rc_tune = deflateTune(&defstrm, 8, 4, 16, 4096);
    uLong crc = crc32(0L, src, (uInt)srcLen);
    uInt half_len = (uInt)(srcLen / 2u);
    uLong ad1 = adler32(0L, src, half_len);
    uLong ad2 = adler32(0L, src + half_len, (uInt)(srcLen - half_len));
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)(srcLen - half_len));
    gzFile gzf_r = gzopen("test_zlib_api_sequence_temp.gz", "rb");
    Bytef *readbuf = new Bytef[(size_t)srcLen + 1];
    memset(readbuf, 0, (size_t)srcLen + 1);
    int readlen = gzread(gzf_r, readbuf, (unsigned int)srcLen);
    int rc_backend = inflateBackEnd(&backstrm);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    int rc_gzclose = gzclose(gzf_r);
    delete [] src;
    delete [] readbuf;
    delete [] window;
    (void)version;
    (void)rc_definit;
    (void)rc_backinit;
    (void)rc_tune;
    (void)crc;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)readlen;
    (void)rc_backend;
    (void)rc_defend;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}