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
//<ID> 93
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "numbers", numbers);
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(30.0));

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "updated");
    cJSON *backup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "backup", backup);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_numbers = cJSON_GetObjectItem(got_config, "numbers");
    int count = cJSON_GetArraySize(got_numbers);
    cJSON_AddNumberToObject(root, "numbers_count", (double)count);
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)set_ret;
    (void)name_str;
    return 66;
    // API sequence test completed successfully
}