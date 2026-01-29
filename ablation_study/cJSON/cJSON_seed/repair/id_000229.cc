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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"pi\":3.1415,\"name\":\"circle\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    double parsed_pi = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "pi"));
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "source", "parsed");
    cJSON_AddItemToObject(root, "data", parsed);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(parsed_pi);
    cJSON_AddItemToObject(root, "pi_value", num);
    cJSON_AddStringToObject(root, "name_copy", cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "name")));

    // step 3: Operate
    cJSON *retrieved_data = cJSON_GetObjectItem(root, "data");
    cJSON *retrieved_pi = cJSON_GetObjectItem(root, "pi_value");
    double pi_from_root = cJSON_GetNumberValue(retrieved_pi);
    cJSON_AddStringToObject(root, "validation", "complete");

    // step 4: Cleanup
    (void)retrieved_data;
    (void)pi_from_root;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}