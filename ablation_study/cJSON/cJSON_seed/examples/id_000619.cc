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
//<ID> 619
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
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *null_item = NULL;

    // step 2: Configure
    cJSON_AddItemToObject(root, "pi", num_item);
    null_item = cJSON_AddNullToObject(root, "missing");
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double pi_value = cJSON_GetNumberValue(parsed_pi);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;pi=%.4f;false=%d",
            version ? version : "null",
            pi_value,
            (int)flag_is_false);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}