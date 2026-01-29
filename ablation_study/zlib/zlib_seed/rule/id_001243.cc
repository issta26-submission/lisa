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
//<ID> 1243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial data preparation
    const uLong ORIG_LEN = 200UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const uInt HALF = (uInt)(ORIG_LEN / 2UL);
    uLong ad1 = adler32(1UL, orig, HALF);
    uLong ad2 = adler32(1UL, orig + HALF, HALF);
    off_t off_half = (off_t)HALF;
    uLong combined_adler = adler32_combine(ad1, ad2, off_half);

    // step 2: Initialize and configure deflate and inflate streams and header
    const char *ver = zlibVersion();
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = 0x5A5A5A5AUL;
    Bytef *name_buf = (Bytef *)malloc(8);
    memset(name_buf, 0, 8);
    memcpy(name_buf, "fname", 6);
    header.name = name_buf;
    header.name_max = 6;

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateSetHeader(&dstrm, &header);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Operate: compress data, feed to inflate stream, sync, and use gzFile
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    inflateSync(&istrm);

    gzFile gf = gzopen("testzlib_tmp.gz", "wb");
    gzwrite(gf, orig, (unsigned int)ORIG_LEN);
    gzclose_r(gf);

    // step 4: Cleanup and finalize
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(name_buf);
    // API sequence test completed successfully
    return 66;
}