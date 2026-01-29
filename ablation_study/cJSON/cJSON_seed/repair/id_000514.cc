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
//<ID> 514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"user\":{\"id\":123},\"active\":false,\"name\":\"tester\"}";
    const char *parse_end = NULL;
    size_t buf_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, buf_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_bool added_meta = cJSON_AddItemToObject(root, "meta", meta);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta, "note", note);

    // step 3: Operate
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(meta, "copied_name", name_str);
    cJSON_AddBoolToObject(meta, "has_name", has_name);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *user = cJSON_GetObjectItem(root, "user");
    cJSON *id = cJSON_GetObjectItem(user, "id");
    double id_value = cJSON_GetNumberValue(id);
    (void)id_value;
    (void)added_meta;
    (void)enabled_item;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}