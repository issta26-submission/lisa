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
//<ID> 248
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
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddItemToObject(child, "num", num);
    cJSON_AddItemToObject(child, "text", str);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON_bool has_before = cJSON_HasObjectItem(root, "child");
    cJSON_AddNumberToObject(root, "has_child_before", (double)has_before);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *detached = cJSON_DetachItemViaPointer(root, got_child);
    cJSON_AddItemToObject(root, "detached_node", detached);

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(detached, 1);
    cJSON_bool same = cJSON_Compare(detached, dup, 1);
    cJSON_AddBoolToObject(root, "detached_equals_duplicate", same);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    double numval = cJSON_GetNumberValue(cJSON_GetObjectItem(detached, "num"));
    int summary = (int)numval + (int)same + (int)has_before;
    cJSON_AddNumberToObject(root, "checksum", (double)summary);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}