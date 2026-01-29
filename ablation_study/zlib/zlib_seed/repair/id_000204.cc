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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    Bytef input_buf[] = "zlib API sequence payload: compress-init, write to gzFile via gzopen64, rewind and cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    const char *ver;
    gzFile gz;
    int ret_def_init;
    int ret_def_end;
    int ret_infb_end;
    int ret_gz_write;
    int ret_gz_rewind;
    int ret_gz_close;
    unsigned int write_len = (unsigned int)input_len;

    // step 2: Setup
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    defstrm.next_in = input_buf;
    defstrm.avail_in = (uInt)input_len;

    // step 3: Core operations
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)input_buf, write_len);
    ret_gz_rewind = gzrewind(gz);
    ret_infb_end = inflateBackEnd(&infstrm);

    // step 4: Cleanup and validation
    ret_def_end = deflateEnd(&defstrm);
    ret_gz_close = gzclose(gz);
    (void)ret_def_init;
    (void)ret_def_end;
    (void)ret_infb_end;
    (void)ret_gz_write;
    (void)ret_gz_rewind;
    (void)ret_gz_close;
    (void)ver;
    (void)write_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}