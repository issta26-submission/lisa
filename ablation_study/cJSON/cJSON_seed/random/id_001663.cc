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
//<ID> 1663
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
    cJSON *initial_count = (cJSON *)0;
    cJSON *replacement_count = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    char *json_unformatted = (char *)0;
    int array_size = 0;
    double count_value = 0.0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    initial_count = cJSON_CreateNumber(10.0);
    replacement_count = cJSON_CreateNumber(100.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "count", initial_count);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    replaced = cJSON_ReplaceItemInObject(root, "count", replacement_count);
    count_item = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(count_item);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(json_unformatted != (char *)0) + (int)(array_size == 2) + (int)(count_value == 100.0) + (int)replaced;
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n1;
    (void)n2;
    (void)initial_count;
    (void)replacement_count;
    (void)count_item;
    (void)array_size;
    (void)count_value;
    (void)replaced;
    // API sequence test completed successfully
    return 66;
}