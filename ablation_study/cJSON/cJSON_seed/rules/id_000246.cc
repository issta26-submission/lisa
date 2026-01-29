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
//<ID> 246
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
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON *n2 = cJSON_CreateNumber(35.0);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *tmp_child = NULL;
    cJSON *tmp_arr0 = NULL;
    cJSON *tmp_arr1 = NULL;
    double v_child = 0.0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(child, "inner", 123.5);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate / Validate
    tmp_child = cJSON_GetObjectItem(root, "child");
    v_child = cJSON_GetNumberValue(cJSON_GetObjectItem(tmp_child, "inner"));
    tmp_arr0 = cJSON_GetArrayItem(arr, 0);
    tmp_arr1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(tmp_arr0);
    v1 = cJSON_GetNumberValue(tmp_arr1);
    sum = v_child + v0 + v1;
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