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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "metadata");
    cJSON *ver = cJSON_AddNumberToObject(meta, "version", 1.2);
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "example");
    cJSON *enabled_item = cJSON_AddFalseToObject(root, "enabled");

    // step 2: Operate
    const cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    const cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_false = cJSON_IsFalse(got_enabled);
    double enabled_flag = 1.0 - (double)is_false;
    cJSON *flag_item = cJSON_AddNumberToObject(root, "enabled_flag", enabled_flag);
    const cJSON *got_version = cJSON_GetObjectItem(meta, "version");
    double version_val = cJSON_GetNumberValue(got_version);
    double derived = version_val * enabled_flag;
    cJSON_AddNumberToObject(root, "derived", derived);

    // step 3: Validate & Serialize
    char *out = cJSON_PrintUnformatted(root);
    unsigned char first_char = (unsigned char)name_str[0];
    cJSON_AddNumberToObject(root, "first_char_code", (double)first_char);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}