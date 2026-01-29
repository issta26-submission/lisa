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
//<ID> 2114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(27.25);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *threshold_item = cJSON_AddNumberToObject(meta, "threshold", 15.0);
    cJSON_AddItemToObject(root, "direct_number", cJSON_CreateNumber(42.0));

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    double detached_val = cJSON_GetNumberValue(detached);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    int digit = (int)detached_val;
    buffer[0] = (char)('0' + (digit % 10));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[1] = (char)('0' + (int)printed_ok);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}