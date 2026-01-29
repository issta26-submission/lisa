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
//<ID> 1213
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
    cJSON *child_num = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(child, "num", child_num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", array);

    // step 2: Configure
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddNumberToObject(item1, "value", 10.5);
    cJSON_AddItemToArray(array, item1);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(array, dup_child);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    double detached_num = cJSON_GetNumberValue(cJSON_GetObjectItem(detached, "num"));
    cJSON *derived = cJSON_CreateObject();
    cJSON_AddNumberToObject(derived, "derived", detached_num + 2.0);
    cJSON_AddItemToArray(array, derived);
    cJSON_AddItemToArray(array, detached);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *mem = cJSON_malloc(64);
    memset(mem, 0, 64);
    ((char *)mem)[0] = out[0];
    ((char *)mem)[1] = out[1];
    cJSON_free(out);
    cJSON_free(mem);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}