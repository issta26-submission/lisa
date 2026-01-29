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
//<ID> 367
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
    cJSON *retrieved = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *raw_json = "{\"k\":true}";
    double value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "value", num_item);
    raw_item = cJSON_AddRawToObject(child, "raw", raw_json);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "child");
    detached = cJSON_DetachItemFromObject(child, "raw");
    cJSON_AddItemToObject(root, "detached_raw", detached);
    value_item = cJSON_GetObjectItem(retrieved, "value");

    // step 5: Validate
    value = cJSON_GetNumberValue(value_item);
    printed = cJSON_PrintUnformatted(root);
    (void)value;
    (void)raw_item;
    (void)num_item;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); printed = (char *)0; }
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}