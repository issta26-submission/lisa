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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char json[] = "{\"value\": 3.14, \"name\": \"pi\"}";
    size_t len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, len);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 3: Operate and Validate
    cJSON *parsed_obj = cJSON_GetObjectItem(root, "parsed");
    cJSON *value_item = cJSON_GetObjectItem(parsed_obj, "value");
    double val = cJSON_GetNumberValue(value_item);
    cJSON *doubled = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, "double_value", doubled);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}