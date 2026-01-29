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
//<ID> 1660
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *replacement_num = (cJSON *)0;
    char *printed = (char *)0;
    int array_size = 0;
    double num_value = 0.0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.0);
    replacement_num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    replaced = cJSON_ReplaceItemInObject(root, "numbers", replacement_num);
    num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "numbers"));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(array_size == 2) + (int)(num_value == 42.0) + (int)(printed != (char *)0) + (int)replaced;
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)arr;
    (void)n1;
    (void)n2;
    (void)replacement_num;
    (void)array_size;
    (void)num_value;
    (void)replaced;
    (void)printed;
    return 66;
    // API sequence test completed successfully
}