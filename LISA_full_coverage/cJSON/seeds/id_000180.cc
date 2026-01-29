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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    char *json_unformatted = cJSON_PrintUnformatted(root);
    size_t json_len = strlen(json_unformatted);
    char *working_buf = (char *)cJSON_malloc(json_len + 1);
    memcpy(working_buf, json_unformatted, json_len + 1);
    cJSON_Minify(working_buf);

    // step 3: Operate and Validate
    cJSON *found = cJSON_GetObjectItem(root, "data");
    int size = cJSON_GetArraySize(found);
    cJSON *first = cJSON_GetArrayItem(found, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    (void)size;
    (void)first_is_null;
    (void)working_buf;

    // step 4: Cleanup
    cJSON_free(json_unformatted);
    cJSON_free(working_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}