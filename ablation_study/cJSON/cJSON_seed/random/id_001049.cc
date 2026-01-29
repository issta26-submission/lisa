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
//<ID> 1049
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *unused_json = "{\"values\":[10,20.5],\"greeting\":\"hello\"}";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_num0 = (cJSON *)0;
    cJSON *fetched_num1 = (cJSON *)0;
    cJSON *greeting_item = (cJSON *)0;
    const char *greeting_value = (const char *)0;
    int arr_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create object, array, numbers and a string item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.5);
    strItem = cJSON_CreateString("hello");

    // step 3: Configure - populate array and attach items to root using case-sensitive add
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    add_ok_arr = cJSON_AddItemToObjectCS(root, "values", arr);
    add_ok_str = cJSON_AddItemToObjectCS(root, "greeting", strItem);

    // step 4: Operate - fetch the array and its elements, then read sizes and numeric values
    fetched_arr = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(fetched_arr);
    fetched_num0 = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_num1 = cJSON_GetArrayItem(fetched_arr, 1);
    val0 = cJSON_GetNumberValue(fetched_num0);
    val1 = cJSON_GetNumberValue(fetched_num1);
    greeting_item = cJSON_GetObjectItem(root, "greeting");
    greeting_value = cJSON_GetStringValue(greeting_item);

    // step 5: Validate - compute a small validation score using the results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_str) + (int)(arr_size == 2) + (int)(val0 == 10.0) + (int)(val1 == 20.5) + (int)(greeting_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}