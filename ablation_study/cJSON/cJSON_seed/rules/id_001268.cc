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
//<ID> 1268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"embedded\":true}";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = NULL;
    cJSON *settings = NULL;
    cJSON *raw_item = NULL;
    cJSON *active_item = NULL;
    cJSON *got_meta = NULL;
    cJSON *got_settings = NULL;
    cJSON *got_active = NULL;
    char *printed = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool active_true = 0;

    // step 2: Setup / Configure
    raw_item = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObjectCS(root, "raw_meta", raw_item);
    meta = cJSON_AddObjectToObject(root, "meta");
    settings = cJSON_AddObjectToObject(meta, "settings");
    active_item = cJSON_AddTrueToObject(settings, "active");

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(raw_item);
    got_meta = cJSON_GetObjectItem(root, "meta");
    got_settings = cJSON_GetObjectItem(got_meta, "settings");
    got_active = cJSON_GetObjectItem(got_settings, "active");
    active_true = cJSON_IsTrue(got_active);
    printed = cJSON_PrintUnformatted(root);
    (void)is_raw;
    (void)active_true;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}