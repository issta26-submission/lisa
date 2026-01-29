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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *pi_item = cJSON_GetObjectItemCaseSensitive(root, "pi");
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    double pi_value = cJSON_GetNumberValue(pi_item);
    cJSON_bool enabled_bool = cJSON_IsTrue(enabled_item);

    // step 3: Operate & Validate
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "pi_read", pi_value);
    cJSON_AddNumberToObject(summary, "enabled_read", (double)enabled_bool);
    cJSON_AddItemToObject(root, "summary", summary);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}