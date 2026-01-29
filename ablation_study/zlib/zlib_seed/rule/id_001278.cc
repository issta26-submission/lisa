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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute adler32, and write a gzip file
    const uLong SRC_LEN = 128UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong checksum = adler32(1UL, src, (uInt)SRC_LEN);
    (void)checksum;
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);

    // step 2: Read back the gzip file using gzread into a buffer
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    uLong read_len = SRC_LEN;
    Bytef *readbuf = (Bytef *)malloc((size_t)read_len);
    gzread(gr, readbuf, (unsigned int)read_len);
    gzclose(gr);

    // step 3: Initialize deflate stream, compress the read buffer
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, read_len);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    dstrm.next_in = readbuf;
    dstrm.avail_in = (uInt)read_len;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4); /* Z_FINISH */

    // step 4: Initialize inflate stream, set a dictionary and finalize inflate internals
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&istrm, src, (uInt)SRC_LEN);
    inflateBackEnd(&istrm);

    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(src);
    free(readbuf);
    free(comp);
    // API sequence test completed successfully
    return 66;
}