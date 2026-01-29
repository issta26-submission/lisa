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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"keep\": 100, \"remove\": 200}";
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *added_num = NULL;
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_keep = NULL;
    cJSON *retrieved_added = NULL;
    char *printed = NULL;
    double keep_val = 0.0;
    double added_val = 0.0;
    cJSON_bool add_item_result = 0;

    // step 2: Setup / Configure
    added_num = cJSON_AddNumberToObject(root, "added_number", 42.5);
    add_item_result = cJSON_AddItemToObject(root, "child", parsed);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "remove");

    // step 3: Operate / Validate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_keep = cJSON_GetObjectItem(retrieved_child, "keep");
    retrieved_added = cJSON_GetObjectItem(root, "added_number");
    keep_val = cJSON_GetNumberValue(retrieved_keep);
    added_val = cJSON_GetNumberValue(retrieved_added);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed[0];
    buffer[1] = printed[1];
    buffer[2] = (char)('0' + ((int)keep_val % 10));
    buffer[3] = (char)('0' + ((int)added_val % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}