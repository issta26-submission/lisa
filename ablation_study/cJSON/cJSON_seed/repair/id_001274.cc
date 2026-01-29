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
//<ID> 1274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[true,false,true],\"meta\":{\"original\":true}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *second = cJSON_GetArrayItem(arr, 1);

    // step 2: Configure
    cJSON *forced_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "forced_false", forced_false);

    // step 3: Operate and Validate
    cJSON *dup_second = cJSON_Duplicate(second, 1);
    cJSON_AddItemToObject(root, "second_copy", dup_second);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}