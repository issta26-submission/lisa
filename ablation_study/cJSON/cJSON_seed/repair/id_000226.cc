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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "test_root");
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *init_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(child, "initial", init_num);

    // step 2: Configure
    const char json_buf[] = "{\"value\": 42.5, \"note\": \"imported\"}";
    size_t json_len = sizeof(json_buf) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json_buf, json_len);
    cJSON *parsed_value = cJSON_GetObjectItem(parsed, "value");
    double imported_value = cJSON_GetNumberValue(parsed_value);
    cJSON *imported_num = cJSON_CreateNumber(imported_value);
    cJSON_AddItemToObject(root, "imported_value", imported_num);
    cJSON_AddStringToObject(root, "import_note", "brought_in");

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "imported_value");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    cJSON *dup_num = cJSON_Duplicate(retrieved, 0);
    cJSON_AddItemToObject(root, "imported_value_copy", dup_num);
    char *out = cJSON_PrintUnformatted(root);
    (void)retrieved_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}