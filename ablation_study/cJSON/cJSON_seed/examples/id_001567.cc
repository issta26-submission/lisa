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
//<ID> 1567
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "base", 10.0);
    cJSON_AddNumberToObject(root, "increment", 2.5);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "extra", extra_num);

    // step 3: Operate & Validate
    cJSON *base_item = cJSON_GetObjectItem(root, "base");
    cJSON *inc_item = cJSON_GetObjectItem(root, "increment");
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    double base_value = cJSON_GetNumberValue(base_item);
    double inc_value = cJSON_GetNumberValue(inc_item);
    double extra_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "extra"));
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);
    double computed = base_value + inc_value + extra_value + (double)enabled_flag;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}