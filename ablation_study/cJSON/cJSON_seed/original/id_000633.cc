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
//<ID> 633
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *raw_item = cJSON_AddRawToObject(config, "raw", "{\"enabled\":true,\"threshold\":3}");
    cJSON *inner = cJSON_AddObjectToObject(config, "inner");
    cJSON_AddNumberToObject(inner, "value", 42.0);

    // step 2: Configure
    cJSON *expected = cJSON_CreateObject();
    cJSON *expected_config = cJSON_CreateObject();
    cJSON_AddRawToObject(expected_config, "raw", "{\"enabled\":true,\"threshold\":3}");
    cJSON *expected_inner = cJSON_AddObjectToObject(expected_config, "inner");
    cJSON_AddNumberToObject(expected_inner, "value", 42.0);
    cJSON_AddItemToObject(expected, "config", expected_config);

    // step 3: Operate
    cJSON_bool is_obj = cJSON_IsObject(config);
    cJSON_bool equal = cJSON_Compare(root, expected, 1);
    cJSON *match_item = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "match", match_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    int validation_score = (int)is_obj + (int)equal;

    // step 4: Validate & Cleanup
    (void)validation_score;
    cJSON_Delete(expected);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}