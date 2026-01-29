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
//<ID> 1092
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong DATA_LEN = 256UL;
    Bytef *data = (Bytef *)malloc((size_t)DATA_LEN);
    memset(data, 'Z', (size_t)DATA_LEN);
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, 1 << 15);
    const char *ver = zlibVersion();

    // step 2: Open a file descriptor and bind a gzFile via gzdopen, write data and query position
    int fd = open("test_zlib_api_sequence.tmp", O_RDWR | O_CREAT | O_TRUNC, 0600);
    gzFile gf = gzdopen(fd, "w+b");
    gzwrite(gf, data, (unsigned int)DATA_LEN);
    off_t position_after_write = gztell(gf);
    gzrewind(gf);
    int first_char = gzgetc_(gf);

    // step 3: Initialize an inflate-back stream and immediately clean it up via inflateBackEnd
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);

    // step 4: Cleanup resources and finalize
    gzclose(gf);
    free(window);
    free(data);
    (void)position_after_write;
    (void)first_char;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}