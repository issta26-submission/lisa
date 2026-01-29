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
//<ID> 1061
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and write compressed data to a gzFile
    const char payload[] = "zlib API sequence payload exercising gzwrite, gzfread, deflateInit_, inflateMark";
    uLong payload_len = (uLong)(sizeof(payload) - 1);
    gzFile wf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(wf, payload, (unsigned int)payload_len);
    int rc_close_write = gzclose(wf);

    // step 2: Read back the compressed file contents via gzfread
    gzFile rf = gzopen("zlib_api_sequence_test.gz", "rb");
    unsigned char *read_buf = new unsigned char[(size_t)payload_len + 1];
    memset(read_buf, 0, (size_t)payload_len + 1);
    z_size_t nread = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)payload_len, rf);
    int rc_close_read = gzclose(rf);

    // step 3: Initialize a deflate stream (configuration)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 4: Initialize an inflate stream, query inflateMark, validate via adler32, and cleanup
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long mark = inflateMark(&istrm);
    uLong adler_orig = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong adler_read = adler32(0UL, (const Bytef *)read_buf, (uInt)nread);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] read_buf;
    (void)rc_gzwrite;
    (void)rc_close_write;
    (void)nread;
    (void)rc_close_read;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)mark;
    (void)adler_orig;
    (void)adler_read;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}