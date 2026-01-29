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
//<ID> 503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "{\"values\":[\"a\",\"b\"],\"num\":7}";
    const char *parse_end = (const char *)0;
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;

    // step 2: Initialize / Parse
    parsed = cJSON_ParseWithLengthOpts((const char *)json, json_len, &parse_end, 1);
    arr = cJSON_GetObjectItem((const cJSON *const)parsed, "values");

    // step 3: Configure / Duplicate and create raw
    is_arr = cJSON_IsArray((const cJSON *const)arr);
    dup_arr = cJSON_Duplicate((const cJSON *)arr, 1);
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    is_raw = cJSON_IsRaw((const cJSON *const)raw_item);
    root = cJSON_CreateObject();
    add_ok1 = cJSON_AddItemToObject(root, "dup_values", dup_arr);
    add_ok2 = cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    (void)parse_end;
    (void)is_arr;
    (void)is_raw;
    (void)add_ok1;
    (void)add_ok2;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}