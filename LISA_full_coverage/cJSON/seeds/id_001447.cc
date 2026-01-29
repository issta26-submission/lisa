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
//<ID> 1447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"meta\":{\"id\":42}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    (void)meta;

    // step 2: Configure
    cJSON *data_obj = cJSON_AddObjectToObject(root, "data");
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(200.0);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON_AddItemToObject(data_obj, "values", arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON *arr_ref = cJSON_CreateArrayReference(first_elem);
    cJSON_AddItemToObject(root, "ref", arr_ref);

    // step 3: Operate & Validate
    cJSON_bool is_array = cJSON_IsArray(arr);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_elem);
    double first_value = cJSON_GetNumberValue(first_elem);
    int count = cJSON_GetArraySize(arr);
    (void)is_array;
    (void)first_is_invalid;
    (void)first_value;
    (void)count;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}