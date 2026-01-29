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
//<ID> 716
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const int numbers[] = {10, 20, 30};
    cJSON *ints_arr = cJSON_CreateIntArray(numbers, 3);
    cJSON *label = cJSON_CreateString("example_label");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "ints", ints_arr);
    cJSON_AddItemToObject(root, "label", label);
    const char json_text[] = "{\"status\":\"ok\",\"count\":3}";
    size_t json_len = (sizeof(json_text) - 1);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    int initial_count = cJSON_GetArraySize(ints_arr);
    cJSON_AddNumberToObject(root, "initial_count", (double)initial_count);
    cJSON *count_item = cJSON_GetObjectItem(parsed, "count");
    double parsed_count = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "parsed_count", parsed_count);

    // step 3: Operate
    cJSON *status_item = cJSON_GetObjectItem(parsed, "status");
    const char *status_str = cJSON_GetStringValue(status_item);
    cJSON *status_copy = cJSON_CreateString(status_str);
    cJSON_AddItemToObject(root, "status_copy", status_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    int final_count = cJSON_GetArraySize(ints_arr);
    cJSON_AddNumberToObject(root, "final_count", (double)final_count);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}