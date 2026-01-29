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
//<ID> 1475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[1,2],\"obj\":{\"key\":3},\"num\":4}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *num = cJSON_GetObjectItem(root, "num");

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, newnum);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(arr, obj_ref);
    cJSON *ref_flag = cJSON_CreateBool(add_ref_ok);
    cJSON_AddItemToObject(root, "ref_added", ref_flag);
    cJSON *num_is_number_flag = cJSON_CreateBool(cJSON_IsNumber(num));
    cJSON_AddItemToObject(root, "num_is_number", num_is_number_flag);
    cJSON *had_obj_flag = cJSON_CreateBool(cJSON_HasObjectItem(root, "obj"));
    cJSON_AddItemToObject(root, "had_obj", had_obj_flag);
    cJSON *detached_obj = cJSON_DetachItemFromObject(root, "obj");
    cJSON_AddItemToArray(arr, detached_obj);
    cJSON_DeleteItemFromObject(root, "num");

    // step 3: Operate & Validate
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
    // API sequence test completed successfully
    return 66;
}