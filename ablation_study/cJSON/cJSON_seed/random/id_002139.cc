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
//<ID> 2139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_n0 = NULL;
    cJSON *got_n1 = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    n1 = cJSON_CreateNumber(3.1415);
    n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "answer", cJSON_CreateNumber(42.0));

    // step 3: Operate
    got_arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    got_n0 = cJSON_GetArrayItem(got_arr, 0);
    got_n1 = cJSON_GetArrayItem(got_arr, 1);
    is_num0 = cJSON_IsNumber(got_n0);
    is_num1 = cJSON_IsNumber(got_n1);
    v0 = cJSON_GetNumberValue(got_n0);
    v1 = cJSON_GetNumberValue(got_n1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_num1 != 0);
    validation ^= (int)(v0 == 3.1415);
    validation ^= (int)(v1 == 2.718);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}