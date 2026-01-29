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
//<ID> 364
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
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *detached_num = (cJSON *)0;
    const char *raw_json = (const char *)"{\"k\":true}";
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "num", num_item);
    raw_item = cJSON_AddRawToObject(child, "raw", raw_json);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    detached_num = cJSON_DetachItemFromObject(child, "num");
    extracted_value = cJSON_GetNumberValue(detached_num);

    // step 5: Validate (use extracted_value to demonstrate meaningful data flow)
    (void)retrieved_child;
    (void)raw_item;
    (void)extracted_value;

    // step 6: Cleanup
    cJSON_Delete(detached_num);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}