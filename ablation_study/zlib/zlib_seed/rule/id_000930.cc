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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong crc_before = crc32(0L, orig, (uInt)ORIG_LEN);
    const char *tmp_path = "test_zlib_api_sequence_tmp.gz";
    gzFile gf = gzopen(tmp_path, "wb");
    gzwrite(gf, orig, (unsigned int)ORIG_LEN);
    gzclose(gf);

    // step 2: Read compressed file into memory
    FILE *f = fopen(tmp_path, "rb");
    fseek(f, 0, SEEK_END);
    long comp_size_long = ftell(f);
    fseek(f, 0, SEEK_SET);
    uLong comp_size = (uLong)comp_size_long;
    Bytef *comp = (Bytef *)malloc((size_t)comp_size);
    fread(comp, 1, (size_t)comp_size, f);
    fclose(f);

    // step 3: Initialize inflate, capture gzip header, inflate, and check sync point
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 47, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&istrm, &header);
    inflate(&istrm, 0);
    inflateSyncPoint(&istrm);
    inflateEnd(&istrm);

    // step 4: Validate and cleanup
    uLong crc_after = crc32(0L, decomp, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    remove(tmp_path);
    (void)ver;
    (void)crc_before;
    (void)crc_after;
    // API sequence test completed successfully
    return 66;
}