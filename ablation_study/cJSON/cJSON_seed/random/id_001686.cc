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
//<ID> 1686
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *single_str = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    const char *names[] = { "alice", "bob", "carol" };
    double retrieved_value = 0.0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(names, 3);
    single_str = cJSON_CreateString("singleton");
    num_item = cJSON_CreateNumber(123.456);

    // step 3: Configure
    cJSON_AddItemToObject(root, "names", str_array);
    cJSON_AddItemToObject(root, "single", single_str);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 4: Operate
    retrieved_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    arr_size = cJSON_GetArraySize(str_array);

    // step 5: Validate
    validation = (int)(retrieved_value == 123.456) + (int)(arr_size == 3);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)str_array;
    (void)single_str;
    (void)num_item;
    (void)retrieved_value;
    (void)arr_size;

    // API sequence test completed successfully
    return 66;
}