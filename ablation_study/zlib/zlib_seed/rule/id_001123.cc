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
//<ID> 1123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers
    const char literal[] = "Hello zlib world!";
    const uLong msg_len = (uLong)sizeof(literal); // includes null terminator
    Bytef *orig = (Bytef *)malloc((size_t)msg_len);
    memcpy(orig, literal, (size_t)msg_len);
    const char *ver = zlibVersion();
    uLong bound = compressBound(msg_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf comp_size = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)msg_len);
    memset(decomp, 0, (size_t)msg_len);

    // step 2: Compress the original data
    compress2(comp, &comp_size, orig, msg_len, 6);

    // step 3: Initialize inflate, reset window bits, and inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset2(&istrm, 15);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)msg_len;
    inflate(&istrm, Z_FINISH);

    // step 4: Write decompressed string to a gzip file and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gf, (const char *)decomp);
    gzclose(gf);

    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}