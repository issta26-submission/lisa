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
//<ID> 2187
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *count = cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON *new_count = cJSON_CreateNumber(3.0);
    cJSON_ReplaceItemInObject(root, "count", new_count);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n3);

    // step 3: Operate and Validate
    cJSON *got_count = cJSON_GetObjectItemCaseSensitive(root, "count");
    double val = cJSON_GetNumberValue(got_count);
    cJSON *marker = cJSON_CreateBool(val == 3.0);
    cJSON_AddItemToObject(meta, "count_is_three", marker);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "numbers");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}