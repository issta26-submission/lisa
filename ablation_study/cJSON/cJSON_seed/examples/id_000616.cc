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
//<ID> 616
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
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNullToObject(root, "empty");

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(snapshot, &parse_end, 1);

    // step 3: Operate & Validate
    cJSON *parsed_num = cJSON_GetObjectItem(parsed, "pi");
    double pi_value = cJSON_GetNumberValue(parsed_num);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;pi=%.5f;flag_false=%d",
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