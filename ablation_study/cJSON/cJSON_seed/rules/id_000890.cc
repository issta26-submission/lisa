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
//<ID> 890
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
    cJSON *retrieved = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    char *printed = NULL;
    double val0 = 0.0;
    double val1 = 0.0;
    int arr_size = 0;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool replaced_ok = 0;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.0);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    is_num0 = cJSON_IsNumber(item0);
    is_num1 = cJSON_IsNumber(item1);
    val0 = cJSON_GetNumberValue(item0);
    val1 = cJSON_GetNumberValue(item1);
    repl = cJSON_CreateNumber(val0 + val1);
    replaced_ok = cJSON_ReplaceItemInArray(arr, 1, repl);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}