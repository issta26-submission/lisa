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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":123}");
    cJSON_AddItemToArray(array, raw_item);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    const char *status_texts[2] = { "not_raw", "is_raw" };
    cJSON *status = cJSON_CreateString(status_texts[(int)is_raw]);
    cJSON_AddItemToObject(root, "raw_status", status);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(printed);
    cJSON_free(printed);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    char *parsed_name_val = cJSON_GetStringValue(parsed_name);
    (void)parsed_name_val;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}