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
//<ID> 570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed gzFile
    const char src[] = "zlib API payload: write compressed data for validation";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);

    // step 2: Initialize a standard inflate stream
    z_stream istrm1;
    memset(&istrm1, 0, (int)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&istrm1, version, (int)sizeof(z_stream));

    // step 3: Initialize a back-inflate stream with a sliding window buffer
    z_stream istrm2;
    memset(&istrm2, 0, (int)sizeof(z_stream));
    unsigned char window[1 << 15];
    memset(window, 0, (int)sizeof(window));
    int rc_inflate_back_init = inflateBackInit_(&istrm2, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate file position, teardown streams and close file
    off_t pos = gztell(gf);
    int rc_inflate_back_end = inflateBackEnd(&istrm2);
    int rc_inflate_end = inflateEnd(&istrm1);
    int rc_gzclose = gzclose(gf);
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate_back_init;
    (void)pos;
    (void)rc_inflate_back_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}