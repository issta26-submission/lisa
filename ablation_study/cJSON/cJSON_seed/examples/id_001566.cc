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
//<ID> 1566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "base", 2.5);
    cJSON *addend = cJSON_CreateNumber(7.25);
    cJSON_AddItemToObject(root, "addend", addend);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    char *printed = cJSON_Print(root);

    // step 3: Operate & Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(got_enabled);
    cJSON *got_base = cJSON_GetObjectItem(root, "base");
    double base_val = cJSON_GetNumberValue(got_base);
    cJSON *got_addend = cJSON_GetObjectItem(root, "addend");
    double addend_val = cJSON_GetNumberValue(got_addend);
    double total = base_val + addend_val + (double)enabled_flag;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}