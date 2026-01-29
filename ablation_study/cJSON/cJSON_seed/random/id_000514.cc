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
//<ID> 514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    const int numbers[3] = {10, 20, 30};
    int array_size = 0;
    const char *title = (const char *)0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_array = cJSON_CreateIntArray(numbers, 3);
    str_item = cJSON_CreateString("number_list");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "numbers", int_array);
    add_ok = cJSON_AddItemToObject(root, "title", str_item);

    // step 4: Operate
    retrieved_array = cJSON_GetObjectItem(root, "numbers");
    array_size = cJSON_GetArraySize(retrieved_array);
    retrieved_str = cJSON_GetObjectItem(root, "title");
    title = cJSON_GetStringValue(retrieved_str);

    // step 5: Validate
    (void)array_size;
    (void)title;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}