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
//<ID> 1095
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLong comp_len = bound;
    compress2(comp, (uLongf *)&comp_len, orig, ORIG_LEN, 6);

    // step 2: Write compressed data to a gzip file using gzwrite
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, comp, (unsigned int)comp_len);
    gzclose(gw);

    // step 3: Initialize inflateBack stream and immediately finalize it with inflateBackEnd
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, 1 << 15);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);

    // step 4: Open the gzip file for reading via gzdopen, read one byte with gzgetc_, get position with gztell, cleanup
    FILE *f = fopen("test_zlib_api_sequence.gz", "rb");
    int fd = fileno(f);
    gzFile gr = gzdopen(fd, "rb");
    int first_byte = gzgetc_(gr);
    off_t pos = gztell(gr);
    gzclose_r(gr);
    fclose(f);

    free(window);
    free(orig);
    free(comp);
    (void)first_byte;
    (void)pos;
    (void)comp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}