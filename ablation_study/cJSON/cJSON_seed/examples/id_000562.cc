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
//<ID> 562
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
    cJSON *raw_orig = cJSON_CreateRaw("{\"note\":\"hello\",\"n\":123}");
    cJSON *raw_dup = cJSON_Duplicate(raw_orig, 1);
    cJSON_AddItemReferenceToObject(meta, "payload", raw_dup);
    cJSON_Delete(raw_orig);
    cJSON_AddNumberToObject(meta, "id", 7.0);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate & Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_payload = cJSON_GetObjectItem(got_meta, "payload");
    char *payload_str = cJSON_GetStringValue(got_payload);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_payload = cJSON_GetObjectItem(parsed_meta, "payload");
    char *parsed_payload_str = cJSON_GetStringValue(parsed_payload);
    cJSON_free(snapshot);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;payload=%s;parsed=%s;name=%s",
            version ? version : "null",
            payload_str ? payload_str : "null",
            parsed_payload_str ? parsed_payload_str : "null",
            name_str ? name_str : "null");
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}