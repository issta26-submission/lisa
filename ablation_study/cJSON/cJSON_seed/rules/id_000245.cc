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
//<ID> 245
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
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *added_num = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    cJSON *tmp0 = NULL;
    cJSON *tmp1 = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    added_num = cJSON_AddNumberToObject(root, "extra_number", 3.14);
    cJSON_AddItemToObject(root, "metadata", cJSON_CreateObject());

    // step 3: Operate / Validate
    tmp0 = cJSON_GetArrayItem(arr, 0);
    tmp1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(tmp0);
    v1 = cJSON_GetNumberValue(tmp1);
    sum = v0 + v1 + cJSON_GetNumberValue(added_num);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)sum;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}