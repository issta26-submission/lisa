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
//<ID> 2391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "note", cJSON_CreateString("note"));

    // step 2: Configure
    char *old_val = cJSON_SetValuestring(str_item, "updated");
    char *printed1 = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool has_arr_before = cJSON_HasObjectItem(root, "arr");
    cJSON_bool has_child_before = cJSON_HasObjectItem(root, "child");
    buffer[0] = printed1 && printed1[0] ? printed1[0] : '\0';
    buffer[1] = (char)(int)(has_arr_before + has_child_before);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_AddItemToArray(arr, detached);
    cJSON *extra = cJSON_CreateNumber(13.0);
    cJSON_AddItemToArray(arr, extra);
    cJSON_bool has_child_after = cJSON_HasObjectItem(root, "child");
    buffer[2] = (char)(int)has_child_after;
    char *printed2 = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}