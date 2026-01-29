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
//<ID> 1646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *payload = cJSON_AddObjectToObject(root, "payload");
    cJSON *embedded = cJSON_CreateString("embedded_value");
    cJSON_AddItemToObject(payload, "embedded_item", embedded);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddStringToObject(payload, "type", "example");

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(payload, "embedded_item");
    const char *retrieved_str = cJSON_GetStringValue(retrieved);
    cJSON *copied_str = cJSON_CreateString(retrieved_str);
    cJSON_AddItemToObject(meta, "copied_embedded", copied_str);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(compact);
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}