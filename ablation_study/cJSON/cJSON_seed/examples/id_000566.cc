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
//<ID> 566
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
    cJSON_AddNumberToObject(child, "value", 123.456);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawkey\":true}");
    cJSON_AddItemToObject(child, "raw", raw_item);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);
    cJSON_AddStringToObject(root, "note", "created_for_test");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "child_ref");
    cJSON *parsed_value = cJSON_GetObjectItem(parsed_child, "value");
    double value_val = cJSON_GetNumberValue(parsed_value);
    cJSON *parsed_raw = cJSON_GetObjectItem(parsed_child, "raw");
    cJSON *parsed_rawkey = cJSON_GetObjectItem(parsed_raw, "rawkey");
    cJSON_bool rawkey_true = cJSON_IsTrue(parsed_rawkey);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;val=%.3f;rawkey=%d",
            version ? version : "null",
            value_val,
            rawkey_true ? 1 : 0);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}