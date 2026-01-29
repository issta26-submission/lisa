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
//<ID> 618
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON_AddNullToObject(root, "missing");
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", flag_false);

    // step 2: Configure
    double pi_val = cJSON_GetNumberValue(pi);
    cJSON *double_pi = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "double_pi", double_pi);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double parsed_pi_val = cJSON_GetNumberValue(parsed_pi);
    cJSON_bool disabled_check = cJSON_IsFalse(cJSON_GetObjectItem(parsed, "disabled"));
    cJSON *parsed_missing = cJSON_GetObjectItem(parsed, "missing");
    cJSON_bool missing_is_null = cJSON_IsNull(parsed_missing);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;pi=%.15g;double_pi=%.15g;disabled=%d;missing_null=%d",
            version ? version : "null",
            parsed_pi_val,
            cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "double_pi")),
            (int)disabled_check,
            (int)missing_is_null);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}