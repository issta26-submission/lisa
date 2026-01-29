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
//<ID> 2127
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
    char *temp_mem = NULL;
    cJSON *array = NULL;
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_num_a = NULL;
    cJSON *got_num_b = NULL;
    cJSON *got_bool = NULL;
    double val_a = 0.0;
    double val_b = 0.0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    temp_mem = (char*)cJSON_malloc(64);
    memset(temp_mem, 0, 64);
    array = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(3.1415);
    num_b = cJSON_CreateNumber(42.0);
    bool_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, num_a);
    cJSON_AddItemToArray(array, num_b);
    cJSON_AddItemToArray(array, bool_item);

    // step 3: Core operations
    got_num_a = cJSON_GetArrayItem(array, 0);
    got_num_b = cJSON_GetArrayItem(array, 1);
    got_bool  = cJSON_GetArrayItem(array, 2);
    val_a = cJSON_GetNumberValue(got_num_a);
    val_b = cJSON_GetNumberValue(got_num_b);
    is_true = cJSON_IsTrue(got_bool);

    // step 4: Validate
    validation ^= (int)(temp_mem != NULL);
    validation ^= (int)(array != NULL);
    validation ^= (int)(num_a != NULL);
    validation ^= (int)(num_b != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_num_a != NULL);
    validation ^= (int)(got_num_b != NULL);
    validation ^= (int)(got_bool != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(val_a == 3.1415);
    validation ^= (int)(val_b == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(temp_mem);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}