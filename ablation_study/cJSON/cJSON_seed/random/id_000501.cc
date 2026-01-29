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
//<ID> 501
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json_text[] = "{\"numbers\": [1, 2, 3], \"name\": \"example\"}";
    size_t json_length = sizeof(json_text) - 1;
    const char *parse_end = (const char *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *numbers = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize / Parse
    parsed = cJSON_ParseWithLengthOpts(json_text, json_length, &parse_end, 1);
    raw_item = cJSON_CreateRaw("UNPARSED_RAW_CONTENT");
    add_ok = cJSON_AddItemToObject(parsed, "raw_field", raw_item);

    // step 3: Configure / Inspect
    numbers = cJSON_GetObjectItem(parsed, "numbers");
    is_arr = cJSON_IsArray((const cJSON *const)numbers);
    is_raw = cJSON_IsRaw((const cJSON *const)raw_item);
    dup = cJSON_Duplicate(parsed, 1);

    // step 4: Operate / Build new object
    root = cJSON_CreateObject();
    add_ok = cJSON_AddItemToObject(root, "duplicate", dup);

    // step 5: Validate / Print
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);
    (void)parse_end;
    (void)is_arr;
    (void)is_raw;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}