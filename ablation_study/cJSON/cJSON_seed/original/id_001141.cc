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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = {"alice", "bob", "carol"};
    cJSON *name_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", name_array);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(child_obj, "member", cJSON_CreateString("inner_value"));
    cJSON_AddItemToObject(root, "child", child_obj);
    cJSON *child_ref = cJSON_CreateObjectReference(child_obj);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 2: Configure
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "count_times_two", count_val * 2.0);
    cJSON *retrieved_ref = cJSON_GetObjectItem(root, "child_ref");
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddStringToObject(replacement, "replaced_member", "new_inner");

    // step 3: Operate
    cJSON_bool did_replace = cJSON_ReplaceItemViaPointer(root, retrieved_ref, replacement);
    cJSON_DeleteItemFromObject(root, "count");
    cJSON *first = cJSON_GetArrayItem(name_array, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(root, "first_name", first_val);
    (void)did_replace;

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}