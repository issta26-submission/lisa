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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    z_stream istrm;
    unsigned char inbuf[256];
    unsigned char outbuf[512];
    gzFile gz = 0;
    memset(&istrm, 0, sizeof(istrm));
    memset(inbuf, 'A', sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    uLong sourceLen = (uLong)sizeof(inbuf);
    uLong cbufMax = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cbufMax);
    uLongf compLen = (uLongf)cbufMax;
    compress2(comp, &compLen, inbuf, sourceLen, 6);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)sizeof(outbuf);
    inflate(&istrm, 0);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outbuf, (unsigned int)istrm.total_out);
    gzflush(gz, 0);
    gzrewind(gz);
    int first_byte = gzgetc(gz);
    (void)first_byte;
    gzclose(gz);
    inflateEnd(&istrm);
    free(comp);
    return 66;
}