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
//<ID> 617
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
    cJSON *num_pi = cJSON_CreateNumber(3.14159);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON_AddNullToObject(root, "empty");

    // step 2: Configure
    double pi_value_local = cJSON_GetNumberValue(num_pi);
    cJSON_AddItemToObject(root, "pi_copy", cJSON_CreateNumber(pi_value_local));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double parsed_pi_value = cJSON_GetNumberValue(parsed_pi);
    cJSON_bool parsed_flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(parsed, "flag"));
    cJSON *parsed_empty = cJSON_GetObjectItem(parsed, "empty");
    const char *empty_str = cJSON_GetStringValue(parsed_empty);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;pi_local=%.5f;pi_parsed=%.5f;flag_false=%d;empty=%s",
            version ? version : "null",
            pi_value_local,
            parsed_pi_value,
            (int)parsed_flag_is_false,
            empty_str ? empty_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}