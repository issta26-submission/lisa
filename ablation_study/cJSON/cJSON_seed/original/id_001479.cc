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
//<ID> 1479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[10,20],\"obj\":{\"k\":5},\"name\":\"root\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(99.9);
    cJSON_AddItemToArray(arr, newnum);
    cJSON *ref_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(ref_obj, "r", cJSON_CreateNumber(7.0));
    cJSON_AddItemToObject(root, "refobj", ref_obj);
    cJSON *ref_wrapper = cJSON_CreateObjectReference(ref_obj);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(arr, ref_wrapper);

    // step 3: Operate & Validate
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    cJSON_bool has_refobj = cJSON_HasObjectItem(root, "refobj");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed ? printed : "", printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    (void)add_ref_ok;
    (void)first_is_num;
    (void)has_refobj;

    // step 4: Cleanup
    cJSON *detached_obj = cJSON_DetachItemFromObject(root, "obj");
    cJSON_DeleteItemFromObject(root, "name");
    cJSON_Delete(detached_obj);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}