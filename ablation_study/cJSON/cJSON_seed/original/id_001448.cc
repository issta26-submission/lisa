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
//<ID> 1448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[1,2,3],\"val\":true}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    (void)arr_is_array;

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *arr_dup = cJSON_Duplicate(arr, 1); /* make a safe copy instead of a reference */
    cJSON_AddItemToObject(nested, "ref", arr_dup);
    cJSON *new_num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 1, new_num);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    (void)first_invalid;
    double first_value = cJSON_GetNumberValue(first);
    (void)first_value;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    cJSON_PrintPreallocated(root, buffer, (int)(printed_len + 1), 0);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}