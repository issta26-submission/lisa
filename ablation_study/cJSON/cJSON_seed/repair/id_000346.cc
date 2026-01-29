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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *base = cJSON_CreateNumber(7.0);
    cJSON *name = cJSON_CreateString("example");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "base", base);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    cJSON *got_base = cJSON_GetObjectItem(root, "base");
    double base_val = cJSON_GetNumberValue(got_base);
    cJSON *triple = cJSON_CreateNumber(base_val * 3.0);
    cJSON_AddItemToObject(root, "triple", triple);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    double first_char_code = (double)(name_str[0]);
    cJSON *first_code = cJSON_CreateNumber(first_char_code);
    cJSON_AddItemToObject(root, "first_char_code", first_code);

    // step 3: Operate and Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_bool = cJSON_IsTrue(got_enabled);
    double enabled_num_val = (double)enabled_bool;
    cJSON *enabled_num = cJSON_CreateNumber(enabled_num_val);
    cJSON_AddItemToObject(root, "enabled_numeric", enabled_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}