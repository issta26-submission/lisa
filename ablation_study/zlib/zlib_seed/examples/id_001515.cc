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
//<ID> 1515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api seq: gzputs + gzseek64 + gzclearerr + deflateInit/end";
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const char *version = zlibVersion();
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int rc_puts = gzputs(gzf, payload);
    off64_t seek_pos = gzseek64(gzf, 0, 0);
    gzclearerr(gzf);

    // step 3: Operate and Validate
    int rc_deflate_end = deflateEnd(&strm);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    (void)rc_init;
    (void)rc_puts;
    (void)seek_pos;
    (void)rc_deflate_end;
    (void)rc_close;
    (void)version;
    // API sequence test completed successfully
    return 66;
}