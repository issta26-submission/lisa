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
//<ID> 2125
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
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    cJSON *got_num = NULL;
    cJSON *got_true = NULL;
    int arr_size = 0;
    double num_value = 0.0;
    cJSON_bool is_true_flag = 0;
    int validation = 0;
    char *print_buffer = NULL;

    // step 2: Setup
    print_buffer = (char *)cJSON_malloc(128);
    memset(print_buffer, 0, 128);
    root = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.5);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(root, num_item);
    cJSON_AddItemToArray(root, true_item);

    // step 3: Core operations
    arr_size = cJSON_GetArraySize(root);
    got_num = cJSON_GetArrayItem(root, 0);
    got_true = cJSON_GetArrayItem(root, 1);
    num_value = cJSON_GetNumberValue(got_num);
    cJSON_PrintPreallocated(root, print_buffer, 128, 0);
    is_true_flag = cJSON_IsTrue(got_true);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(num_value == 42.5);
    validation ^= (int)(print_buffer[0] != '\0');
    validation ^= (int)(is_true_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(print_buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}