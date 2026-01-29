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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "{\"nums\":[1,2,3],\"name\":\"example\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = (const char *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *parsed_nums = (cJSON *)0;
    cJSON *dup_nums = (cJSON *)0;
    cJSON *dup_raw = (cJSON *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_arr_dup = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize (parse JSON with explicit length and null-termination requirement)
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 3: Configure (create a raw item and attach it to the parsed object)
    raw_item = cJSON_CreateRaw("unparsed:raw_value");
    add_ok = cJSON_AddItemToObject(parsed, "raw", raw_item);

    // step 4: Operate (duplicate the whole parsed tree and fetch array/raw members)
    dup = cJSON_Duplicate(parsed, 1);
    parsed_nums = cJSON_GetObjectItem(parsed, "nums");
    dup_nums = cJSON_GetObjectItem(dup, "nums");
    dup_raw = cJSON_GetObjectItem(dup, "raw");

    // step 5: Validate (inspect types on original and duplicated items)
    is_arr = cJSON_IsArray((const cJSON *const)parsed_nums);
    is_arr_dup = cJSON_IsArray((const cJSON *const)dup_nums);
    is_raw = cJSON_IsRaw((const cJSON *const)dup_raw);
    (void)parse_end;
    (void)add_ok;
    (void)is_arr;
    (void)is_arr_dup;
    (void)is_raw;

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}