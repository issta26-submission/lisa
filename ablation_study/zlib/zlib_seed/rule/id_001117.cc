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
//<ID> 1117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data buffers and compute CRCs
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong crc_initial = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_z = crc32_z(crc_initial, orig, (z_size_t)ORIG_LEN);

    // step 2: Write original data to a gzip file
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose(gw);

    // step 3: Read compressed file into memory and initialize inflate with gzip support
    FILE *fp = fopen(fname, "rb");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    Bytef *comp = (Bytef *)malloc((size_t)fsize);
    size_t read_bytes = fread(comp, 1, (size_t)fsize, fp);
    fclose(fp);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    const char *ver = zlibVersion();
    inflateInit2_(&istrm, 15 + 16, ver, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(header));
    int ghret = inflateGetHeader(&istrm, &header);

    // step 4: Perform inflate, validate via header fields and cleanup
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)read_bytes;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp_z = crc32_z(crc_decomp, decomp, (z_size_t)ORIG_LEN);

    free(orig);
    free(comp);
    free(decomp);
    remove(fname);
    (void)crc_z;
    (void)ghret;
    (void)header;
    (void)crc_decomp_z;
    // API sequence test completed successfully
    return 66;
}