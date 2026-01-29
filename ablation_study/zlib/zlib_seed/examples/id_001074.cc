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
//<ID> 1074
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare payload and allocate buffers
    const char payload[] = "zlib API sequence payload: gzwrite -> gzread -> inflateMark -> gzclearerr";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    memset((void *)read_buf, 0, (size_t)(payload_len ? payload_len : 1));

    // step 2: Configure - write payload into a gzip file
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzw, (const void *)payload, (unsigned int)payload_len);
    int rc_gzclose_w = gzclose(gzw);

    // step 3: Operate - open the gzip file for reading and read back the data, then clear errors
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int rc_gzread = gzread(gzr, (voidp)read_buf, (unsigned int)payload_len);
    gzclearerr(gzr);
    int rc_gzclose_r = gzclose(gzr);

    // step 4: Validate and cleanup - initialize an inflate stream, inspect mark, cleanup stream and buffers
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] read_buf;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}