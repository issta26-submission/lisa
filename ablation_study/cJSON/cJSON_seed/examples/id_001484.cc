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
//<ID> 1484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int *nums_buf = (int *)cJSON_malloc(sizeof(int) * 3);
    memset(nums_buf, 0, sizeof(int) * 3);
    nums_buf[0] = 10;
    nums_buf[1] = 20;
    nums_buf[2] = 30;
    cJSON *nums = cJSON_CreateIntArray(nums_buf, 3);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "fuzz_test");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool nums_is_array = cJSON_IsArray(cJSON_GetObjectItem(root, "nums"));
    (void)nums_is_array;

    // step 3: Operate & Validate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    (void)pi_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(nums_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}