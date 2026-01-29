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
//<ID> 1514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char payload[] = "zlib api sequence payload for gzputs + deflate demo";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buffer = new Bytef[(size_t)srcLen];
    memset(buffer, 0, (size_t)srcLen);
    memcpy(buffer, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&strm, srcLen);
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_puts = gzputs(gzf, payload);

    // step 3: Operate / Validate
    gzclearerr(gzf);
    off64_t pos = gzseek64(gzf, 0LL, 0);

    // step 4: Cleanup
    int rc_close = gzclose(gzf);
    int rc_def_end = deflateEnd(&strm);
    delete [] buffer;
    (void)rc_def_init;
    (void)bound;
    (void)rc_puts;
    (void)pos;
    (void)rc_close;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}