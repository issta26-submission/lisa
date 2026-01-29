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
//<ID> 2022
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num_in_obj = NULL;
    cJSON *str_in_obj = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;
    double extracted_value = 0.0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr = 0;
    int validation = 0;

    // step 2: Setup (create root, array and numeric items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);

    // step 3: Configure (populate array and object using various add APIs)
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    num_in_obj = cJSON_AddNumberToObject(root, "pi", 3.1415);
    str_in_obj = cJSON_AddStringToObject(root, "name", "cjson_test");

    // step 4: Operate (retrieve from array and extract numeric value, print)
    got_item = cJSON_GetArrayItem(arr, 1);
    extracted_value = cJSON_GetNumberValue(got_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate (aggregate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(added_num1 != 0);
    validation ^= (int)(added_num2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(num_in_obj != NULL);
    validation ^= (int)(str_in_obj != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(extracted_value == 20.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}