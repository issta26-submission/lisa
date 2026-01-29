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
//<ID> 519
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    int numbers[] = {10, 20, 30, 40};
    int arr_size = 0;
    char *name_val = (char *)0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(numbers, 4);
    name_item = cJSON_CreateString("example");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "numbers", arr);
    add_ok = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_name = cJSON_GetObjectItem(root, "name");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    name_val = cJSON_GetStringValue(retrieved_name);

    // step 5: Validate
    (void)arr_size;
    (void)name_val;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}