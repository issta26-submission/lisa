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
//<ID> 2450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateArray();
    cJSON_AddItemToArray(child, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(child, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(child, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(root, "numbers", child);
    cJSON *label = cJSON_CreateString("values");
    cJSON_AddItemToObject(root, "label", label);
    cJSON *arr_ref = cJSON_CreateArrayReference(child);
    cJSON *total_item = cJSON_AddNumberToObject(root, "total", 60.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(child, 1);
    const char *label_str = cJSON_GetStringValue(label);
    buffer[1] = label_str && label_str[0] ? label_str[0] : '\0';
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    buffer[2] = (char)(int)has_numbers;
    cJSON_AddItemToArray(child, cJSON_CreateNumber(40.0));
    cJSON *fourth = cJSON_GetArrayItem(child, 3);
    buffer[3] = fourth ? (char)(fourth->valueint) : '\0';

    // step 4: Validate and Cleanup
    cJSON_Delete(arr_ref);
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}