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
//<ID> 2027
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
    cJSON *added_pi = NULL;
    cJSON *name_item = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool added_list = 0;
    double got_val = 0.0;
    double pi_val = 0.0;
    int validation = 0;

    // step 2: Setup (create root, array and numeric items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(10.5);
    num_b = cJSON_CreateNumber(-3.25);

    // step 3: Configure (populate array and add named members)
    added_a = cJSON_AddItemToArray(arr, num_a);
    added_b = cJSON_AddItemToArray(arr, num_b);
    added_list = cJSON_AddItemToObject(root, "list", arr);
    added_pi = cJSON_AddNumberToObject(root, "pi", 3.1415);
    name_item = cJSON_AddStringToObject(root, "name", "example");

    // step 4: Operate (retrieve array items and numeric values)
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    got_val = cJSON_GetNumberValue(got1);
    pi_val = cJSON_GetNumberValue(added_pi);

    // step 5: Validate (aggregate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_a != NULL);
    validation ^= (int)(num_b != NULL);
    validation ^= (int)(added_a != 0);
    validation ^= (int)(added_b != 0);
    validation ^= (int)(added_list != 0);
    validation ^= (int)(name_item != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(got_val == -3.25);
    validation ^= (int)(pi_val > 3.14 && pi_val < 3.142);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}