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
//<ID> 536
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
    cJSON *str_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    cJSON_bool is_meta_raw = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    double values[3];
    values[0] = 1.1;
    values[1] = 2.2;
    values[2] = 3.3;
    arr = cJSON_CreateDoubleArray(values, 3);
    str_item = cJSON_CreateString("unit");
    raw_item = cJSON_CreateRaw("unparsed-meta");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, str_item);
    add_ok2 = cJSON_AddItemToObject(root, "readings", arr);
    add_ok3 = cJSON_AddItemToObject(root, "meta", raw_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "readings");
    retrieved_elem = cJSON_GetArrayItem(retrieved_arr, 3);
    retrieved_str = cJSON_GetStringValue(retrieved_elem);
    is_meta_raw = cJSON_IsRaw(cJSON_GetObjectItem(root, "meta"));

    // step 5: Validate
    (void)retrieved_str;
    (void)is_meta_raw;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}