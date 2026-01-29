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
//<ID> 599
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
    cJSON *sitem = (cJSON *)0;
    cJSON *nitem = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *added_size_num = (cJSON *)0;
    cJSON *added_second_num = (cJSON *)0;
    char *sval = (char *)0;
    int arr_size = 0;
    double second_val = 0.0;
    double check_size = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    sitem = cJSON_CreateString("alpha");
    nitem = cJSON_CreateNumber(7.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, sitem);
    cJSON_AddItemToArray(arr, nitem);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("greeting"));

    // step 4: Operate
    retr_arr = cJSON_GetObjectItem(root, "array");
    arr_size = cJSON_GetArraySize(retr_arr);
    added_size_num = cJSON_AddNumberToObject(root, "size_as_number", (double)arr_size);
    first_item = cJSON_GetArrayItem(retr_arr, 0);
    label_item = cJSON_GetObjectItem(root, "label");
    sval = cJSON_GetStringValue(label_item);
    second_item = cJSON_GetArrayItem(retr_arr, 1);
    second_val = cJSON_GetNumberValue(second_item);
    added_second_num = cJSON_AddNumberToObject(root, "second_plus_one", second_val + 1.0);

    // step 5: Validate
    check_size = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "size_as_number"));
    (void)first_item;
    (void)sval;
    (void)added_second_num;
    (void)check_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}