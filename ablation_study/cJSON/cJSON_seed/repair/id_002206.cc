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
//<ID> 2206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    float floats_in[3] = {1.1f, 2.2f, 3.3f};
    cJSON *floatArray = cJSON_CreateFloatArray(floats_in, 3);
    cJSON_AddItemToObject(root, "floats", floatArray);

    // step 2: Configure
    cJSON *flag = cJSON_AddFalseToObject(root, "enabled");
    cJSON *replacement = cJSON_CreateNumber(9.9);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(floatArray, 1, replacement);
    (void)replaced;
    (void)flag;

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "floats");
    cJSON *second_item = cJSON_GetArrayItem(retrieved, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *added = cJSON_AddNumberToObject(root, "second_value", second_val);
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
    (void)added;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}