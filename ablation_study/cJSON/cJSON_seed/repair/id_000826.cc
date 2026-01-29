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
//<ID> 826
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *pi_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_num);
    cJSON *ver_added = cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    const char *json_text = "{\"info\":\"sample\",\"remove_me\":123}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *detached_from_parsed = cJSON_DetachItemFromObject(parsed, "remove_me");

    // step 3: Operate and Validate
    cJSON *detached_by_ptr = cJSON_DetachItemViaPointer(items, n1);
    cJSON *info_item = cJSON_GetObjectItemCaseSensitive(parsed, "info");
    cJSON_bool info_is_string = cJSON_IsString(info_item);
    cJSON *ver_item = cJSON_GetObjectItemCaseSensitive(meta, "version");
    double ver_value = cJSON_GetNumberValue(ver_item);
    int items_count = cJSON_GetArraySize(items);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);
    (void)info_is_string;
    (void)ver_value;
    (void)items_count;

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(detached_by_ptr);
    cJSON_Delete(detached_from_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}