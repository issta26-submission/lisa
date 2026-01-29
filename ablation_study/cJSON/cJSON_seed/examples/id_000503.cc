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
//<ID> 503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *app_name = "cjson_test_app";
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "app", cJSON_CreateString(app_name));
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON *meta = cJSON_CreateObject();
    char summary_buf[128];
    memset(summary_buf, 0, sizeof(summary_buf));
    char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    sprintf(summary_buf, "app=%s;first=%s", app_name ? app_name : "null", first_val ? first_val : "null");
    cJSON_AddStringToObject(meta, "summary", summary_buf);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    char *parsed_summary = cJSON_GetStringValue(cJSON_GetObjectItem(parsed_meta, "summary"));
    (void)parsed_summary;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}