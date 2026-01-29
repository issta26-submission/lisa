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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"num\":10,\"text\":\"orig\"}";
    size_t json_len = sizeof("{\"num\":10,\"text\":\"orig\"}") - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double new_num = cJSON_SetNumberHelper(num_item, 42.5);
    cJSON_AddNumberToObject(child, "fromSet", new_num);

    cJSON *text_item = cJSON_GetObjectItem(root, "text");
    const char *prev_text = cJSON_GetStringValue(text_item);
    cJSON_AddStringToObject(root, "prev_text", prev_text);
    char *set_result = cJSON_SetValuestring(text_item, "modified");
    (void)set_result;

    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "childRef", child_ref);

    cJSON *raw_added = cJSON_AddRawToObject(root, "raw", "{\"rawnum\":7}");
    (void)raw_added;

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(child);

    // API sequence test completed successfully
    return 66;
}