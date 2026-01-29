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
//<ID> 507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "{\"parsed\": [1, 2, 3], \"raw\": \"<raw>\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = (const char *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Setup (parse JSON from a buffer)
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 3: Configure (extract array, duplicate it, create a raw item and add items to objects)
    arr = cJSON_GetObjectItem(parsed, "parsed");
    is_arr = cJSON_IsArray((const cJSON *const)arr);
    dup_arr = cJSON_Duplicate((const cJSON *)arr, 1);
    raw_item = cJSON_CreateRaw("injected_raw_content");
    add_ok = cJSON_AddItemToObject(parsed, "injected_raw", raw_item);
    root = cJSON_CreateObject();
    add_ok = cJSON_AddItemToObject(root, "dup_values", dup_arr);

    // step 4: Operate (type checks)
    is_raw = cJSON_IsRaw((const cJSON *const)raw_item);

    // step 5: Validate (print result)
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_arr;
    (void)is_raw;
    (void)add_ok;
    return 66;
}