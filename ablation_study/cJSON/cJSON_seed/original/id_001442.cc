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
//<ID> 1442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"root\":{\"arr\":[1,2],\"child\":{\"x\":10}},\"flag\":true}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *arr = cJSON_GetObjectItem(root_obj, "arr");
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_elem);

    // step 2: Configure
    cJSON *added_obj = cJSON_AddObjectToObject(root_obj, "added");
    cJSON_AddNumberToObject(added_obj, "n", 42.0);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON *new_number = cJSON_CreateNumber(99.0);
    cJSON_InsertItemInArray(arr_ref, 1, new_number);

    // step 3: Operate & Validate
    cJSON_bool arr_is_array = cJSON_IsArray(arr_ref);
    cJSON *inserted = cJSON_GetArrayItem(arr_ref, 1);
    double inserted_val = cJSON_GetNumberValue(inserted);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    (void)first_is_invalid;
    (void)arr_is_array;
    (void)inserted_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}