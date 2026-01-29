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
//<ID> 1464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[1,2],\"obj\":{\"key\":\"value\"}}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);

    // step 2: Configure
    cJSON_bool added_ref = cJSON_AddItemReferenceToArray(arr, first);
    cJSON *new_string = cJSON_CreateString("newval");
    cJSON_bool replaced_in_object = cJSON_ReplaceItemInObject(obj, "key", new_string);

    // step 3: Operate & Validate
    cJSON *replacement_num = cJSON_CreateNumber(99.0);
    cJSON_bool replaced_via_ptr = cJSON_ReplaceItemViaPointer(arr, second, replacement_num);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    (void)added_ref;
    (void)replaced_in_object;
    (void)replaced_via_ptr;
    (void)first_invalid;

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}