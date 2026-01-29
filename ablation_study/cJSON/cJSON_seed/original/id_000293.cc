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
//<ID> 293
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
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, f);
    cJSON *inner_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(inner_obj, "inner_key", "value");
    cJSON_AddItemToArray(arr, inner_obj);

    // step 2: Configure
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_blob", "{\"raw_number\":123}");
    cJSON *replacement = cJSON_CreateNumber(99.99);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, replacement);

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_false = cJSON_IsFalse(second_item);
    cJSON_bool inner_is_object = cJSON_IsObject(inner_obj);
    cJSON_AddNumberToObject(root, "second_is_false", (double)second_is_false);
    cJSON_AddNumberToObject(root, "inner_is_object", (double)inner_is_object);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}