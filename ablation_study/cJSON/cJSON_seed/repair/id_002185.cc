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
//<ID> 2185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", values);
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(values, num1);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(values, num2);

    // step 2: Configure
    cJSON *count = cJSON_AddNumberToObject(data, "count", 2.0);
    cJSON *new_count = cJSON_CreateNumber(3.0);
    cJSON_ReplaceItemInObject(data, "count", new_count);

    // step 3: Operate and Validate
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(data, "values_copy", values_copy);
    cJSON_DeleteItemFromObjectCaseSensitive(data, "values");
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}