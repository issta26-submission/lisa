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
//<ID> 1866
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
    cJSON *numA = NULL;
    cJSON *dup_num = NULL;
    cJSON *retrieved_arr_item = NULL;
    cJSON *retrieved_obj_item = NULL;
    char *out = NULL;
    double v_obj = 0.0;
    double v_arr = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    num1 = cJSON_AddNumberToObject(root, "n1", 10.5);
    num2 = cJSON_AddNumberToObject(root, "n2", 20.25);
    numA = cJSON_CreateNumber(30.75);
    cJSON_AddItemToArray(arr, numA);
    dup_num = cJSON_Duplicate(num2, 1);
    cJSON_AddItemToArray(arr, dup_num);

    // step 3: Core operations
    retrieved_arr_item = cJSON_GetArrayItem(arr, 1);
    retrieved_obj_item = cJSON_GetObjectItem(root, "n1");

    // step 4: Validate
    out = cJSON_PrintUnformatted(root);
    v_obj = cJSON_GetNumberValue(retrieved_obj_item);
    v_arr = cJSON_GetNumberValue(retrieved_arr_item);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(numA != NULL);
    validation ^= (int)(dup_num != NULL);
    validation ^= (int)(retrieved_arr_item != NULL);
    validation ^= (int)(retrieved_obj_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(v_obj == 10.5);
    validation ^= (int)(v_arr == 20.25);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}