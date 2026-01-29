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
//<ID> 1441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"init\":true}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *parent = cJSON_AddObjectToObject(root, "parent");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(parent, "arr", arr);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, str);

    // step 2: Configure
    cJSON *ref = cJSON_CreateArrayReference(arr);
    cJSON_bool ref_is_array = cJSON_IsArray(ref);
    (void)ref_is_array;
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_elem);
    (void)first_is_invalid;
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, inserted);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON *retrieved = cJSON_GetObjectItem(parent, "arr");
    cJSON_bool retrieved_is_array = cJSON_IsArray(retrieved);
    (void)retrieved_is_array;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}