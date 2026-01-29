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
//<ID> 366
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
    cJSON *num_item2 = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double got_value = 0.0;
    const char *raw_json = "{\"k\":10}";

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "num", num_item);
    raw_item = cJSON_AddRawToObject(root, "raw", raw_json);

    // step 4: Operate
    got_num = cJSON_GetObjectItem(root, "num");
    got_value = cJSON_GetNumberValue(got_num);
    num_item2 = cJSON_CreateNumber(got_value * 2.0);
    cJSON_AddItemToObject(root, "num_doubled", num_item2);
    detached = cJSON_DetachItemFromObject(root, "raw");

    // step 5: Validate
    (void)got_value;
    (void)got_num;
    (void)raw_item;
    (void)num_item;
    (void)num_item2;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}