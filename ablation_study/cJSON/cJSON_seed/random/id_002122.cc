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
//<ID> 2122
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
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *flag = NULL;
    void *raw_mem = NULL;
    cJSON_bool is_flag_true = 0;
    char *printed = NULL;
    int validation = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.0);
    flag = cJSON_CreateTrue();
    raw_mem = cJSON_malloc(64);
    memset(raw_mem, 0, 64);

    // step 3: Operate
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, flag);
    is_flag_true = cJSON_IsTrue(flag);
    printed = cJSON_PrintUnformatted(arr);

    // step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(flag != NULL);
    validation ^= (int)(raw_mem != NULL);
    validation ^= (int)(is_flag_true != 0);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(raw_mem);
    cJSON_free(printed);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}