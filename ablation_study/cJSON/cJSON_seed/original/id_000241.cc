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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *inner_str = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(child, "inner", inner_str);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *child_ptr = cJSON_GetObjectItem(root, "child");
    cJSON *dup = cJSON_Duplicate(child_ptr, 1);
    cJSON_bool same_before = cJSON_Compare(child_ptr, dup, 1);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, child_ptr);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON_bool has_detached = cJSON_HasObjectItem(root, "detached");
    cJSON_bool same_after = cJSON_Compare(detached, dup, 1);
    char *printed = cJSON_PrintUnformatted(root);
    const char *sval = cJSON_GetStringValue(cJSON_GetObjectItem(detached, "inner"));
    int summary = (int)has_child + (int)has_detached + (int)same_before + (int)same_after + (sval ? (int)sval[0] : 0);
    void *tmp = cJSON_malloc(32);
    memset(tmp, 0, 32);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}