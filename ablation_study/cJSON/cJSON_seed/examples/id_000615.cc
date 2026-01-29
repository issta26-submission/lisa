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
//<ID> 615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "measurement", num_item);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);

    // step 3: Validate
    cJSON *parsed_num = cJSON_GetObjectItem(parsed, "measurement");
    double measured_value = cJSON_GetNumberValue(parsed_num);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    cJSON *parsed_optional = cJSON_GetObjectItem(parsed, "optional");
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;value=%.3f;is_false=%d;optional_ptr=%p",
            version ? version : "null",
            measured_value,
            (int)flag_is_false,
            (void *)parsed_optional);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}