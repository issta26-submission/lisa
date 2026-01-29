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
//<ID> 1314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *manual_null = cJSON_CreateNull();
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(settings, "manual_null", manual_null);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNullToObject(root, "auto_null");

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(settings, manual_null);
    cJSON_AddItemToObject(root, "detached_null", detached);

    // step 3: Operate and Validate
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(retrieved_flag);
    cJSON_bool flag_is_false = cJSON_IsFalse(retrieved_flag);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char first_char = out ? out[0] : 0;
    ((char *)scratch)[0] = first_char;
    ((char *)scratch)[1] = (char)('0' + (flag_is_bool ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (flag_is_false ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}