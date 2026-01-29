#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    double set_ret = 0.0;
    cJSON_bool print_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "answer", num);

    // step 3: Operate / Validate
    set_ret = cJSON_SetNumberHelper(num, 42.0);
    print_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    volatile char probe = buffer[0];
    (void)probe;
    (void)set_ret;
    (void)print_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}