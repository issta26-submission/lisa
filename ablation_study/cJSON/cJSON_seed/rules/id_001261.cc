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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"inner\":42,\"name\":\"raw\"}";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = NULL;
    cJSON *raw_item = NULL;
    cJSON_bool add_result = 0;
    cJSON *status_item = NULL;
    cJSON *got_raw = NULL;
    char *raw_value = NULL;
    cJSON *copied_string = NULL;
    char *printed = NULL;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    meta = cJSON_AddObjectToObject(root, "meta");
    raw_item = cJSON_CreateRaw(raw_json);
    add_result = cJSON_AddItemToObjectCS(meta, "raw_blob", raw_item);
    status_item = cJSON_AddTrueToObject(root, "status");

    // step 3: Operate / Validate
    got_raw = cJSON_GetObjectItem(meta, "raw_blob");
    raw_value = cJSON_GetStringValue(got_raw);
    copied_string = cJSON_CreateString(raw_value);
    cJSON_AddItemToObject(root, "copied_raw", copied_string);
    is_true = cJSON_IsTrue(status_item);
    printed = cJSON_PrintUnformatted(root);
    (void)add_result;
    (void)is_true;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}