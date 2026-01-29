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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    const char json_text[] = "{\"user\":{\"name\":\"alice\",\"age\":30},\"status\":\"active\"}";
    size_t json_len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    void * temp_buffer = cJSON_malloc(32);
    memset(temp_buffer, 0, 32);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "data", parsed);
    cJSON_AddStringToObject(root, "note", "created by test_cJSON_api_sequence");
    cJSON *label = cJSON_CreateString("unit-test");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *user = cJSON_GetObjectItemCaseSensitive(data, "user");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(user, "name");
    const char *name_value = cJSON_GetStringValue(name);
    cJSON *status = cJSON_GetObjectItemCaseSensitive(data, "status");
    const char *status_value = cJSON_GetStringValue(status);
    (void)name_value;
    (void)status_value;
    (void)parse_end;
    (void)json_len;
    (void)data;
    (void)user;

    // step 4: Cleanup
    cJSON_free(temp_buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}