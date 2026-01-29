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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *str_ref = cJSON_CreateStringReference("label_ref");
    cJSON *parsed = NULL;
    cJSON *count_item = NULL;
    cJSON *child_item = NULL;
    double count_val = 0.0;
    cJSON_bool is_raw_flag = 0;

    // step 2: Setup / Configure
    cJSON *added_count = cJSON_AddNumberToObject(root, "count", 5.0);
    cJSON_AddItemToObject(child, "info", cJSON_CreateStringReference("child_info"));
    cJSON_AddItemToObject(child, "label", str_ref);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    count_item = cJSON_GetObjectItem(parsed, "count");
    count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(parsed, "double_count", count_val * 2.0);
    child_item = cJSON_GetObjectItem(parsed, "child");
    is_raw_flag = cJSON_IsRaw(child_item);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "count");
    buffer[0] = (char)('0' + (int)count_val);
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}