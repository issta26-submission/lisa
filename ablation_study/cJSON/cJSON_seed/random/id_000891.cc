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
//<ID> 891
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_array = "data";
    const char *key_pi = "pi";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double number_value = 0.0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup (create object, array and numeric items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_pi = cJSON_CreateNumber(3.1415);
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);

    // step 3: Configure (populate array and attach items to root object)
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, key_array, arr);
    cJSON_AddItemToObject(root, key_pi, num_pi);

    // step 4: Operate (retrieve items, test type, print and extract numeric value)
    retrieved_arr = cJSON_GetObjectItem(root, key_array);
    is_array = cJSON_IsArray(retrieved_arr);
    printed = cJSON_Print(root);
    retrieved_pi = cJSON_GetObjectItem(root, key_pi);
    number_value = cJSON_GetNumberValue(retrieved_pi);

    // step 5: Validate (aggregate simple checks into a score)
    validation_score = (int)is_array + (int)(printed != (char *)0) + (int)number_value;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}