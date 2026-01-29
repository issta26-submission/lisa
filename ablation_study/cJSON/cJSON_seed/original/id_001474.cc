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
//<ID> 1474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[10,20],\"obj\":{\"key\":3},\"x\":7}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, newnum);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(arr, obj_ref);
    cJSON *detached_x = cJSON_DetachItemFromObject(root, "x");
    cJSON_AddNumberToObject(root, "temp", 1.5);
    cJSON_DeleteItemFromObject(root, "temp");

    // step 3: Operate & Validate
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON_bool has_obj = cJSON_HasObjectItem(root, "obj");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(detached_x);
    cJSON_Delete(root);
    (void)add_ref_ok;
    (void)first_is_number;
    (void)has_obj;
    // API sequence test completed successfully
    return 66;
}