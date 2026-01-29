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
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and write it to a gzFile
    const char payload[] = "zlib API sequence payload exercising gzwrite, gzread, inflateInit_, gzflush and deflateParams";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    unsigned int write_len = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, write_len);
    int rc_gzflush = gzflush(gf, 0);

    // step 2: Initialize a deflate stream, adjust parameters, then clean it up
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&dstrm, 5, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Rewind the gzFile and read the compressed data back into a buffer
    int rc_gzrewind = gzrewind(gf);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    unsigned int read_len = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, read_len);

    // step 4: Initialize inflate, set input/output, perform inflate, cleanup all resources
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);

    delete [] read_buf;
    delete [] out_buf;

    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_end;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}