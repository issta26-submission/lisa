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
//<ID> 1519
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: gzputs + gzseek64 + gzclearerr + deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure and Operate (open gz file, write, seek, clear error)
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_puts = gzputs(gzf, payload);
    off64_t pos = gzseek64(gzf, 0, 0);
    gzclearerr(gzf);

    // step 3: Validate (close gz and finalize deflate stream)
    int rc_close = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&strm);

    // step 4: Cleanup
    delete [] buf;
    (void)rc_deflate_init;
    (void)rc_puts;
    (void)pos;
    (void)rc_close;
    (void)rc_deflate_end;
    (void)version;
    (void)srcLen;
    // API sequence test completed successfully
    return 66;
}