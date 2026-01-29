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
//<ID> 1343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[3] = {10, 20, 30};
    cJSON *intarr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", intarr);
    cJSON *arr = cJSON_CreateArray();
    cJSON *orphan = cJSON_CreateStringReference("orphaned_string");
    cJSON_AddItemToArray(arr, orphan);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, flag_false);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *greeting_ref = cJSON_CreateStringReference("hello_world");
    cJSON_AddItemToObject(root, "greeting", greeting_ref);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(arr, orphan);
    cJSON_AddItemToObject(root, "moved", detached);
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", metadata);
    cJSON *meta_note = cJSON_CreateStringReference("meta_info");
    cJSON_AddItemToObject(metadata, "note", meta_note);

    // step 3: Operate and Validate
    int size_ints = cJSON_GetArraySize(intarr);
    cJSON *first_int = cJSON_GetArrayItem(intarr, 0);
    double first_val = cJSON_GetNumberValue(first_int);
    cJSON *greet = cJSON_GetObjectItem(root, "greeting");
    const char *greet_str = cJSON_GetStringValue(greet);
    cJSON_bool ints_are_array = cJSON_IsArray(intarr);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(80);
    memset(scratch, 0, 80);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = greet_str ? greet_str[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (size_ints % 10));
    ((char *)scratch)[3] = (char)('0' + ((int)first_val % 10));
    ((char *)scratch)[4] = (char)('0' + (ints_are_array ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}