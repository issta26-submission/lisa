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
//<ID> 573
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(info, "name", name_item);
    cJSON *value_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(info, "value", value_item);
    cJSON *flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(info, "flag", flag_item);
    cJSON *summary_item = cJSON_CreateString("placeholder");
    cJSON_AddItemToObject(root, "summary", summary_item);

    // step 2: Configure
    char *ret1 = cJSON_SetValuestring(name_item, "updated");
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 3: Operate and Validate
    char *name_val = cJSON_GetStringValue(name_item);
    double num_val = cJSON_GetNumberValue(value_item);
    cJSON_bool was_false = cJSON_IsFalse(flag_item);
    double computed = num_val + (double)was_false;
    sprintf(buffer, "%s:%.3f", name_val, computed);
    char *ret2 = cJSON_SetValuestring(summary_item, buffer);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}