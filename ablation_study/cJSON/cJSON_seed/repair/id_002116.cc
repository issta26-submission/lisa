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
//<ID> 2116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", array);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(array, n1);
    cJSON *n3 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(array, n3);
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 314159.0 / 100000.0);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(array, 0);
    double detached_val = cJSON_GetNumberValue(detached);
    cJSON_bool printed_ok;
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = (char)('0' + (int)printed_ok);
    buffer[1] = (char)('0' + (int)detached_val);
    buffer[2] = (char)('0' + (int)cJSON_GetNumberValue(pi_item));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}