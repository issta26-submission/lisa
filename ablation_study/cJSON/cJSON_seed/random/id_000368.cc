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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *found_num = (cJSON *)0;
    cJSON *detached_raw = (cJSON *)0;
    const char *raw_json = "{\"nested\":5}";
    double extracted_value = 0.0;
    int validation_flag = 0;
    cJSON_bool add_result = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    add_result = cJSON_AddItemToObject(root, "num", num_item);
    raw_item = cJSON_AddRawToObject(root, "raw", raw_json);

    // step 4: Operate
    found_num = cJSON_GetObjectItem(root, "num");
    extracted_value = cJSON_GetNumberValue(found_num);
    detached_raw = cJSON_DetachItemFromObject(root, "raw");

    // step 5: Validate
    validation_flag = ((found_num != (cJSON *)0) ? 1 : 0) + ((detached_raw != (cJSON *)0) ? 1 : 0) + (add_result ? 1 : 0);
    (void)extracted_value;
    (void)validation_flag;

    // step 6: Cleanup
    cJSON_Delete(detached_raw);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}