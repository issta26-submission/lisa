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
//<ID> 1309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "TestUser");
    cJSON *active_flag = cJSON_CreateTrue();
    cJSON *disabled_flag = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "is_active", active_flag);
    cJSON_AddItemToObject(root, "is_disabled", disabled_flag);

    // step 2: Configure
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON_AddStringToObject(root_copy, "copy_note", "duplicated");

    // step 3: Operate and Validate
    cJSON_bool active_is_bool = cJSON_IsBool(active_flag);
    cJSON_bool active_is_true = cJSON_IsTrue(active_flag);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (active_is_bool ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (active_is_true ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}