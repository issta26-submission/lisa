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
//<ID> 82
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
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON *n2 = cJSON_CreateNumber(4.5);
    cJSON *t = cJSON_CreateTrue();
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    int arr_size = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    double truth_as_num = 0.0;
    double sum = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);
    truth_as_num = (double)cJSON_IsTrue(cJSON_GetArrayItem(arr, 2));
    sum = v0 + v1 + truth_as_num + (double)arr_size;
    cJSON_AddNumberToObject(root, "sum", sum);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}