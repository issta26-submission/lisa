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
//<ID> 2188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(numbers, n0);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *scalar = cJSON_AddNumberToObject(root, "scalar", 10.5);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *new_scalar = cJSON_CreateNumber(20.25);
    cJSON_ReplaceItemInObject(root, "scalar", new_scalar);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(numbers);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 2));
    double sum = v0 + v1 + v2;
    cJSON_AddNumberToObject(meta, "sum", sum);
    cJSON_AddNumberToObject(meta, "count", (double)count);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "numbers");
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}