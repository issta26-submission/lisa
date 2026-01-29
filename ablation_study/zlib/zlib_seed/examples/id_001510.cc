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
//<ID> 1510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: gzputs + gzseek64 + gzclearerr + deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    uLong crc = crc32(0L, (const Bytef *)payload, (uInt)srcLen);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.adler = crc;
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate stream)
    int rc_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 3: Operate (write payload to gz file, seek, and clear errors)
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_puts = gzputs(gzf, payload);
    off64_t pos = gzseek64(gzf, (off64_t)0, 0);
    gzclearerr(gzf);

    // step 4: Cleanup (end deflate and close gz file)
    int rc_end = deflateEnd(&strm);
    int rc_close = gzclose(gzf);
    (void)srcLen;
    (void)crc;
    (void)rc_init;
    (void)rc_puts;
    (void)pos;
    (void)rc_end;
    (void)rc_close;
    (void)version;
    // API sequence test completed successfully
    return 66;
}