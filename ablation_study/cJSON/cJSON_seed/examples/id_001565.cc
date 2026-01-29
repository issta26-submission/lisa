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
//<ID> 1565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *base_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "base", base_num);
    cJSON_AddNumberToObject(root, "increment", 3.14);

    // step 2: Configure
    cJSON *base_item = cJSON_GetObjectItem(root, "base");
    cJSON *inc_item = cJSON_GetObjectItem(root, "increment");
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_item);
    double base_val = cJSON_GetNumberValue(base_item);
    double inc_val = cJSON_GetNumberValue(inc_item);

    // step 3: Operate & Validate
    double total = base_val + inc_val + (double)is_enabled;
    cJSON *computed = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", computed);
    cJSON_AddNumberToObject(root, "total_plus_one", total + 1.0);

    // step 4: Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}