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
//<ID> 1948
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
    cJSON *num_c = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *got_pi = NULL;
    char *out = NULL;
    double pi_val = 0.0;
    double item0_val = 0.0;
    cJSON_bool add_ok = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(10.0);
    num_b = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, num_b);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    num_c = cJSON_CreateNumber(7.0);
    add_ok = cJSON_AddItemToObject(root, "lucky", num_c);
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(got_pi);
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    item0_val = cJSON_GetNumberValue(arr_item0);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_a != NULL);
    validation ^= (int)(num_b != NULL);
    validation ^= (int)(num_c != NULL);
    validation ^= (int)(add_ok != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(arr_item0 != NULL);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(item0_val == 10.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}