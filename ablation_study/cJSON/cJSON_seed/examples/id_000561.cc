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
//<ID> 561
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
    cJSON *orig = cJSON_CreateObject();
    cJSON_AddNumberToObject(orig, "value", 123.456);
    cJSON_AddItemReferenceToObject(root, "ref", orig);
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":true}");
    cJSON_AddItemToObject(data, "raw", raw);
    char *raw_text = cJSON_GetStringValue(raw);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_ref = cJSON_GetObjectItem(parsed, "ref");
    cJSON *parsed_ref_value = cJSON_GetObjectItem(parsed_ref, "value");
    double parsed_value = cJSON_GetNumberValue(parsed_ref_value);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    cJSON *parsed_raw = cJSON_GetObjectItem(parsed_data, "raw");

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;val=%.3f;raw=%s",
            version ? version : "null",
            parsed_value,
            raw_text ? raw_text : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}