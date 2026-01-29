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
//<ID> 1512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddStringToObject(settings, "mode", "test");
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddNumberToObject(root, "version", 1.5);
    cJSON_AddItemToObject(root, "flag_null", cJSON_CreateNull());
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(85.0));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(90.5));
    cJSON_AddItemToObject(root, "scores", scores);

    // step 2: Configure
    const char json_text[] = "{\"orphan\": null, \"keep\": 123}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsed_copy = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_copy", parsed_copy);
    cJSON *null_item = cJSON_GetObjectItem(root, "flag_null");
    cJSON_bool is_null_before = cJSON_IsNull(null_item);
    cJSON_bool has_flag_before = cJSON_HasObjectItem(root, "flag_null");

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag_null");
    cJSON_bool has_flag_after = cJSON_HasObjectItem(root, "flag_null");
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool has_parsed = cJSON_HasObjectItem(root, "parsed_copy");
    (void)is_null_before;
    (void)has_flag_before;
    (void)has_flag_after;
    (void)has_parsed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}