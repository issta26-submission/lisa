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
//<ID> 1928
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
    cJSON_AddItemToObject(root, "items", array);
    cJSON *optional_null = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *true_item1 = cJSON_CreateTrue();
    cJSON *true_item2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, true_item1);
    cJSON_AddItemToArray(array, true_item2);

    // step 3: Operate and Validate
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    cJSON_bool is_null_flag = cJSON_IsNull(optional_null);
    int array_size = cJSON_GetArraySize(array);
    buffer[252] = (char)('0' + ((int)printed_ok % 10));
    buffer[253] = (char)('0' + ((int)is_null_flag % 10));
    buffer[254] = (char)('0' + (array_size % 10));
    buffer[255] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}