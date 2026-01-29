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
//<ID> 563
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *count_node = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    char *printed_items = (char *)0;
    char *printed_root = (char *)0;
    double count_val = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.5);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    count_node = cJSON_AddNumberToObject(root, "count", 2.0);
    add_ok3 = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    printed_items = cJSON_PrintUnformatted(retrieved_items);
    printed_root = cJSON_PrintUnformatted(root);
    count_val = cJSON_GetNumberValue(count_node);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)count_val;
    (void)printed_items;
    (void)printed_root;
    (void)retrieved_items;

    // step 6: Cleanup
    cJSON_free(printed_items);
    cJSON_free(printed_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}