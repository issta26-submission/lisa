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
//<ID> 240
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
    cJSON *msg = cJSON_CreateString("integration test");
    cJSON *arr_n1 = cJSON_CreateNumber(1.5);
    cJSON *arr_n2 = cJSON_CreateNumber(2.5);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    double v1 = 0.0;
    double v2 = 0.0;
    double child_val = 0.0;
    double root_val = 0.0;
    double total = 0.0;
    cJSON *tmp1 = NULL;
    cJSON *tmp2 = NULL;
    cJSON *tmp_child_num = NULL;
    cJSON *tmp_root_num = NULL;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(child, "child_number", 42.0);
    cJSON_AddItemToArray(arr, arr_n1);
    cJSON_AddItemToArray(arr, arr_n2);
    cJSON_AddItemToObject(child, "array", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "message", msg);
    cJSON_AddNumberToObject(root, "root_number", 7.77);

    // step 3: Operate / Validate
    tmp1 = cJSON_GetArrayItem(arr, 0);
    tmp2 = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(tmp1);
    v2 = cJSON_GetNumberValue(tmp2);
    tmp_child_num = cJSON_GetObjectItem(child, "child_number");
    child_val = cJSON_GetNumberValue(tmp_child_num);
    tmp_root_num = cJSON_GetObjectItem(root, "root_number");
    root_val = cJSON_GetNumberValue(tmp_root_num);
    total = v1 + v2 + child_val + root_val;
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)total;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}