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
//<ID> 1562
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
    cJSON *created_num = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "created", created_num);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(got_enabled);
    cJSON *got_base = cJSON_GetObjectItem(root, "base");
    cJSON *got_created = cJSON_GetObjectItem(root, "created");

    // step 3: Operate & Validate
    double base_val = cJSON_GetNumberValue(got_base);
    double created_val = cJSON_GetNumberValue(got_created);
    double sum = base_val + created_val + (double)is_enabled;
    cJSON *computed = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "computed_sum", computed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}