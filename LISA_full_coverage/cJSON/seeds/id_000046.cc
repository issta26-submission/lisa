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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_InsertItemInArray(flags, 0, true_item);
    cJSON_InsertItemInArray(flags, 1, false_item);

    // step 3: Operate and Validate
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(flags));
    double pi_val = cJSON_GetNumberValue(pi_item);
    double count_val = cJSON_GetNumberValue(count_item);
    int summary = (int)pi_val + (int)count_val + cJSON_GetArraySize(flags);
    (void)summary;
    (void)pi_val;
    (void)count_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}