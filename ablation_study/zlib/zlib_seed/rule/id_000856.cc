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
//<ID> 856
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong adl_before = adler32(1UL, input, (uInt)INPUT_LEN);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;

    // step 2: Initialize deflate, compress data, and finalize deflate stream
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong compLen = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, decompress into buffer, write/read via gz, and compute checksums
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(out, 0, (size_t)INPUT_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)INPUT_LEN;
    inflate(&istrm, 4); /* Z_FINISH */
    uLong adl_after_inflate = adler32(1UL, out, (uInt)istrm.total_out);

    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gw, out, (unsigned int)istrm.total_out);
    gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(readbuf, 0, (size_t)INPUT_LEN);
    int readlen = gzread(gr, readbuf, (unsigned int)istrm.total_out);
    gzclose(gr);
    uLong adl_after_gzread = adler32(1UL, readbuf, (uInt)readlen);

    // step 4: Cleanup and finalize
    inflateEnd(&istrm);
    free(input);
    free(comp);
    free(out);
    free(readbuf);
    (void)ver;
    (void)bound;
    (void)adl_before;
    (void)adl_after_inflate;
    (void)adl_after_gzread;
    (void)compLen;
    (void)readlen;
    // API sequence test completed successfully
    return 66;
}