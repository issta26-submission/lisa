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
//<ID> 1205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "value", 42.5);

    // step 2: Configure
    char *out_before = cJSON_Print(root);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "value");
    cJSON_bool invalid_flag = cJSON_IsInvalid(detached);
    cJSON *report = cJSON_CreateObject();
    cJSON_AddNumberToObject(report, "detached_is_invalid", (double)invalid_flag);
    cJSON_AddNumberToObject(report, "detached_value", detached->valuedouble);
    char *out_report = cJSON_Print(report);

    // step 4: Cleanup
    cJSON_free(out_before);
    cJSON_free(out_report);
    cJSON_Delete(report);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}