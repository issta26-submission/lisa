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
//<ID> 247
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
    cJSON *num_a = cJSON_CreateNumber(1.5);
    cJSON *num_b = cJSON_CreateNumber(2.5);
    cJSON *label = cJSON_CreateString("example");
    cJSON *pi_item = NULL;
    cJSON *retrieved_pi = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *arr_item1 = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    double pi_val = 0.0;

    // step 2: Setup / Configure
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, num_b);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    arr_item1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(arr_item0);
    v1 = cJSON_GetNumberValue(arr_item1);
    sum = v0 + v1;
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(retrieved_pi);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)sum;
    (void)pi_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}