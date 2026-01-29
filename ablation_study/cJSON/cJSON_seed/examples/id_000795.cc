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
//<ID> 795
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label_item = cJSON_CreateString("sensor");
    cJSON *greeting_obj = cJSON_CreateObject();
    cJSON *greet_text = cJSON_CreateString("hello");
    cJSON_AddItemToObject(greeting_obj, "text", greet_text);
    cJSON_AddItemToObject(root, "greeting", greeting_obj);
    cJSON_AddItemToObject(root, "label", label_item);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_label_cs = cJSON_GetObjectItemCaseSensitive(parsed, "label");
    const char *label_val = cJSON_GetStringValue(parsed_label_cs);
    cJSON *parsed_greeting = cJSON_GetObjectItem(parsed, "greeting");
    cJSON *parsed_greet_text = cJSON_GetObjectItem(parsed_greeting, "text");
    const char *greeting_val = cJSON_GetStringValue(parsed_greet_text);
    size_t l1 = strlen(label_val);
    size_t l2 = strlen(greeting_val);
    size_t total = l1 + 1 + l2 + 1;
    char *combined = (char *)cJSON_malloc(total);
    memset(combined, 0, total);
    memcpy(combined, label_val, l1);
    combined[l1] = ':';
    memcpy(combined + l1 + 1, greeting_val, l2);
    combined[total - 1] = '\0';
    cJSON *combined_item = cJSON_CreateString(combined);
    cJSON_AddItemToObject(parsed, "combined", combined_item);
    const char *verified = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "combined"));
    (void)verified;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_free(combined);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}