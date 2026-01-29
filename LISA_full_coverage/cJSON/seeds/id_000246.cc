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
//<ID> 246
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "number", num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *ref = cJSON_CreateObject();
    cJSON_AddStringToObject(ref, "str", "hello");
    cJSON_AddItemToObject(root, "ref", ref);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "ref");
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);

    // step 3: Operate and Validate
    cJSON_bool comp_same = cJSON_Compare(detached, child, 1);
    cJSON *dup_ref = cJSON_Duplicate(ref, 1);
    cJSON_bool comp_ref = cJSON_Compare(ref, dup_ref, 1);
    char *printed = cJSON_PrintUnformatted(root);
    char *newval = cJSON_SetValuestring(ref, "world");
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = (int)has_child + (int)has_ref + (int)comp_same + (int)comp_ref + (printed ? (int)printed[0] : 0) + (newval ? (int)newval[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    cJSON_Delete(dup_ref);

    // API sequence test completed successfully
    return 66;
}