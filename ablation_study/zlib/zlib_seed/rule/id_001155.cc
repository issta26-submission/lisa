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
//<ID> 1155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, checksums and dictionary
    const uLong ORIG_LEN = 64UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    Bytef dict_buf[16];
    memset(dict_buf, 'D', sizeof(dict_buf));
    const char *ver = zlibVersion();
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong adler_orig = adler32(0UL, orig, (uInt)ORIG_LEN);

    // step 2: Compress using deflate with a preset dictionary
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&dstrm, dict_buf, (uInt)sizeof(dict_buf));
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);

    // step 3: Decompress using inflate, supply dictionary and adjust undermine setting
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 4);
    inflateSetDictionary(&istrm, dict_buf, (uInt)sizeof(dict_buf));
    inflateUndermine(&istrm, 1);
    inflateEnd(&istrm);

    // step 4: Write compressed blob to a gzip file, read it back with gzgets, and cleanup
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, comp, (unsigned int)dstrm.total_out);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char read_line[128];
    memset(read_line, 0, sizeof(read_line));
    gzgets(gr, read_line, (int)sizeof(read_line));
    gzclose(gr);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_orig;
    (void)adler_orig;
    (void)read_line;
    // API sequence test completed successfully
    return 66;
}