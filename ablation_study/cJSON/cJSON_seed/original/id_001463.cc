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
//<ID> 1463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[1,2],\"obj\":{\"key\":\"value\"},\"val\":10}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInObject(root, "val", new_num);
    cJSON *created_array = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(created_array, obj);
    cJSON_AddItemToObject(root, "refs", created_array);
    cJSON *replacement = cJSON_CreateNumber(7.5);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_ReplaceItemViaPointer(arr, second, replacement);

    // step 3: Operate & Validate
    cJSON *ref_node = cJSON_GetArrayItem(created_array, 0);
    cJSON_bool ref_is_invalid = cJSON_IsInvalid(ref_node);
    cJSON_AddItemReferenceToArray(arr, new_num);
    double replaced_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    (void)first_invalid;
    (void)ref_is_invalid;
    (void)replaced_value;

    // step 4: Cleanup
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}