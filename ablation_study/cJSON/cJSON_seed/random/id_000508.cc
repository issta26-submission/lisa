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
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "[1, 2, 3]";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = (const char *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize / Parse
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 3: Configure / Build objects
    root = cJSON_CreateObject();
    dup = cJSON_Duplicate((const cJSON *)parsed, 1);
    add_ok = cJSON_AddItemToObject(root, "arr_copy", dup);
    raw_item = cJSON_CreateRaw("unprocessed_raw_fragment");
    add_ok = cJSON_AddItemToObject(root, "raw_fragment", raw_item);

    // step 4: Operate / Inspect
    is_arr = cJSON_IsArray((const cJSON *const)parsed);
    is_raw = cJSON_IsRaw((const cJSON *const)raw_item);

    // step 5: Validate / Print
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)is_arr;
    (void)is_raw;
    (void)add_ok;
    (void)parse_end;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}