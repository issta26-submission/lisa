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
//<ID> 597
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
    cJSON *obj = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retr_obj = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *first_in_arr = (cJSON *)0;
    char *first_str = (char *)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    obj = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello_world");
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "myarray", arr);
    cJSON_AddItemToObject(root, "subobj", obj);
    cJSON_AddNumberToObject(obj, "added_number", 3.14159);

    // step 4: Operate
    retr_obj = cJSON_GetObjectItem(root, "subobj");
    retr_arr = cJSON_GetObjectItem(root, "myarray");
    arr_size = cJSON_GetArraySize(retr_arr);
    first_in_arr = cJSON_GetArrayItem(retr_arr, 0);
    first_str = cJSON_GetStringValue(first_in_arr);

    // step 5: Validate
    (void)retr_obj;
    (void)arr_size;
    (void)first_str;
    (void)first_in_arr;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}