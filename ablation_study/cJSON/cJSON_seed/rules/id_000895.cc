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
//<ID> 895
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *repl = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    char *printed = NULL;
    double v0 = 0.0;
    double v1 = 0.0;
    double scalar = 0.0;
    double sum = 0.0;
    cJSON_bool is_num0 = 0;
    cJSON_bool replaced = 0;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "nums", arr);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.0);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "nums");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    is_num0 = cJSON_IsNumber(item0);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);
    scalar = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "scalar"));
    sum = v0 + v1 + scalar;
    repl = cJSON_CreateNumber(99.75);
    replaced = cJSON_ReplaceItemInArray(retrieved_arr, 1, repl);
    printed = cJSON_Print(root);
    scratch[0] = printed[0];
    scratch[1] = '\0';
    (void)is_num0;
    (void)replaced;
    (void)sum;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}