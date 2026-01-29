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
//<ID> 1201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzdopen/deflateCopy/adler32_z/inflateInit_/gzeof demo";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream src;
    z_stream copy;
    z_stream istrm;
    memset(&src, 0, (size_t)sizeof(z_stream));
    memset(&copy, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *outbuf = new unsigned char[4096];
    memset(outbuf, 0, (size_t)4096);
    int fd = 1;
    gzFile gzf = gzdopen(fd, "wb");

    // step 2: Setup (initialize deflate source and inflate stream)
    src.next_in = (Bytef *)payload;
    src.avail_in = (uInt)payload_len;
    src.next_out = outbuf;
    src.avail_out = (uInt)4096;
    int rc_def_init = deflateInit_(&src, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (compute adler32, copy deflate state, perform a deflate call, write and check gz)
    uLong checksum = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    int rc_defcopy = deflateCopy(&copy, &src);
    int rc_deflate = deflate(&src, 0);
    int rc_gzputs = gzputs(gzf, payload);
    int rc_gzeof = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_def_end_src = deflateEnd(&src);
    int rc_def_end_copy = deflateEnd(&copy);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] outbuf;
    (void)version;
    (void)rc_def_init;
    (void)rc_inflate_init;
    (void)checksum;
    (void)rc_defcopy;
    (void)rc_deflate;
    (void)rc_gzputs;
    (void)rc_gzeof;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}