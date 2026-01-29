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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateFalse();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *name = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, b_true);
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddItemToObjectCS(root, "flag", b_false);
    cJSON_AddItemToObjectCS(root, "count", num);
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 3: Operate & Validate
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON *printed_flag = cJSON_CreateBool(printed);
    cJSON_AddItemToObjectCS(root, "printed", printed_flag);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObjectCS(root, "items_size", size_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}