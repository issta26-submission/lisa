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
//<ID> 502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    const char *json_text = "{\"nested\":[{\"k\":1},{\"k\":2}],\"num\":10}";
    const char *parse_end = (const char *)0;
    size_t json_len = (size_t)strlen(json_text);
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);

    // step 3: Configure
    arr = cJSON_GetObjectItem(parsed, "nested");
    dup = cJSON_Duplicate(arr, 1);
    raw_item = cJSON_CreateRaw("{\"inner\":3}");
    add_ok = cJSON_AddItemToObject(parsed, "raw", raw_item);
    add_ok = cJSON_AddItemToObject(root, "copied", dup);

    // step 4: Operate
    is_arr = cJSON_IsArray((const cJSON *const)arr);
    is_raw = cJSON_IsRaw((const cJSON *const)raw_item);

    // step 5: Validate
    (void)is_arr;
    (void)is_raw;
    (void)add_ok;
    (void)parse_end;

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}