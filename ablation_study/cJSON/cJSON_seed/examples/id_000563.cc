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
//<ID> 563
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
    const char *raw_json = "{\"note\":\"raw content\",\"num\":7}";
    cJSON *raw_item = cJSON_CreateRaw(raw_json);
    cJSON_AddItemReferenceToObject(root, "rawdata", raw_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "inner", inner);
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(inner, "value", num_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_raw = cJSON_GetObjectItem(parsed, "rawdata");
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_inner = cJSON_GetObjectItem(parsed_meta, "inner");
    cJSON *parsed_value = cJSON_GetObjectItem(parsed_inner, "value");
    double parsed_val_num = cJSON_GetNumberValue(parsed_value);
    const char *parsed_raw_str = cJSON_GetStringValue(parsed_raw);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;val=%.2f;raw=%s",
            version ? version : "null",
            parsed_val_num,
            parsed_raw_str ? parsed_raw_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}