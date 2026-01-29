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
//<ID> 1267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"embedded\":true,\"n\":123}";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = NULL;
    cJSON *raw = NULL;
    cJSON *got_raw = NULL;
    char *printed = NULL;
    const char *raw_val = NULL;
    cJSON_bool has_meta = 0;
    cJSON_bool is_status_true = 0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "status");
    raw = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObjectCS(root, "rawdata", raw);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    got_raw = cJSON_GetObjectItem(root, "rawdata");
    raw_val = cJSON_GetStringValue(got_raw);
    has_meta = cJSON_HasObjectItem(root, "meta");
    is_status_true = cJSON_IsTrue(cJSON_GetObjectItem(meta, "status"));
    (void)raw_val;
    (void)has_meta;
    (void)is_status_true;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}