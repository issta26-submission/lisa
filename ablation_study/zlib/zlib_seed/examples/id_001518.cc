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
//<ID> 1518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char payload[] = "zlib api sequence payload for gzputs + gzseek64 + gzclearerr + deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    // step 2: Setup / Configure
    int rc_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong ad = adler32(1UL, src, (uInt)srcLen);

    // step 3: Operate & Validate
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_puts = gzputs(gzf, payload);
    off64_t pos = gzseek64(gzf, 0, 0);
    gzclearerr(gzf);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&strm);
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)rc_init;
    (void)ad;
    (void)rc_puts;
    (void)pos;
    (void)rc_defend;
    (void)rc_close;
    (void)version;
    // API sequence test completed successfully
    return 66;
}