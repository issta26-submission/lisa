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
//<ID> 1513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence: gzputs + gzseek64 + gzclearerr + deflateEnd";
    gzFile gzf = gzopen64("test_zlib_api_sequence_output.gz", "wb");
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(strm));
    const char *ver = zlibVersion();

    // step 2: Initialize (prepare a deflate stream)
    int rc_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Operate (write payload to gz, seek and clear errors)
    int rc_puts = gzputs(gzf, payload);
    off64_t pos = gzseek64(gzf, 0LL, 0);
    gzclearerr(gzf);

    // step 4: Cleanup and finalize
    int rc_end = deflateEnd(&strm);
    int rc_close = gzclose(gzf);
    (void)rc_init;
    (void)rc_puts;
    (void)pos;
    (void)rc_end;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}