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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "container", container);

    // step 2: Configure
    double nums[3];
    nums[0] = 1.5;
    nums[1] = 2.5;
    nums[2] = 3.5;
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToArray(container, dbl_array);
    cJSON *num_total = cJSON_AddNumberToObject(root, "total", 3.0);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(dbl_array);
    cJSON *len = cJSON_AddNumberToObject(root, "len", (double)size);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}