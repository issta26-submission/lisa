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
//<ID> 504
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
    cJSON *vals = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *added = (cJSON *)0;
    char *printed = (char *)0;
    const char *parse_end = (const char *)0;
    const char json[] = "{\"name\":\"example\",\"values\":[1,2,3]}";
    size_t json_len = (size_t)(sizeof(json) - 1);
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize (parse JSON with explicit length and null-termination requirement)
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 3: Configure (create a raw item and attach it to the parsed root)
    raw_item = cJSON_CreateRaw("unparsed:raw");
    add_ok = cJSON_AddItemToObject(root, "rawfield", raw_item);

    // step 4: Operate (duplicate the root, inspect array and raw types, then modify duplicate)
    dup = cJSON_Duplicate(root, 1);
    vals = cJSON_GetObjectItem(root, "values");
    is_arr = cJSON_IsArray((const cJSON *const)vals);
    is_raw = cJSON_IsRaw((const cJSON *const)raw_item);
    added = cJSON_CreateString("injected");
    add_ok = cJSON_AddItemToObject(dup, "injected_field", added);

    // step 5: Validate (print duplicate and release printed buffer)
    printed = cJSON_PrintUnformatted((const cJSON *)dup);
    cJSON_free((void *)printed);
    (void)is_arr;
    (void)is_raw;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}