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
//<ID> 557
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
    cJSON *base_num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(settings, "base", base_num);
    cJSON *threshold = cJSON_AddNumberToObject(settings, "threshold", 3.5);

    // step 2: Configure
    double new_base = cJSON_SetNumberHelper(base_num, 20.0);
    cJSON *adjusted = cJSON_AddNumberToObject(settings, "adjusted", new_base + 2.0);
    cJSON *extra = cJSON_CreateNumber(new_base * 1.5);
    cJSON_AddItemToObject(settings, "extra", extra);

    // step 3: Operate and Validate
    cJSON *settings_dup = cJSON_Duplicate(settings, 1);
    cJSON *dup_base = cJSON_GetObjectItem(settings_dup, "base");
    double dup_base_val = cJSON_GetNumberValue(dup_base);
    cJSON *computed = cJSON_CreateNumber(dup_base_val + 7.0);
    cJSON_AddItemToObject(settings_dup, "computed", computed);
    char *out = cJSON_PrintUnformatted(settings_dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(settings_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}