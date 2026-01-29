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
//<ID> 614
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
    cJSON *num_pi = cJSON_CreateNumber(3.141592653589793);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "scale", 100.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double pi_value = cJSON_GetNumberValue(parsed_pi);
    cJSON_bool parsed_false = cJSON_IsFalse(cJSON_GetObjectItem(parsed, "flag"));
    cJSON *parsed_missing = cJSON_GetObjectItem(parsed, "missing");
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;pi=%.12f;flag_false=%d;missing=%p",
            version ? version : "null",
            pi_value,
            (int)parsed_false,
            (void*)parsed_missing);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}