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
//<ID> 505
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "{\"data\":[{\"raw\":\"RAW_CONTENT\",\"value\":123}],\"meta\":\"info\"}";
    size_t buffer_length = sizeof(json) - 1;
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *data_array = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *dup_elem = (cJSON *)0;
    cJSON *out = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok = 0;

    // step 2: Parse input JSON with explicit length and null-termination requirement
    root = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 1);

    // step 3: Extract array and inspect types
    data_array = cJSON_GetObjectItem(root, "data");
    is_arr = cJSON_IsArray((const cJSON *const)data_array);
    first_elem = cJSON_GetArrayItem(data_array, 0);
    raw_item = cJSON_GetObjectItem(first_elem, "raw");
    is_raw = cJSON_IsRaw((const cJSON *const)raw_item);

    // step 4: Duplicate an element and add it into a new object
    dup_elem = cJSON_Duplicate((const cJSON *)first_elem, 1);
    out = cJSON_CreateObject();
    add_ok = cJSON_AddItemToObject(out, "copied", dup_elem);

    // step 5: Validate by printing and use flags to show flow
    printed = cJSON_PrintUnformatted((const cJSON *)out);
    (void)is_arr;
    (void)is_raw;
    (void)add_ok;
    (void)parse_end;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}