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
//<ID> 1469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[10,20],\"obj\":{\"key\":\"val\"}}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(99.9);
    cJSON_AddItemToArray(arr, newnum);
    // create a reference to the existing object 'obj' (valid use of CreateObjectReference)
    cJSON *first_ref = cJSON_CreateObjectReference(obj);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(arr, first_ref);
    cJSON *replacement_str = cJSON_CreateString("replaced");
    cJSON_bool replace_obj_ok = cJSON_ReplaceItemInObject(obj, "key", replacement_str);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *replacement_num = cJSON_CreateNumber(123.0);
    cJSON_bool replace_via_ptr_ok = cJSON_ReplaceItemViaPointer(arr, second, replacement_num);

    // step 3: Operate & Validate
    cJSON_bool first_ref_invalid = cJSON_IsInvalid(first_ref);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    (void)add_ref_ok;
    (void)replace_obj_ok;
    (void)replace_via_ptr_ok;
    (void)first_ref_invalid;
    // API sequence test completed successfully
    return 66;
}