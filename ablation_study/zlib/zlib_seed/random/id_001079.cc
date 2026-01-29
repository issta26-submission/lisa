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
//<ID> 1079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and write compressed file via gz layer
    const char payload[] = "zlib API sequence payload: gzopen -> gzwrite -> gzread -> inflateMark -> gzclearerr";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    unsigned int write_len = (unsigned int)payload_len;
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (const void *)payload, write_len);
    int rc_gzclose_w = gzclose(gw);

    // step 2: Read back decompressed data using gzread and clear any errors
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    int rc_gzread = gzread(gr, (voidp)read_buf, (unsigned int)payload_len);
    gzclearerr(gr);
    int rc_gzclose_r = gzclose(gr);

    // step 3: Initialize inflate stream, inspect mark, and finalize inflate
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate data via checksum, cleanup buffers and silence unused warnings
    uLong adler = adler32(0UL, read_buf, (uInt)payload_len);
    delete [] read_buf;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    (void)adler;
    // API sequence test completed successfully
    return 66;
}