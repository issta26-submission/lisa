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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_to_object = 0;
    cJSON_bool is_raw = 0;
    const char *raw_json = "{\"embedded\":true}";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw(raw_json);

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, raw_item);
    added_to_object = cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    printed = cJSON_PrintBuffered(root, 256, 1);
    is_raw = cJSON_IsRaw(raw_item);

    // step 5: Validate
    (void)printed;
    (void)is_raw;
    (void)added_to_array;
    (void)added_to_object;
    (void)raw_json;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}