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
//<ID> 559
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "answer", num);
    cJSON_AddBoolToObject(meta, "valid", 1);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "maybe", maybe);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_answer = cJSON_GetObjectItem(parsed_meta, "answer");
    double answer_val = cJSON_GetNumberValue(parsed_answer);
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed, "name");
    char *name_str = cJSON_GetStringValue(parsed_name_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;ans=%.2f;name=%s",
            version ? version : "null",
            answer_val,
            name_str ? name_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}