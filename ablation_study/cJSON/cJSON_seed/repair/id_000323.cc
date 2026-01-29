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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.141592653589793);

    // step 2: Configure
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    cJSON *double_pi = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "double_pi", double_pi);
    cJSON *got_double = cJSON_GetObjectItem(root, "double_pi");
    double double_val = cJSON_GetNumberValue(got_double);
    (void)enabled_item;
    (void)pi_val;
    (void)double_val;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}