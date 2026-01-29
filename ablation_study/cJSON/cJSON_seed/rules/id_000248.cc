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
//<ID> 248
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
    cJSON *num1 = cJSON_CreateNumber(1.23);
    cJSON *num2 = cJSON_CreateNumber(4.56);
    cJSON *nested = cJSON_CreateObject();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    cJSON *tmp = NULL;
    cJSON *extra = NULL;
    double v0 = 0.0;
    double v1 = 0.0;
    double extra_val = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddNumberToObject(root, "extra", 7.89);
    cJSON_AddNumberToObject(nested, "subnum", 10.0);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    tmp = cJSON_GetArrayItem(arr, 0);
    v0 = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(tmp);
    extra = cJSON_GetObjectItem(root, "extra");
    extra_val = cJSON_GetNumberValue(extra);
    sum = v0 + v1 + extra_val;
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)sum;
    (void)extra_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}