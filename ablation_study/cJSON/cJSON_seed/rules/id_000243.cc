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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_in_arr = cJSON_CreateNumber(100.0);
    cJSON *num_from_arr = NULL;
    cJSON *child_item = NULL;
    cJSON *pi_item = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    double val_arr = 0.0;
    double val_pi = 0.0;
    double total = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(child, "pi", 3.1415);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, num_in_arr);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    num_from_arr = cJSON_GetArrayItem(arr, 0);
    val_arr = cJSON_GetNumberValue(num_from_arr);
    child_item = cJSON_GetObjectItem(root, "child");
    pi_item = cJSON_GetObjectItem(child_item, "pi");
    val_pi = cJSON_GetNumberValue(pi_item);
    total = val_arr + val_pi;
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)total;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}