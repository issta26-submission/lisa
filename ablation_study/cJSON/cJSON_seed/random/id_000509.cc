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
//<ID> 509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"numbers\": [10, 20], \"name\": \"alice\"}";
    size_t json_len = 0;
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *numbers = (cJSON *)0;
    cJSON *dup_raw = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    json_len = strlen(json_text);
    root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    raw_item = cJSON_CreateRaw("/*raw-content*/42");
    num_item = cJSON_CreateNumber(99.0);

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "embedded_raw", raw_item);
    add_ok = cJSON_AddItemToObject(root, "extra_number", num_item);

    // step 4: Operate
    numbers = cJSON_GetObjectItem(root, "numbers");
    is_arr = cJSON_IsArray((const cJSON *const)numbers);
    dup_root = cJSON_Duplicate(root, 1);
    dup_raw = cJSON_GetObjectItem(dup_root, "embedded_raw");
    is_raw = cJSON_IsRaw((const cJSON *const)dup_raw);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)dup_root);
    (void)is_arr;
    (void)is_raw;
    (void)add_ok;
    (void)parse_end;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}