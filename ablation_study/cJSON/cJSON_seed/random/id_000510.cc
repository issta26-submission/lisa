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
//<ID> 510
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
    cJSON *name = (cJSON *)0;
    cJSON *retrieved_ints = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    int nums[4];
    int arr_size = 0;
    char *printed = (char *)0;
    cJSON_bool add_ok = 0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    root = cJSON_CreateObject();
    ints = cJSON_CreateIntArray(nums, 4);
    name = cJSON_CreateString("example");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "ints", ints);
    add_ok = cJSON_AddItemToObject(root, "name", name);

    // step 4: Operate
    retrieved_ints = cJSON_GetObjectItem(root, "ints");
    arr_size = cJSON_GetArraySize(retrieved_ints);
    retrieved_name = cJSON_GetObjectItem(root, "name");

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)retrieved_name;
    (void)add_ok;
    (void)buffer;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}