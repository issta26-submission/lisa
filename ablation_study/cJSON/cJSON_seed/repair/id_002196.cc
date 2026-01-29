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
//<ID> 2196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    int numbers[3] = {1, 2, 3};
    cJSON *int_arr = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "ints", int_arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(15.0);
    cJSON_AddItemToArray(items, n2);
    cJSON *copied_int_arr = cJSON_Duplicate(int_arr, 1);
    cJSON_InsertItemInArray(items, 1, copied_int_arr);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(items, 0);
    double extracted = cJSON_GetNumberValue(detached);
    cJSON *newnum = cJSON_CreateNumber(extracted + 10.0);
    cJSON_InsertItemInArray(items, 0, newnum);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}