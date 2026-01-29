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
//<ID> 244
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
    cJSON_AddNumberToObject(child, "n", 42.0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON *dup = cJSON_Duplicate(detached, 1);
    cJSON_AddItemToObject(root, "detached_copy", dup);

    // step 3: Operate and Validate
    cJSON_bool compare_equal = cJSON_Compare(detached, dup, 1);
    cJSON *detached_copy_node = cJSON_GetObjectItem(root, "detached_copy");
    cJSON *nnode = cJSON_GetObjectItem(detached_copy_node, "n");
    double nval = cJSON_GetNumberValue(nnode);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = (int)has_child + (int)compare_equal + (int)nval + cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "summary", (double)summary);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}