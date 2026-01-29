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
//<ID> 1522
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: gzopen + gzwrite + crc32_z + zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong crc_before = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Configure (get version and initialize deflate stream)
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 3: Operate (open gz file and write payload)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);
    int valid_write = (rc_write == (int)srcLen);

    // step 4: Validate and Cleanup (close, end deflate, free resources)
    int rc_close = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&strm);
    delete [] src;
    (void)crc_before;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_write;
    (void)valid_write;
    (void)rc_close;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}