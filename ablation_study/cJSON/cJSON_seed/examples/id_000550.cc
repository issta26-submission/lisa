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
//<ID> 550
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

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON *greet_item = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greet_item);

    // step 3: Operate & Validate
    char *orig_greet = (char *)cJSON_GetStringValue(greet_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_greet_item = cJSON_GetObjectItem(parsed, "greeting");
    char *parsed_greet = (char *)cJSON_GetStringValue(parsed_greet_item);
    cJSON *parsed_num_item = cJSON_GetObjectItem(parsed, "num");
    double parsed_num = cJSON_GetNumberValue(parsed_num_item);
    cJSON *parsed_bool_item = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool parsed_bool_true = cJSON_IsTrue(parsed_bool_item);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;orig=%s;parsed=%s;num=%.3f;enabled=%d",
            version ? version : "null",
            orig_greet ? orig_greet : "null",
            parsed_greet ? parsed_greet : "null",
            parsed_num,
            (int)parsed_bool_true);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}