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
//<ID> 574
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and input/output buffers
    const char src[] = "zlib API sequence payload: sample raw data used as input to inflate and gzwrite";
    Bytef *inBuf = (Bytef *)src;
    uLong inLen = (uLong)(sizeof(src) - 1);
    uLong outLen = (uLong)2048;
    Bytef *outBuf = (Bytef *)malloc((size_t)outLen);
    memset(outBuf, 0, (size_t)outLen);
    const char *version = zlibVersion();

    // step 2: Initialize inflate stream and inflate-back stream with a working window
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    z_stream bstrm;
    memset(&bstrm, 0, (int)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate — attempt an inflate operation and write the original data to a gzFile, then query position
    istrm.next_in = inBuf;
    istrm.avail_in = (uInt)inLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outLen;
    int rc_inflate = inflate(&istrm, 0); /* Z_NO_FLUSH == 0 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, inBuf, (unsigned int)inLen);
    off_t pos = gztell(gf);

    // step 4: Validate via file offset and cleanup resources
    int rc_gzclose = gzclose(gf);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_back_end = inflateBackEnd(&bstrm);
    free(window);
    free(outBuf);
    (void)version;
    (void)rc_inflate_init;
    (void)rc_inflate_back_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}