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
//<ID> 2182
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *value = cJSON_AddNumberToObject(root, "value", 3.1415);

    // step 2: Configure
    cJSON *item1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, item1);
    cJSON *item2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, item2);
    cJSON *newval = cJSON_CreateNumber(2.71828);
    cJSON_ReplaceItemInObject(root, "value", newval);

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "array");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}