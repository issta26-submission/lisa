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
//<ID> 1479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload for crc32/adler32/deflateTune/gzread/inflateBackEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));

    // step 2: Setup (initialize deflate and compute checksums)
    uLong crc = crc32(0L, src, (uInt)srcLen);
    uLong ad1 = adler32(1L, src, (uInt)(srcLen / 2));
    uLong ad2 = adler32(1L, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)(srcLen - (srcLen / 2)));
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_tune = deflateTune(&defstrm, 32, 258, 258, 1024);

    // step 3: Core operations (write gz file, then read it back)
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_write = gzwrite(gzf_w, src, (unsigned int)srcLen);
    int rc_close_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readBuf = new Bytef[(size_t)srcLen];
    memset(readBuf, 0, (size_t)srcLen);
    int bytes_read = gzread(gzf_r, readBuf, (unsigned int)srcLen);

    // step 4: Cleanup and finalize streams
    int rc_defend = deflateEnd(&defstrm);
    int rc_inflatebackend = inflateBackEnd(&infstrm);
    int rc_close_r = gzclose(gzf_r);
    delete [] src;
    delete [] readBuf;
    (void)version;
    (void)crc;
    (void)ad_combined;
    (void)rc_definit;
    (void)rc_tune;
    (void)rc_write;
    (void)rc_close_w;
    (void)bytes_read;
    (void)rc_defend;
    (void)rc_inflatebackend;
    (void)rc_close_r;
    // API sequence test completed successfully
    return 66;
}