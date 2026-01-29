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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *name_item = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON_AddItemToObject(config, "flag", true_item);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *first_char_num = cJSON_CreateNumber((double)name_str[0]);
    cJSON_AddItemToObject(root, "first_char_code", first_char_num);

    // step 3: Operate and Validate
    cJSON_bool flag_true = cJSON_IsTrue(true_item);
    cJSON_AddBoolToObject(root, "validated_true", flag_true);
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    const char *retrieved = cJSON_GetStringValue(got_name);
    cJSON *retrieved_code = cJSON_CreateNumber((double)retrieved[0]);
    cJSON_AddItemToObject(root, "retrieved_code", retrieved_code);

    // step 4: Cleanup
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}