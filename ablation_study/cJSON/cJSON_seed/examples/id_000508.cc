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
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *app_name = "cjson_fuzz_demo";
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "name", app_name);
    cJSON_AddNumberToObject(root, "version", 1.23);

    // step 2: Configure
    cJSON *entry0 = cJSON_CreateObject();
    cJSON_AddStringToObject(entry0, "id", "item0");
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(entry0, "value", num0);
    cJSON_AddItemToArray(items, entry0);
    cJSON *entry1 = cJSON_CreateString("second_item");
    cJSON_AddItemToArray(items, entry1);

    // step 3: Operate & Validate
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed, "name");
    const char *parsed_name = cJSON_GetStringValue(parsed_name_item);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    sprintf(status_buf, "name=%s;items_present=%p;equal=%d", parsed_name ? parsed_name : "null", (void*)parsed_items, (int)equal);
    (void)status_buf;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}