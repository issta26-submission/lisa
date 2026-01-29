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
//<ID> 1916
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
    cJSON *num = NULL;
    cJSON *num_copy = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_snum = NULL;
    char *strset = NULL;
    cJSON_bool added_ok1 = 0;
    cJSON_bool added_ok2 = 0;
    double value_pi = 0.0;
    double value_snum = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14);
    added_ok1 = cJSON_AddItemToObject(root, "pi", num);
    num_copy = cJSON_CreateNumber(0.0);
    strset = cJSON_SetValuestring(num_copy, "as_string");
    added_ok2 = cJSON_AddItemToObject(root, "snum", num_copy);

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    value_pi = cJSON_GetNumberValue(got_pi);
    got_snum = cJSON_GetObjectItem(root, "snum");
    value_snum = cJSON_GetNumberValue(got_snum);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(num_copy != NULL);
    validation ^= (int)(strset != NULL);
    validation ^= (int)(added_ok1 != 0);
    validation ^= (int)(added_ok2 != 0);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_snum != NULL);
    validation ^= (int)(value_pi == 3.14);
    validation ^= (int)(value_snum == 0.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}