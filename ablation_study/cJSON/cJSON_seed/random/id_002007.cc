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
//<ID> 2007
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_answer = 0;
    int array_size = 0;
    char *strval = NULL;
    int validation = 0;

    // step 2: Setup (Initialize objects and values)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("example");

    // step 3: Configure (build structure and add boolean)
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    added_answer = cJSON_AddItemToObject(root, "answer", num2);

    // step 4: Operate (inspect array and get string value)
    array_size = cJSON_GetArraySize(arr);
    strval = cJSON_GetStringValue(str_item);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_answer != 0);
    validation ^= (int)(array_size == 2);
    validation ^= (int)(strval != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}