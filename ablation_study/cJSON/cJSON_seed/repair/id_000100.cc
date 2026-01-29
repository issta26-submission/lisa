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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");

    // step 2: Configure
    cJSON_AddNumberToObject(settings, "version", 1.234);
    cJSON *name = cJSON_AddStringToObject(root, "name", "example");
    cJSON *enabled = cJSON_AddFalseToObject(settings, "enabled");

    // step 3: Operate and Validate
    char *name_val = cJSON_GetStringValue(name);
    double enabled_flag = (double)cJSON_IsFalse(enabled);
    cJSON_AddNumberToObject(root, "enabled_flag", enabled_flag);
    cJSON_AddNumberToObject(root, "name_first_char", (double)(unsigned char)name_val[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}