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
//<ID> 1947
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
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *ten_item = NULL;
    cJSON *got_pi = NULL;
    cJSON *arr_item0 = NULL;
    double pi_val = 0.0;
    double ten_val = 0.0;
    double first_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num_a = cJSON_CreateNumber(1.5);
    num_b = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, num_b);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddItemToObject(root, "ten", cJSON_CreateNumber(10.0));

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(got_pi);
    ten_item = cJSON_GetObjectItem(root, "ten");
    ten_val = cJSON_GetNumberValue(ten_item);
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(arr_item0);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_a != NULL);
    validation ^= (int)(num_b != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(ten_item != NULL);
    validation ^= (int)(arr_item0 != NULL);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(ten_val == 10.0);
    validation ^= (int)(first_val == 1.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}