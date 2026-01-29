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
//<ID> 1263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"embedded\":42}";
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *raw_item = NULL;
    cJSON *parsed = NULL;
    cJSON *payload = NULL;
    char *printed = NULL;
    const char *extracted_raw = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool compare_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw_item = cJSON_CreateRaw(raw_json);
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON_AddItemToObjectCS(root, "payload", raw_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    payload = cJSON_GetObjectItemCaseSensitive(parsed, "payload");
    is_raw = cJSON_IsRaw(payload);
    extracted_raw = cJSON_GetStringValue(payload);
    compare_equal = cJSON_Compare(root, parsed, 1);
    (void)extracted_raw;
    (void)is_raw;
    (void)compare_equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}