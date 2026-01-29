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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(7.0);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *retrieved_arr = NULL;
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *retrieved_flag = NULL;
    double sum = 0.0;
    cJSON *sum_item = NULL;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "extra", n3);
    cJSON_AddItemReferenceToArray(arr, n3);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    elem2 = cJSON_GetArrayItem(retrieved_arr, 2);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    sum = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1) + cJSON_GetNumberValue(elem2) + (double)cJSON_IsTrue(retrieved_flag);
    cJSON_AddNumberToObject(root, "sum", sum);
    sum_item = cJSON_GetObjectItem(root, "sum");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)sum_item;
    return 66;
}