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
//<ID> 1564
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *threshold_item = cJSON_AddNumberToObject(root, "threshold", 3.14);
    cJSON *enabled_item = cJSON_AddTrueToObject(root, "enabled");
    cJSON *extra_number = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "extra", extra_number);

    // step 2: Configure
    cJSON *enabled_ref = cJSON_GetObjectItem(root, "enabled");
    cJSON *threshold_ref = cJSON_GetObjectItem(root, "threshold");
    cJSON *extra_ref = cJSON_GetObjectItem(root, "extra");
    double threshold_val = cJSON_GetNumberValue(threshold_ref);
    double extra_val = cJSON_GetNumberValue(extra_ref);
    double adjusted = threshold_val + ((double)cJSON_IsTrue(enabled_ref) * extra_val);

    // step 3: Operate & Validate
    cJSON *computed = cJSON_CreateNumber(adjusted);
    cJSON_AddItemToObject(root, "computed", computed);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *computed_ref = cJSON_GetObjectItem(root, "computed");
    double computed_val = cJSON_GetNumberValue(computed_ref);
    (void)threshold_item; (void)enabled_item; (void)computed_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}