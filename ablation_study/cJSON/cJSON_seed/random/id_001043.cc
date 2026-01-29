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
//<ID> 1043
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key = "arr";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    int arr_size = 0;
    double val0 = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.5);
    str0 = cJSON_CreateString("hello");

    // step 3: Operate - assemble the array and attach to root using CS add
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToObjectCS(root, key, arr);

    // step 4: Extract - measure array size and read a numeric value
    arr_size = cJSON_GetArraySize(arr);
    fetched0 = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(fetched0);

    // step 5: Validate - compute a small validation score using results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr_size == 3) + (int)(val0 == 10.0);
    (void)validation_score;

    // step 6: Cleanup - free allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}