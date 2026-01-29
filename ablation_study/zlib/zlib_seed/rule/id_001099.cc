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
//<ID> 1099
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong LEN = 256UL;
    Bytef *buf = (Bytef *)malloc((size_t)LEN);
    memset(buf, 'Z', (size_t)LEN);
    const char *ver = zlibVersion();

    // step 2: Create a gzip file and write data
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, buf, (unsigned int)LEN);
    gzclose(gw);

    // step 3: Open file descriptor and exercise gzdopen, gzgetc_, gztell, and close
    int fd = open("test_zlib_api_sequence_tmp.gz", 0);
    gzFile gr = gzdopen(fd, "rb");
    int first_byte = gzgetc_(gr);
    off_t position = gztell(gr);
    gzclose(gr);

    // step 4: Initialize inflateBack and then call inflateBackEnd, cleanup
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);
    free(window);
    free(buf);
    remove("test_zlib_api_sequence_tmp.gz");
    (void)first_byte;
    (void)position;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}