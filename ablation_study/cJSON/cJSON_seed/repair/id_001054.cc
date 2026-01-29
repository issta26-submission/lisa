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
//<ID> 1054
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
    cJSON_AddStringToObject(child, "name", "fuzz_target");
    cJSON_AddStringToObject(child, "category", "example");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(arr, dup_child);
    cJSON_AddStringToObject(root, "state", "configured");

    // step 3: Operate and Validate
    char *out = cJSON_Print(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}