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
//<ID> 2121
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
    void *mem_block = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *true_item = NULL;
    cJSON *got_true = NULL;
    char *printed = NULL;
    cJSON_bool is_true = 0;
    double val0 = 0.0;
    int validation = 0;

    // step 2: Setup
    mem_block = cJSON_malloc(64);
    memset(mem_block, 0, 64);
    ((char *)mem_block)[0] = 'x';
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, true_item);

    // step 3: Core operations
    got_true = cJSON_GetArrayItem(arr, 2);
    is_true = cJSON_IsTrue(got_true);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    printed = cJSON_PrintUnformatted(arr);

    // step 4: Validate
    validation ^= (int)(mem_block != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(val0 == 3.14);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_free(mem_block);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}