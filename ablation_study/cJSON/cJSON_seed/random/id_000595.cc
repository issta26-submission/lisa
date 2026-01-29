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
//<ID> 595
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *added_count = (cJSON *)0;
    int arr_size = 0;
    double first_val = 0.0;
    char *label_str = (char *)0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.0);
    n1 = cJSON_CreateNumber(2.0);
    n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example_label"));

    // step 3: Configure
    retr_arr = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(retr_arr);
    added_count = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    first_item = cJSON_GetArrayItem(retr_arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    label_item = cJSON_GetObjectItem(root, "label");
    label_str = cJSON_GetStringValue(label_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)added_count;
    (void)first_val;
    (void)label_str;
    (void)arr_size;
    (void)first_item;
    (void)retr_arr;
    (void)printed;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}