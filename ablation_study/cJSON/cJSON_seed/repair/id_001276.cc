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
//<ID> 1276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[true,false],\"meta\":{\"orig\":\"yes\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *first = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *first_dup = cJSON_Duplicate(first, 1);
    cJSON *new_false = cJSON_CreateFalse();
    cJSON_bool add_res = cJSON_AddItemToObject(root, "added_false", new_false);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = (add_res ? '1' : '0');

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(first_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}