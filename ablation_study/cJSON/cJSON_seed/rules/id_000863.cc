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
//<ID> 863
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
    cJSON *sref1 = NULL;
    cJSON *sref2 = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    char *printed = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    int arr_size = 0;
    const char *retrieved_str = NULL;
    double retrieved_num = 0.0;
    cJSON_bool insert_ok1 = 0;
    cJSON_bool insert_ok2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    sref1 = cJSON_CreateStringReference("alpha");
    sref2 = cJSON_CreateStringReference("beta");
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);
    cJSON_AddNumberToObject(root, "initial", 0.0);

    // step 3: Operate / Validate
    cJSON_AddItemToArray(arr, sref1);
    insert_ok1 = cJSON_InsertItemInArray(arr, 0, num1);
    insert_ok2 = cJSON_InsertItemInArray(arr, 1, sref2);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    printed = cJSON_PrintUnformatted(root);
    retrieved_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 2));
    retrieved_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    buffer[0] = (char)('0' + (arr_size % 10));
    (void)retrieved_str;
    (void)retrieved_num;
    (void)insert_ok1;
    (void)insert_ok2;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}