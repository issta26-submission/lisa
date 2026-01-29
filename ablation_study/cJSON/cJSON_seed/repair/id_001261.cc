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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[\"alpha\",\"beta\",\"gamma\"],\"name\":\"orig\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    int arr_size = cJSON_GetArraySize(arr);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *tag = cJSON_CreateString("generated-tag");
    cJSON_AddItemToObject(meta, "tag", tag);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *mirror = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "mirror", mirror);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}