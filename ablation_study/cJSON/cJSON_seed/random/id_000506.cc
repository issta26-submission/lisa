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
//<ID> 506
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *new_raw = (cJSON *)0;
    char *printed = (char *)0;
    const char json[] = "{\"arr\":[1,2,3],\"name\":\"example\"}";
    size_t json_len = (size_t)(sizeof(json) - 1);
    const char *parse_end_ptr = (const char *)0;
    cJSON_bool require_null_terminated = 1;
    cJSON_bool is_array = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end_ptr, require_null_terminated);

    // step 3: Configure
    arr = cJSON_GetObjectItem((const cJSON *const)root, "arr");
    is_array = cJSON_IsArray((const cJSON *const)arr);
    dup = cJSON_Duplicate(root, 1);
    new_raw = cJSON_CreateRaw("42");
    add_ok = cJSON_AddItemToObject(dup, "injected", new_raw);
    is_raw = cJSON_IsRaw((const cJSON *const)new_raw);

    // step 4: Operate
    printed = cJSON_PrintUnformatted((const cJSON *)dup);

    // step 5: Validate
    (void)parse_end_ptr;
    (void)require_null_terminated;
    (void)is_array;
    (void)is_raw;
    (void)add_ok;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}