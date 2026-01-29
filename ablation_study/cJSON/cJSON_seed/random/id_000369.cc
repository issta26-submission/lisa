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
//<ID> 369
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
    cJSON *got_child = (cJSON *)0;
    cJSON *got_number = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const char *raw_json = "{\"inner\":123}";
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "number", num_item);
    raw_item = cJSON_AddRawToObject(child, "embedded", raw_json);

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "child");
    got_number = cJSON_GetObjectItem(got_child, "number");
    detached = cJSON_DetachItemFromObject(child, "number");
    extracted_value = cJSON_GetNumberValue(detached);

    // step 5: Validate
    (void)got_child;
    (void)got_number;
    (void)raw_item;
    (void)extracted_value;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}