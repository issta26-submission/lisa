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
//<ID> 971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Write compressed data to a gzip file and close the write side
    gzFile wf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(wf, comp, (unsigned int)comp_len);
    gzclose_w(wf);

    // step 3: Reopen the gzip file for reading and read compressed data back
    gzFile rf = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *comp2 = (Bytef *)malloc((size_t)comp_len);
    memset(comp2, 0, (size_t)comp_len);
    int read_bytes = gzread(rf, comp2, (unsigned int)comp_len);
    gzclose(rf);

    // step 4: Initialize inflate stream, validate, inflate the read-compressed data, cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateValidate(&istrm, 1);
    istrm.next_in = comp2;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    free(orig);
    free(comp);
    free(comp2);
    free(decomp);
    (void)ver;
    (void)read_bytes;
    (void)comp_len;
    // API sequence test completed successfully
    return 66;
}