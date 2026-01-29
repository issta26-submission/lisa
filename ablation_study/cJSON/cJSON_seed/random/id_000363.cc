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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *found_child = (cJSON *)0;
    cJSON *found_num = (cJSON *)0;
    const char *raw_json = "{\"nested\": [1, 2, 3]}";
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "pi", num_item);
    raw_item = cJSON_AddRawToObject(child, "raw", raw_json);

    // step 4: Operate
    found_child = cJSON_GetObjectItem(root, "child");
    detached = cJSON_DetachItemFromObject(child, "raw");
    cJSON_AddItemToObject(root, "detached_raw", detached);

    // step 5: Validate
    found_num = cJSON_GetObjectItem(found_child, "pi");
    extracted_value = cJSON_GetNumberValue(found_num);
    (void)extracted_value;

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}