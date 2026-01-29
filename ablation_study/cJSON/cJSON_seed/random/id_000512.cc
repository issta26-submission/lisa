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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ints = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *retrieved_ints = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    const char *label_str = (const char *)0;
    int nums[4];
    int count = 4;
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    root = cJSON_CreateObject();
    ints = cJSON_CreateIntArray((const int *)nums, count);
    label_item = cJSON_CreateString("my_numbers");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "ints", ints);
    add_ok2 = cJSON_AddItemToObject(root, "label", label_item);

    // step 4: Operate
    retrieved_ints = cJSON_GetObjectItem(root, "ints");
    retrieved_label = cJSON_GetObjectItem(root, "label");
    arr_size = cJSON_GetArraySize(retrieved_ints);
    label_str = cJSON_GetStringValue(retrieved_label);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)arr_size;
    (void)label_str;
    (void)scratch;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}