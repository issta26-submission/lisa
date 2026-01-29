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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"config\": {\"enabled\": true}, \"values\": [1,2,3]}";
    size_t json_len = strlen(json_text);
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);
    cJSON *root = cJSON_CreateObject();
    int numbers[3] = {10, 20, 30};
    cJSON *ints = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "ints", ints);

    // step 2: Configure
    cJSON_AddItemToObject(root, "source", parsed);
    cJSON *nul = cJSON_AddNullToObject(root, "nothing");
    (void)nul;
    cJSON *fals = cJSON_AddFalseToObject(root, "flag_false");
    (void)fals;

    // step 3: Operate and Validate
    cJSON_bool has_cfg = cJSON_HasObjectItem(parsed, "config");
    cJSON_AddBoolToObject(root, "has_config", has_cfg);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}