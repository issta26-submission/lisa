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
//<ID> 553
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
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    cJSON_AddStringToObject(data, "name", "example");
    cJSON_AddBoolToObject(data, "enabled", 1);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(data, "value", num);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(data, "optional", nul);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_data, "name");
    char *name_str = (char *)cJSON_GetStringValue(parsed_name);
    cJSON *parsed_value = cJSON_GetObjectItem(parsed_data, "value");
    double parsed_val = cJSON_GetNumberValue(parsed_value);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;name=%s;val=%.2f",
            version ? version : "null",
            name_str ? name_str : "null",
            parsed_val);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}