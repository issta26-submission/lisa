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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_AddObjectToObject(root, "info");
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *versionItem = cJSON_AddNumberToObject(info, "version", 1.0);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    char *name_str = cJSON_GetStringValue(name);
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)name_str[0]);
    cJSON_AddNumberToObject(root, "enabled_is_false", (double)cJSON_IsFalse(enabled));

    // step 3: Operate and Validate
    double v_version = cJSON_GetNumberValue(versionItem);
    double v_char = (double)(unsigned char)name_str[0];
    double v_false = (double)cJSON_IsFalse(enabled);
    cJSON *derived = cJSON_AddObjectToObject(root, "derived");
    cJSON_AddNumberToObject(derived, "combined", v_version + v_char + v_false);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}