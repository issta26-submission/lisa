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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream dstrm;
    z_stream istrm;
    Bytef input_buf[] = "zlib API sequence test payload for gzopen64/gzwrite with deflate/inflate backend lifecycle.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    unsigned char window_buf[1];
    Bytef write_buf[512];
    const char *ver;
    gzFile gz;
    uLong bound_estimate;
    unsigned int to_write1;
    unsigned int to_write2;
    int ret_deflate_init;
    int ret_inflateBackInit;
    int ret_deflate_end;
    int ret_inflateBackEnd;
    int ret_gzwrite1;
    int ret_gzrewind;
    int ret_gzwrite2;
    int ret_gzclose;

    // step 2: Setup
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(window_buf, 0, sizeof(window_buf));
    memset(write_buf, 0, sizeof(write_buf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    ret_inflateBackInit = inflateBackInit_(&istrm, 15, window_buf, ver, (int)sizeof(z_stream));

    // step 3: Core operations
    bound_estimate = deflateBound(&dstrm, input_len);
    to_write1 = (unsigned int)((input_len <= (uLong)sizeof(write_buf)) ? input_len : (uLong)sizeof(write_buf));
    to_write2 = (unsigned int)((bound_estimate <= (uLong)sizeof(write_buf)) ? bound_estimate : (uLong)sizeof(write_buf));
    memcpy(write_buf, input_buf, (size_t)to_write1);
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gzwrite1 = gzwrite(gz, (voidpc)write_buf, to_write1);
    ret_gzrewind = gzrewind(gz);
    ret_gzwrite2 = gzwrite(gz, (voidpc)write_buf, to_write2);

    // step 4: Cleanup and validation
    ret_gzclose = gzclose(gz);
    ret_inflateBackEnd = inflateBackEnd(&istrm);
    ret_deflate_end = deflateEnd(&dstrm);
    (void)ret_deflate_init;
    (void)ret_inflateBackInit;
    (void)ret_deflate_end;
    (void)ret_inflateBackEnd;
    (void)ret_gzwrite1;
    (void)ret_gzrewind;
    (void)ret_gzwrite2;
    (void)ret_gzclose;
    (void)ver;
    (void)bound_estimate;
    (void)to_write1;
    (void)to_write2;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}