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
//<ID> 1097
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffer
    const uLong DATA_LEN = 128UL;
    Bytef *data = (Bytef *)malloc((size_t)DATA_LEN);
    memset(data, 'Z', (size_t)DATA_LEN);
    const char *ver = zlibVersion();

    // step 2: Create a gzip file via gzdopen and write data with gzwrite, record position with gztell, close writer
    int fdw = open("test_zlib_api_sequence.gz", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    gzFile gw = gzdopen(fdw, "wb");
    int written = gzwrite(gw, (voidpc)data, (unsigned int)DATA_LEN);
    off_t write_pos = gztell(gw);
    gzclose_w(gw);

    // step 3: Open the gzip file for reading via gzdopen, read one character with gzgetc_, query position, close reader
    int fdr = open("test_zlib_api_sequence.gz", O_RDONLY);
    gzFile gr = gzdopen(fdr, "rb");
    int first_char = gzgetc_(gr);
    off_t read_pos = gztell(gr);
    gzclose_r(gr);

    // step 4: Initialize inflateBack state and then finalize it; cleanup allocations
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, 1 << 15);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);
    free(window);
    free(data);
    (void)written;
    (void)write_pos;
    (void)first_char;
    (void)read_pos;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}