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
//<ID> 2028
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *got_sum_item = NULL;
    cJSON_bool added_n1 = 0;
    cJSON_bool added_n2 = 0;
    cJSON_bool added_arr = 0;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    double extracted_sum = 0.0;
    int validation = 0;

    // step 2: Setup (create object, array and number items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(2.5);
    n2 = cJSON_CreateNumber(7.5);

    // step 3: Configure (populate array and object with string and numbers)
    added_n1 = cJSON_AddItemToArray(arr, n1);
    added_n2 = cJSON_AddItemToArray(arr, n2);
    added_arr = cJSON_AddItemToObject(root, "readings", arr);
    cJSON_AddStringToObject(root, "sensor", "alpha");

    // step 4: Operate (compute sum from created numbers and add to object)
    v1 = cJSON_GetNumberValue(n1);
    v2 = cJSON_GetNumberValue(n2);
    sum = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    got_sum_item = cJSON_GetObjectItem(root, "sum");
    extracted_sum = cJSON_GetNumberValue(got_sum_item);

    // step 5: Validate (aggregate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(added_n1 != 0);
    validation ^= (int)(added_n2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(extracted_sum == sum);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}