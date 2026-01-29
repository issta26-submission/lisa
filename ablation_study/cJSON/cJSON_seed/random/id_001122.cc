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
//<ID> 1122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_in_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_num = 0.0;
    cJSON_bool is_num = (cJSON_bool)0;
    int arr_size = 0;
    cJSON_bool added_cs = (cJSON_bool)0;

    // step 2: Setup - create root object and add a numeric member
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "initial", 7.0);

    // step 3: Setup - create array and populate it with numbers
    arr = cJSON_CreateArray();
    num_in_arr = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_in_arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.718));

    // step 4: Configure - attach array to root using case-sensitive add
    added_cs = cJSON_AddItemToObjectCS(root, "values", arr);

    // step 5: Operate & Validate - retrieve values and serialize unformatted
    fetched = cJSON_GetObjectItem(root, "initial");
    is_num = cJSON_IsNumber(fetched);
    retrieved_num = cJSON_GetNumberValue(fetched);
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "values"));
    printed = cJSON_PrintUnformatted(root);

    // step 6: Cleanup - free printed buffer and delete root which frees children
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)added_cs;
    (void)is_num;
    (void)retrieved_num;
    (void)arr_size;
    (void)num_in_arr;
    return 66;
}