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
//<ID> 567
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
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "num", num);
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greet", greet);
    cJSON_AddItemReferenceToObject(root, "childRef", child);
    cJSON *raw = cJSON_CreateRaw("{\"rawKey\":true}");
    cJSON_AddItemToObject(root, "rawData", raw);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "childRef");
    cJSON *parsed_num = cJSON_GetObjectItem(parsed_child, "num");
    double num_val = cJSON_GetNumberValue(parsed_num);
    cJSON *parsed_greet = cJSON_GetObjectItem(parsed_child, "greet");
    char *greet_str = cJSON_GetStringValue(parsed_greet);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;num=%.2f;greet=%s",
            version ? version : "null",
            num_val,
            greet_str ? greet_str : "null");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}