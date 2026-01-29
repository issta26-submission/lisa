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
//<ID> 1568
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *base_item = cJSON_AddNumberToObject(root, "base", 3.14);
    cJSON *extra_item = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "extra", extra_item);
    cJSON *enabled_item = cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    char *printed = cJSON_Print(root);

    // step 3: Operate & Validate
    cJSON *got_base = cJSON_GetObjectItem(root, "base");
    cJSON *got_extra = cJSON_GetObjectItem(root, "extra");
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    double base_val = cJSON_GetNumberValue(got_base);
    double extra_val = cJSON_GetNumberValue(got_extra);
    cJSON_bool is_enabled = cJSON_IsTrue(got_enabled);
    cJSON *computed = cJSON_CreateNumber(base_val + extra_val + (double)is_enabled);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}