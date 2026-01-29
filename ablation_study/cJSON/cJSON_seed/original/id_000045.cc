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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "initial", 1.0);
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *negflag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "negflag", negflag);

    // step 2: Configure
    cJSON *new_n = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(numbers, 1, new_n);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(numbers);
    cJSON *item0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *item1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *item2 = cJSON_GetArrayItem(numbers, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double total = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", total);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON *negflag_item = cJSON_GetObjectItem(root, "negflag");
    cJSON_bool is_flag = cJSON_IsTrue(flag_item);
    cJSON_bool is_negflag = cJSON_IsFalse(negflag_item);
    double summary = total + (double)size + (double)is_flag + (double)is_negflag;
    cJSON_AddNumberToObject(root, "summary", summary);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)v0; (void)v1; (void)v2; (void)size; (void)is_flag; (void)is_negflag; (void)summary;
    // API sequence test completed successfully
    return 66;
}