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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream dstrm;
    gzFile gzf;
    Bytef dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    Bytef getbuf[64];
    uInt getbuf_len = (uInt)sizeof(getbuf);
    uInt returned_len = 0;
    off64_t seek_ret;
    int ret_deflate_init;
    int ret_setdict;
    int ret_reset;
    int ret_getdict;
    int bytes_written;
    int ret_deflate_end;
    int ret_gzclose;

    // step 2: Setup
    memset(&dstrm, 0, sizeof(dstrm));
    ret_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), sizeof(z_stream));
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb+");

    // step 3: Operate & Validate
    ret_setdict = deflateSetDictionary(&dstrm, dict, dict_len);
    ret_reset = deflateReset(&dstrm);
    returned_len = getbuf_len;
    ret_getdict = deflateGetDictionary(&dstrm, getbuf, &returned_len);
    seek_ret = gzseek64(gzf, (off64_t)10, SEEK_SET);
    bytes_written = gzwrite(gzf, getbuf, returned_len);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&dstrm);
    ret_gzclose = gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_deflate_init; (void)ret_setdict; (void)ret_reset; (void)ret_getdict; (void)seek_ret; (void)bytes_written; (void)ret_deflate_end; (void)ret_gzclose;
    return 66;
}