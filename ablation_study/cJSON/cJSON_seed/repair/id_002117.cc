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
//<ID> 2117
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(arr, n2);
    cJSON *scale = cJSON_AddNumberToObject(root, "scale", 3.5);
    cJSON *label = cJSON_CreateString("fuzz_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    double detached_val = cJSON_GetNumberValue(detached);
    int digit = (int)detached_val;
    buffer[0] = (char)('0' + (digit % 10));
    buffer[1] = (char)('0' + ((int)cJSON_GetNumberValue(scale) % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}