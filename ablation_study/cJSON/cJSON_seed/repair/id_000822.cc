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
//<ID> 822
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
    cJSON *num_created = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "answer", num_created);
    cJSON_AddNumberToObject(meta, "version", 1.23);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddNumberToObject(settings, "threshold", 2.718);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    const char *json_text = "{\"keep\":\"hello\",\"remove\":\"bye\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *remove_ptr = cJSON_GetObjectItem(parsed, "remove");
    cJSON *detached_remove = cJSON_DetachItemViaPointer(parsed, remove_ptr);

    const char *json_text2 = "{\"a\":123,\"b\":\"text\"}";
    cJSON *parsed2 = cJSON_Parse(json_text2);
    cJSON *detached_a = cJSON_DetachItemFromObject(parsed2, "a");
    cJSON_AddItemToObject(root, "detached_remove", detached_remove);
    cJSON_AddItemToObject(root, "detached_a", detached_a);

    // step 3: Operate and Validate
    cJSON *check_remove = cJSON_GetObjectItem(root, "detached_remove");
    cJSON *check_b = cJSON_GetObjectItem(parsed2, "b");
    cJSON_bool is_remove_string = cJSON_IsString(check_remove);
    cJSON_bool is_b_string = cJSON_IsString(check_b);
    double version_value = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "version"));
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);
    char *snapshot_parsed2 = cJSON_PrintUnformatted(parsed2);
    (void)is_remove_string;
    (void)is_b_string;
    (void)version_value;

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_free(snapshot_parsed2);
    cJSON_Delete(parsed);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}