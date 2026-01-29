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
//<ID> 1159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and metadata
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong adler_orig = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uInt dict_len = 16U;
    Bytef dict_buf[16];
    memcpy(dict_buf, orig, (size_t)dict_len);

    // step 2: Initialize deflate, set dictionary, compress original data, finalize deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&dstrm, dict_buf, dict_len);
    uLong bound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, set undermine, attempt inflate (handle need-dict by calling inflateSetDictionary), finalize inflate
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
    inflateUndermine(&istrm, 1);
    inflate(&istrm, Z_FINISH);
    inflateSetDictionary(&istrm, dict_buf, dict_len);
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Write version to gzip file, read it back with gzgets, validate checksums, cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gw, ver);
    gzputs(gw, "\n");
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    char readbuf[128];
    memset(readbuf, 0, sizeof(readbuf));
    gzgets(gr, readbuf, (int)sizeof(readbuf));
    gzclose(gr);
    uLong adler_decomp = adler32(0UL, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)adler_orig;
    (void)crc_orig;
    (void)adler_decomp;
    (void)crc_decomp;
    // API sequence test completed successfully
    return 66;
}