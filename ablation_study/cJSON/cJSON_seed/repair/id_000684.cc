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
//<ID> 684
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "A", objA);
    cJSON *objB = cJSON_AddObjectToObject(root, "B");
    cJSON_AddNumberToObject(objA, "num", 10.0);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(objA, "nil_marker", null_item);

    // step 2: Configure and Operate
    cJSON *num_item = cJSON_GetObjectItem(objA, "num");
    double prev_val = cJSON_GetNumberValue(num_item);
    cJSON *detached_num = cJSON_DetachItemViaPointer(objA, num_item);
    double new_val = cJSON_SetNumberHelper(detached_num, 123.456);
    cJSON_AddItemToObject(objB, "moved_num", detached_num);
    cJSON *direct_added = cJSON_AddNumberToObject(objB, "direct_added", 99.5);

    // step 3: Validate
    double sum_val = prev_val + new_val + cJSON_GetNumberValue(direct_added);
    cJSON_AddNumberToObject(root, "sum", sum_val);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddStringToObject(root, "snapshot_raw", buffer);
    cJSON_free(buffer);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}