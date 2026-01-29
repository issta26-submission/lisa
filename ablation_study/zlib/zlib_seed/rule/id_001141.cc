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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and compress data
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate, configure stream, and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);

    // step 3: Write compressed data to a gzip file, read it back with gzfread, and clear errors
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_size);
    gzclose(gf);
    gzFile gf_read = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)bound);
    memset(readbuf, 0, (size_t)bound);
    z_size_t items_read = gzfread(readbuf, 1, (z_size_t)comp_size, gf_read);
    (void)items_read;
    gzclearerr(gf_read);
    gzclose(gf_read);

    // step 4: Finalize inflate/back-end, cleanup resources
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    free(readbuf);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}