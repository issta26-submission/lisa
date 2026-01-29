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
//<ID> 1473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"obj\":{\"to_detach\":256},\"arr\":[1,2,3]}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *first_in_arr = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromObject(obj, "to_detach");
    cJSON *collector = cJSON_CreateArray();
    cJSON *det_ref = cJSON_CreateObjectReference(detached);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(collector, det_ref);
    cJSON_AddItemToObject(root, "collector", collector);
    cJSON_DeleteItemFromObject(root, "arr");

    // step 3: Operate & Validate
    cJSON_bool is_num = cJSON_IsNumber(detached);
    cJSON_bool has_collector = cJSON_HasObjectItem(root, "collector");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    (void)add_ref_ok;
    (void)is_num;
    (void)has_collector;
    (void)first_in_arr;
    // API sequence test completed successfully
    return 66;
}